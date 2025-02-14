// overlay.h
#pragma once
#include <Windows.h>
#include <d3d11.h>
#include "vector.h"
#include "SDK.hpp"

class Overlay {
public:
    Overlay(int width, int height, const wchar_t* windowName);
    ~Overlay();

    void Begin();
    void End();
    void DrawBox(float x, float y, float width, float height, D3DCOLOR color);
    void DrawLine(float x1, float y1, float x2, float y2, D3DCOLOR color);
    void DrawText(float x, float y, const char* text, D3DCOLOR color);
    void DrawFilledRect(float x, float y, float width, float height, D3DCOLOR color);
    void DrawHealthBar(float x, float y, float width, float height, float health, float maxHealth);

private:
    HWND overlayWindow;
    int screenWidth;
    int screenHeight;
    ID3D11Device* d3dDevice = nullptr;
    ID3D11DeviceContext* d3dDeviceContext = nullptr;
    IDXGISwapChain* swapChain = nullptr;
    ID3D11RenderTargetView* mainRenderTargetView = nullptr;

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    bool CreateDeviceD3D(HWND hWnd);
    void CleanupDeviceD3D();
    void CreateRenderTarget();
    void CleanupRenderTarget();
};