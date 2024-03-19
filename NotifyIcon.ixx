module;

#define WIN32_LEAN_AND_MEAN
#define NOMAXMIN
#include <Windows.h>
#include <shellapi.h>

#ifdef __EDG__
#include <print>
#endif // __EDG__


export module NotifyIcon;

import std;
import MouseHook;

namespace NotifyArea
{
    constexpr UINT NOTIFY_MESSAGE = WM_USER;
    constexpr UINT WM_MENU_STATUS = 1;
    constexpr UINT WM_MENU_QUIT = 2;

    HMENU hMenu = nullptr;
    HWND  hWnd  = nullptr;

    LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM w, LPARAM l) {
        if (msg == NOTIFY_MESSAGE && l == WM_RBUTTONUP) {

            POINT pt; GetCursorPos(&pt);

            SetForegroundWindow(hWnd);

            int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD, pt.x, pt.y, NULL, hWnd, NULL);

            if (cmd == WM_MENU_QUIT) {
                exit(0);
            }
            else if (cmd == WM_MENU_STATUS) {
                MessageBoxExW(
                    nullptr,
                    std::format(L"Block Count: {}", LowLevelMouseHook::count).c_str(),
                    L"Double Clock Blocker",
                    MB_OK, 0
                );
            }
        }
        else {
            return DefWindowProcW(hWnd, msg, w, l);
        }
    }

    export void Start() {
        WNDCLASSEXW wc = {
            .cbSize = sizeof wc,
            .style = CS_CLASSDC,
            .lpfnWndProc = WndProc,
            .cbClsExtra = 0,
            .cbWndExtra = 0,
            .hInstance = GetModuleHandleW(nullptr),
            .hIcon = LoadIconW(nullptr, IDI_APPLICATION),
            .hCursor = LoadCursorW(nullptr, IDC_ARROW),
            .hbrBackground = CreateSolidBrush(0xcccccc),
            .lpszMenuName = nullptr,
            .lpszClassName = L"DBCLK_BLOCKER_WND",
            .hIconSm = nullptr
        };

        ATOM reg = RegisterClassExW(&wc);

        hWnd = CreateWindowExW
        (
            0L,
            L"DBCLK_BLOCKER_WND",
            L"DBCLK_BLOCKER",
            WS_POPUP | WS_VISIBLE | WS_CHILD,
            0, 0, 0, 0,
            nullptr, nullptr, GetModuleHandleW(nullptr), (void*)reg
        );

        ShowWindow(hWnd, SW_HIDE);
        UpdateWindow(hWnd);

        hMenu = CreatePopupMenu();
        AppendMenuW(hMenu, MF_STRING, WM_MENU_STATUS, L"Status");
        AppendMenuW(hMenu, MF_STRING, WM_MENU_QUIT, L"Quit");

        NOTIFYICONDATAW Icon = {
            .cbSize = sizeof Icon,
            .hWnd = hWnd,
            .uID = 1,
            .uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE | NIF_INFO,
            .uCallbackMessage = NOTIFY_MESSAGE,
            .hIcon = LoadIconW(nullptr, IDI_APPLICATION),
            .szTip = L"Double Click Blocker",
        };

        Shell_NotifyIconW(NIM_ADD, &Icon);

        for (MSG msg = {}; GetMessageW(&msg, nullptr, 0, 0);)
        {
            if (msg.message == WM_DESTROY || msg.message == WM_QUIT)
                break;
            DispatchMessageW(&msg);
        }

        Shell_NotifyIconW(NIM_DELETE, &Icon);
        DestroyMenu(hMenu);
        DestroyWindow(Icon.hWnd);
        UnregisterClassW(L"DBCLK_BLOCKER_WND", GetModuleHandleW(nullptr));
    }
}
