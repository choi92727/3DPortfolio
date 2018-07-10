#include "stdafx.h"
#include "Wave.h"
#include "WaveVertex.h"

Landscape::Wave::Wave(ExecuteValues * values)
	: values(values)
	, position(0, 2, 0)
	, bWireframe(false)
{
	material = new Material(Shaders + L"002_Wave.hlsl");
	//
	material->SetDiffuse(0x3099B8EF);
	worldBuffer = new WorldBuffer();

	{
		D3D11_RASTERIZER_DESC desc;
		States::GetRasterizerDesc(&desc);
		States::CreateRasterizer(&desc, &rasterizer[0]);

		desc.FillMode = D3D11_FILL_WIREFRAME;
		States::CreateRasterizer(&desc, &rasterizer[1]);
	}

	{
		D3D11_BLEND_DESC desc;
		States::GetBlendDesc(&desc);
		States::CreateBlend(&desc, &blend[0]);

		desc.RenderTarget[0].BlendEnable = true;
		States::CreateBlend(&desc, &blend[1]);
	}

	vertex = new WaveVertex(values, 63, 63, 4);
}

Landscape::Wave::~Wave()
{
	SAFE_DELETE(vertex);

	SAFE_RELEASE(rasterizer[0]);
	SAFE_RELEASE(rasterizer[1]);

	SAFE_RELEASE(blend[0]);
	SAFE_RELEASE(blend[1]);

	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(material);
}

void Landscape::Wave::Update()
{
	vertex->Update();


	D3DXMATRIX T;
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);

	worldBuffer->SetMatrix(T);
}

void Landscape::Wave::Render()
{
	worldBuffer->SetVSBuffer(1);
	material->SetBuffer();

	D3D::GetDC()->OMSetBlendState(blend[1], NULL, 0xFF);

	if (bWireframe == true)
		D3D::GetDC()->RSSetState(rasterizer[1]);

	vertex->Render();

	D3D::GetDC()->RSSetState(rasterizer[0]);
	D3D::GetDC()->OMSetBlendState(blend[0], NULL, 0xFF);
}

void Landscape::Wave::PostRender()
{
	if (values->GuiSettings->bShowLandscapeWindow == true)
	{
		GUI::Begin("Landscape", &values->GuiSettings->bShowLandscapeWindow);

		GUI::Checkbox("Water Wireframe", &bWireframe);
		GUI::ColorEdit4("Water Color", (float*)material->GetDiffuse());

		GUI::End();
	}

	vertex->PostRender();
}