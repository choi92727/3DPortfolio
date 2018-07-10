#pragma once

namespace Water
{
	class OceanVertex
	{
	public:
		friend class Ocean;

	public:
		OceanVertex(ExecuteValues* values, UINT width, UINT height, float thick);
		~OceanVertex();

		void Update();
		void Render();
		void PostRender();

	private:
		void CreateBuffer();

	private:
		ExecuteValues* values;

		ID3D11Buffer* vertexBuffer;
		vector<OceanVertexType> vertices;

		ID3D11Buffer* indexBuffer;
		vector<UINT> indices;

		bool bStop;

		UINT width;
		UINT height;
		float thick;

	};
}
