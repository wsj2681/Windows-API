#include "D3DClass.h"

bool D3DClass::Initialize(int screenwidth, int screenheight, bool vsync, HWND hWnd, bool fullscreen, float screenDepth, float screenNear)
{
	IDXGIFactory* factory = nullptr;
	IDXGIAdapter* adapter = nullptr;
	IDXGIOutput* adapteroutput = nullptr;
	unsigned int numModes = 0;
	DXGI_MODE_DESC* displayModeList = nullptr;


	this->vsync_enabled = vsync;

	HR(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory));
	HR(factory->EnumAdapters(0, &adapter));
	HR(adapter->EnumOutputs(0, &adapteroutput));
	HR(adapteroutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL));
	
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	HR(adapteroutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList));


	unsigned int numerator = 0, denominator = 0;

	for (unsigned int i = 0; i < numModes; ++i)
	{
		if (displayModeList[i].Width == (unsigned int)screenwidth && displayModeList[i].Height == (unsigned int)screenheight)
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
		}
	}

	DXGI_ADAPTER_DESC adapterDesc;
	HR(adapter->GetDesc(&adapterDesc));
	this->GPUMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	
	size_t stringLength;
	int error = wcstombs_s(&stringLength, this->GPUdesc, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	SAFE_DELETEARR(displayModeList);

	SAFE_RELEASE(adapteroutput);
	SAFE_RELEASE(adapter);
	SAFE_RELEASE(factory);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width = screenwidth;
	swapChainDesc.BufferDesc.Height = screenheight;

	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;

	HR(D3D11CreateDeviceAndSwapChain(
		nullptr, 
		D3D_DRIVER_TYPE_HARDWARE, 
		NULL, 
		0, 
		&featureLevel, 
		1, 
		D3D11_SDK_VERSION, 
		&swapChainDesc, 
		&swapchain, 
		&dev, 
		nullptr, 
		&devcon));


	ID3D11Texture2D* backBufferPtr = nullptr;
	HR(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr));
	HR(dev->CreateRenderTargetView(backBufferPtr, nullptr, &renderTargetView));

	backBufferPtr->Release();
	backBufferPtr = nullptr;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthBufferDesc.Width = screenwidth;
	depthBufferDesc.Height = screenheight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	HR(dev->CreateTexture2D(&depthBufferDesc, nullptr, &depthStencilBuffer));
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	HR(dev->CreateDepthStencilState(&depthStencilDesc, &depthStencilState));
	devcon->OMSetDepthStencilState(depthStencilState, 1);

	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	HR(dev->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView));

	devcon->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	HR(dev->CreateRasterizerState(&rasterDesc, &rasterState));
	devcon->RSSetState(this->rasterState);

	D3D11_VIEWPORT viewport;
	viewport.Width = (float)screenwidth;
	viewport.Height = (float)screenheight;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;

	devcon->RSSetViewports(1, &viewport);

	float fieldOfView = (float)XM_PI / 4.f;
	float screenAspect = (float)screenwidth / (float)screenheight;

	this->projection = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
	this->world = XMMatrixIdentity();

	XMMatrixOrthographicLH((float)screenwidth, (float)screenheight, screenNear, screenDepth);

	return true;
}

void D3DClass::Destroy()
{
	if (swapchain)
	{
		swapchain->SetFullscreenState(false, nullptr);
	}

	SAFE_RELEASE(rasterState);
	SAFE_RELEASE(depthStencilView);
	SAFE_RELEASE(depthStencilState);
	SAFE_RELEASE(depthStencilBuffer);
	SAFE_RELEASE(renderTargetView);
	SAFE_RELEASE(devcon);
	SAFE_RELEASE(dev);
	SAFE_RELEASE(swapchain);
}

void D3DClass::BeginScene(float r, float g, float b, float a)
{
	float color[4] = {r, g, b, a};
	devcon->ClearRenderTargetView(renderTargetView, color);
	devcon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);

}

void D3DClass::EndScene()
{
	if (vsync_enabled)
	{
		swapchain->Present(1, 0);
	}
	else
	{
		swapchain->Present(0, 0);
	}
}

ID3D11Device* D3DClass::getDevice()
{
	return this->dev;
}

ID3D11DeviceContext* D3DClass::getDeviceContext()
{
	return this->devcon;
}

void D3DClass::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = this->projection;
}

void D3DClass::GetWorldMatrix(XMMATRIX& worldMatrix)
{
	worldMatrix = this->world;
}

void D3DClass::GetOrthoMatrix(XMMATRIX& orthoMatrix)
{
	orthoMatrix = this->ortho;
}

void D3DClass::GetGPUInfo(char* gpuname, int& memory)
{
	strcpy_s(gpuname, 128, GPUdesc);
	memory = GPUMemory;
}
