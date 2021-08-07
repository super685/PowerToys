﻿// Copyright (c) Microsoft Corporation
// The Microsoft Corporation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

using System;
using System.Runtime.CompilerServices;
using Microsoft.PowerToys.Settings.UI.Library.Helpers;
using Microsoft.PowerToys.Settings.UI.Library.Interfaces;

namespace Microsoft.PowerToys.Settings.UI.Library.ViewModels
{
    public class ShortcutGuideViewModel : Observable
    {
        private GeneralSettings GeneralSettingsConfig { get; set; }

        private ShortcutGuideSettings Settings { get; set; }

        private const string ModuleName = ShortcutGuideSettings.ModuleName;

        private Func<string, int> SendConfigMSG { get; }

        private string _settingsConfigFileFolder = string.Empty;

        public ShortcutGuideViewModel(ISettingsRepository<GeneralSettings> settingsRepository, ISettingsRepository<ShortcutGuideSettings> moduleSettingsRepository, Func<string, int> ipcMSGCallBackFunc, string configFileSubfolder = "")
        {
            // Update Settings file folder:
            _settingsConfigFileFolder = configFileSubfolder;

            // To obtain the general PowerToys settings.
            if (settingsRepository == null)
            {
                throw new ArgumentNullException(nameof(settingsRepository));
            }

            GeneralSettingsConfig = settingsRepository.SettingsConfig;

            // To obtain the shortcut guide settings, if the file exists.
            // If not, to create a file with the default settings and to return the default configurations.
            if (moduleSettingsRepository == null)
            {
                throw new ArgumentNullException(nameof(moduleSettingsRepository));
            }

            Settings = moduleSettingsRepository.SettingsConfig;

            // set the callback functions value to hangle outgoing IPC message.
            SendConfigMSG = ipcMSGCallBackFunc;

            _isEnabled = GeneralSettingsConfig.Enabled.ShortcutGuide;
            _pressTime = Settings.Properties.PressTime.Value;
            _opacity = Settings.Properties.OverlayOpacity.Value;

            string theme = Settings.Properties.Theme.Value;

            if (theme == "dark")
            {
                _themeIndex = 0;
            }

            if (theme == "light")
            {
                _themeIndex = 1;
            }

            if (theme == "system")
            {
                _themeIndex = 2;
            }
        }

        private bool _isEnabled;
        private int _themeIndex;
        private int _pressTime;
        private int _opacity;

        public bool IsEnabled
        {
            get
            {
                return _isEnabled;
            }

            set
            {
                if (value != _isEnabled)
                {
                    _isEnabled = value;

                    // To update the status of shortcut guide in General PowerToy settings.
                    GeneralSettingsConfig.Enabled.ShortcutGuide = value;
                    OutGoingGeneralSettings snd = new OutGoingGeneralSettings(GeneralSettingsConfig);

                    SendConfigMSG(snd.ToString());
                    OnPropertyChanged(nameof(IsEnabled));
                }
            }
        }

        public int ThemeIndex
        {
            get
            {
                return _themeIndex;
            }

            set
            {
                if (_themeIndex != value)
                {
                    if (value == 0)
                    {
                        // set theme to dark.
                        Settings.Properties.Theme.Value = "dark";
                        _themeIndex = value;
                        NotifyPropertyChanged();
                    }

                    if (value == 1)
                    {
                        // set theme to light.
                        Settings.Properties.Theme.Value = "light";
                        _themeIndex = value;
                        NotifyPropertyChanged();
                    }

                    if (value == 2)
                    {
                        // set theme to system default.
                        Settings.Properties.Theme.Value = "system";
                        _themeIndex = value;
                        NotifyPropertyChanged();
                    }
                }
            }
        }

        public int PressTime
        {
            get
            {
                return _pressTime;
            }

            set
            {
                if (_pressTime != value)
                {
                    _pressTime = value;
                    Settings.Properties.PressTime.Value = value;
                    NotifyPropertyChanged();
                }
            }
        }

        public int OverlayOpacity
        {
            get
            {
                return _opacity;
            }

            set
            {
                if (_opacity != value)
                {
                    _opacity = value;
                    Settings.Properties.OverlayOpacity.Value = value;
                    NotifyPropertyChanged();
                }
            }
        }

        public string GetSettingsSubPath()
        {
            return _settingsConfigFileFolder + "\\" + ModuleName;
        }

        public void NotifyPropertyChanged([CallerMemberName] string propertyName = null)
        {
            OnPropertyChanged(propertyName);
            SndShortcutGuideSettings outsettings = new SndShortcutGuideSettings(Settings);
            SndModuleSettings<SndShortcutGuideSettings> ipcMessage = new SndModuleSettings<SndShortcutGuideSettings>(outsettings);
            SendConfigMSG(ipcMessage.ToJsonString());
        }
    }
}
