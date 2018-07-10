#pragma once

namespace Environment
{
	class Sun
	{
	public:
		class Buffer : public ShaderBuffer
		{
		public:
			Buffer() : ShaderBuffer(&Data, sizeof(Data))
			{
				
			}

			struct Struct
			{
				D3DXVECTOR4 temp;
			} Data;
		};

		Sun(ExecuteValues* values);
		~Sun();

		void Update();
		void Render();
		void PostRender();

	private:
		ExecuteValues* values;

		wstring shaderFile;
		wstring modelFile;

		Model::BinModel* model;

		Buffer* buffer;

		ID3D11BlendState* blendState[2];
	};
}