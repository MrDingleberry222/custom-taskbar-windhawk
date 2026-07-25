// ==WindhawkMod==
// @id              custom-taskbar-windhawk
// @name            Custom Taskbar (Windows 10)
// @description     Customize your Windows 10 taskbar layout with positions, offsets, and alignment settings
// @version         1.0
// @author          MrDingleberry222
// @github          https://github.com/MrDingleberry222/custom-taskbar-windhawk
// @homepage        https://github.com/MrDingleberry222/custom-taskbar-windhawk
// @include         explorer.exe
// @compilerOptions -lcomdlg32
// @license         MIT
// ==/WindhawkMod==


// ==WindhawkModSettings==
// {
//   "alignment": {
//     "type": "select",
//     "label": "Taskbar Icon Alignment",
//     "options": {
//       "left": "Left",
//       "center": "Center",
//       "right": "Right"
//     },
//     "default": "center"
//   },
//   "offsetX": {
//     "type": "int",
//     "label": "Horizontal Offset",
//     "default": 0
//   },
//   "offsetY": {
//     "type": "int",
//     "label": "Vertical Offset",
//     "default": 0
//   },
//   "refresh": {
//     "type": "int",
//     "label": "Refresh Rate (ms)",
//     "default": 500
//   }
// }
// ==/WindhawkModSettings==


#include <windows.h>
#include <string>


std::wstring alignment = L"center";

int offsetX = 0;
int offsetY = 0;
int refreshRate = 500;


HWND taskbar = nullptr;
HWND taskList = nullptr;


void LoadSettings()
{
    PCWSTR align = Wh_GetStringSetting(L"alignment");

    if (align)
    {
        alignment = align;
        Wh_FreeStringSetting(align);
    }

    offsetX = Wh_GetIntSetting(L"offsetX");
    offsetY = Wh_GetIntSetting(L"offsetY");
    refreshRate = Wh_GetIntSetting(L"refresh");
}


void MoveTaskbarIcons()
{
    if (!taskbar)
    {
        taskbar = FindWindow(L"Shell_TrayWnd", nullptr);
    }

    if (!taskbar)
        return;


    HWND rebar = FindWindowEx(
        taskbar,
        nullptr,
        L"ReBarWindow32",
        nullptr
    );


    if (!rebar)
        return;


    taskList = FindWindowEx(
        rebar,
        nullptr,
        L"MSTaskListWClass",
        nullptr
    );


    if (!taskList)
        return;



    RECT taskbarRect;
    RECT taskListRect;


    GetClientRect(taskbar, &taskbarRect);
    GetWindowRect(taskList, &taskListRect);


    int taskbarWidth =
        taskbarRect.right -
        taskbarRect.left;


    int iconWidth =
        taskListRect.right -
        taskListRect.left;



    int newX = 0;


    if (alignment == L"center")
    {
        newX = (taskbarWidth - iconWidth) / 2;
    }
    else if (alignment == L"right")
    {
        newX = taskbarWidth - iconWidth;
    }
    else
    {
        newX = 0;
    }


    newX += offsetX;



    SetWindowPos(
        taskList,
        nullptr,
        newX,
        offsetY,
        0,
        0,
        SWP_NOZORDER |
        SWP_NOSIZE |
        SWP_NOACTIVATE
    );
}



DWORD WINAPI TaskbarThread(LPVOID)
{
    while (true)
    {
        MoveTaskbarIcons();
        Sleep(refreshRate);
    }

    return 0;
}



BOOL Wh_ModInit()
{
    LoadSettings();

    CreateThread(
        nullptr,
        0,
        TaskbarThread,
        nullptr,
        0,
        nullptr
    );


    return TRUE;
}



void Wh_ModSettingsChanged()
{
    LoadSettings();
}
