#include <windows.h>
#include <shellapi.h>
#include <tlhelp32.h>
#include <iostream>

// For PathFileExists()
#include <shlwapi.h>

#include "resource.h"

#define TRAY_ICON_ID IDI_MYICON
#define WM_TRAYICON (WM_USER + 1)
#define ID_BUTTON 1
#define ID_CHECKBOX1 101
#define ID_CHECKBOX2 102
#define ID_CHECKBOX3 103


bool hookEnabled = false;

//const char* REG_PATH = "Software\\ProcessHitman";
//const char* RUN_KEY = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";

NOTIFYICONDATA nid;
HWND hwnd, hwndLargeText, hwndSmallText, hwndButton;
HWND hwndCheckbox1, hwndCheckbox2, hwndCheckbox3;
HBRUSH hbrBkgnd;
HFONT hLargeFont;
HFONT hSmallFont;
bool buttonToggled = false;

DWORD GetProcessIdOfActiveWindow() {
    HWND hwnd = GetForegroundWindow();
    DWORD processID = 0;
    GetWindowThreadProcessId(hwnd, &processID);
    return processID;
}

/*void KillProcess(DWORD processID) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processID);
    if (hProcess != NULL) {
        TerminateProcess(hProcess, 0);
        CloseHandle(hProcess);
    }
    else
    {
        std::cerr << "Failed to open process!" << std::endl;
    }
}*/

// global keyboard hook
/*LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && hookEnabled) {
        if (GetAsyncKeyState(VK_F5) & 0x8000)
        {
            if (GetAsyncKeyState(VK_MENU) & 0x8000)
            {
                DWORD processID = GetProcessIdOfActiveWindow();
                if (processID != 0)
                {
                    KillProcess(processID); 
                }
            }
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}*/

void AddTrayIcon(HWND hwnd) {
    memset(&nid, 0, sizeof(nid));
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = TRAY_ICON_ID;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    strcpy(nid.szTip, "Process Hitman");

    Shell_NotifyIcon(NIM_ADD, &nid);
}

void RemoveTrayIcon() {
    Shell_NotifyIcon(NIM_DELETE, &nid);
}

void HideToTray(HWND hwnd) {
    ShowWindow(hwnd, SW_HIDE);
    AddTrayIcon(hwnd);
}

void RestoreFromTray(HWND hwnd) {
    ShowWindow(hwnd, SW_SHOW);
    RemoveTrayIcon();
}
/*
void AddToStartup() {
    HKEY hKey;
    const char* appName = "ProcessHitman";
    char appPath[MAX_PATH];

    // Get the path of the app
    GetModuleFileName(NULL, appPath, MAX_PATH);

    if (RegOpenKeyEx(HKEY_CURRENT_USER, RUN_KEY, 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        RegSetValueEx(hKey, appName, 0, REG_SZ, (BYTE*)appPath, (DWORD)(strlen(appPath) + 1));
        RegCloseKey(hKey);
    }
}

void RemoveFromStartup() {
    HKEY hKey;
    const char* appName = "ProcessHitman";

    if (RegOpenKeyEx(HKEY_CURRENT_USER, RUN_KEY, 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        RegDeleteValue(hKey, appName);
        RegCloseKey(hKey);
    }
}*/

void EnableHook(HWND hwndButton) {
    buttonToggled = true;
    SetWindowText(hwndButton, "Disable Alt+F5");
    hookEnabled = true;
}

void DisableHook(HWND hwndButton) {
    buttonToggled = false;
    SetWindowText(hwndButton, "Enable Alt+F5");
    hookEnabled = false;
}

// Toggle button text & hook state when clicked
void ToggleButtonText(HWND hwndButton) {
    if (buttonToggled) {
        DisableHook(hwndButton);
    } else {
        EnableHook(hwndButton);
    }
}

// Save to registry
/*void SaveCheckboxStates() {
    HKEY hKey;
    DWORD checkbox1State = (SendMessage(hwndCheckbox1, BM_GETCHECK, 0, 0) == BST_CHECKED) ? 1 : 0;
    DWORD checkbox2State = (SendMessage(hwndCheckbox2, BM_GETCHECK, 0, 0) == BST_CHECKED) ? 1 : 0;
    DWORD checkbox3State = (SendMessage(hwndCheckbox3, BM_GETCHECK, 0, 0) == BST_CHECKED) ? 1 : 0;

    if (RegCreateKeyEx(HKEY_CURRENT_USER, REG_PATH, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        RegSetValueEx(hKey, "Checkbox1", 0, REG_DWORD, (BYTE*)&checkbox1State, sizeof(DWORD));
        RegSetValueEx(hKey, "Checkbox2", 0, REG_DWORD, (BYTE*)&checkbox2State, sizeof(DWORD));
        RegSetValueEx(hKey, "Checkbox3", 0, REG_DWORD, (BYTE*)&checkbox3State, sizeof(DWORD));
        RegCloseKey(hKey);
    }

    // Add to registry if open on startup is checked
    if (checkbox1State == 1) {
        AddToStartup();
    } else {
        RemoveFromStartup();
    }
}

// Load from registry
void LoadCheckboxStates() {
    HKEY hKey;
    DWORD checkbox1State = 0, checkbox2State = 0, checkbox3State = 0;
    DWORD dataSize = sizeof(DWORD);

    if (RegOpenKeyEx(HKEY_CURRENT_USER, REG_PATH, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        RegQueryValueEx(hKey, "Checkbox1", NULL, NULL, (LPBYTE)&checkbox1State, &dataSize);
        RegQueryValueEx(hKey, "Checkbox2", NULL, NULL, (LPBYTE)&checkbox2State, &dataSize);
        RegQueryValueEx(hKey, "Checkbox3", NULL, NULL, (LPBYTE)&checkbox3State, &dataSize);
        RegCloseKey(hKey);
    }

    // Tray checkbox
    if (checkbox2State == 1) {
        HideToTray(hwnd);
    }

    // Enable on startup checkbox
    if (checkbox3State == 1) {
        EnableHook(hwndButton);
    }

    // Set checkbox states based on loaded values
    SendMessage(hwndCheckbox1, BM_SETCHECK, checkbox1State ? BST_CHECKED : BST_UNCHECKED, 0);
    SendMessage(hwndCheckbox2, BM_SETCHECK, checkbox2State ? BST_CHECKED : BST_UNCHECKED, 0);
    SendMessage(hwndCheckbox3, BM_SETCHECK, checkbox3State ? BST_CHECKED : BST_UNCHECKED, 0);
}*/

// Creates the UI
void CreateUIElements(HWND hwnd) {
    // The texts
    hwndLargeText = CreateWindowEx(0, "STATIC", "Process Hitman by zyapguy", WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 20, 400, 40, hwnd, NULL, NULL, NULL);
    SendMessage(hwndLargeText, WM_SETFONT, (WPARAM)hLargeFont, TRUE); // Apply large font

    hwndSmallText = CreateWindowEx(0, "STATIC", "Press Ctrl+T to hide to tray.", WS_CHILD | WS_VISIBLE | SS_CENTER, 50, 50, 300, 20, hwnd, NULL, NULL, NULL);
    SendMessage(hwndSmallText, WM_SETFONT, (WPARAM)hSmallFont, TRUE); // Apply large font

    // Checkboxes
    hwndCheckbox1 = CreateWindowEx(0, "BUTTON", "Open on startup", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 70, 300, 20, hwnd, (HMENU)ID_CHECKBOX1, NULL, NULL);
    hwndCheckbox2 = CreateWindowEx(0, "BUTTON", "Start hidden", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 100, 300, 20, hwnd, (HMENU)ID_CHECKBOX2, NULL, NULL);
    hwndCheckbox3 = CreateWindowEx(0, "BUTTON", "Enable on start", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 50, 130, 300, 20, hwnd, (HMENU)ID_CHECKBOX3, NULL, NULL);

    SendMessage(hwndCheckbox1, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
    SendMessage(hwndCheckbox2, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
    SendMessage(hwndCheckbox3, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);

    // Button
    hwndButton = CreateWindowEx(0, "BUTTON", "Enable Alt+F5", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                150, 180, 100, 30, hwnd, (HMENU)ID_BUTTON, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
    SendMessage(hwndButton, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);

    // Load the saved checkbox states
    //LoadCheckboxStates();
}

// Window Procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            // Create a brush for the background
            hbrBkgnd = CreateSolidBrush(GetSysColor(COLOR_WINDOW));

            // Create a large font
            hLargeFont = CreateFont(
                28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_SWISS, "Arial");

            // Create a smaller font
            // also I find it hilarious that the font weight is DONTCARE. I think its default, but there is also NORMAL
            hSmallFont = CreateFont(
                16, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_SWISS, "Arial");

            CreateUIElements(hwnd); // Create the UI elements when the window is created
            break;

        case WM_CTLCOLORSTATIC:
            // Set background color of things to match the window's background
            SetBkMode((HDC)wParam, TRANSPARENT);  // Make text background transparent
            return (LRESULT)hbrBkgnd;

        case WM_TRAYICON:
            if (lParam == WM_LBUTTONUP) {
                RestoreFromTray(hwnd);
            }
            break;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_BUTTON:
                    ToggleButtonText(hwndButton);
                    break;
                case ID_CHECKBOX1:
                    if (HIWORD(wParam) == BN_CLICKED) {
                        SendMessage(hwndCheckbox1, BM_SETCHECK, (SendMessage(hwndCheckbox1, BM_GETCHECK, 0, 0) == BST_CHECKED) ? BST_UNCHECKED : BST_CHECKED, 0);
                    }
                    break;
                case ID_CHECKBOX2:
                    if (HIWORD(wParam) == BN_CLICKED) {
                        SendMessage(hwndCheckbox2, BM_SETCHECK, (SendMessage(hwndCheckbox2, BM_GETCHECK, 0, 0) == BST_CHECKED) ? BST_UNCHECKED : BST_CHECKED, 0);
                    }
                    break;
                case ID_CHECKBOX3:
                    if (HIWORD(wParam) == BN_CLICKED) {
                        SendMessage(hwndCheckbox3, BM_SETCHECK, (SendMessage(hwndCheckbox3, BM_GETCHECK, 0, 0) == BST_CHECKED) ? BST_UNCHECKED : BST_CHECKED, 0);
                    }
                    break;
            }
            break;

        case WM_DESTROY:
            // Save data
            //SaveCheckboxStates();

            // Cleanup
            RemoveTrayIcon();
            DeleteObject(hLargeFont);
            DeleteObject(hbrBkgnd);
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "ProcessHitman";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); // Set window background color
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON)); // Use the custom icon instead of the default one
    RegisterClass(&wc);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int windowWidth = 400;
    int windowHeight = 250;

    int windowPosX = (screenWidth - windowWidth) / 2;
    int windowPosY = (screenHeight - windowHeight) / 2;

    // Create the window
    hwnd = CreateWindowEx(0, CLASS_NAME, "Process", WS_OVERLAPPEDWINDOW, windowPosX, windowPosY, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);

    /*HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    if (hHook == NULL) {
        std::cerr << "Failed to install hook!" << std::endl;
        return 1;
    }*/

    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && GetAsyncKeyState('T') & 0x8000) {
            HideToTray(hwnd);
        }
    }

    //UnhookWindowsHookEx(hHook);
    return 0;
}