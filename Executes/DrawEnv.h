#pragma once

namespace Environment { class Sky; class Sun; }

class DrawEnv : public Execute
{
public:
	DrawEnv(ExecuteValues* values);
	~DrawEnv();

	void Update();

	void PreRender() {}
	void Render();
	void PostRender();

	void ResizeScreen() {}

private:
	Environment::Sky* sky;
	Environment::Sun* sun;
};