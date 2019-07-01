#pragma once

namespace Landscape
{
	class TessAmountBuffer : public ShaderBuffer
	{
	public:
		TessAmountBuffer()
			: ShaderBuffer(&Data, sizeof(Data))
		{
			Data.maxTess = 5.f;
			Data.distance = 2500.f;
		}

		struct Struct
		{
			float maxTess;
			float distance;
			float Padding[2];
		} Data;
	};


	class Terrain
	{
	public:
		class GridBuffer :public ShaderBuffer
		{
		public:
			GridBuffer() :ShaderBuffer(&Grid, sizeof(Grid))
			{
				Grid.Color = D3DXCOLOR(1, 1, 1, 1);
				Grid.Spacing = 5;
				Grid.Thickness = 0.1f;
				Grid.bView = 1;

			}
			struct Struct
			{
				D3DXCOLOR Color;
				float Spacing;
				float Thickness;
				UINT bView;

				float Padding;
			}Grid;
		};


	public:
		Terrain(ExecuteValues* values);
		~Terrain();

		void Update();
		void Render();
		void PostRender();

	private:
		ExecuteValues* values;
		class Data* data;
		class Fog* fog;
		class QuadTree* quadTree;
		class Frustum* frustum;

		Material* material;
		WorldBuffer* worldBuffer;
		TessAmountBuffer* tessAmountBuffer;

		ID3D11SamplerState* diffuseSampler;
		ID3D11SamplerState* detailSampler;
		GridBuffer* gridBuffer;


		ID3D11RasterizerState* rasterizer[2];
	};
}