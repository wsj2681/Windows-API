#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
using namespace DirectX;
using namespace std;

class ColorShader
{
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11InputLayout* inputLayout = nullptr;
	ID3D11Buffer* matrixBuffer = nullptr;

	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader() noexcept;
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*) noexcept;

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);
	void RenderShader(ID3D11DeviceContext*, int) noexcept;

public:
	ColorShader() = default;
	ColorShader(const ColorShader&) = delete;
	ColorShader& operator=(const ColorShader&) = delete;
	ColorShader(ColorShader&&) = delete;
	ColorShader&& operator=(const ColorShader&&) = delete;
	~ColorShader() = default;

	bool Initialize(ID3D11Device*, HWND);
	void Destroy() noexcept;
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX);
};

//HR
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x) \
      if (FAILED((x))) \
      { \
         LPVOID errorLog = nullptr; \
         FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, \
            nullptr, (x), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), \
            reinterpret_cast<LPWSTR>(&errorLog), 0, nullptr); \
         fprintf(stderr, "%s", static_cast<char*>(errorLog)); \
         LocalFree(errorLog); \
         __debugbreak(); \
      }
#endif
#else
#ifndef HR
#define   HR(x) (x);
#endif
#endif

#define SAFE_RELEASE(ptr) {if (ptr) ptr->Release(); ptr = nullptr; }
#define SAFE_DELETE(ptr) {if (ptr) delete ptr; ptr = nullptr; }
#define SAFE_DELETEARR(ptr) {if (ptr) delete[] ptr; ptr = nullptr; }
#define CHECK_MAKE(ptr) {if (!ptr) return false;}
#define CHECK_RESULT(ptr, str) {if (!ptr) {MessageBox(hWnd, str, L"Error", MB_OK); return false;} }