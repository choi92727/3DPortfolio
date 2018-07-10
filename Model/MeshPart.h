#pragma once
#include "../Interfaces/ICloneable.h"

namespace Fbx 
{ 
	struct MeshData; 
	class FbxModel;
}

namespace Model
{
	class MeshPart : public ICloneable
	{
	public:
		friend struct Fbx::MeshData;
		friend class Fbx::FbxModel;

	private:
		MeshPart();

	public:
		MeshPart(Fbx::MeshData& data);
		~MeshPart();

		void Clone(void** clone);

		Material* GetMaterial() { return material; }

		void SetWorld(D3DXMATRIX& world) { this->world = world; }

		void Update();
		void Render();

	private:
		void CreateBuffer();
	
	private:
		string name;

		Material* material;

		D3DXMATRIX world;
		WorldBuffer* worldBuffer;

		vector<VertexTextureNormal> vertexData;
		vector<UINT> indexData;

		ID3D11Buffer* vertexBuffer, *indexBuffer;
	};
}