#include "GraphicsClass.h"

bool GraphicsClass::Render()
{
    XMMATRIX world, view, projection;

    d3d->BeginScene(0.6f, 0.3f, 0.6f, 1.f);

    camera->Render();

    camera->GetViewMatrix(view);
    d3d->GetWorldMatrix(world);
    d3d->GetProjectionMatrix(projection);

    model->Render(d3d->getDeviceContext());

    if(!colorshader->Render(d3d->getDeviceContext(), model->GetIndexCount(), world, view, projection))
    {
        return false;
    }

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

    camera = new Camera;
    if (!camera)
    {
        return false;
    }
    camera->SetPosition(0.f, 0.f, -10.f);

    model = new Model;
    if (!model)
    {
        return false;
    }
    result = model->Initialize(d3d->getDevice());
    if (!result)
    {
        MessageBox(hWnd, L"model not initialize", L"Error", MB_OK);
        return false;
    }

    colorshader = new ColorShader;
    colorshader->Initialize(d3d->getDevice(), hWnd);
    if (!colorshader)
    {
        MessageBox(hWnd, L"colorshader not initialize", L"Error", MB_OK);
        return false;
    }

    return true;
}

void GraphicsClass::Destroy()
{
    if (colorshader)
    {
        colorshader->Destroy();
        delete colorshader;
        colorshader = nullptr;
    }

    if (model)
    {
        model->Destroy();
        delete model;
        model = nullptr;
    }

    if (camera)
    {
        delete camera;
        camera = nullptr;
    }

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
