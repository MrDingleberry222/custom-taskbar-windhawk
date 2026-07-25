// ==WindhawkMod==
// @id              custom-taskbar-windhawk
// @name            Custom Taskbar (Windows 10)
// @description     Customize Windows 10 taskbar icon position and layout
// @version         2.1
// @author          MrDingleberry222
// @github          https://github.com/MrDingleberry222/custom-taskbar-windhawk
// @homepage        https://github.com/MrDingleberry222/custom-taskbar-windhawk
// @include         explorer.exe
// @compilerOptions -lcomdlg32
// @license         MIT
// ==/WindhawkMod==


// ==WindhawkModSettings==
{
    "alignment": {
        "type": "select",
        "label": "Icon Alignment",
        "options": {
            "left": "Left",
            "center": "Center",
            "right": "Right"
        },
        "default": "center"
    },

    "offsetX": {
        "type": "int",
        "label": "Horizontal Offset",
        "default": 0
    },

    "offsetY": {
        "type": "int",
        "label": "Vertical Offset",
        "default": 0
    },

    "refresh": {
        "type": "int",
        "label": "Refresh Rate (ms)",
        "default": 500
    }
}
// ==WindhawkModSettings==


#include <windows.h>
#include <string>


HWND g_taskbar = nullptr;
HWND g_taskList = nullptr;


std::wstring g_alignment = L"center";

int g_offsetX = 0;
int g_offsetY = 0;
int g_refresh = 500;



void LoadSettings()
{
    PCWSTR alignment = Wh_GetStringSetting(L"alignment");

    if (alignment)
    {
        g_alignment = alignment;
        Wh_FreeStringSetting(alignment);
    }


    g_offsetX = Wh_GetIntSetting(L"offsetX");
    g_offsetY = Wh_GetIntSetting(L"offsetY");
    g_refresh = Wh_GetIntSetting(L"refresh");


    if (g_refresh < 50)
        g_refresh = 50;
}



HWND FindTaskList()
{
    HWND taskbar = FindWindow(
        L"Shell_TrayWnd",
        nullptr
    );


    if (!taskbar)
        return nullptr;



    HWND rebar = FindWindowEx(
        taskbar,
        nullptr,
        L"ReBarWindow32",
        nullptr
    );


    if (!rebar)
        return nullptr;



    HWND taskList = FindWindowEx(
        rebar,
        nullptr,
        L"MSTaskListWClass",
        nullptr
    );


    return taskList;
}




void UpdateTaskbar()
{
    if (!g_taskList || !IsWindow(g_taskList))
    {
        g_taskList = FindTaskList();
    }


    if (!g_taskList)
        return;



    HWND taskbar = FindWindow(
        L"Shell_TrayWnd",
        nullptr
    );


    if (!taskbar)
        return;



    RECT taskbarRect;
    RECT iconRect;



    GetClientRect(
        taskbar,
        &taskbarRect
    );


    GetWindowRect(
        g_taskList,
        &iconRect
    );



    int taskbarWidth =
        taskbarRect.right -
        taskbarRect.left;


    int iconWidth =
        iconRect.right -
        iconRect.left;



    int x = 0;



    if (g_alignment == L"center")
    {
        x = (taskbarWidth - iconWidth) / 2;
    }
    else if (g_alignment == L"right")
    {
        x = taskbarWidth - iconWidth;
    }
    else
    {
        x = 0;
    }



    x += g_offsetX;



    SetWindowPos(
        g_taskList,
        nullptr,
        x,
        g_offsetY,
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
        UpdateTaskbar();

        Sleep(g_refresh);
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
