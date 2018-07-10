#pragma once

namespace Water
{
	class OceanVSBuffer : public ShaderBuffer
	{
	public:
		OceanVSBuffer() : ShaderBuffer(&Data, sizeof(Data))
		{
			Data.WaveFrequency = 0.05f;
			Data.WaveAmplitude = 1.0f;
			Data.Time = 0.0f;
			Data.TextureScale = D3DXVECTOR2(14, 14); //파도의 간격
			Data.BumpSpeed = D3DXVECTOR2(0.0f, 0.05f);
			Data.BumpHeight = 0.5f;
		}

		struct Struct
		{
			float Time;
			float WaveFrequency;
			float WaveAmplitude;

			float VS_OceanPadding1;

			D3DXVECTOR2 TextureScale;
			D3DXVECTOR2 BumpSpeed;

			float BumpHeight;
			float VS_OceanPadding2[3];

		} Data;
	};

	class OceanPSBuffer : public ShaderBuffer
	{
	public:
		OceanPSBuffer() : ShaderBuffer(&Data, sizeof(Data))
		{
			Data.ShallowColor = D3DXCOLOR(0.7f, 0.85f, .8f, 1.0f);
			Data.DeepColor = D3DXCOLOR(.2f, .5f, .95f, 1.0f);

			Data.FresnelBias = 0.8f;
			Data.FresnelPower = 0.5f;

			Data.HeightRatio = 10.0f;
			Data.ShoreBlend = 35.0f;
			Data.OceanSize = D3DXVECTOR2(0, 0);

			Data.FresnelAmount = 0.5f;
			Data.SpecularAmount = 0.5f;
		}


		struct Struct
		{
			D3DXCOLOR ShallowColor; //얕은색
			D3DXCOLOR DeepColor; //깊은색

			float FresnelBias; //편향
			float FresnelPower; //강도
			float FresnelAmount;
			float SpecularAmount;

			D3DXVECTOR2 OceanSize; //버택스 전체 크기
			float HeightRatio;
			float ShoreBlend; //알파값 변화정도
		} Data;

	};

	class Ocean
	{
	public:
		Ocean(ExecuteValues* values);
		~Ocean();

		void Update();
		void Render();
		void PostRender();

	private:
		ExecuteValues* values;
		class OceanVertex* vertex;

		OceanPSBuffer* psBuffer;
		OceanVSBuffer* vsBuffer;

		bool bWireframe;

		Material* material;
		WorldBuffer* worldBuffer;

		Texture* heightMap;

		ID3D11RasterizerState* rasterizer[2];
		ID3D11BlendState* blend[2];

		D3DXVECTOR3 position;
		D3DXVECTOR2 vertexSize;


		float timeMultiflier;

	//	Model::BinModel* test;

	};
}