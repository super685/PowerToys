#include "pch.h"
#include "CppUnitTest.h"
#include <keyboardmanager/common/KeyboardManagerState.h>
#include <keyboardmanager/ui/LoadingAndSavingRemappingHelper.h>
#include "TestHelpers.h"
#include <common/interop/shared_constants.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RemappingUITests
{
    // Tests for methods in the LoadingAndSavingRemappingHelper namespace
    TEST_CLASS (LoadingAndSavingRemappingTests)
    {
        std::wstring testApp1 = L"testtrocess1.exe";
        std::wstring testApp2 = L"testprocess2.exe";

    public:
        TEST_METHOD_INITIALIZE(InitializeTestEnv)
        {
        }

        // Test if the CheckIfRemappingsAreValid method is successful when no remaps are passed
        TEST_METHOD (CheckIfRemappingsAreValid_ShouldReturnNoError_OnPassingNoRemaps)
        {
            RemapBuffer remapBuffer;

            // Assert that remapping set is valid
            bool isSuccess = (LoadingAndSavingRemappingHelper::CheckIfRemappingsAreValid(remapBuffer) == KeyboardManagerHelper::ErrorType::NoError);
            Assert::AreEqual(true, isSuccess);
        }

        // Test if the CheckIfRemappingsAreValid method is successful when valid key to key remaps are passed
        TEST_METHOD (CheckIfRemappingsAreValid_ShouldReturnNoError_OnPassingValidKeyToKeyRemaps)
        {
            RemapBuffer remapBuffer;

            // Remap A to B and B to C
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ 0x41, 0x42 }), std::wstring()));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ 0x42, 0x43 }), std::wstring()));

            // Assert that remapping set is valid
            bool isSuccess = (LoadingAndSavingRemappingHelper::CheckIfRemappingsAreValid(remapBuffer) == KeyboardManagerHelper::ErrorType::NoError);
            Assert::AreEqual(true, isSuccess);
        }

        // Test if the CheckIfRemappingsAreValid method is successful when valid key to shortcut remaps are passed
        TEST_METHOD (CheckIfRemappingsAreValid_ShouldReturnNoError_OnPassingValidKeyToShortcutRemaps)
        {
            RemapBuffer remapBuffer;

            // Remap A to Ctrl+V and B to Alt+Tab
            Shortcut s1;
            s1.SetKey(VK_CONTROL);
            s1.SetKey(0x56);
            Shortcut s2;
            s2.SetKey(VK_MENU);
            s2.SetKey(VK_TAB);
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ 0x41, s1 }), std::wstring()));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ 0x42, s2 }), std::wstring()));

            // Assert that remapping set is valid
            bool isSuccess = (LoadingAndSavingRemappingHelper::CheckIfRemappingsAreValid(remapBuffer) == KeyboardManagerHelper::ErrorType::NoError);
            Assert::AreEqual(true, isSuccess);
        }

        // Test if the CheckIfRemappingsAreValid method is successful when valid shortcut to key remaps are passed
        TEST_METHOD (CheckIfRemappingsAreValid_ShouldReturnNoError_OnPassingValidShortcutToKeyRemaps)
        {
            RemapBuffer remapBuffer;

            // Remap Ctrl+V to A and Alt+Tab to B
            Shortcut s1;
            s1.SetKey(VK_CONTROL);
            s1.SetKey(0x56);
            Shortcut s2;
            s2.SetKey(VK_MENU);
            s2.SetKey(VK_TAB);
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ s1, 0x41 }), std::wstring()));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ s2, 0x42 }), std::wstring()));

            // Assert that remapping set is valid
            bool isSuccess = (LoadingAndSavingRemappingHelper::CheckIfRemappingsAreValid(remapBuffer) == KeyboardManagerHelper::ErrorType::NoError);
            Assert::AreEqual(true, isSuccess);
        }

        // Test if the CheckIfRemappingsAreValid method is successful when valid shortcut to shortcut remaps are passed
        TEST_METHOD (CheckIfRemappingsAreValid_ShouldReturnNoError_OnPassingValidShortcutToShortcutRemaps)
        {
            RemapBuffer remapBuffer;

            // Remap Ctrl+V to Ctrl+D and Alt+Tab to Win+A
            Shortcut src1;
            src1.SetKey(VK_CONTROL);
            src1.SetKey(0x56);
            Shortcut dest1;
            dest1.SetKey(VK_CONTROL);
            dest1.SetKey(0x44);
            Shortcut src2;
            src2.SetKey(VK_MENU);
            src2.SetKey(VK_TAB);
            Shortcut dest2;
            dest2.SetKey(CommonSharedConstants::VK_WIN_BOTH);
            dest2.SetKey(0x41);
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ src1, dest1 }), std::wstring()));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ src2, dest2 }), std::wstring()));

            // Assert that remapping set is valid
            bool isSuccess = (LoadingAndSavingRemappingHelper::CheckIfRemappingsAreValid(remapBuffer) == KeyboardManagerHelper::ErrorType::NoError);
            Assert::AreEqual(true, isSuccess);
        }

        // Test if the CheckIfRemappingsAreValid method is successful when valid remaps are passed
        TEST_METHOD (CheckIfRemappingsAreValid_ShouldReturnNoError_OnPassingValidRemapsOfAllTypes)
        {
            RemapBuffer remapBuffer;

            // Remap Ctrl+V to Ctrl+D, Alt+Tab to A, A to B and B to Win+A
            Shortcut src1;
            src1.SetKey(VK_CONTROL);
            src1.SetKey(0x56);
            Shortcut dest1;
            dest1.SetKey(VK_CONTROL);
            dest1.SetKey(0x44);
            Shortcut src2;
            src2.SetKey(VK_MENU);
            src2.SetKey(VK_TAB);
            Shortcut dest2;
            dest2.SetKey(CommonSharedConstants::VK_WIN_BOTH);
            dest2.SetKey(0x41);
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ src1, dest1 }), std::wstring()));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ src2, 0x41 }), std::wstring()));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ 0x41, 0x42 }), std::wstring()));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ 0x42, dest2 }), std::wstring()));

            // Assert that remapping set is valid
            bool isSuccess = (LoadingAndSavingRemappingHelper::CheckIfRemappingsAreValid(remapBuffer) == KeyboardManagerHelper::ErrorType::NoError);
            Assert::AreEqual(true, isSuccess);
        }

        // Test if the CheckIfRemappingsAreValid method is unsuccessful when remaps with null keys are passed
        TEST_METHOD (CheckIfRemappingsAreValid_ShouldReturnRemapUnsuccessful_OnPassingRemapsWithNullKeys)
        {
            RemapBuffer remapBuffer;

            // Remap A to NULL
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ 0x41, NULL }), std::wstring()));

            // Assert that remapping set is invalid
            bool isSuccess = (LoadingAndSavingRemappingHelper::CheckIfRemappingsAreValid(remapBuffer) == KeyboardManagerHelper::ErrorType::RemapUnsuccessful);
            Assert::AreEqual(true, isSuccess);
        }

        // Test if the CheckIfRemappingsAreValid method is unsuccessful when remaps with invalid shortcuts are passed
        TEST_METHOD (CheckIfRemappingsAreValid_ShouldReturnRemapUnsuccessful_OnPassingRemapsWithInvalidShortcut)
        {
            RemapBuffer remapBuffer;

            // Remap A to incomplete shortcut (Ctrl)
            Shortcut src1;
            src1.SetKey(VK_CONTROL);
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ 0x41, src1 }), std::wstring()));

            // Assert that remapping set is invalid
            bool isSuccess = (LoadingAndSavingRemappingHelper::CheckIfRemappingsAreValid(remapBuffer) == KeyboardManagerHelper::ErrorType::RemapUnsuccessful);
            Assert::AreEqual(true, isSuccess);
        }

        // Test if the CheckIfRemappingsAreValid method is unsuccessful when remaps with the same key remapped twice are passed
        TEST_METHOD (CheckIfRemappingsAreValid_ShouldReturnRemapUnsuccessful_OnPassingRemapsWithSameKeyRemappedTwice)
        {
            RemapBuffer remapBuffer;

            // Remap A to B and A to Ctrl+C
            Shortcut src1;
            src1.SetKey(VK_CONTROL);
            src1.SetKey(0x43);
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ 0x41, 0x42 }), std::wstring()));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ 0x41, src1 }), std::wstring()));

            // Assert that remapping set is invalid
            bool isSuccess = (LoadingAndSavingRemappingHelper::CheckIfRemappingsAreValid(remapBuffer) == KeyboardManagerHelper::ErrorType::RemapUnsuccessful);
            Assert::AreEqual(true, isSuccess);
        }

        // Test if the CheckIfRemappingsAreValid method is unsuccessful when remaps with the same shortcut remapped twice are passed
        TEST_METHOD (CheckIfRemappingsAreValid_ShouldReturnRemapUnsuccessful_OnPassingRemapsWithSameShortcutRemappedTwice)
        {
            RemapBuffer remapBuffer;

            // Remap Ctrl+A to B and Ctrl+A to Ctrl+V
            Shortcut src1;
            src1.SetKey(VK_CONTROL);
            src1.SetKey(0x41);
            Shortcut dest1;
            dest1.SetKey(VK_CONTROL);
            dest1.SetKey(0x56);
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ src1, 0x42 }), std::wstring()));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ src1, dest1 }), std::wstring()));

            // Assert that remapping set is invalid
            bool isSuccess = (LoadingAndSavingRemappingHelper::CheckIfRemappingsAreValid(remapBuffer) == KeyboardManagerHelper::ErrorType::RemapUnsuccessful);
            Assert::AreEqual(true, isSuccess);
        }

        // Test if the CheckIfRemappingsAreValid method is unsuccessful when app specific remaps with the same shortcut remapped twice for the same target app are passed
        TEST_METHOD (CheckIfRemappingsAreValid_ShouldReturnRemapUnsuccessful_OnPassingAppSpecificRemapsWithSameShortcutRemappedTwiceForTheSameTargetApp)
        {
            RemapBuffer remapBuffer;

            // Remap Ctrl+A to B and Ctrl+A to Ctrl+V for testApp1
            Shortcut src1;
            src1.SetKey(VK_CONTROL);
            src1.SetKey(0x41);
            Shortcut dest1;
            dest1.SetKey(VK_CONTROL);
            dest1.SetKey(0x56);
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ src1, 0x42 }), testApp1));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ src1, dest1 }), testApp1));

            // Assert that remapping set is invalid
            bool isSuccess = (LoadingAndSavingRemappingHelper::CheckIfRemappingsAreValid(remapBuffer) == KeyboardManagerHelper::ErrorType::RemapUnsuccessful);
            Assert::AreEqual(true, isSuccess);
        }

        // Test if the CheckIfRemappingsAreValid method is successful when app specific remaps with the same shortcut remapped twice for different target apps are passed
        TEST_METHOD (CheckIfRemappingsAreValid_ShouldReturnNoError_OnPassingAppSpecificRemapsWithSameShortcutRemappedTwiceForDifferentTargetApps)
        {
            RemapBuffer remapBuffer;

            // Remap Ctrl+A to B for testApp1 and Ctrl+A to Ctrl+V for testApp2
            Shortcut src1;
            src1.SetKey(VK_CONTROL);
            src1.SetKey(0x41);
            Shortcut dest1;
            dest1.SetKey(VK_CONTROL);
            dest1.SetKey(0x56);
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ src1, 0x42 }), testApp1));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ src1, dest1 }), testApp2));

            // Assert that remapping set is valid
            bool isSuccess = (LoadingAndSavingRemappingHelper::CheckIfRemappingsAreValid(remapBuffer) == KeyboardManagerHelper::ErrorType::NoError);
            Assert::AreEqual(true, isSuccess);
        }

        // Test if the GetOrphanedKeys method return an empty vector on passing no remaps
        TEST_METHOD (GetOrphanedKeys_ShouldReturnEmptyVector_OnPassingNoRemaps)
        {
            RemapBuffer remapBuffer;

            // Assert that there are no orphaned keys
            Assert::AreEqual(true, LoadingAndSavingRemappingHelper::GetOrphanedKeys(remapBuffer).empty());
        }

        // Test if the GetOrphanedKeys method return one orphaned on passing one key remap
        TEST_METHOD (GetOrphanedKeys_ShouldReturnOneOrphanedKey_OnPassingOneKeyRemap)
        {
            RemapBuffer remapBuffer;

            // Remap A to B
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ 0x41, 0x42 }), std::wstring()));

            // Assert that only A is orphaned
            Assert::AreEqual((size_t)1, LoadingAndSavingRemappingHelper::GetOrphanedKeys(remapBuffer).size());
            Assert::AreEqual((DWORD)0x41, LoadingAndSavingRemappingHelper::GetOrphanedKeys(remapBuffer)[0]);
        }

        // Test if the GetOrphanedKeys method return an empty vector on passing swapped key remaps
        TEST_METHOD (GetOrphanedKeys_ShouldReturnEmptyVector_OnPassingSwappedKeyRemap)
        {
            RemapBuffer remapBuffer;

            // Remap A to B and B to A
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ 0x41, 0x42 }), std::wstring()));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ 0x42, 0x41 }), std::wstring()));

            // Assert that there are no orphaned keys
            Assert::AreEqual(true, LoadingAndSavingRemappingHelper::GetOrphanedKeys(remapBuffer).empty());
        }

        // Test if the GetOrphanedKeys method return one orphaned on passing two key remaps where one key is mapped to a remapped key
        TEST_METHOD (GetOrphanedKeys_ShouldReturnOneOrphanedKey_OnPassingTwoKeyRemapsWhereOneKeyIsMappedToARemappedKey)
        {
            RemapBuffer remapBuffer;

            // Remap A to Ctrl+B and C to A
            Shortcut dest1;
            dest1.SetKey(VK_CONTROL);
            dest1.SetKey(0x42);
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ 0x41, dest1 }), std::wstring()));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ 0x43, 0x41 }), std::wstring()));

            // Assert that only C is orphaned
            Assert::AreEqual((size_t)1, LoadingAndSavingRemappingHelper::GetOrphanedKeys(remapBuffer).size());
            Assert::AreEqual((DWORD)0x43, LoadingAndSavingRemappingHelper::GetOrphanedKeys(remapBuffer)[0]);
        }

        // Test if the PreProcessRemapTable method combines all the modifier pairs when the left and right modifiers are remapped to the same target
        TEST_METHOD (PreProcessRemapTable_ShouldCombineAllPairs_OnPassingLeftAndRightModifiersRemappedToTheSameTarget)
        {
            SingleKeyRemapTable remapTable;

            // Remap LCtrl and RCtrl to A, LAlt and RAlt to B, LShift and RShift to C, LWin and RWin to D
            remapTable[VK_LCONTROL] = 0x41;
            remapTable[VK_RCONTROL] = 0x41;
            remapTable[VK_LMENU] = 0x42;
            remapTable[VK_RMENU] = 0x42;
            remapTable[VK_LSHIFT] = 0x43;
            remapTable[VK_RSHIFT] = 0x43;
            remapTable[VK_LWIN] = 0x44;
            remapTable[VK_RWIN] = 0x44;

            // Pre process table
            LoadingAndSavingRemappingHelper::PreProcessRemapTable(remapTable);

            // Expected Ctrl remapped to A, Alt to B, Shift to C, Win to D
            SingleKeyRemapTable expectedTable;
            expectedTable[VK_CONTROL] = 0x41;
            expectedTable[VK_MENU] = 0x42;
            expectedTable[VK_SHIFT] = 0x43;
            expectedTable[CommonSharedConstants::VK_WIN_BOTH] = 0x44;

            bool areTablesEqual = (expectedTable == remapTable);
            Assert::AreEqual(true, areTablesEqual);
        }

        // Test if the PreProcessRemapTable method does not combines any of the modifier pairs when the left and right modifiers are remapped to different targets
        TEST_METHOD (PreProcessRemapTable_ShouldNotCombineAnyPairs_OnPassingLeftAndRightModifiersRemappedToTheDifferentTargets)
        {
            SingleKeyRemapTable remapTable;

            // Remap left modifiers to A and right modifiers to B
            remapTable[VK_LCONTROL] = 0x41;
            remapTable[VK_RCONTROL] = 0x42;
            remapTable[VK_LMENU] = 0x41;
            remapTable[VK_RMENU] = 0x42;
            remapTable[VK_LSHIFT] = 0x41;
            remapTable[VK_RSHIFT] = 0x42;
            remapTable[VK_LWIN] = 0x41;
            remapTable[VK_RWIN] = 0x42;

            // Pre process table
            LoadingAndSavingRemappingHelper::PreProcessRemapTable(remapTable);

            // Expected unchanged table
            SingleKeyRemapTable expectedTable;
            expectedTable[VK_LCONTROL] = 0x41;
            expectedTable[VK_RCONTROL] = 0x42;
            expectedTable[VK_LMENU] = 0x41;
            expectedTable[VK_RMENU] = 0x42;
            expectedTable[VK_LSHIFT] = 0x41;
            expectedTable[VK_RSHIFT] = 0x42;
            expectedTable[VK_LWIN] = 0x41;
            expectedTable[VK_RWIN] = 0x42;

            bool areTablesEqual = (expectedTable == remapTable);
            Assert::AreEqual(true, areTablesEqual);
        }

        // Test if the ApplySingleKeyRemappings method resets the keyboard manager state's single key remappings on passing an empty buffer
        TEST_METHOD (ApplySingleKeyRemappings_ShouldResetSingleKeyRemappings_OnPassingEmptyBuffer)
        {
            KeyboardManagerState testState;
            RemapBuffer remapBuffer;

            // Remap A to B
            testState.AddSingleKeyRemap(0x41, 0x42);

            // Apply the single key remaps from the buffer to the keyboard manager state variable
            LoadingAndSavingRemappingHelper::ApplySingleKeyRemappings(testState, remapBuffer, false);

            // Assert that single key remapping in the kbm state variable is empty
            Assert::AreEqual((size_t)0, testState.singleKeyReMap.size());
        }

        // Test if the ApplySingleKeyRemappings method copies only the valid remappings to the keyboard manager state variable when some of the remappings are invalid
        TEST_METHOD (ApplySingleKeyRemappings_ShouldCopyOnlyValidRemappings_OnPassingBufferWithSomeInvalidRemappings)
        {
            KeyboardManagerState testState;
            RemapBuffer remapBuffer;

            // Add A->B, B->Ctrl+V, C to incomplete shortcut and D to incomplete key remappings to the buffer
            Shortcut s1;
            s1.SetKey(VK_CONTROL);
            s1.SetKey(0x56);
            Shortcut s2;
            s2.SetKey(VK_LMENU);
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ 0x41, 0x42 }), std::wstring()));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ 0x42, s1 }), std::wstring()));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ 0x43, NULL }), std::wstring()));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ 0x44, s2 }), std::wstring()));

            // Apply the single key remaps from the buffer to the keyboard manager state variable
            LoadingAndSavingRemappingHelper::ApplySingleKeyRemappings(testState, remapBuffer, false);

            // Expected A remapped to B, B remapped to Ctrl+V
            SingleKeyRemapTable expectedTable;
            expectedTable[0x41] = 0x42;
            expectedTable[0x42] = s1;

            bool areTablesEqual = (expectedTable == testState.singleKeyReMap);
            Assert::AreEqual(true, areTablesEqual);
        }

        // Test if the ApplySingleKeyRemappings method splits common modifiers to their left and right version when copying to the keyboard manager state variable if remappings from common modifiers are passed
        TEST_METHOD (ApplySingleKeyRemappings_ShouldSplitRemappingsFromCommonModifiers_OnPassingBufferWithSomeMappingsFromCommonModifiers)
        {
            KeyboardManagerState testState;
            RemapBuffer remapBuffer;

            // Add Ctrl->A, Alt->B, Shift->C and Win->D remappings to the buffer
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ VK_CONTROL, 0x41 }), std::wstring()));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ VK_MENU, 0x42 }), std::wstring()));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ VK_SHIFT, 0x43 }), std::wstring()));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ CommonSharedConstants::VK_WIN_BOTH, 0x44 }), std::wstring()));

            // Apply the single key remaps from the buffer to the keyboard manager state variable
            LoadingAndSavingRemappingHelper::ApplySingleKeyRemappings(testState, remapBuffer, false);

            // Expected LCtrl/RCtrl remapped to A, LAlt/RAlt to B, LShift/RShift to C, LWin/RWin to D
            SingleKeyRemapTable expectedTable;
            expectedTable[VK_LCONTROL] = 0x41;
            expectedTable[VK_RCONTROL] = 0x41;
            expectedTable[VK_LMENU] = 0x42;
            expectedTable[VK_RMENU] = 0x42;
            expectedTable[VK_LSHIFT] = 0x43;
            expectedTable[VK_RSHIFT] = 0x43;
            expectedTable[VK_LWIN] = 0x44;
            expectedTable[VK_RWIN] = 0x44;

            bool areTablesEqual = (expectedTable == testState.singleKeyReMap);
            Assert::AreEqual(true, areTablesEqual);
        }

        // Test if the ApplyShortcutRemappings method resets the keyboard manager state's os level and app specific shortcut remappings on passing an empty buffer
        TEST_METHOD (ApplyShortcutRemappings_ShouldResetShortcutRemappings_OnPassingEmptyBuffer)
        {
            KeyboardManagerState testState;
            RemapBuffer remapBuffer;

            // Remap Ctrl+A to Ctrl+B for all apps and Ctrl+C to Alt+V for testApp1
            Shortcut src1;
            src1.SetKey(VK_CONTROL);
            src1.SetKey(0x41);
            Shortcut dest1;
            dest1.SetKey(VK_CONTROL);
            dest1.SetKey(0x42);
            Shortcut src2;
            src2.SetKey(VK_CONTROL);
            src2.SetKey(0x43);
            Shortcut dest2;
            dest2.SetKey(VK_MENU);
            dest2.SetKey(0x56);
            testState.AddOSLevelShortcut(src1, dest1);
            testState.AddAppSpecificShortcut(testApp1, src1, dest1);

            // Apply the shortcut remaps from the buffer to the keyboard manager state variable
            LoadingAndSavingRemappingHelper::ApplyShortcutRemappings(testState, remapBuffer, false);

            // Assert that shortcut remappings in the kbm state variable is empty
            Assert::AreEqual((size_t)0, testState.osLevelShortcutReMap.size());
            Assert::AreEqual((size_t)0, testState.appSpecificShortcutReMap.size());
        }

        // Test if the ApplyShortcutRemappings method copies only the valid remappings to the keyboard manager state variable when some of the remappings are invalid
        TEST_METHOD (ApplyShortcutRemappings_ShouldCopyOnlyValidRemappings_OnPassingBufferWithSomeInvalidRemappings)
        {
            KeyboardManagerState testState;
            RemapBuffer remapBuffer;

            // Add Ctrl+A->Ctrl+B, Ctrl+C->Alt+V, Ctrl+F->incomplete shortcut and Ctrl+G->incomplete key os level remappings to buffer
            // Add Ctrl+F->Alt+V, Ctrl+G->Ctrl+B, Ctrl+A->incomplete shortcut and  Ctrl+C->incomplete key app specific remappings to buffer
            Shortcut src1;
            src1.SetKey(VK_CONTROL);
            src1.SetKey(0x41);
            Shortcut dest1;
            dest1.SetKey(VK_CONTROL);
            dest1.SetKey(0x42);
            Shortcut src2;
            src2.SetKey(VK_CONTROL);
            src2.SetKey(0x43);
            Shortcut dest2;
            dest2.SetKey(VK_MENU);
            dest2.SetKey(0x56);
            Shortcut src3;
            src3.SetKey(VK_CONTROL);
            src3.SetKey(0x46);
            Shortcut src4;
            src4.SetKey(VK_CONTROL);
            src4.SetKey(0x47);
            Shortcut dest4;
            dest4.SetKey(VK_CONTROL);
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ src1, dest1 }), std::wstring()));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ src2, dest2 }), std::wstring()));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ src3, NULL }), std::wstring()));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ src4, dest4 }), std::wstring()));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ src3, dest2 }), testApp1));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ src4, dest1 }), testApp1));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ src1, NULL }), testApp1));
            remapBuffer.push_back(std::make_pair(RemapBufferItem({ src2, dest4 }), testApp1));

            // Apply the shortcut remaps from the buffer to the keyboard manager state variable
            LoadingAndSavingRemappingHelper::ApplyShortcutRemappings(testState, remapBuffer, false);

            // Ctrl+A->Ctrl+B and Ctrl+C->Alt+V
            ShortcutRemapTable expectedOSLevelTable;
            expectedOSLevelTable[src1] = RemapShortcut(dest1);
            expectedOSLevelTable[src2] = RemapShortcut(dest2);

            // Ctrl+F->Alt+V and Ctrl+G->Ctrl+B for testApp1
            AppSpecificShortcutRemapTable expectedAppSpecificLevelTable;
            expectedAppSpecificLevelTable[testApp1][src3] = RemapShortcut(dest2);
            expectedAppSpecificLevelTable[testApp1][src4] = RemapShortcut(dest1);

            bool areOSLevelTablesEqual = (expectedOSLevelTable == testState.osLevelShortcutReMap);
            bool areAppSpecificTablesEqual = (expectedAppSpecificLevelTable == testState.appSpecificShortcutReMap);
            Assert::AreEqual(true, areOSLevelTablesEqual);
            Assert::AreEqual(true, areAppSpecificTablesEqual);
        }
    };
}
