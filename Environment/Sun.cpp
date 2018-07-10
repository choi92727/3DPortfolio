#include "stdafx.h"
#include "Sun.h"

Environment::Sun::Sun(ExecuteValues * values)
	: values(values)
	, shaderFile(Shaders + L"003_Sun.hlsl")
	, modelFile(BinModels + L"Meshes/Quad.model")
{
	Fbx::FbxModels::ReadBinary(modelFile, &model);

	Material* material = model->GetMeshPart(0)->GetMaterial();
	material->SetShader(shaderFile);
	material->SetDiffuseMap(Contents + L"Environment/Glow.png");

	buffer = new Buffer();

	{
		D3D11_BLEND_DESC desc;
		States::GetBlendDesc(&desc);
		States::CreateBlend(&desc, &blendState[0]);

		desc.RenderTarget[0].BlendEnable = true;
		States::CreateBlend(&desc, &blendState[1]);
	}
}

Environment::Sun::~Sun()
{
	SAFE_DELETE(buffer);
	SAFE_DELETE(model);

	SAFE_RELEASE(blendState[0]);
	SAFE_RELEASE(blendState[1]);
}

void Environment::Sun::Update()
{
	if (Keyboard::Get()->Press('O'))
		Time::Get()->AddWorldTime(500);
	else if (Keyboard::Get()->Press('L'))
		Time::Get()->AddWorldTime(-500);
	
	float worldTime = Time::Get()->GetWorldTime();
	float dayTimeRatio = Time::Get()->GetDaytimeRatio();

	float sunAngle = worldTime * dayTimeRatio;

	D3DXVECTOR3 sunDirection = D3DXVECTOR3(0, (float)-sinf(sunAngle), (float)cosf(sunAngle));

	float dot = D3DXVec3Dot(&sunDirection, &D3DXVECTOR3(0, 1, 0));

	float sunIntensity =  Math::Clamp(dot * 1.5f, 0, 1);
	D3DXCOLOR sunColor = D3DXVECTOR3(1, 0.5f + sunIntensity / 2, sunIntensity);

	D3DXCOLOR ambientColor = D3DXCOLOR(1,1 - sunIntensity / 10 , 1 - sunIntensity / 5, 1);

	/////////////////
	values->GlobalLight->Data.SunAngle = sunAngle;
	values->GlobalLight->Data.SunIntensity = sunIntensity;
	values->GlobalLight->Data.SunColor = sunColor;
	values->GlobalLight->Data.AmbientColor = ambientColor;
	values->GlobalLight->Data.SunDirection = sunDirection;
	////////////////

	D3DXVECTOR3 cameraPosition;
	values->MainCamera->GetPosition(&cameraPosition);

	D3DXVECTOR3 sunPosition;
	sunPosition.x = cameraPosition.x;
	sunPosition.y = -sunDirection.y * 400;
	sunPosition.z = cameraPosition.z + sunDirection.z * 400;

	D3DXMATRIX T, S;
	D3DXMatrixTranslation(&T, sunPosition.x, sunPosition.y, sunPosition.z);
	D3DXMatrixScaling(&S, 40, 40, 1);

	model->GetMeshPart(0)->SetWorld(S * T);
	model->GetMeshPart(0)->Update();
}

void Environment::Sun::Render()
{
	//buffer->SetPSBuffer(2);
	D3D::GetDC()->OMSetBlendState(blendState[1], NULL, 0XFF);
	model->GetMeshPart(0)->Render();
	D3D::GetDC()->OMSetBlendState(blendState[0], NULL, 0XFF);
}

void Environment::Sun::PostRender()
{
	if (values->GuiSettings->bShowEnvironmentWindow == true)
	{
		
	}
}