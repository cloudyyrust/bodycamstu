// overlay.cpp
#include "overlay.h"
#include <dwmapi.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "d3dcompiler.lib")

Overlay::Overlay(int width, int height, const wchar_t* windowName) {
    screenWidth = width;
    screenHeight = height;

    WNDCLASSEXW wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = L"OverlayClass";
    RegisterClassExW(&wc);

    overlayWindow = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
        L"OverlayClass",
        windowName,
        WS_POPUP,
        0, 0, width, height,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    );

    SetLayeredWindowAttributes(overlayWindow, RGB(0, 0, 0), 255, LWA_ALPHA);

    MARGINS margins = { -1, -1, -1, -1 };
    ::DwmExtendFrameIntoClientArea(overlayWindow, &margins);

    if (!CreateDeviceD3D(overlayWindow)) {
        CleanupDeviceD3D();
        UnregisterClassW(L"OverlayClass", nullptr);
        throw std::runtime_error("Failed to create D3D device");
    }

    ShowWindow(overlayWindow, SW_SHOW);
    UpdateWindow(overlayWindow);
}