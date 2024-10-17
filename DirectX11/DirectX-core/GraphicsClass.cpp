#include "GraphicsClass.h"

bool GraphicsClass::Render()
{
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

bool GraphicsClass::Initialize(int, int, HWND)
{
    return true;
}

void GraphicsClass::Destroy()
{
}

bool GraphicsClass::Frame()
{
    return true;
}
