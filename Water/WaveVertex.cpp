#include "stdafx.h"
#include "WaveVertex.h"

#include "Utilities/Math.h"

Water::WaveVertex::WaveVertex(ExecuteValues* values, UINT width, UINT height, float thick)
	: values(values), width(width), height(height), thick(thick)
	, time(0.0f), bStop(false)
{
	UINT count = (width + 1) * (height + 1);
	prev.assign(count, WaveVertexType());
	vertices.assign(count, WaveVertexType());
	tangent.assign(count, D3DXVECTOR3());


	for (UINT y = 0; y <= height; y++)
	{
		for (UINT x = 0; x <= width; x++)
		{
			UINT w = y * (width + 1) + x;
			prev[w].position = D3DXVECTOR3(x * (float)thick, 0, y * (float)thick);
			prev[w].normal = D3DXVECTOR3(0, 1, 0);

			vertices[w] = prev[w];

			tangent[w] = D3DXVECTOR3(1, 0, 0);
		}
	}

	UINT indexCount = width * height * 6;
	indices.assign(indexCount, UINT());

	UINT index = 0;
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
	ReCalcValues();
}

Water::WaveVertex::~WaveVertex()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

void Water::WaveVertex::Update()
{
	if (bStop == true) return;

	time += Time::Delta();
	if (time < desc.TimeStep)
		return;

	time = 0.0f;

	UINT x = Math::Random(2, width - 2);
	UINT y = Math::Random(2, height - 2);
	float r = Math::Random(1.0f, 3.0f);

	Disturb(x, y, r);

	for (UINT y = 1; y <= height - 1; y++)
	{
		for (UINT x = 1; x <= width - 1; x++)
		{
			UINT w = width + 1;

			float temp = k[0] * prev[y * w + x].position.y;
			float temp2 = k[1] * vertices[y * w + x].position.y;

			float temp3 = 0.0f;
			temp3 += vertices[(y + 1) * w + x].position.y;
			temp3 += vertices[(y - 1) * w + x].position.y;
			temp3 += vertices[y * w + x + 1].position.y;
			temp3 += vertices[y * w + x - 1].position.y;
			temp3 *= k[2];

			prev[y * w + x].position.y = temp + temp2 + temp3;
		}
	}

	vector<WaveVertexType> temp;
	prev.swap(vertices);

	for (UINT y = 1; y <= height - 1; y++)
	{
		for (UINT x = 1; x <= width - 1; x++)
		{
			UINT w = width + 1;

			float l = vertices[y * w + x - 1].position.y;
			float r = vertices[y * w + x + 1].position.y;
			float t = vertices[(y - 1) * w + x].position.y;
			float b = vertices[(y + 1) * w + x].position.y;


			D3DXVECTOR3 normal;
			normal.x = -r + l;
			normal.y = 2.0f * desc.SpatialStep;
			normal.z = b - t;

			D3DXVec3Normalize(&vertices[y * w + x].normal, &normal);


			D3DXVECTOR3 vec;
			vec.x = 2.0f * desc.SpatialStep;
			vec.y = r - l;
			vec.z = 0.0f;

			D3DXVec3Normalize(&tangent[y * w + x], &vec);
		}
	}

	D3D11_MAPPED_SUBRESOURCE subResource;
	HRESULT hr = D3D::GetDC()->Map
	(
		vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy
	(
		subResource.pData, &vertices[0], sizeof(WaveVertexType) * vertices.size()
	);

	D3D::GetDC()->Unmap(vertexBuffer, 0);
}

void Water::WaveVertex::Render()
{
	UINT stride = sizeof(WaveVertexType);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D::GetDC()->DrawIndexed(indices.size(), 0, 0);
}

void Water::WaveVertex::PostRender()
{
	WaveVertexDesc temp = desc;

	if (values->GuiSettings->bShowWaterWindow == true)
	{
		GUI::Begin("Water", &values->GuiSettings->bShowWaterWindow);

		GUI::Checkbox("Stop Wave", &bStop);
		GUI::SliderFloat("SpatialStep", &temp.SpatialStep, 1.0f, 20.0f);
		GUI::SliderFloat("TimeStep", &temp.TimeStep, 0.01f, 0.1f);
		GUI::SliderFloat("Speed", &temp.Speed, 0.1f, 10.0f);
		GUI::SliderFloat("Damping", &temp.Damping, 0.05f, 1.5f);
		GUI::End();
	}

	if (temp != desc)
	{
		desc = temp;
		ReCalcValues();
	}
}

void Water::WaveVertex::CreateBuffer()
{
	//VertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DYNAMIC;
		/*
		default -> gpu 에서 read write 가능
		immutable -> read-only 한번 넣으면 묶인다
		dynamic -> gpu read , cpu write
		*/
		desc.ByteWidth = sizeof(WaveVertexType) * vertices.size();
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HRESULT hr;
		hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &vertexBuffer);
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
		data = { 0 };
		data.pSysMem = &indices[0];

		HRESULT hr;
		hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
		assert(SUCCEEDED(hr));
	}
}

void Water::WaveVertex::ReCalcValues()
{
	time = 0;

	float d = desc.Damping + 2.0f;
	float e = 0.0f;
	e += pow(desc.Speed, 2);
	e *= pow(desc.TimeStep, 2) / pow(desc.SpatialStep, 2);

	k[0] = (desc.Damping * desc.TimeStep - 2.0f) / d;
	k[1] = (4.0f - 8.0f * e) / d;
	k[2] = (2.0f * e) / d;

	//펄린 노이즈 수식으로 바꾸기
}

void Water::WaveVertex::Disturb(UINT x, UINT y, float magnitude)
{
	UINT w = width + 1;

	vertices[y * w + x].position.y += magnitude;

	float m2 = 0.5f * magnitude;
	vertices[y * w + x + 1].position.y += m2;
	vertices[y * w + x - 1].position.y += m2;
	vertices[(y + 1) * w + x].position.y += m2;
	vertices[(y - 1) * w + x].position.y += m2;

	//이부분이 셰이더로 간다
}
