#pragma once

namespace Landscape
{
	class Frustum
	{
	public:
		Frustum();
		~Frustum();

		void Update();
		void Render();
		void PostRender();

	public:
		void ConstructFrustum(float screenDepth, D3DXMATRIX projectionMatrix, D3DXMATRIX viewMatrix);


		bool CheckPoint(float x, float y, float z);
		bool CheckCube(float xCenter, float yCenter, float zCenter, float radius);
		bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);
		bool CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);
	private:
		D3DXPLANE m_planes[6];


	};


}