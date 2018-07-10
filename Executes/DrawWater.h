#pragma once

namespace Water 
{ 
	class Ocean; 
}

class DrawWater : public Execute
{
public:
	DrawWater(ExecuteValues* values);
	~DrawWater();

	void Update();

	void PreRender() {}
	void Render();
	void PostRender();

	void ResizeScreen() {}

private:
	Water::Ocean* ocean;
};