#include "pch.h"
#include "BlazeEngineGraphics/Core/OpenGL/OpenGLWrapper/OpenGLCommon.h"

namespace Blaze::Graphics::OpenGLWrapper
{
	Result OpenGLResult;

	bool FlushOpenGLResult()
	{
		bool wasFull = OpenGLResult.operator bool();
		OpenGLResult = Result();
		return wasFull;
	} 

}