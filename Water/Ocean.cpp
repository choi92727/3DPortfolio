#include "stdafx.h"
#include "Ocean.h"
#include "OceanVertex.h"

#include "../Model/MeshPart.h"

Water::Ocean::Ocean(ExecuteValues * values)
	: values(values)
	, position(0, 2, 0)
	, bWireframe(false)
	, timeMultiflier(1.0f)
	, vertexSize(128, 128)
{
	vertex = new OceanVertex(values, 128, 128, 2);

	material = new Material(Shaders + L"003_Ocean.hlsl");
	material->SetNormalMap(Contents + L"Water/WaveNormal.png");
	material->SetDiffuse(0xA099B8EF);
	material->SetShininess(500);

	worldBuffer = new WorldBuffer();

	psBuffer = new OceanPSBuffer();
	vsBuffer = new OceanVSBuffer();

	/*Fbx::FbxModels::ReadBinary(BinModels + L"Meshes/Sphere.model", &test);
	Material* materil = test->GetMeshPart(0)->GetMaterial();
	materil->SetShader(Shaders + L"test.hlsl");
	materil->SetDiffuseMap(Contents + L"Environment/Star.jpg");
*/

	psBuffer->Data.OceanSize = D3DXVECTOR2(vertexSize.x * 2, vertexSize.y * 2);

	heightMap = new Texture(Contents + L"Landscape/HeightMap256.png");

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


}

Water::Ocean::~Ocean()
{
	SAFE_DELETE(vertex);

	SAFE_RELEASE(rasterizer[0]);
	SAFE_RELEASE(rasterizer[1]);

	SAFE_RELEASE(blend[0]);
	SAFE_RELEASE(blend[1]);

	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(material);

	SAFE_DELETE(vsBuffer);
	SAFE_DELETE(psBuffer);

	SAFE_DELETE(heightMap);

//	SAFE_DELETE(test);
}

void Water::Ocean::Update()
{
	vertex->Update();


	D3DXMATRIX T;
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);

	worldBuffer->SetMatrix(T);

	vsBuffer->Data.Time = Time::Get()->Running() * timeMultiflier;

	//D3DXMATRIX testWorld;
	//D3DXMatrixIdentity(&testWorld);
	//D3DXMatrixScaling(&testWorld, 100, 100, 100);
	//test->GetMeshPart(0)->SetWorld(testWorld);
	//test->GetMeshPart(0)->Update();
}

void Water::Ocean::Render()
{
	psBuffer->SetPSBuffer(2);
	vsBuffer->SetVSBuffer(2);

	worldBuffer->SetVSBuffer(1);
	material->SetBuffer();

	heightMap->SetShaderResource(5);
	heightMap->SetShaderSampler(5);
	

	D3D::GetDC()->OMSetBlendState(blend[1], NULL, 0xFF);

	if (bWireframe == true)
		D3D::GetDC()->RSSetState(rasterizer[1]);

	vertex->Render();

	D3D::GetDC()->RSSetState(rasterizer[0]);
	D3D::GetDC()->OMSetBlendState(blend[0], NULL, 0xFF);

	//test->GetMeshPart(0)->Render();
}

void Water::Ocean::PostRender()
{
	if (values->GuiSettings->bShowLandscapeWindow == true)
	{
		GUI::Begin("Water", &values->GuiSettings->bShowLandscapeWindow);

		GUI::Checkbox("Water Wireframe", &bWireframe);
		GUI::ColorEdit4("Water Color", (float*)material->GetDiffuse());

		GUI::SliderFloat("Time", &timeMultiflier, 0, 10, "%.01f");
		GUI::SliderFloat("Amplitude", &vsBuffer->Data.WaveAmplitude, 0, 10, "%.01f");
		GUI::SliderFloat("Frequency", &vsBuffer->Data.WaveFrequency, 0, 1, "%.001f");
		GUI::SliderFloat2("TextureScale", (float*)&vsBuffer->Data.TextureScale, 0, 255, "%.1f");
		GUI::SliderFloat2("BumpSpeed", (float*)&vsBuffer->Data.BumpSpeed, 0, 10, "%.01f");
		GUI::SliderFloat("BumpHeight", &vsBuffer->Data.BumpHeight, 0, 10, "%.1f");

		GUI::SliderFloat2("OceanSize", psBuffer->Data.OceanSize, 0, 512, "%.1f");
		GUI::SliderFloat("HeightRatio", &psBuffer->Data.HeightRatio, 5, 30, "%.1f");
		GUI::SliderFloat("FresnelAmount", &psBuffer->Data.FresnelAmount, 0, 2, "%.1f");
		GUI::SliderFloat("SpecularAmount", &psBuffer->Data.SpecularAmount, 0, 2, "%.1f");


		GUI::End();
	}

	
}