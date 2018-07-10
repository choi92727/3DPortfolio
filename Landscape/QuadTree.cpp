#include "stdafx.h"
#include "QuadTree.h"

#include "Data.h"
#include "Frustum.h"


Landscape::QuadTree::QuadTree(Data * data)
	: drawCount(0)
	, data(data)
{

	D3DXVECTOR3 LT;
	D3DXVECTOR3 RB;
	Calculate(&LT, &RB);

	parentNode = new Node();
	CreateNode(parentNode, LT, RB);

	int a = 10;
}

void Landscape::QuadTree::Calculate(D3DXVECTOR3 * LT, D3DXVECTOR3 * RB)
{
	*LT = D3DXVECTOR3(0, 0, 0);
	*RB = D3DXVECTOR3(0, 0, 0);

	float minX = 1e+8f, maxX = -1e+8f;
	float minZ = 1e+8f, maxZ = -1e+8f;

	for (UINT i = 0; i < data->GetVertices().size(); i++)
	{
		TerrainVertexType vertex = data->GetVertices()[i];

		if (minX > vertex.position.x) minX = vertex.position.x;
		if (maxX < vertex.position.x) maxX = vertex.position.x;
		if (minZ > vertex.position.z) minZ = vertex.position.z;
		if (maxZ < vertex.position.z) maxZ = vertex.position.z;
	}
	
	*LT = D3DXVECTOR3(minX, 0.0f, maxZ);
	*RB = D3DXVECTOR3(maxX, 0.0f, minZ);

}

void Landscape::QuadTree::CreateNode(Node * node, D3DXVECTOR3 LT, D3DXVECTOR3 RB)
{
	node->LT = LT;
	node->RB = RB;	   
	node->isRender = false;
	float radiusX = fabsf(LT.x - RB.x)*0.5f;
	float radiusZ = fabsf(LT.z - RB.z)*0.5f;

	float radius = max(radiusX, radiusZ);

	


	
	if (radius <= UnitNode_Radius)// 단위 노드
	{
		
		node->isRender = true;
		vector<TerrainVertexType> vertices;
		vector<UINT> indices;
		UINT nodeWidth = fabsf(RB.x - LT.x);
		UINT nodeHeight = fabsf(RB.z - LT.z);

		UINT vertexCount = (nodeWidth)*(nodeHeight);


		vertices.assign(vertexCount, TerrainVertexType());

		UINT index = 0;
		for (UINT z =(UINT) RB.z; z < (UINT)LT.z; z++)
		{
			for (UINT x = (UINT)LT.x; x < (UINT)RB.x; x++)
			{
				TerrainVertexType vertex = data->GetVertices()[x+(z*(data->GetWidth()+1))];
				vertices[index] = vertex;
				if (index == vertexCount - 1)
					vertex.position.y = 1000.0f;

				index++;
			}
		}

		UINT indexCount = (nodeWidth-1)*(nodeHeight-1) * 6;
		indices.assign(indexCount, UINT());
		index = 0;
		for (UINT z = 0; z < nodeWidth-1; z++)
		{
			for (UINT x = 0; x < nodeHeight-1; x++)
			{
				indices[index + 0] = (nodeWidth) * z + x;
				indices[index + 1] = (nodeWidth) * (z + 1) + x;
				indices[index + 2] = (nodeWidth) * z + x + 1;
				indices[index + 3] = (nodeWidth) * z + x + 1;
				indices[index + 4] = (nodeWidth) * (z + 1) + x;
				indices[index + 5] = (nodeWidth) * (z + 1) + (x + 1);

				index += 6;
			}
		}
		
		//VertexBuffer
		{
			D3D11_BUFFER_DESC desc = { 0 };
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.ByteWidth = sizeof(TerrainVertexType) * vertices.size();
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

			D3D11_SUBRESOURCE_DATA data = { 0 };
			data.pSysMem = &vertices[0];

			HRESULT hr;
			hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &node->VertexBuffer);
			assert(SUCCEEDED(hr));
		}

		//IndexBuffer
		{
			D3D11_BUFFER_DESC desc = { 0 };
			desc = { 0 };
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.ByteWidth = sizeof(UINT) * indices.size();
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

			D3D11_SUBRESOURCE_DATA data = { 0 };
			data = { 0 };
			data.pSysMem = &indices[0];

			HRESULT hr;
			hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &node->IndexBuffer);
			assert(SUCCEEDED(hr));
		}
		node->DrawCount = indices.size();
		vertices.clear();
		indices.clear();
		return;
	}

	if (radius > UnitNode_Radius)
	{
		node->VertexBuffer = NULL;
		node->IndexBuffer = NULL;

		float width =  (RB.x - LT.x) * .5f;
		float height = (LT.z - RB.z) * .5f;

		D3DXVECTOR3 centerTop	 =	 D3DXVECTOR3(LT.x+ width, 0, LT.z		  );
		D3DXVECTOR3 centerRight  =	 D3DXVECTOR3(RB.x,	 	  0, RB.z + height);
		D3DXVECTOR3 centerLeft	 =	 D3DXVECTOR3(LT.x,	 	  0, RB.z + height);
		D3DXVECTOR3 center		 =	 D3DXVECTOR3(LT.x+ width, 0, RB.z + height);
		D3DXVECTOR3 centerBottom =	 D3DXVECTOR3(LT.x+ width, 0, RB.z		  );

		for (int i = 0; i < 4; i++)
		{
			D3DXVECTOR3 tempLT(0, 0, 0);
			D3DXVECTOR3 tempRB(0, 0, 0);

			switch (i)
			{
			case 0: //좌하단
			{
				tempLT = centerLeft;
				tempRB = centerBottom;
				break;
			}

			case 1: //우하단
			{
				tempLT = center;
				tempRB = RB;
				break;
			}

			case 2: //좌상
			{
				tempLT = LT;
				tempRB = center;

				break;
			}

			case 3: //우상
			{
				tempLT = centerTop;
				tempRB = centerRight;
				break;
			}
			}



			node->Childs[i] = new Node();
			CreateNode(node->Childs[i], tempLT, tempRB);

		}//for(i)
		return;
	}//if(containedCount)

}


Landscape::QuadTree::~QuadTree()
{
	if (parentNode != NULL)
	{
		DeleteNode(parentNode);
		SAFE_DELETE(parentNode);
	}
}

void Landscape::QuadTree::DeleteNode(Node * node)
{
	for (int i = 0; i < 4; i++)
	{
		if (node->Childs[i] != NULL)
			DeleteNode(node->Childs[i]);
	}

	if(node->VertexBuffer!= NULL)
	SAFE_RELEASE(node->VertexBuffer);
	if (node->IndexBuffer != NULL)
	SAFE_RELEASE(node->IndexBuffer);

	for (int i = 0; i < 4; i++)
		SAFE_DELETE(node->Childs[i]);
}



void Landscape::QuadTree::Update()
{

}

void Landscape::QuadTree::Render(Frustum* frustum)
{
	drawCount = 0;

	if(parentNode != NULL)
		RenderNode(parentNode , frustum);
}

void Landscape::QuadTree::PostRender()
{

	ImGui::Begin("quadTree");
	{
		if (ImGui::TreeNode("Basic trees"))
		{
			PostRenderNode(parentNode);
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void Landscape::QuadTree::RenderNode(Node * node , Frustum* frustum)
{
	//TODO: 보여질 영역 판단
	bool result;
	float xRadius, zRadius , xCenter, zCenter;

	xRadius = fabsf(node->RB.x - node->LT.x)/2.f;
	zRadius = fabsf(node->LT.z - node->RB.z)/2.f;
	xCenter = xRadius + node->LT.x;
	zCenter = zRadius + node->RB.z;

	
	result = frustum->CheckCube(xCenter,0.0f,zCenter,xRadius);
	if (!result)
	{
		node->isRender = false;
		return;
	}
	else
		node->isRender = true;



	UINT count = 0;
	for (int i = 0; i < 4; i++)
	{
		if (node->Childs[i] != NULL)
		{
			RenderNode(node->Childs[i] , frustum);

			count++;
		}//if(node)
	}//for(i)
	
	if (count != 0)return;

	if (node->isRender == false)return;
	
	UINT stride = sizeof(VertexTextureNormal);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &node->VertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(node->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	D3D::GetDC()->DrawIndexed(node->DrawCount, 0, 0);
	
}

void Landscape::QuadTree::PostRenderNode(Node * node)
{
	UINT count = 0;
	for (int i = 0; i < 4; i++)
	{
		if (node->Childs[i] != NULL)
		{
			PostRenderNode(node->Childs[i]);

			count++;
		}//if(node)
	}//for(i)

	if (count != 0)return;

	//ImGui::BeginGroup();
	{
		ImGui::Text("LT : %3.0f,%3.0f,RB : %3.0f,%3.0f", node->LT.x, node->LT.z, node->RB.x, node->RB.z);
		ImGui::SameLine();
		ImGui::Checkbox("aa", &(node->isRender));

	}
	//ImGui::EndGroup();
	
	
		
	return;	
}
