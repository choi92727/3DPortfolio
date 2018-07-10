#pragma once

namespace Landscape
{
	class FogVSBuffer : public ShaderBuffer
	{
	public:
		FogVSBuffer()
			: ShaderBuffer(&Data, sizeof(Data))
		{
			Data.fogStart = 100;
			Data.fogEnd = 400;
		}

		struct Struct
		{
			float fogStart;
			float fogEnd;
			float Padding[2];
		} Data;
	};

	class FogPSBuffer : public ShaderBuffer
	{
	public:
		FogPSBuffer()
			: ShaderBuffer(&Data, sizeof(Data))
		{
			Data.Color = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1);
		}

		struct Struct
		{
			D3DXCOLOR Color;
		} Data;
	};


	class Fog
	{
	public:
		Fog(ExecuteValues* values);
		~Fog();

		void Render();
		void PostRender();

	private:
		ExecuteValues* values;

		FogVSBuffer* vsBuffer;
		FogPSBuffer* psBuffer;
	};
}
