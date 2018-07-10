#pragma once
#include "stdafx.h"
#include "Enums.h"
#include "TextureSequence.h"
#include "ParticleInfo.h"
namespace Particle
{
	struct TimeSequenceInfo
	{
		float StartTime = 0.0f;
		float Duration = 0.0f;
		SequenceStyle Style = SequenceStyle::None;
		ParticleInfo* ParticleInfo = NULL;
		TextureSequence* TextureSequence = NULL;

		bool operator ==(const TimeSequenceInfo& val)
		{
			bool b = true;
			b &= StartTime == val.StartTime;
			b &= Duration == val.Duration;
			b &= Style == val.Style;
			b &= ParticleInfo == val.ParticleInfo;
			b &= TextureSequence == val.TextureSequence;

			return b;
		}
	};


	struct TimeSequenceData
	{
		TimeSequenceInfo sequenceInfo;
		class TimeSequence* owner = NULL;

		SequenceStyle style = SequenceStyle::None;
		SequenceState state = SequenceState::None;
		SequenceStyle GetStyle()
		{
			return style;
		}
		void SetStyle(SequenceStyle style)
		{
			this->style = style;
		}
		SequenceState GetState()
		{
			return state;
		}
		void SetState(SequenceState state)
		{
			this->state = state;

		}
		TimeSequence* GetOwner() { return owner; }

		void SetOwner(TimeSequence* owner) { this->owner = owner; }

		TimeSequenceInfo GetSequenceInfo() { return sequenceInfo; }

		bool operator==(const TimeSequenceData& val)
		{
			bool b = true;
			b &= sequenceInfo == val.sequenceInfo;
			b &= owner == val.owner;
			b &= style == val.style;
			b &= state == val.state;

			return b;
		}



		TimeSequenceData(TimeSequenceInfo info)
		{
			sequenceInfo = info;

			if (sequenceInfo.ParticleInfo != NULL)
				sequenceInfo.ParticleInfo->Initialize();

			SetStyle(info.Style);
		}
	};


	struct TimeSequence
	{

		vector<TimeSequenceData> timeSequenceDataList;
		bool enable;
		bool visible;
		float localTime = 0.0f;
		float duration = 0.0f;
		bool active = false;     //  default is false
		bool infinite = false;



		bool IsActive() { return active; }

		bool IsInfinite() { return infinite; }

		float GetlocalTime() { return localTime; }
		void SetlocalTime(float value) { localTime = value; }


		size_t GetCount() { return timeSequenceDataList.size(); }


		static void SetOwnerEnable(TimeSequenceData data, bool enable)
		{
			if (data.GetOwner() != NULL)
			{
				data.GetOwner()->enable = enable;
				data.GetOwner()->visible = enable;
			}
		}

		void AddSequence(TimeSequenceData data)
		{
			timeSequenceDataList.push_back(data);

			SetOwnerEnable(data, false);

			UpdateDuration();
		}
		void RemoveSequence(TimeSequenceData data)
		{
			size_t i = 0;
			for (; i < timeSequenceDataList.size(); i++)
			{
				if (timeSequenceDataList[i] == data)
					break;
			}
			timeSequenceDataList.erase(timeSequenceDataList.begin() + i);
		}


		void RemoveSequene(int index)
		{
			timeSequenceDataList.erase(timeSequenceDataList.begin() + index);

			UpdateDuration();
		}

		/// <summary>
		/// removes all time data
		/// </summary>
		void RemoveAllSequence()
		{
			timeSequenceDataList.clear();

			duration = 0.0f;
		}


		TimeSequenceData GetSequence(int index)
		{
			return timeSequenceDataList[index];
		}


		void UpdateDuration()
		{
			duration = 0.0f;
			infinite = false;

			for (size_t i = 0; i < timeSequenceDataList.size(); i++)
			{
				TimeSequenceInfo info = timeSequenceDataList[i].sequenceInfo;

				if (info.Duration <= 0.0f)
					infinite = true;

				if (info.StartTime + info.Duration > duration)
					duration = info.StartTime + info.Duration;
			}
		}


		void Reset()
		{
			localTime = 0.0f;
			active = true;

			for (size_t i = 0; i < timeSequenceDataList.size(); i++)
			{
				TimeSequenceData data = timeSequenceDataList[i];

				data.state = SequenceState::None;
				SetOwnerEnable(data, false);

				/*      if (data.Owner is Particle)
				{
				Particle particle = data.Owner as Particle;

				particle.Stop();
				particle.Start();
				}*/
			}
		}


		void Stop()
		{
			localTime = 0.0f;
			active = false;

			for (size_t i = 0; i < timeSequenceDataList.size(); i++)
			{
				TimeSequenceData data = timeSequenceDataList[i];

				data.state = SequenceState::None;
				SetOwnerEnable(data, false);

				/*if (data.Owner is Particle)
				{
				Particle particle = data.Owner as Particle;

				particle.Stop();
				}*/
			}
		}
		void SetRefMatrix(bool enabled, D3DXMATRIX* matrix)
		{
			for (size_t i = 0; i < timeSequenceDataList.size(); i++)
			{
				TimeSequenceData data = timeSequenceDataList[i];

				/*if (data.Owner is Particle)
				{
				Particle particle = data.Owner as Particle;

				particle.SetRefMatrixEnable(enabled);
				particle.SetRefMatrix(matrix);
				}*/
			}
		}


		void OnUpdate()
		{
			if (infinite == false && !active)
				return;

			for (size_t i = 0; i < timeSequenceDataList.size(); i++)
			{
				TimeSequenceData data = timeSequenceDataList[i];

				if (data.style == SequenceStyle::Disable)
					continue;

				if (data.state == SequenceState::Active)
				{
					//  If activate
					if (data.sequenceInfo.Duration > 0.0f)
					{
						// If the Duration is 0, time is infinite
						if (localTime >= (data.sequenceInfo.StartTime +
							data.sequenceInfo.Duration))
						{
							data.state = SequenceState::None;
							SetOwnerEnable(data, false);
						}
					}
				}
				else
				{
					//  Starting time..
					if (localTime >= data.sequenceInfo.StartTime)
					{
						data.state = SequenceState::Active;
						SetOwnerEnable(data, true);
					}
				}
			}

			localTime += (float)Time::Get()->Delta();

			//  Finished
			if (localTime > duration)
			{
				active = false;
			}
		}
	};
}