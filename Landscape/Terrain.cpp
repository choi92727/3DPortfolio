#include "stdafx.h"
#include "Terrain.h"
#include "Data.h"

#include "Fog.h"
#include "QuadTree.h"
#include "Frustum.h"

Landscape::Terrain::Terrain(ExecuteValues *  values)
	:values(values)
{
	material = new Material(Shaders + L"Tesselation.hlsl");
	material->SetDiffuseMap(Contents + L"Landscape/Dirt3.png");
	material->SetNormalMap(Contents + L"Landscape/Dirt3_normal.png");
	material->SetDetailMap(Contents + L"Landscape/Detail.png");

	worldBuffer = new WorldBuffer();

	gridBuffer = new GridBuffer();
	tessAmountBuffer = new TessAmountBuffer();


	data = new Data(Contents + L"Landscape/HeightMap256.png");
	fog = new Fog(values);
	frustum = new Frustum();

	quadTree = new QuadTree(data, values);

	D3D11_SAMPLER_DESC samplerDesc;
	States::GetSamplerDesc(&samplerDesc);
	States::CreateSampler(&samplerDesc, &diffuseSampler);

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	States::CreateSampler(&samplerDesc, &detailSampler);

	{
		D3D11_RASTERIZER_DESC desc;
		States::GetRasterizerDesc(&desc);
		States::CreateRasterizer(&desc, &rasterizer[0]);

		desc.FillMode = D3D11_FILL_WIREFRAME;
		States::CreateRasterizer(&desc, &rasterizer[1]);

	}


}

Landscape::Terrain::~Terrain()
{
	SAFE_DELETE(gridBuffer);
	SAFE_DELETE(tessAmountBuffer);

	SAFE_DELETE(data);
	SAFE_DELETE(fog);
	SAFE_DELETE(frustum);
	SAFE_DELETE(quadTree);

	SAFE_RELEASE(diffuseSampler);
	SAFE_RELEASE(detailSampler);

	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(material);
}

void Landscape::Terrain::Update()
{
	//quadTree->Update();
	D3DXMATRIX view, projection;
	values->MainCamera->GetMatrix(&view);
	values->Perspective->GetMatrix(&projection);
	frustum->ConstructFrustum(1000.f, projection, view);
}

void Landscape::Terrain::Render()
{
	worldBuffer->SetDSBuffer(1);
	worldBuffer->SetHSBuffer(1);
	gridBuffer->SetPSBuffer(3);
	tessAmountBuffer->SetHSBuffer(10);

	material->SetBuffer();

	//fog->Render();
	//data->Render();
	D3D::GetDC()->RSSetState(rasterizer[1]);
	quadTree->Render(frustum);
	D3D::GetDC()->RSSetState(rasterizer[0]);
	D3D::GetDC()->HSSetShader(NULL, NULL, 0);
	D3D::GetDC()->DSSetShader(NULL, NULL, 0);


}

void Landscape::Terrain::PostRender()
{
	fog->PostRender();
	quadTree->PostRender();
	ImGui::Begin("quadTree");
	{
		if (ImGui::TreeNode("Tess Amount"))
		{
			ImGui::InputFloat("Max TessAmount", &(tessAmountBuffer->Data.maxTess));
			ImGui::InputFloat("Distance", &(tessAmountBuffer->Data.distance));
			ImGui::TreePop();
		}
	}
	ImGui::End();



}
