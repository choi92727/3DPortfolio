#pragma once

namespace Environment
{
	class Sky
	{
	public:
		class Buffer : public ShaderBuffer
		{
		public:
			Buffer() : ShaderBuffer(&Data, sizeof(Data))
			{
				Data.HorizonColorTwilight = D3DXCOLOR(0.4f, 0.16f, 0, 1) * 1.5f;
				Data.HorizonColorDay	  = D3DXCOLOR(1, 1, 1, 1);
				Data.HorizonColorNight    = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1);
				Data.CeilingColorTwilight = D3DXCOLOR(0.17f, 0.15f, 0.15f, 1);
				Data.CeilingColorDay      = D3DXCOLOR(0.72f, 0.75f, 0.98f, 1);
				Data.CeilingColorNight	  = D3DXCOLOR(0.1f, 0.1f, 0.15f, 1);

				Data.Height = 0.4f;
			}

			struct Struct
			{
				D3DXCOLOR HorizonColorTwilight; //Ȳȥ �����÷�
				D3DXCOLOR HorizonColorDay;		//�ְ� �����÷�
				D3DXCOLOR HorizonColorNight;	//�߰� �����÷�

				D3DXCOLOR CeilingColorTwilight; //Ȳȥ �����÷�
				D3DXCOLOR CeilingColorDay;		//�ְ� �����÷�
				D3DXCOLOR CeilingColorNight;	//�߰� �����÷�

				float Height;
				float Time;
				float Padding[2];
			} Data;
		};

		Sky(ExecuteValues* values);
		~Sky();

		void Update();
		void Render();
		void PostRender();

	private:
		ExecuteValues* values;

		wstring shaderFile;
		wstring modelFile;

		Model::BinModel* model;

		Buffer* buffer;

		ID3D11RasterizerState* rasterizer[2];
		ID3D11DepthStencilState* depthStencil[2];
	};
}