////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"

bool Model::Initialize(ID3D11Device* device)
{
	// Initialize the vertex and index buffers.
	CHECK_MAKE(InitializeBuffers(device));

	return true;
}


void Model::Destroy()
{
	// Destroy the vertex and index buffers.
	ShutdownBuffers();
}


void Model::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);
}


int Model::GetIndexCount()
{
	return indexcount;
}


bool Model::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;

	verteccount = 3;
	vertices = new VertexType[verteccount];
	CHECK_MAKE(vertices);


	indexcount = 3;
	indices = new unsigned long[indexcount];
	CHECK_MAKE(indices);

	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * verteccount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

    HR(device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexbuffer));

    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexcount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	HR(device->CreateBuffer(&indexBufferDesc, &indexData, &indexbuffer));

	SAFE_DELETEARR(vertices);
	SAFE_DELETEARR(indices);

	return true;
}


void Model::ShutdownBuffers()
{
	SAFE_RELEASE(indexbuffer);
	SAFE_RELEASE(vertexbuffer);
}


void Model::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;
	

	deviceContext->IASetVertexBuffers(0, 1, &vertexbuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(indexbuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}