#pragma once
#include "stdafx.h"
#include "Enums.h"

namespace Particle
{
	struct KeyframeTable
	{
		int Count = 0;
		float Step = 0.0f;
		bool bFixedInterval = true;
		vector<float> Table;
		vector<float> Time;

		void Initialize()
		{
			Step = (Count == 0) ? 0.0f : 1.0 / (float)Count;
			Table.clear();
			Time.clear();
		}

		float GetKeyframe(float t, Interpolation mode)
		{
			int index = -1;
			float speed = 0.0f;

			if (Time.size() > 0)
				index = GetTimeIndex(t);
			else
				index = GetIndex(t);

			if (index >= Count)
				index = Count - 1;

			switch (mode)
			{
			case Interpolation::Lerp:
			{
				if (index < (Count - 1))
				{
					if (Time.size() > 0)//TODO
						speed = (t - Time[index + 1] - Time[index]); // speed = (t - Time[index]) / (Time[index + 1] - Time[index]);
					else
						speed = (t - ((float)index * Step)) * (float)Count;

					return Table[index] + ((Table[index + 1] - Table[index]) * speed);
				}

				return Table[index];
			}
			break;

			default:
				return Table[index];
			}
		}

		int GetIndex(float t)
		{
			return (int)(t * Count);
		}

		int GetTimeIndex(float t)
		{
			int s = 0, m = 0, e = Count - 1;

			if (t >= Time[e])
				return e;

			do
			{
				m = (s + e) * 0.5f;

				if ((e - s) <= 1) break;
				else if (Time[m] < t) s = m;
				else if (Time[m] > t) e = m;
				else
				{
					s = m;
					break;
				}
			} while ((e - s) > 1);

			return s;
		}
	};
}