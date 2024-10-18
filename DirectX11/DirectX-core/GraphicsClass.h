#pragma once
#include "D3DClass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "colorshaderclass.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
    D3DClass* d3d = nullptr;
    Camera* camera = nullptr;
    Model* model = nullptr;
    ColorShader* colorshader = nullptr;
	bool Render();

public:
	GraphicsClass() = default;
	GraphicsClass(const GraphicsClass&) = delete;
	GraphicsClass& operator=(const GraphicsClass&) = delete;
	GraphicsClass(GraphicsClass&&) = delete;
	GraphicsClass&& operator=(const GraphicsClass&&) = delete;
	~GraphicsClass() = default;

    bool Initialize(int, int, HWND);
    void Destroy() noexcept;
    bool Frame();
};

