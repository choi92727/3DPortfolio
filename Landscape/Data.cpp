#include "stdafx.h"
#include "Data.h"

Landscape::Data::Data(wstring file, float heightRatio)
	: width(0), height(0)
	, vertexBuffer(NULL), indexBuffer(NULL)
	, vertices(NULL), indices(NULL)
	, heightMap(NULL)
{
	SetHeightMapFile(file);
}

Landscape::Data::~Data()
{
	Clear();
}

float Landscape::Data::GetVertexHeight(D3DXVECTOR3 position)
{
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	if (x < 0 || x >= width)
		return 0.0f;

	if (z < 0 || z >= height)
		return 0.0f;

	UINT index0 = (width + 1) * z + x;
	UINT index1 = (width + 1) * (z + 1) + x;
	UINT index2 = (width + 1) * z + x + 1;
	UINT index3 = (width + 1) * (z + 1) + (x + 1);

	D3DXVECTOR3 v0 = vertices[index0].position;
	D3DXVECTOR3 v1 = vertices[index1].position;
	D3DXVECTOR3 v2 = vertices[index2].position;
	D3DXVECTOR3 v3 = vertices[index3].position;

	float deltaX = position.x - v0.x;
	float deltaZ = position.z - v0.z;

	D3DXVECTOR3 temp;
	if (deltaX + deltaZ <= 1)
		temp = v0 + (v2 - v0) * deltaX + (v1 - v0) * deltaZ;
	else
	{
		deltaX = 1 - deltaX;
		deltaZ = 1 - deltaZ;

		temp = v3 + (v2 - v3) * deltaX + (v1 - v3) * deltaZ;
	}

	return temp.y;
}

void Landscape::Data::SetHeightMapFile(wstring file, float heightRatio)
{
	if (file.length() < 1) 
		return;
	
	Clear();

	heightMap = new Texture(file);

	FillVertices(heightRatio);
	FillNormalData();

	CreateBuffer();
}

void Landscape::Data::UpdateVertexData(D3D11_BOX * box)
{
	FillNormalData();

	D3D::GetDC()->UpdateSubresource
	(
		vertexBuffer, 0, box, &vertices[0], sizeof(TerrainVertexType) * vertices.size(), 0
	);
}

void Landscape::Data::Render()
{
	UINT stride = sizeof(TerrainVertexType);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D::GetDC()->DrawIndexed(indices.size(), 0, 0);
}

void Landscape::Data::Clear()
{
	vertices.clear();
	indices.clear();

	SAFE_DELETE(heightMap);

	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

void Landscape::Data::FillVertices(float heightRatio)
{
	vector<D3DXCOLOR> pixels;
	D3D11_TEXTURE2D_DESC desc = heightMap->ReadPixels(DXGI_FORMAT_R8G8B8A8_UNORM, &pixels);

	width = desc.Width ;
	height = desc.Height ;

	UINT vertexCount = (width + 1) * (height + 1);
	vertices.assign(vertexCount, TerrainVertexType());


	UINT index = 0;
	UINT pixelIndex = 0;
	for (UINT z = 0; z <= height; z++)
	{
		for (UINT x = 0; x <= width; x++)
		{
			
			pixelIndex = x + z*width;
			
			vertices[index].position.x = (float)x;
			
			if (pixelIndex < width*height - 1)
				vertices[index].position.y = (float)(pixels[pixelIndex].r * 255.0f) / heightRatio;
			else if(index == vertexCount -1)
				vertices[index].position.y = (float)(pixels[pixelIndex-width-1].r * 255.0f) / heightRatio;
			else if (index > pixels.size())					
				vertices[index].position.y = (float)(pixels[pixelIndex - width].r * 255.0f) / heightRatio;
			
			vertices[index].position.z = (float)z;

			vertices[index].uv.x = min( 1.f,x / (float)(width));
			vertices[index].uv.y = min(1.f, z / (float)(height));

			vertices[index].normal = D3DXVECTOR3(0, 0, 0); 
			index++;   		

		}//for(x)
	}//for(y)	


	UINT indexCount = (width) * (height) * 6;
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
	
}

void Landscape::Data::FillNormalData()
{
	for (UINT i = 0; i < (indices.size() / 3); i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		TerrainVertexType v0 = vertices[index0];
		TerrainVertexType v1 = vertices[index1];
		TerrainVertexType v2 = vertices[index2];

		D3DXVECTOR3 d1 = v1.position - v0.position;
		D3DXVECTOR3 d2 = v2.position - v0.position;

		D3DXVECTOR3 normal;
		D3DXVec3Cross(&normal, &d1, &d2);

		vertices[index0].normal += normal;
		vertices[index1].normal += normal;
		vertices[index2].normal += normal;
	}

	for (UINT i = 0; i < vertices.size(); i++)
		D3DXVec3Normalize(&vertices[i].normal, &vertices[i].normal);
}

void Landscape::Data::CreateBuffer()
{
	//VertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(TerrainVertexType) * vertices.size();
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
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * indices.size();
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data = { 0 };
		data.pSysMem = &indices[0];

		HRESULT hr;
		hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
		assert(SUCCEEDED(hr));
	}
}
