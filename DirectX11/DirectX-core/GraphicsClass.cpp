#include "GraphicsClass.h"

bool GraphicsClass::Render()
{
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

    d3d->BeginScene(0.8f, 0.8f, 0.f, 1.f);
    camera->Render();
    d3d->GetWorldMatrix(worldMatrix);
    camera->GetViewMatrix(viewMatrix);
    d3d->GetProjectionMatrix(projectionMatrix);
    
    model->Render(d3d->getDeviceContext());

    bool result = colorshader->Render(d3d->getDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
    if (!result)
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

    camera = new CameraClass;
    if (!camera)
    {
        return false;
    }
    camera->SetPosition(0.f, 0.f, -10.f);

    model = new ModelClass;
    if (!model)
    {
        return false;
    }
    result = model->Initialize(d3d->getDevice());
    if (!result)
    {
        MessageBox(hWnd, L"Could not initialize the model object.", L"Error", MB_OK);
        return false;
    }

    colorshader = new ColorShaderClass;
    result = colorshader->Initialize(d3d->getDevice(), hWnd);
    if (!result)
    {
        MessageBox(hWnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
        return false;
    }

    return true;
}

void GraphicsClass::Destroy()
{
    if (colorshader)
    {
        colorshader->Shutdown();
        delete colorshader;
        colorshader = 0;
    }

    if (model)
    {
        model->Shutdown();
        delete model;
        model = 0;
    }

    if (camera)
    {
        delete camera;
        camera = 0;
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
