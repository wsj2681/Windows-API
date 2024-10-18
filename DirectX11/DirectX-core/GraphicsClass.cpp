#include "GraphicsClass.h"

bool GraphicsClass::Initialize(int screenwidth, int screenheight, HWND hWnd)
{
    d3d = new D3DClass;
    CHECK_RESULT(d3d->Initialize(screenwidth, screenheight, VSYNC_ENABLED, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR), L"d3d not initialize");

    camera = new Camera;
    CHECK_MAKE(camera);
    camera->SetPosition(0.f, 0.f, -10.f);

    model = new Model;
    CHECK_MAKE(model);
    CHECK_RESULT(model->Initialize(d3d->getDevice()), L"Could not initialize the model object.")
 
    colorshader = new ColorShader;
    CHECK_RESULT(colorshader->Initialize(d3d->getDevice(), hWnd), L"Could not initialize the color shader object.");

    return true;
}

void GraphicsClass::Destroy() noexcept
{
    if (colorshader)
    {
        colorshader->Destroy();
        delete colorshader;
        colorshader = 0;
    }

    if (model)
    {
        model->Destroy();
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

bool GraphicsClass::Render()
{
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

    d3d->BeginScene(0.8f, 0.8f, 0.f, 1.f);

    camera->Render();

    d3d->GetWorldMatrix(worldMatrix);
    camera->GetViewMatrix(viewMatrix);
    d3d->GetProjectionMatrix(projectionMatrix);
    
    model->Render(d3d->getDeviceContext());

    if (!colorshader->Render(d3d->getDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix))
    {
        return false;
    }

    d3d->EndScene();
    return true;
}
