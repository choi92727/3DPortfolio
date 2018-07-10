#include "stdafx.h"
#include "OceanVertex.h"

#include "Utilities/Math.h"

Water::OceanVertex::OceanVertex(ExecuteValues* values, UINT width, UINT height, float thick)
	: values(values), width(width), height(height), thick(thick)
	, bStop(false)
{
	UINT count = (width + 1) * (height + 1);
	vertices.assign(count, OceanVertexType());

	UINT index = 0;
	for (UINT z = 0; z <= height; z++)
	{
		for (UINT x = 0; x <= width; x++)
		{
			vertices[index].position.x = (float)x * thick;
			vertices[index].position.y = 0.0f;
			vertices[index].position.z = (float)z * thick;

			vertices[index].uv.x = (float)x / (float)width;
			vertices[index].uv.y = (float)z / (float)height;
			index++;
		}
	}

	UINT indexCount = width * height * 6;
	indices.assign(indexCount, UINT());

	index = 0;
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			indices[index + 0] = (width + 1) * z + x;
			indices[index + 1] = (width + 1) * (z + 1) + x;
			indices[index + 2] = (width + 1) * z + x + 1;
			indices[index + 3] = (width + 1) * z + x + 1;
			indices[index + 4] = (width + 1) * (z + 1) + x;
			indices[index + 5] = (width + 1) * (z + 1) + (x + 1);

			index += 6;
		}
	}

	CreateBuffer();
}

Water::OceanVertex::~OceanVertex()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

void Water::OceanVertex::Update()
{
}

void Water::OceanVertex::Render()
{
	UINT stride = sizeof(OceanVertexType);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	D3D::GetDC()->DrawIndexed(indices.size(), 0, 0);
}

void Water::OceanVertex::PostRender()
{
	
}

void Water::OceanVertex::CreateBuffer()
{
	//VertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.ByteWidth = sizeof(OceanVertexType) * vertices.size();
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = &vertices[0];

		HRESULT hr;
		hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	//IndexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc = { 0 };
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.ByteWidth = sizeof(UINT) * indices.size();
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = &indices[0];

		HRESULT hr;
		hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
		assert(SUCCEEDED(hr));
	}
}
