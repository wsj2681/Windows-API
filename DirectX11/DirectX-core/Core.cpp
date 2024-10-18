#include "Core.h"

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

	if (!graphics->Initialize(screenWidth, screenHeight, hWnd))
	{
		return false;
	}

	return true;
}

void Core::Destroy() noexcept
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

void Core::Run() noexcept
{
	MSG msg;
	bool done = false;
	bool result = false;

	while (!done)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
}

LRESULT Core::MessageHandler(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
	{
		input->KeyDown((unsigned int)wparam);
		break;
	}
	case WM_KEYUP:
	{
		input->KeyUp((unsigned int)wparam);
		break;
	}
	default:
	{
		return DefWindowProc(hWnd, msg, wparam, lparam);
		break;
	}
	}

	return 0;
}

bool Core::Frame()
{
	if (input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	if (!graphics->Frame())
	{
		return false;
	}

	return true;
}

void Core::InitializeWindows(int& width, int& height) noexcept
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int x, y;

	ApplicationHandle = this;
	hInstance = GetModuleHandleW(NULL);
	windowsname = L"Core";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = windowsname;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)width;
		dmScreenSettings.dmPelsHeight = (unsigned long)height;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		x = y = 0;
	}
	else
	{
		width = 800;
		height = 600;

		x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}

	hWnd = CreateWindowEx(
		WS_EX_APPWINDOW, 
		windowsname, 
		windowsname,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		x, 
		y, 
		width, 
		height, 
		NULL, 
		NULL, 
		hInstance, 
		NULL);
	
	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	ShowCursor(true);
}

void Core::ShutdownWindows() noexcept
{
	ShowCursor(true);

	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(nullptr, 0);
	}

	DestroyWindow(hWnd);
	hWnd = nullptr;

	UnregisterClassW(windowsname, hInstance);
	hInstance = nullptr;

	ApplicationHandle = nullptr;
}

LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		break;
	}
	default:
	{
		return ApplicationHandle->MessageHandler(hWnd, msg, wparam, lparam);
		break;
	}
	}
	return 0;
}
