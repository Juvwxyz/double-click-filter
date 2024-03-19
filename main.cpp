
#include <chrono>
#include <print>
#include <thread>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>

import MouseHook;
import NotifyIcon;

#ifdef _DEBUG
int main()
#else
int WINAPI wWinMain(
    _In_ HINSTANCE hInst,
    _In_opt_ HINSTANCE,
    _In_ LPWSTR pArgs,
    _In_ int
)
#endif // _DEBUG
{

    std::thread t{ NotifyArea::Start };

    for (MSG msg{}; GetMessageW(&msg, nullptr, 0, 0);) {
        DispatchMessageW(&msg);
    }

    t.join();

    return 0;
}
