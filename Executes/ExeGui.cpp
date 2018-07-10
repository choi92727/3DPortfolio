#include "stdafx.h"
#include "ExeGui.h"

ExeGui::ExeGui(ExecuteValues* values)
	: Execute(values)
{
	Json::Value light = (*values->JsonRoot)["Global Light"];
	if (light.isNull() == false)
		Json::GetValue(light, "Overcast", values->GlobalLight->Data.Overcast);

	Json::Value window = (*values->JsonRoot)["Window"];
	if (window.isNull() == false)
	{
		Json::GetValue(window, "Environment Window", values->GuiSettings->bShowEnvironmentWindow);
		Json::GetValue(window, "Landscape Window", values->GuiSettings->bShowLandscapeWindow);;
		Json::GetValue(window, "Global Light Window", values->GuiSettings->bShowGlobalLightWindow);
		Json::GetValue(window, "System Info Window", values->GuiSettings->bShowSystemInfoWindow);
	}
}

ExeGui::~ExeGui()
{
	Json::Value light;
	Json::SetValue(light, "Overcast", values->GlobalLight->Data.Overcast);

	(*values->JsonRoot)["Global Light"] = light;

	Json::Value window;
	Json::SetValue(window, "Environment Window", values->GuiSettings->bShowEnvironmentWindow);
	Json::SetValue(window, "Landscape Window", values->GuiSettings->bShowLandscapeWindow);;
	Json::SetValue(window, "Global Light Window", values->GuiSettings->bShowGlobalLightWindow);
	Json::SetValue(window, "System Info Window", values->GuiSettings->bShowSystemInfoWindow);

	(*values->JsonRoot)["Window"] = window;
}

void ExeGui::Update()
{
	if (Keyboard::Get()->Press(VK_CONTROL))
	{
		if (Keyboard::Get()->Down(VK_F11))
			ChangeShowSystemInfoWindow();

		if (Keyboard::Get()->Down(VK_F12))
			ChangeShowDemoWindow();

		if (Keyboard::Get()->Down(VK_F1))
			ChangeShowEnvironmentWindow();

		if (Keyboard::Get()->Down(VK_F2))
			ChangeShowLandscapeWindow();

		if (Keyboard::Get()->Down(VK_F10))
			ChangeShowGlobalLightWindow();
	}
}

void ExeGui::Render()
{
	
}

void ExeGui::PostRender()
{
	if (GUI::BeginMainMenuBar())
	{
		if (GUI::BeginMenu("View"))
		{
			if (GUI::MenuItem("EnvironMent", "Ctrl+F1"))
				ChangeShowEnvironmentWindow();

			if (GUI::MenuItem("Landscape", "Ctrl+F2"))
				ChangeShowLandscapeWindow();

			if (GUI::MenuItem("SystemInfo", "Ctrl+F11"))
				ChangeShowSystemInfoWindow();

			if(GUI::MenuItem("Global Light", "Ctrl+F10"))
				ChangeShowGlobalLightWindow();

			if (GUI::MenuItem("Demo", "Ctrl+F12"))
				ChangeShowDemoWindow();

			GUI::EndMenu();
		}

		GUI::EndMainMenuBar();
	}

	
	D3DXVECTOR3 cameraPosition;
	D3DXVECTOR2 cameraRotation;
	values->MainCamera->GetPosition(&cameraPosition);
	values->MainCamera->GetRotationDegree(&cameraRotation);

	if (values->GuiSettings->bShowSystemInfoWindow == true)
	{
		GUI::Begin("System Info", &values->GuiSettings->bShowSystemInfoWindow);
		GUI::Text("Frame Per Second : %4.0f", GUI::GetIO().Framerate);

		UINT hour = Time::Get()->GetHour();
		string strHour = hour < 10 ? "0" + to_string(hour) : to_string(hour);

		UINT minute = Time::Get()->GetMinute();
		string strMinute = minute < 10 ? "0" + to_string(minute) : to_string(minute);

		GUI::Text("Current Time : %s", (strHour + ":" + strMinute).c_str());

		GUI::Separator();
		GUI::Text
		(
			"Camera Position : %3.0f, %3.0f, %3.0f"
			, cameraPosition.x, cameraPosition.y, cameraPosition.z
		);
		GUI::Text
		(
			"Camera Rotation : %3.0f, %3.0f", cameraRotation.x, cameraRotation.y
		);
		


		GUI::Separator();
		
		GUI::End();
	}

	if (values->GuiSettings->bShowGlobalLightWindow == true)
	{
		GUI::Begin("Global Light", &values->GuiSettings->bShowGlobalLightWindow);

		GUI::SliderFloat3("Direction", values->GlobalLight->Data.SunDirection, -1, 1);
		GUI::SliderFloat("Overcast", &values->GlobalLight->Data.Overcast, 0, 1, "%.01f");

		GUI::End();
	}

	if (values->GuiSettings->bShowDemoWindow == true)
	{
		GUI::ShowDemoWindow(&values->GuiSettings->bShowDemoWindow);
	}
}

void ExeGui::ChangeShowGlobalLightWindow()
{
	values->GuiSettings->bShowGlobalLightWindow =
		!values->GuiSettings->bShowGlobalLightWindow;
}

void ExeGui::ChangeShowSystemInfoWindow()
{
	values->GuiSettings->bShowSystemInfoWindow =
		!values->GuiSettings->bShowSystemInfoWindow;
}

void ExeGui::ChangeShowDemoWindow()
{
	values->GuiSettings->bShowDemoWindow =
		!values->GuiSettings->bShowDemoWindow;
}

void ExeGui::ChangeShowEnvironmentWindow()
{
	values->GuiSettings->bShowEnvironmentWindow =
		!values->GuiSettings->bShowEnvironmentWindow;
}

void ExeGui::ChangeShowLandscapeWindow()
{
	values->GuiSettings->bShowLandscapeWindow =
		!values->GuiSettings->bShowLandscapeWindow;
}
