#include "ColorShader.h"

ColorShader::ColorShader()
{
}

ColorShader::ColorShader(const ColorShader&)
{
}

ColorShader::~ColorShader()
{
}

bool ColorShader::Initialize(ID3D11Device* dev, HWND hWnd)
{
	bool result = this->InitializeShader(dev, hWnd, L"Color.vs", L"Color.ps");

	if (!result)
	{
		return false;
	}
	return true;
}

void ColorShader::Destroy()
{
	ShutdoenShader();
}

bool ColorShader::Render(ID3D11DeviceContext* devcon, int indexcount, XMMATRIX world, XMMATRIX view, XMMATRIX projection)
{
	bool result = SetShaderParameters(devcon, world, view, projection);
	if (!result)
	{
		return false;
	}
	RenderShader(devcon, indexcount);

	return true;
}

bool ColorShader::InitializeShader(ID3D11Device* dev, HWND hWnd, const WCHAR* vsfilename, const WCHAR* psfilename)
{
	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;

	HR(D3DCompileFromFile(vsfilename, nullptr, nullptr, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage));
	HR(D3DCompileFromFile(psfilename, nullptr, nullptr, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage));

	HR(dev->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &vertexshader));
	HR(dev->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &pixelshader));

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
	HR(dev->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &inputlayout));

	SAFE_RELEASE(vertexShaderBuffer);
	SAFE_RELEASE(pixelShaderBuffer);

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	HR(dev->CreateBuffer(&matrixBufferDesc, nullptr, &matrixbuffer));

	return true;
}

void ColorShader::ShutdownShader()
{
	SAFE_RELEASE(matrixbuffer);
	SAFE_RELEASE(inputlayout);
	SAFE_RELEASE(pixelshader);
	SAFE_RELEASE(vertexshader);
}

void ColorShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hWnd, WCHAR* shaderFilename)
{
	char* compileErrors = nullptr;
	
	ofstream of;

	compileErrors = (char*)(errorMessage->GetBufferPointer());
	unsigned int bufferSize = errorMessage->GetBufferSize();
	of.open("shader-error.txt");
	for (int i = 0; i < bufferSize; ++i)
	{
		of << compileErrors[i];
	}
	of.close();

	SAFE_RELEASE(errorMessage);
	MessageBox(hWnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);


}

bool ColorShader::SetShaderParameters(ID3D11DeviceContext* devcon, XMMATRIX world, XMMATRIX view, XMMATRIX projection)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;


	// 행렬을 transpose하여 셰이더에서 사용할 수 있게 합니다.
	XMMatrixTranspose(world);
	XMMatrixTranspose(view);
	XMMatrixTranspose(projection);

	// 상수 버퍼의 내용을 쓸 수 있도록 잠급니다.
	HR(devcon->Map(matrixbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = projection;
	devcon->Unmap(matrixbuffer, 0);

	bufferNumber = 0;

	devcon->VSSetConstantBuffers(bufferNumber, 1, &matrixbuffer);

	return true;
}

void ColorShader::RenderShader(ID3D11DeviceContext* devcon, int indexCount)
{
	devcon->IASetInputLayout(inputlayout);

	devcon->VSSetShader(vertexshader, nullptr, 0);
	devcon->PSSetShader(pixelshader, nullptr, 0);

	devcon->DrawIndexed(indexCount, 0, 0);
}
