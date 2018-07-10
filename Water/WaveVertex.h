#pragma once

namespace Water
{
	struct WaveVertexDesc
	{
		float SpatialStep = 5.0f;
		float TimeStep = 0.07f;
		float Speed = 3.25f;
		float Damping = 0.4f;

		bool Equal(const WaveVertexDesc& val)
		{
			bool b = true;
			b &= SpatialStep == val.SpatialStep;
			b &= TimeStep == val.TimeStep;
			b &= Speed == val.Speed;
			b &= Damping == val.Damping;

			return b;
		}

		bool operator==(const WaveVertexDesc& val)
		{
			return Equal(val);
		}

		bool operator!=(const WaveVertexDesc& val)
		{
			return !Equal(val);
		}
	};

	class WaveVertex
	{
	public:
		friend class Wave;

	public:
		WaveVertex(ExecuteValues* values, UINT width, UINT height, float thick);
		~WaveVertex();

		void Update();
		void Render();
		void PostRender();

	private:
		void CreateBuffer();

		void ReCalcValues();
		void Disturb(UINT x, UINT y, float magnitude);

	private:
		ExecuteValues* values;
		WaveVertexDesc desc;

		ID3D11Buffer* vertexBuffer;
		vector<WaveVertexType> vertices;

		ID3D11Buffer* indexBuffer;
		vector<UINT> indices;

		bool bStop;

		UINT width;
		UINT height;
		float thick;

		float time;
		float k[3];

		vector<WaveVertexType> prev;
		vector<D3DXVECTOR3> tangent;

	};
}
