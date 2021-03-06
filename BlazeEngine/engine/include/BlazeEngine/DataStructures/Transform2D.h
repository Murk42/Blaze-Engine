#pragma once
#include "BlazeEngine/Core/EngineCore.h"
#include "BlazeEngine/DataStructures/Vector.h"
#include "BlazeEngine/DataStructures/Matrix.h"
#include "BlazeEngine/Math/Math.h"

namespace Blaze
{
	enum class Align
	{
		Top,
		TopRight,
		Right,
		BottomRight,
		Bottom,
		BottomLeft,
		Left,
		TopLeft,
		Center
	};

	class BLAZE_API Transform2D
	{
	public:
		Vec2f pos;
		float depth;
		Vec2f size;
		float scale;
		Align align;
		
		Vec2f absolutePos;
		Vec2f absoluteSize;

		Align parentAlign;
		Transform2D* parent;

		Mat4f mat;

		bool dirty;

		Transform2D();		

		void Update();
	};
}