#pragma once
#include "Execute.h"

class ExeGui : public Execute
{
public:
	ExeGui(ExecuteValues* values);
	~ExeGui();

	void Update();

	void PreRender() {}
	void Render();
	void PostRender();

	void ResizeScreen() {}

private:
	void ChangeShowGlobalLightWindow();

	void ChangeShowSystemInfoWindow();
	void ChangeShowDemoWindow();

	void ChangeShowEnvironmentWindow();
	void ChangeShowLandscapeWindow();
};