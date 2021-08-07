#define WIN32_LEAN_AND_MEAN
#include "Generated Files/resource.h"

#include <Windows.h>
#include <shellapi.h>

#include <filesystem>
#include <string_view>

#include <common/updating/updating.h>
#include <common/updating/installer.h>
#include <common/updating/http_client.h>
#include <common/updating/dotnet_installation.h>

#include <common/utils/elevation.h>
#include <common/utils/process_path.h>
#include <common/utils/resources.h>

#include <common/SettingsAPI/settings_helpers.h>

#include <common/logger/logger.h>

#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.Storage.h>
#include <Msi.h>

#include "../runner/tray_icon.h"
#include "../runner/action_runner_utils.h"

auto Strings = create_notifications_strings();

int uninstall_msi_action()
{
    const auto package_path = updating::get_msi_package_path();
    if (package_path.empty())
    {
        return 0;
    }
    if (!updating::uninstall_msi_version(package_path, Strings))
    {
        return -1;
    }

    // Launch PowerToys again, since it's been terminated by the MSI uninstaller
    std::wstring runner_path{ winrt::Windows::ApplicationModel::Package::Current().InstalledLocation().Path() };
    runner_path += L"\\PowerToys.exe";
    SHELLEXECUTEINFOW sei{ sizeof(sei) };
    sei.fMask = { SEE_MASK_FLAG_NO_UI | SEE_MASK_NOASYNC };
    sei.lpFile = runner_path.c_str();
    sei.nShow = SW_SHOWNORMAL;
    ShellExecuteExW(&sei);

    return 0;
}

namespace fs = std::filesystem;

std::optional<fs::path> copy_self_to_temp_dir()
{
    std::error_code error;
    auto dst_path = fs::temp_directory_path() / "action_runner.exe";
    fs::copy_file(get_module_filename(), dst_path, fs::copy_options::overwrite_existing, error);
    if (error)
    {
        return std::nullopt;
    }
    return std::move(dst_path);
}

bool install_new_version_stage_1(const std::wstring_view installer_filename, const bool must_restart = false)
{
    const fs::path installer{ updating::get_pending_updates_path() / installer_filename };

    if (!fs::is_regular_file(installer))
    {
        return false;
    }

    if (auto copy_in_temp = copy_self_to_temp_dir())
    {
        // detect if PT was running
        const auto pt_main_window = FindWindowW(pt_tray_icon_window_class, nullptr);
        const bool launch_powertoys = must_restart || pt_main_window != nullptr;
        if (pt_main_window != nullptr)
        {
            SendMessageW(pt_main_window, WM_CLOSE, 0, 0);
        }

        std::wstring arguments{ UPDATE_NOW_LAUNCH_STAGE2_CMDARG };
        arguments += L" \"";
        arguments += installer.c_str();
        arguments += L"\" \"";
        arguments += get_module_folderpath();
        arguments += L"\" ";
        arguments += launch_powertoys ? UPDATE_STAGE2_RESTART_PT_CMDARG : UPDATE_STAGE2_DONT_START_PT_CMDARG;
        SHELLEXECUTEINFOW sei{ sizeof(sei) };
        sei.fMask = { SEE_MASK_FLAG_NO_UI | SEE_MASK_NOASYNC };
        sei.lpFile = copy_in_temp->c_str();
        sei.nShow = SW_SHOWNORMAL;

        sei.lpParameters = arguments.c_str();
        return ShellExecuteExW(&sei) == TRUE;
    }
    else
    {
        return false;
    }
}

bool install_new_version_stage_2(std::wstring installer_path, std::wstring_view install_path, bool launch_powertoys)
{
    std::transform(begin(installer_path), end(installer_path), begin(installer_path), ::towlower);

    bool success = true;

    if (installer_path.ends_with(L".msi"))
    {
        success = MsiInstallProductW(installer_path.data(), nullptr) == ERROR_SUCCESS;
    }
    else
    {
        // If it's not .msi, then it's our .exe installer
        SHELLEXECUTEINFOW sei{ sizeof(sei) };
        sei.fMask = { SEE_MASK_FLAG_NO_UI | SEE_MASK_NOASYNC | SEE_MASK_NOCLOSEPROCESS | SEE_MASK_NO_CONSOLE };
        sei.lpFile = installer_path.c_str();
        sei.nShow = SW_SHOWNORMAL;
        std::wstring parameters = L"--no_full_ui";
        if (launch_powertoys)
        {
            // .exe installer launches the main app by default
            launch_powertoys = false;
        }
        else
        {
            parameters += L"--no_start_pt";
        }
        sei.lpParameters = parameters.c_str();

        success = ShellExecuteExW(&sei) == TRUE;
        // Wait for the install completion
        if (success)
        {
            WaitForSingleObject(sei.hProcess, INFINITE);
            CloseHandle(sei.hProcess);
        }
    }

    std::error_code _;
    fs::remove(installer_path, _);

    if (!success)
    {
        return false;
    }

    if (launch_powertoys)
    {
        std::wstring new_pt_path{ install_path };
        new_pt_path += L"\\PowerToys.exe";
        SHELLEXECUTEINFOW sei{ sizeof(sei) };
        sei.fMask = { SEE_MASK_FLAG_NO_UI | SEE_MASK_NOASYNC };
        sei.lpFile = new_pt_path.c_str();
        sei.nShow = SW_SHOWNORMAL;
        sei.lpParameters = UPDATE_REPORT_SUCCESS;
        return ShellExecuteExW(&sei) == TRUE;
    }
    return true;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    int nArgs = 0;
    LPWSTR* args = CommandLineToArgvW(GetCommandLineW(), &nArgs);
    if (!args || nArgs < 2)
    {
        return 1;
    }
    std::wstring_view action{ args[1] };

    std::filesystem::path logFilePath(PTSettingsHelper::get_root_save_folder_location());
    logFilePath.append(LogSettings::actionRunnerLogPath);
    Logger::init(LogSettings::actionRunnerLoggerName, logFilePath.wstring(), PTSettingsHelper::get_log_settings_file_location());

    if (action == RUN_NONELEVATED_CMDARG)
    {
        int nextArg = 2;

        std::wstring_view target;
        std::wstring_view pidFile;
        std::wstring params;

        while (nextArg < nArgs)
        {
            if (std::wstring_view(args[nextArg]) == L"-target" && nextArg + 1 < nArgs)
            {
                target = args[nextArg + 1];
                nextArg += 2;
            }
            else if (std::wstring_view(args[nextArg]) == L"-pidFile" && nextArg + 1 < nArgs)
            {
                pidFile = args[nextArg + 1];
                nextArg += 2;
            }
            else
            {
                params += args[nextArg];
                params += L' ';
                nextArg++;
            }
        }

        HANDLE hMapFile = NULL;
        PDWORD pidBuffer = NULL;

        if (!pidFile.empty())
        {
            hMapFile = OpenFileMappingW(FILE_MAP_WRITE, FALSE, pidFile.data());
            if (hMapFile)
            {
                pidBuffer = reinterpret_cast<PDWORD>(MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(DWORD)));
                if (pidBuffer)
                {
                    *pidBuffer = 0;
                }
            }
        }

        run_same_elevation(target.data(), params, pidBuffer);

        // cleanup
        if (!pidFile.empty())
        {
            if (pidBuffer)
            {
                FlushViewOfFile(pidBuffer, sizeof(DWORD));
                UnmapViewOfFile(pidBuffer);
            }

            if (hMapFile)
            {
                FlushFileBuffers(hMapFile);
                CloseHandle(hMapFile);
            }
        }
    }
    else if (action == UNINSTALL_MSI_CMDARG)
    {
        return uninstall_msi_action();
    }
    else if (action == UPDATE_NOW_LAUNCH_STAGE1_CMDARG)
    {
        std::wstring_view installerFilename{ args[2] };
        return !install_new_version_stage_1(installerFilename);
    }
    else if (action == UPDATE_NOW_LAUNCH_STAGE1_START_PT_CMDARG)
    {
        std::wstring_view installerFilename{ args[2] };
        return !install_new_version_stage_1(installerFilename, true);
    }
    else if (action == UPDATE_NOW_LAUNCH_STAGE2_CMDARG)
    {
        using namespace std::string_view_literals;
        return !install_new_version_stage_2(args[2], args[3], args[4] == std::wstring_view{ UPDATE_STAGE2_RESTART_PT_CMDARG });
    }

    return 0;
}
