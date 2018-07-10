#include "stdafx.h"
#include "DrawWater.h"
#include "../Water/Ocean.h"

using namespace Water;

DrawWater::DrawWater(ExecuteValues* values)
	: Execute(values)
{
	ocean = new Ocean(values);
}

DrawWater::~DrawWater()
{
	SAFE_DELETE(ocean);
}

void DrawWater::Update()
{
	ocean->Update();
}

void DrawWater::Render()
{
	ocean->Render();
}

void DrawWater::PostRender()
{
	ocean->PostRender();
}