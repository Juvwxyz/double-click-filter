module;

#define WIN32_LEAN_AND_MEAN
#define NOMAXMIN
#include <Windows.h>

#ifdef __EDG__
#include <chrono>
#include <print>
#endif // __EDG__

export module MouseHook;

import std;

namespace LowLevelMouseHook
{

    using namespace std::chrono_literals;
    using std::chrono::milliseconds;

    export size_t count = 0;

    export struct AutoHooker
    {
        using clock = std::chrono::high_resolution_clock;
        using time = clock::time_point;
        using dura = clock::duration;

        static time last;
        static bool down;
        static milliseconds threshold;
        HHOOK hHook;

        static LRESULT CALLBACK MouseHookProc(int nCode, WPARAM w, LPARAM l)
        {
            if (nCode < 0)
                return CallNextHookEx(nullptr, nCode, w, l);
            if (w != WM_LBUTTONDOWN && w != WM_LBUTTONUP)
                return CallNextHookEx(nullptr, nCode, w, l);

            auto now = clock::now();
            auto delta = now - last;

            if (w == WM_LBUTTONDOWN && delta < threshold) {
                count++;
                down = true;
                return 1;
            }
            if (down && w == WM_LBUTTONUP) {
                down = false;
                return 1;
            }

            last = now;

            return CallNextHookEx(nullptr, nCode, w, l);
        }

        AutoHooker()
        {
            hHook = SetWindowsHookExW(WH_MOUSE_LL, MouseHookProc, nullptr, 0);
        }
        ~AutoHooker()
        {
            UnhookWindowsHookEx(hHook);
        }
    } hooker = {};

    AutoHooker::time AutoHooker::last = AutoHooker::clock::now();
    bool AutoHooker::down = false;
    milliseconds AutoHooker::threshold = 25ms;
};

