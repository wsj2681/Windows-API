#include "GraphicsClass.h"

bool GraphicsClass::Render()
{
    d3d->BeginScene(0.8f, 0.8f, 0.f, 1.f);

    d3d->EndScene();
    return true;
}

GraphicsClass::GraphicsClass()
{
}

GraphicsClass::GraphicsClass(const GraphicsClass&)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenwidth, int screenheight, HWND hWnd)
{
    d3d = new D3DClass;
    bool result = d3d->Initialize(screenwidth, screenheight, VSYNC_ENABLED, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
    if (!result)
    {
        MessageBox(hWnd, L"d3d not initialize", L"Error", MB_OK);
        return false;
    }


    return true;
}

void GraphicsClass::Destroy()
{
    if (d3d)
    {
        d3d->Destroy();
        delete d3d;
        d3d = nullptr;
    }
    
}

bool GraphicsClass::Frame()
{
    if (!Render())
    {
        return false;
    }
    return true;
}
