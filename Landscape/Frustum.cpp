#include "stdafx.h"
#include "Frustum.h"

Landscape::Frustum::Frustum()
{
}

Landscape::Frustum::~Frustum()
{
}

void Landscape::Frustum::Update()
{
}

void Landscape::Frustum::Render()
{
}

void Landscape::Frustum::PostRender()
{
}

void Landscape::Frustum::ConstructFrustum(float screenDepth, D3DXMATRIX projectionMatrix, D3DXMATRIX viewMatrix)
{
	float zMinimum, r;
	D3DXMATRIX matrix;

	// Calculate the minimum Z distance in the frustum.
	zMinimum = -projectionMatrix._43 / projectionMatrix._33;
	r = screenDepth / (screenDepth - zMinimum);
	projectionMatrix._33 = r;
	projectionMatrix._43 = -r * zMinimum;

	// Create the frustum matrix from the view matrix and updated projection matrix.
	D3DXMatrixMultiply(&matrix, &viewMatrix, &projectionMatrix);

	// Calculate near plane of frustum.
	m_planes[0].a = matrix._14 + matrix._13;
	m_planes[0].b = matrix._24 + matrix._23;
	m_planes[0].c = matrix._34 + matrix._33; 
	m_planes[0].d = matrix._44 + matrix._43;
	D3DXPlaneNormalize(&m_planes[0], &m_planes[0]);

	// Calculate far plane of frustum.
	m_planes[1].a = matrix._14 - matrix._13;
	m_planes[1].b = matrix._24 - matrix._23;
	m_planes[1].c = matrix._34 - matrix._33;
	m_planes[1].d = matrix._44 - matrix._43;
	D3DXPlaneNormalize(&m_planes[1], &m_planes[1]);

	// Calculate left plane of frustum.
	m_planes[2].a = matrix._14 + matrix._11;
	m_planes[2].b = matrix._24 + matrix._21;
	m_planes[2].c = matrix._34 + matrix._31;
	m_planes[2].d = matrix._44 + matrix._41;
	D3DXPlaneNormalize(&m_planes[2], &m_planes[2]);

	// Calculate right plane of frustum.
	m_planes[3].a = matrix._14 - matrix._11;
	m_planes[3].b = matrix._24 - matrix._21;
	m_planes[3].c = matrix._34 - matrix._31;
	m_planes[3].d = matrix._44 - matrix._41;
	D3DXPlaneNormalize(&m_planes[3], &m_planes[3]);

	// Calculate top plane of frustum.
	m_planes[4].a = matrix._14 - matrix._12;
	m_planes[4].b = matrix._24 - matrix._22;
	m_planes[4].c = matrix._34 - matrix._32;
	m_planes[4].d = matrix._44 - matrix._42;
	D3DXPlaneNormalize(&m_planes[4], &m_planes[4]);

	// Calculate bottom plane of frustum.
	m_planes[5].a = matrix._14 + matrix._12;
	m_planes[5].b = matrix._24 + matrix._22;
	m_planes[5].c = matrix._34 + matrix._32;
	m_planes[5].d = matrix._44 + matrix._42;
	D3DXPlaneNormalize(&m_planes[5], &m_planes[5]);

	return;

	//D3DXVECTOR3	m_vtx[8];
	//D3DXVECTOR3	m_vPos;
	//m_vtx[0].x = -1.0f;	m_vtx[0].y = -1.0f;	m_vtx[0].z = 1.0f;
	//m_vtx[1].x = 1.0f;	m_vtx[1].y = -1.0f;	m_vtx[1].z = 1.0f;
	//m_vtx[2].x = 1.0f;	m_vtx[2].y = -1.0f;	m_vtx[2].z = 1000.0f;
	//m_vtx[3].x = -1.0f;	m_vtx[3].y = -1.0f;	m_vtx[3].z = 1000.0f;
	//m_vtx[4].x = -1.0f;	m_vtx[4].y = 1.0f;	m_vtx[4].z = 1.0f;
	//m_vtx[5].x = 1.0f;	m_vtx[5].y = 1.0f;	m_vtx[5].z = 1.0f;
	//m_vtx[6].x = 1.0f;	m_vtx[6].y = 1.0f;	m_vtx[6].z = 1000.0f;
	//m_vtx[7].x = -1.0f;	m_vtx[7].y = 1.0f;	m_vtx[7].z = 1000.0f;
	//int				i;
	//D3DXMATRIX	matInv;
	//matInv = viewMatrix * projectionMatrix;
	////D3DXMatrixInverse(&matInv, NULL, &matInv);

	//for (i = 0; i < 8; i++)
	//	D3DXVec3TransformCoord(&m_vtx[i], &m_vtx[i], &matInv);

	//m_vPos = (m_vtx[0] + m_vtx[5]) / 2.0f;
	//D3DXPlaneFromPoints(&m_planes[0], m_vtx + 4, m_vtx + 7, m_vtx + 6);
	//D3DXPlaneFromPoints(&m_planes[1], m_vtx, m_vtx + 1, m_vtx + 2);
	//D3DXPlaneFromPoints(&m_planes[2], m_vtx, m_vtx + 4, m_vtx + 5);
	//D3DXPlaneFromPoints(&m_planes[3], m_vtx + 2, m_vtx + 6, m_vtx + 7);
	//D3DXPlaneFromPoints(&m_planes[4], m_vtx, m_vtx + 3, m_vtx + 7);
	//D3DXPlaneFromPoints(&m_planes[5], m_vtx + 1, m_vtx + 5, m_vtx + 6);
	//return;

}

bool Landscape::Frustum::CheckPoint(float x, float y, float z)
{
	return false;
}

bool Landscape::Frustum::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
	int i;


	// Check if any one point of the cube is in the view frustum.
	for (i = 0; i<6; i++)
	{
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - radius), (yCenter - radius), (zCenter - radius))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + radius), (yCenter - radius), (zCenter - radius))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - radius), (yCenter + radius), (zCenter - radius))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + radius), (yCenter + radius), (zCenter - radius))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - radius), (yCenter - radius), (zCenter + radius))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + radius), (yCenter - radius), (zCenter + radius))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - radius), (yCenter + radius), (zCenter + radius))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + radius), (yCenter + radius), (zCenter + radius))) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}

bool Landscape::Frustum::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	return false;
}

bool Landscape::Frustum::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	return false;
}
