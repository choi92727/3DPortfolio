#pragma once


#define multipleValue 2.f
namespace Landscape
{

	class TerrainTessBuffer : public ShaderBuffer
	{
	public:
		TerrainTessBuffer()
			: ShaderBuffer(&Data, sizeof(Data))
		{
			Data.tessellationAmount = 2;
		}

		struct Struct
		{
			float tessellationAmount;
			float Padding[3];
		} Data;
	};

	class QuadTree
	{
	private:
		const float UnitNode_Radius = 4.f;

	public:
		QuadTree(class Data* data, ExecuteValues *  values);
		~QuadTree();

		void Update();
		void Render(class Frustum* frustum);
		void PostRender();

	private:
		struct Node
		{
			Node* Childs[4];

			D3DXVECTOR3 LT;
			D3DXVECTOR3 RB;

			ID3D11Buffer* VertexBuffer, *IndexBuffer;
			UINT DrawCount;
			UINT Width;

			bool isRender;

			Node()
			{
				LT = RB = D3DXVECTOR3(0, 0, 0);

				for (int i = 0; i < 4; i++)
					Childs[i] = NULL;
			}
		};

	private:
		void Calculate(D3DXVECTOR3* LT, D3DXVECTOR3* RB);
		void CreateNode(Node* node, D3DXVECTOR3 LT, D3DXVECTOR3 RB);



		void RenderNode(Node* node, class Frustum* frustum);
		void PostRenderNode(Node* node);

		void DeleteNode(Node* node);





	private:
		ExecuteValues* values;
		class Data* data;

		Node* parentNode;
		UINT drawCount;
		TerrainTessBuffer* tessBuffer;

	};
}