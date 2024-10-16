#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "InputClass.h"
#include "GraphicsClass.h"

class Core
{
	LPCWSTR windowsname;
	HINSTANCE hInstance;
	HWND hWnd;

	InputClass* input = nullptr;
	GraphicsClass* graphics = nullptr;

public:
	Core();
	Core(const Core&);
	~Core();

	bool Initalize();
	void Destroy();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:

	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static Core* ApplicationHandle = nullptr;