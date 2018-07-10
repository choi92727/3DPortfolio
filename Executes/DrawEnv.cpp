#include "stdafx.h"
#include "DrawEnv.h"
#include "../Environment/Sky.h"
#include "../Environment/Sun.h"

DrawEnv::DrawEnv(ExecuteValues* values)
	: Execute(values)
{
	//sky = new Environment::Sky(values);
	sun = new Environment::Sun(values);
}

DrawEnv::~DrawEnv()
{
	//SAFE_DELETE(sky);
	SAFE_DELETE(sun);
}

void DrawEnv::Update()
{

	//sky->Update();
	sun->Update();
}

void DrawEnv::Render()
{
	
	//sky->Render();
	sun->Render();
}

void DrawEnv::PostRender()
{
	//sky->PostRender();
	sun->PostRender();
}