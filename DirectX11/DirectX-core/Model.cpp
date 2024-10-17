#include "Model.h"

Model::Model()
{
}

Model::Model(const Model&)
{
}

Model::~Model()
{
}

bool Model::Initialize(ID3D11Device* dev)
{
	bool result = InitializeBuffers(dev);
	if (!result)
	{
		return false;
	}
	return true;
}

void Model::Destroy()
{
	ShutdownBuffers();
}

void Model::Render(ID3D11DeviceContext* devcon)
{
	RenderBuffers(devcon);
}

int Model::GetIndexCount()
{
	return this->indexcount;
}

bool Model::InitializeBuffers(ID3D11Device* dev)
{
	this->vertexcount = 3;
	VertexType* vertices = new VertexType[vertexcount];
	if (!vertices)
	{
		return false;
	}

	this->indexcount = 3;
	unsigned long* indices = new unsigned long[indexcount];
	if (!indices)
	{
		return false;
	}

	vertices[0].position = XMFLOAT3(-1.f, -1.f, 0.f);
	vertices[0].color = XMFLOAT4(0.f, 1.f, 0.f, 1.f);

	vertices[1].position = XMFLOAT3(0.f, 1.f, 0.f);
	vertices[1].color = XMFLOAT4(0.f, 1.f, 0.f, 1.f);

	vertices[2].position = XMFLOAT3(1.f, -1.f, 0.f);
	vertices[2].color = XMFLOAT4(0.f, 1.f, 0.f, 1.f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexcount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexdata;
	vertexdata.pSysMem = vertices;
	vertexdata.SysMemPitch = 0;
	vertexdata.SysMemSlicePitch = 0;
	HR(dev->CreateBuffer(&vertexBufferDesc, &vertexdata, &vertexbuffer));


	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexcount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	
	D3D11_SUBRESOURCE_DATA indexdata;
	indexdata.pSysMem = indices;
	indexdata.SysMemPitch = 0;
	indexdata.SysMemSlicePitch = 0;
	HR(dev->CreateBuffer(&indexBufferDesc, &indexdata, &indexbuffer));

	SAFE_DELETEARR(vertices);
	SAFE_DELETEARR(indices);

	return true;
}

void Model::ShutdownBuffers()
{
	SAFE_RELEASE(indexbuffer);
	SAFE_RELEASE(vertexbuffer);
}

void Model::RenderBuffers(ID3D11DeviceContext* devcon)
{
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	devcon->IASetVertexBuffers(0, 1, &vertexbuffer, &stride, &offset);
	devcon->IASetIndexBuffer(indexbuffer, DXGI_FORMAT_R32_UINT, 0);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}
