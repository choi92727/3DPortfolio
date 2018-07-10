#pragma once
#include "stdafx.h"
#include "Enums.h"

namespace Particle
{
	struct TextureSequence
	{
		string TextureFileName = "";
		bool IsUseStaticTime = true;
		bool IsRepeat = false;
		bool IsRandomMode = false;
		bool IsFixedFrameMode = false;
		float FrameWidth = 0.0f;     //  Width rate (maximum is 1.0)
		float FrameHeight = 0.0f;    //  Height rate (maximum is 1.0)
		float StaticInterval = 0.01666667f;    // 60 FPS
		UINT Count = 0;
		UINT StartIndex = 0;
		vector<float> TimeTable;

		// Volatile Members        
		UINT style = 0;
		int lineCount = 0;
		float frameWidthPixel = 0.0f;
		float frameHeightPixel = 0.0f;
		float rInterval = 0.0f;
		float widthFactor = 0.0f;
		float heightFactor = 0.0f;
		float duration = 0.0f;
		Texture* texture = nullptr;

		Texture* GeTexture()
		{
			return texture;
		}

		bool GetUseStaticTime()
		{
			return IsUseStaticTime;
		}

		void SetUseStaticTime(bool value)
		{
			IsUseStaticTime = value;

			if (value)
			{
				style |= (UINT)Styles::StaticTime;
			}
			else
			{
				style &= ~(UINT)Styles::StaticTime;

				if (TimeTable.size() > 0)
					TimeTable.clear();
			}
		}

		void LoadTexture(string file)
		{
			wstring temp = String::StringToWString(file);
			texture = new Texture(temp);
		}

		void SetTexture(Texture* texture)
		{
			this->texture = texture;

			CalcParams();
		}

		void CalcParams()
		{
			if (texture == nullptr)
				return;

			if (texture->GetWidth() == 0 || texture->GetHeight() == 0)
				return;

			if (StaticInterval == 0.0f)
				return;

			duration = StaticInterval * Count;
			rInterval = 1.0f / StaticInterval;

			frameWidthPixel = (float)texture->GetWidth() * FrameWidth;
			frameHeightPixel = (float)texture->GetHeight() * FrameHeight;

			lineCount = (int)((float)texture->GetWidth() / frameWidthPixel);

			widthFactor = frameWidthPixel / (float)texture->GetWidth();
			heightFactor = frameHeightPixel / (float)texture->GetHeight();

			SetUseStaticTime(IsUseStaticTime);
		}

		void GetUV(float time, OUT D3DXVECTOR2* uv1, OUT D3DXVECTOR2* uv2)
		{
			UINT x = 0, y = 0, idx = 0;
			float u = 0.0f, v = 0.0f;

			if (IsRepeat)
				time = Math::Moudulo(time, duration);

			if (IsRandomMode)
				idx = (UINT)Math::Random(0,Count);
			else
			{
				if (IsUseStaticTime)
				{
					//  Calculate index by static interval time
					idx = (UINT)(time * rInterval);

					if (idx >= Count)
						idx = Count - 1;
				}
				else
				{
					UINT start = 0;
					UINT end = Count - 1;

					//  Calculate index by dynamic interval time
					do
					{
						idx = (start + end) / 2;

						if (TimeTable[(int)idx] > time)
							end = idx - 1;
						else
							start = idx + 1;
					} while (start < end);
				}
			}

			idx += StartIndex;

			y = idx / (UINT)lineCount;
			v = y * heightFactor;

			x = idx % (UINT)lineCount;
			u = x * widthFactor;

			*uv1 =D3DXVECTOR2(u, v);
			*uv2 =D3DXVECTOR2(u + widthFactor, v + heightFactor);
		}
	};
}