#pragma once

namespace Particle
{
	//KeyframeTable
	enum class Interpolation
	{
		None, 
		Lerp, 
		Slerp,//±¸¸é
		Spline,//°î¸é
	};

	//ParticleInfo
	enum class ParamStyles
	{
		None = 0x00,
		Random = 0x01,
		Interpolate = 0x02,
		Clamp = 0x04,
		Gravity = 0x10,
		RandomInterpolate = 0x20,
	};

	enum class ParticleObjectType
	{
		PointSprite = 0,
		Sprite = 1,
		Billboard = 2,
		Scene = 3,
		AnchorBillboard = 4,
	};

	enum class EmitObjectType
	{
		Cone = 0,
		Sphere = 1,
		Disk = 2,
		RectPlane = 3,
		CirclePlane = 4,
	};

	enum class FuncType
	{
		None = 0,
		Constant = 1,
		Sin = 2,
		Cos = 3,
		Rnd = 4,
		Table = 5,
	};

	//TimeSequence
	enum class SequenceStyle
	{
		None = 0,
		Disable,
	};

	enum class SequenceState
	{
		None = 0,
		Active,
	};

	//TextureSequence
	enum class Styles
	{
		None = 0x00000000,
		Repeat = 0x00000001,
		StaticTime = 0x00000002,
		Random = 0x00000004,
		FixedFrame = 0x00000008,
	};
}