#pragma once
#include "BlazeEngine/Core/EngineCore.h"
#include "BlazeEngine/DataStructures/Vector.h"  
#include "BlazeEngine/DataStructures/Color.h" 
#include "BlazeEngine/DataStructures/Matrix.h"
#include "BlazeEngine/DataStructures/StringViewUTF8.h"
#include "BlazeEngine/Graphics/Renderers/Line2DRenderer.h"
#include "BlazeEngine/Graphics/Renderers/Line3DRenderer.h"
#include "BlazeEngine/Graphics/Renderers/Point2DRenderer.h"
#include "BlazeEngine/Graphics/Renderers/TextRenderer.h"
   
namespace Blaze 
{   
	class Font;

	namespace Graphics
	{

		BLAZE_API void DrawPoint2D(Vec2f pos, ColorRGBA color, float radius);		
		BLAZE_API void DrawLine2D(Vec2f pos1, Vec2f pos2, ColorRGBA color, float width);
		//BLAZE_API void DrawCircleOutline(Vec2f pos, float radius, ColorRGBA color, float width);
		//BLAZE_API void DrawCircle(Vec2f pos, float radius, ColorRGBA color, float width);
		BLAZE_API void DrawBoxOutline2D(Vec2f pos1, Vec2f pos2, ColorRGBA color, float width);
		//BLAZE_API void DrawBox2D(Vec2f pos1, Vec2f pos2, ColorRGBA color, float width);

		//BLAZE_API void DrawPoint3D(Vec3f pos, ColorRGBA color, float radius);
		BLAZE_API void DrawLine3D(Vec3f pos1, Vec3f pos2, ColorRGBA color, float width);
		BLAZE_API void DrawBoxOutline3D(Vec3f pos1, Vec3f pos2, ColorRGBA color, float width);
		//BLAZE_API void DrawBox3D(Vec3f pos1, Vec3f pos2, ColorRGBA color, float width);

		BLAZE_API void Write(const StringViewUTF8& text, float resolution, Vec2f pos, ColorRGBA color);
		BLAZE_API void Write(TextRenderData& data, Vec2f pos, ColorRGBA color);

		BLAZE_API TextRenderData GetTextRenderData(const StringViewUTF8& text, float height);

		BLAZE_API Point2DRenderer& GetPoint2DRenderer();
		BLAZE_API Line2DRenderer& GetLine2DRenderer();
		BLAZE_API Line3DRenderer& GetLine3DRenderer();
		BLAZE_API TextRenderer& GetTextRenderer();

		BLAZE_API Font& GetDefaultFont();		

		BLAZE_API void Set3DViewMatrix(const Mat4f&);		
		BLAZE_API void Set3DProjectionMatrix(const Mat4f&);		
		BLAZE_API Mat4f Get3DViewMatrix();
		BLAZE_API Mat4f Get3DProjectionMatrix();
		BLAZE_API void Flush();

		BLAZE_API Vec3f WorldToScreenSpace(Vec3f position);
	}
}