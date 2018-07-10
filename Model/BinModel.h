#pragma once
#include "../Interfaces/ICloneable.h"

namespace Fbx { class FbxModel; }

namespace Model
{
	class BinModel : public ICloneable
	{
	public:
		friend class Fbx::FbxModel;

	private:
		BinModel();

	public:
		~BinModel();
		
		void Clone(void** clone);

		void Update();
		void Render();

		UINT GetMeshPartSize()
		{
			return meshParts.size();
		}

		class MeshPart* GetMeshPart(UINT index)
		{
			return meshParts[index];
		}

	private:
		string name;

		vector<class MeshPart *> meshParts;
	};
}