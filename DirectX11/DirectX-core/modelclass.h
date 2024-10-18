#pragma once

#include <stdio.h>
#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class Model
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	ID3D11Buffer* vertexbuffer = nullptr;
	ID3D11Buffer* indexbuffer = nullptr;
	int verteccount = 0;
	int indexcount = 0;

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

public:
	Model() = default;
	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;
	Model(Model&&) = delete;
	Model&& operator=(const Model&&) = delete;
	~Model() = default;

	bool Initialize(ID3D11Device*);
	void Destroy();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
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