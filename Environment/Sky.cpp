#include "stdafx.h"
#include "Sky.h"

Environment::Sky::Sky(ExecuteValues * values)
	: values(values)
	, shaderFile(Shaders + L"003_Sky.hlsl")
	, modelFile(BinModels + L"Meshes/Sphere.model")
{
	Fbx::FbxModels::ReadBinary(modelFile, &model);

	Material* material = model->GetMeshPart(0)->GetMaterial();
	material->SetShader(shaderFile);
	material->SetDiffuseMap(Contents + L"Environment/Stars.png");

	buffer = new Buffer();

	{
		D3D11_RASTERIZER_DESC desc;
		States::GetRasterizerDesc(&desc);
		States::CreateRasterizer(&desc, &rasterizer[0]);

		desc.FrontCounterClockwise = true;
		States::CreateRasterizer(&desc, &rasterizer[1]);
	}

	{
		D3D11_DEPTH_STENCIL_DESC desc;
		States::GetDepthStencilDesc(&desc);
		States::CreateDepthStencil(&desc, &depthStencil[0]);

		desc.DepthEnable = false;
		States::CreateDepthStencil(&desc, &depthStencil[1]);
	}
}

Environment::Sky::~Sky()
{
	SAFE_DELETE(buffer);
	SAFE_DELETE(model);

	SAFE_RELEASE(rasterizer[0]);
	SAFE_RELEASE(rasterizer[1]);

	SAFE_RELEASE(depthStencil[0]);
	SAFE_RELEASE(depthStencil[1]);

	
}

void Environment::Sky::Update()
{
	D3DXVECTOR3 position;
	values->MainCamera->GetPosition(&position);

	D3DXMATRIX world;
	D3DXMatrixScaling(&world, 1, 1, 1);
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);

	buffer->Data.Time = Time::Get()->Running() / 7.0f;
	model->GetMeshPart(0)->SetWorld(world);
	model->GetMeshPart(0)->Update();
}

void Environment::Sky::Render()
{
	D3D::GetDC()->OMSetDepthStencilState(depthStencil[1], 1);
	D3D::GetDC()->RSSetState(rasterizer[1]);

	buffer->SetPSBuffer(2);
	model->GetMeshPart(0)->Render();

	D3D::GetDC()->RSSetState(rasterizer[0]);
	D3D::GetDC()->OMSetDepthStencilState(depthStencil[0], 1);
}

void Environment::Sky::PostRender()
{
	if (values->GuiSettings->bShowEnvironmentWindow == true)
	{
		/*ImGui::Begin("Environment", &values->GuiSettings->bShowEnvironmentWindow);
		{
			ImGui::ColorEdit4("Center", buffer->Data.Center);
			ImGui::ColorEdit4("Apex", buffer->Data.Apex);
			ImGui::SliderFloat("Height", &buffer->Data.Height, 0, 10);
		}
		ImGui::End();*/
	}
}