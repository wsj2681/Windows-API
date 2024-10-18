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
	Core() = default;
	Core(const Core&) = delete;
	Core& operator=(const Core&) = delete;
	Core(Core&&) = delete;
	Core&& operator=(const Core&&) = delete;
	~Core() = default;

	bool Initalize();
	void Destroy() noexcept;
	void Run() noexcept;

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:

	bool Frame();
	void InitializeWindows(int& screenWidth, int& screenHeight) noexcept;
	void ShutdownWindows() noexcept;

};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static Core* ApplicationHandle = nullptr;

