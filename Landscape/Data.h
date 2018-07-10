#pragma once

namespace Landscape
{
	class Data
	{
	public:
		Data(wstring file = L"", float heightRatio = 10.0f);
		~Data();

		float GetVertexHeight(D3DXVECTOR3 position);

		UINT GetHeight() { return height; }
		UINT GetWidth() { return width; }

		UINT GetVertexCount() { return vertices.size(); }
		UINT GetIndexCount() { return indices.size(); }

		const vector<TerrainVertexType>& GetVertices() { return vertices; }
		const vector<UINT>& GetIndices() { return indices; }

		void SetHeightMapFile(wstring file, float heightRatio = 10.0f);
		void UpdateVertexData(D3D11_BOX* box = NULL);

		void Render();

	private:
		void Clear();

		void FillVertices(float heightRatio);
		void FillNormalData();
		
		void CreateBuffer();

	private:
		Texture* heightMap;

		UINT width, height;

		ID3D11Buffer* vertexBuffer, *indexBuffer;

		vector<TerrainVertexType> vertices;
		vector<UINT> indices;
	};
}