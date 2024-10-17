#pragma once

#include <stdio.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <fstream>

using namespace std;
using namespace DirectX;

class ColorShader
{
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	ID3D11VertexShader* vertexshader = nullptr;
	ID3D11PixelShader* pixelshader = nullptr;
	ID3D11InputLayout* inputlayout = nullptr;
	ID3D11Buffer* matrixbuffer = nullptr;

public:
	ColorShader();
	ColorShader(const ColorShader&);
	~ColorShader();

	bool Initialize(ID3D11Device* dev, HWND hWnd);
	void Destroy();
	bool Render(ID3D11DeviceContext* devcon, int, XMMATRIX world, XMMATRIX view, XMMATRIX projection);

private:
	bool InitializeShader(ID3D11Device* dev, HWND hWnd, const WCHAR*, const WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND hWnd, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext* devcon, XMMATRIX, XMMATRIX, XMMATRIX);
	void RenderShader(ID3D11DeviceContext* devcon, int);

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