#include "stdafx.h"
#include "MeshPart.h"
#include "FbxData.h"

Model::MeshPart::MeshPart()
{

}

Model::MeshPart::MeshPart(Fbx::MeshData & data)
{
	D3DXMatrixIdentity(&world);

	name = data.Name;
	world = data.World;

	vertexData.assign(data.Vertices.begin(), data.Vertices.end());
	indexData.assign(data.Indices.begin(), data.Indices.end());

	worldBuffer = new WorldBuffer();

	CreateBuffer();
}

Model::MeshPart::~MeshPart()
{
	SAFE_DELETE(material);
	SAFE_DELETE(worldBuffer);
	
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

void Model::MeshPart::Clone(void ** clone)
{
	MeshPart* meshPart = new MeshPart();

	meshPart->name = this->name;
	this->material->Clone((void **)&meshPart->material);

	meshPart->world = this->world;
	meshPart->worldBuffer = new WorldBuffer();

	meshPart->vertexData.assign(this->vertexData.begin(), this->vertexData.end());
	meshPart->indexData.assign(this->indexData.begin(), this->indexData.end());

	meshPart->CreateBuffer();

	*clone = meshPart;
}

void Model::MeshPart::Update()
{
	worldBuffer->SetMatrix(world);
}

void Model::MeshPart::Render()
{
	UINT stride = sizeof(VertexTextureNormal);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	worldBuffer->SetVSBuffer(1);
	material->SetBuffer();

	D3D::GetDC()->DrawIndexed(indexData.size(), 0, 0);;
}

void Model::MeshPart::CreateBuffer()
{
	UINT vertexCount = vertexData.size();
	UINT indexCount = indexData.size();


	HRESULT hr;
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;

	//1. Vertex Buffer
	{
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexTextureNormal) * vertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = &vertexData[0];

		hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	//2. Index Buffer
	{
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * indexCount;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = &indexData[0];

		hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
		assert(SUCCEEDED(hr));
	}
}