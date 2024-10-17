#pragma once
#include <stdio.h>

#include <d3d11.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <DirectXMath.h>
#include <Mmsystem.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

using namespace DirectX;

class D3DClass
{
	bool vsync_enabled = false;
	int GPUMemory = 0;
	char GPUdesc[128];

    IDXGISwapChain* swapchain = nullptr;
    ID3D11Device* dev = nullptr;
    ID3D11DeviceContext* devcon = nullptr;
    ID3D11RenderTargetView* renderTargetView = nullptr;
    ID3D11Texture2D* depthStencilBuffer = nullptr;
    ID3D11DepthStencilState* depthStencilState = nullptr;
    ID3D11DepthStencilView* depthStencilView = nullptr;
    ID3D11RasterizerState* rasterState = nullptr;
    XMMATRIX projection;
    XMMATRIX world;
    XMMATRIX ortho;

public:

    D3DClass();
    D3DClass(const D3DClass&);
    ~D3DClass();

    bool Initialize(int, int, bool, HWND, bool, float, float);
    void Destroy();

    void BeginScene(float, float, float, float);
    void EndScene();

    ID3D11Device* getDevice();
    ID3D11DeviceContext* getDeviceContext();

    void GetProjectionMatrix(XMMATRIX&);
    void GetWorldMatrix(XMMATRIX&);
    void GetOrthoMatrix(XMMATRIX&);

    void GetGPUInfo(char*, int&);
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