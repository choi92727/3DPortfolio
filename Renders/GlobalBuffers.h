#pragma once
#include "stdafx.h"

class WorldBuffer : public ShaderBuffer
{
public:
	WorldBuffer() : ShaderBuffer(&Data, sizeof(Struct))
	{
		D3DXMatrixIdentity(&Data.World);
	}

	void SetMatrix(D3DXMATRIX mat)
	{
		Data.World = mat;
		D3DXMatrixTranspose(&Data.World, &Data.World);
	}

	struct Struct
	{
		D3DXMATRIX World;
	};

private:
	Struct Data;
};

class ViewProjectionBuffer : public ShaderBuffer
{
public:
	ViewProjectionBuffer() : ShaderBuffer(&Data, sizeof(Struct))
	{
		D3DXMatrixIdentity(&Data.View);
		D3DXMatrixIdentity(&Data.Projection);
	}

	void SetView(D3DXMATRIX mat)
	{
		Data.View = mat;
		D3DXMatrixInverse(&Data.ViewInverse, NULL, &Data.View);
		D3DXMatrixTranspose(&Data.View, &Data.View);
		D3DXMatrixTranspose(&Data.ViewInverse, &Data.ViewInverse);
	}

	void SetProjection(D3DXMATRIX mat)
	{
		Data.Projection = mat;
		D3DXMatrixTranspose(&Data.Projection, &Data.Projection);
	}

	D3DXMATRIX View() { 
		return Data.View; 
	}
	D3DXMATRIX Projection() { 
		return Data.Projection; 
	}

	struct Struct
	{
		D3DXMATRIX View;
		D3DXMATRIX Projection;
		D3DXMATRIX ViewInverse;
	};

private:
	Struct Data;
};

class LightBuffer : public ShaderBuffer
{
public:
	LightBuffer() : ShaderBuffer(&Data, sizeof(Struct))
	{
		Data.SunDirection = D3DXVECTOR3(-1, -1, -1);
		Data.SunIntensity = 0;
		Data.SunColor = D3DXCOLOR(1, 1, 1, 1);
	
		Data.AmbientColor = D3DXCOLOR(0, 0, 0, 1);
		Data.AmbientIntensity = 0.2f;

		Data.Overcast = 1;
		Data.SunAngle = 0;
	}

	struct Struct
	{
		//sun
		D3DXVECTOR3 SunDirection;
		float SunIntensity;
		D3DXCOLOR SunColor;
	
		//Ambient RGB alpha->intensity
		D3DXCOLOR AmbientColor;
		float AmbientIntensity;

		float Overcast;
		float SunAngle;

		float Padding;

	};

	Struct Data;
};