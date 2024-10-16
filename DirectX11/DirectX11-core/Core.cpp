#include "Core.h"

Core::Core()
{
	
}

Core::Core(const Core&)
{
}

Core::~Core()
{
}

bool Core::Initalize()
{
	int screenWidth = 0;
	int screenHeight = 0;

	bool result = false;

	InitializeWindows(screenWidth, screenHeight);

	input = new InputClass;
	if (!input)
	{
		return false;
	}
	input->Initialize();

	graphics = new GraphicsClass;
	if (!graphics)
	{
		return false;
	}
	result = graphics->Initialize(screenWidth, screenHeight, hWnd);

	if (!result)
	{
		return false;
	}

	return true;
}

void Core::Destroy()
{
	if (graphics)
	{
		graphics->Destroy();
		delete graphics;
		graphics = nullptr;
	}

	if (input)
	{
		delete input;
		input = nullptr;
	}

	ShutdownWindows();
}

void Core::Run()
{

}

LRESULT Core::MessageHandler(HWND, UINT, WPARAM, LPARAM)
{
	return LRESULT();
}

bool Core::Frame()
{
	return false;
}

void Core::InitializeWindows(int& width, int& height)
{
}

void Core::ShutdownWindows()
{
}
