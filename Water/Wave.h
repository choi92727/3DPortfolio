#pragma once

namespace Water
{
	class Wave
	{
	public:
		Wave(ExecuteValues* values);
		~Wave();

		void Update();
		void Render();
		void PostRender();

	private:
		ExecuteValues* values;
		class WaveVertex* vertex;

		bool bWireframe;

		Material* material;
		WorldBuffer* worldBuffer;

		ID3D11RasterizerState* rasterizer[2];
		ID3D11BlendState* blend[2];

		D3DXVECTOR3 position;
	};
}