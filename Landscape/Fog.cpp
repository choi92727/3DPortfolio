#include "stdafx.h"
#include "Fog.h"

//Linear Fog = (FogEnd - ViewpointDistance) / (FogEnd - FogStart)
//Exponential Fog = 1.0 / 2.71828 power(ViewpointDistance * FogDensity)
//Exponential Fog 2 = 1.0 / 2.71828 power((ViewpointDistance * FogDensity) * (ViewpointDistance * FogDensity))
//http://telnet.or.kr/directx/graphics/programmingguide/advancedtopics/framebuffer/fogtypes/fogformulas.htm

Landscape::Fog::Fog(ExecuteValues* values)
	:values(values)
{
	vsBuffer = new FogVSBuffer();
	psBuffer = new FogPSBuffer();
}

Landscape::Fog::~Fog()
{
	SAFE_DELETE(vsBuffer);
	SAFE_DELETE(psBuffer);
}

void Landscape::Fog::Render()
{
	vsBuffer->SetVSBuffer(2);
	psBuffer->SetPSBuffer(2);
}

void Landscape::Fog::PostRender()
{
	if (values->GuiSettings->bShowEnvironmentWindow == true)
	{
		GUI::Begin("Environment", &values->GuiSettings->bShowEnvironmentWindow);
		{
			GUI::SliderFloat("Fog Start", &vsBuffer->Data.fogStart, 0, 1000);
			GUI::SliderFloat("Fog End", &vsBuffer->Data.fogEnd, 0, 1000);

			GUI::ColorEdit4("Fog Color", psBuffer->Data.Color);

			GUI::Separator();
		}
		GUI::End();
	}
}
