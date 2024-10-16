#pragma once
class GraphicsClass
{
	bool Render();

public:
    GraphicsClass();
    GraphicsClass(const GraphicsClass&);
    ~GraphicsClass();

    bool Initialize(int, int, HWND);
    void Destroy();
    bool Frame();
};

