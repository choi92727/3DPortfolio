#pragma once
#include "stdafx.h"
#include "Enums.h"
#include "KeyframeTable.h"

namespace Particle
{
	struct VertexData
	{
		bool bHasPosition = false;
		bool bHasNormal = false;
		bool bHasColor = false;
		bool bHasTextureCoord = false;
		bool bHasIndex = false;

		vector<D3DXVECTOR3> Position;
		vector<D3DXVECTOR3> Normal;
		vector<D3DXCOLOR> Color;
		vector<D3DXVECTOR2> TextureCoord;
		vector<UINT> Index;
	};

	struct ParticleInfo
	{
		string Name = "";
		ParticleObjectType ParticleType = ParticleObjectType::PointSprite;
		VertexData* MeshData = nullptr;
		string TextureFileName = "";
		bool AlphaBlendEnable = true;
		bool DepthBufferEnable = false;
		D3D11_BLEND SourceBlend = D3D11_BLEND_ONE;
		D3D11_BLEND DestinationBlend = D3D11_BLEND_ONE;
		D3D11_BLEND_OP BlendFunction = D3D11_BLEND_OP_ADD;
		float LifeTime = 0.0f;
		float ObjectLifeTime = 0.0f;
		float MassMin = 0.0f;
		float MassMax = 0.0f;
		int InitialObjectCount = 0;
		int MaxObjectCount = 0;
		int EmitCount = 0;
		bool Volatile = false;

		EmitObjectType EmitType = EmitObjectType::Cone;
		D3DXVECTOR3 EmitPosition = D3DXVECTOR3(0, 0, 0);
		D3DXVECTOR3 EmitDirection = D3DXVECTOR3(0, 0, 1);//¿ø·¡ -1ÀÓ
		float EmitAngle = 0.0f;
		float EmitInterval = 0.0f;
		D3DXVECTOR3 UpVector = D3DXVECTOR3(0, 1, 0);

		UINT PositionStyle = 0;
		float PositionUpdateInterval = 0.0f;
		vector<FuncType> PositionFunc;
		vector<float> PositionInit;
		vector<float> PositionFactor;
		vector<KeyframeTable*> PositionTable;
		float PositionMin = 0.0f;
		float PositionMax = 0.0f;
		float PositionInitialRandomFactor = 1.0f;
		D3DXVECTOR3 PositionRandomFactor = D3DXVECTOR3(1, 1, 1);
		float PositionRandomInterval = 0.0f;

		UINT ScaleStyle = 0;
		float ScaleUpdateInterval = 0.0f;
		vector<FuncType> ScaleFunc;
		vector<float> ScaleInit;
		vector<float> ScaleFactor;
		vector<KeyframeTable*> ScaleTable;
		float ScaleInitialRandomFactor = 1.0f;
		float ScaleMin = 0.0f;
		float ScaleMax = 0.0f;
		D3DXVECTOR3 ScaleMask = D3DXVECTOR3(1, 1, 1);
		float ScaleBillboardFactor = 1.0f;

		UINT RotateStyle = 0;
		float RotateUpdateInterval = 0.0f;
		float RotateRandomFactor = 1.0f;
		FuncType RotateFunc = FuncType::None;
		float RotateInit = 0.0f;
		float RotateFactor = 1.0f;
		KeyframeTable* RotateTable = nullptr;

		UINT ColorStyle = 0;
		float ColorUpdateInterval = 0.0f;
		FuncType RgbFunc = FuncType::None;
		string RgbInit = "";
		KeyframeTable* Rtable = nullptr;
		KeyframeTable* Gtable = nullptr;
		KeyframeTable* Btable = nullptr;
		KeyframeTable* Atable = nullptr;

		FuncType AlphaFunc = FuncType::None;
		UINT AlphaInit = 255;

		D3DXCOLOR eRgbInit = D3DXCOLOR(0, 0, 0, 1);
		const int FuncCount = 2;

		D3DXCOLOR GetRgbInitValue()
		{
			return eRgbInit;
		}

		bool IsPositionStyle(UINT param)
		{
			return ((PositionStyle & param) > 0);
		}

		bool IsPositionStyle(ParamStyles param)
		{
			return ((PositionStyle & (UINT)param) > 0);
		}

		bool IsScaleStyle(UINT param)
		{
			return ((ScaleStyle & param) > 0);
		}

		bool IsScaleStyle(ParamStyles param)
		{
			return ((ScaleStyle & (UINT)param) > 0);
		}

		bool IsRotateStyle(UINT param)
		{
			return ((RotateStyle & param) > 0);
		}

		bool IsRotateStyle(ParamStyles param)
		{
			return ((RotateStyle & (UINT)param) > 0);
		}

		bool IsColorStyle(UINT param)
		{
			return ((ColorStyle & param) > 0);
		}

		bool IsColorStyle(ParamStyles param)
		{
			return ((ColorStyle & (UINT)param) > 0);
		}

		void Initialize()
		{
			//  Initialize KeyFrameTable
			if (PositionTable.size() > 0)
			{
				for (int i = 0; i < PositionTable.size(); i++)
					PositionTable[i]->Initialize();
			}

			if (ScaleTable.size() > 0)
			{
				for (int i = 0; i < ScaleTable.size(); i++)
					ScaleTable[i]->Initialize();
			}

			if (RotateTable != nullptr)
				RotateTable->Initialize();

			if (Rtable != nullptr)
				Rtable->Initialize();

			if (Gtable != nullptr)
				Gtable->Initialize();

			if (Btable != nullptr)
				Btable->Initialize();

			if (Atable != nullptr)
				Atable->Initialize();

			vector<string> temp;
			String::SplitString(&temp, RgbInit, ",");

			eRgbInit = D3DXCOLOR
			(//atoi
				atof(temp[0].c_str()) / 255.0f,
				atof(temp[1].c_str()) / 255.0f,
				atof(temp[2].c_str()) / 255.0f,
				atof(temp[3].c_str()) / 255.0f
			);
		}
	};
}