#pragma once
#include "BlazeEngine/Core/EngineCore.h"
#include "BlazeEngine/DataStructures/Color.h"
#include "BlazeEngine/DataStructures/String.h"
#include "BlazeEngine/DataStructures/Vector.h"

namespace Blaze
{		
	enum class BitmapPixelFormat {
		Red,
		RG,
		RGB,
		RGBA,
		BGR,
		BGRA,
	};
	enum class BitmapPixelType {
		Int8,
		Uint8,
		Int16,
		Uint16,
		Int32,
		Uint32,
		Float,
		Double		
	};	

	class BLAZE_API Bitmap
	{		
		BitmapPixelFormat format;
		BitmapPixelType type;
		Vec2i size;		
		void* pixels;

		Bitmap(const Bitmap&) = delete;
		void operator=(const Bitmap&) = delete;
	public:
		Bitmap();
		~Bitmap();

		bool Load(StringView path, bool emitWarning = true);
		bool Save(StringView path, bool emitWarning = true);
		
		void Create(Vec2i size, BitmapPixelFormat format, BitmapPixelType type, void* pixels);			

		inline BitmapPixelFormat GetPixelFormat() const { return format; }
		inline BitmapPixelType GetPixelType() const { return type; }		
		inline Vec2i GetSize() const { return size; }		
		inline void* GetPixels() const { return pixels; }

		friend class Texture2D;
	};

	class BLAZE_API BitmapView
	{
		void* pixels;
		Vec2i size;
		BitmapPixelFormat format;
		BitmapPixelType type;
	public:
		BitmapView();
		BitmapView(Vec2i size, BitmapPixelFormat format, BitmapPixelType type, void* pixels);
		BitmapView(const Bitmap&);
		BitmapView(const BitmapView&);

		inline void* GetPixels() const { return pixels; }
		inline Vec2i GetSize() const { return size; }
		inline BitmapPixelFormat GetPixelFormat() const { return format; }
		inline BitmapPixelType GetPixelType() const { return type; }
	};
}

