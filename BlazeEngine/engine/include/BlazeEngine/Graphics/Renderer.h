#pragma once
#include "BlazeEngine/Core/EngineCore.h"
#include "BlazeEngine/DataStructures/Color.h"
#include "BlazeEngine/DataStructures/Vector.h"
#include "BlazeEngine/DataStructures/Common.h"

#include "BlazeEngine/Graphics/Mesh/Mesh.h"
#include "BlazeEngine/Graphics/Material/Material.h"

#include "BlazeEngine/Graphics/GraphicsLibrary.h"

namespace Blaze
{
	class Window;		

	enum class IndexType
	{
		UnsignedInt = 5125,
	};
	enum class PolygonMode
	{
		Point = 6912,
		Line = 6913,
		Fill = 6914
	};

	namespace Renderer
	{
		BLAZE_API void SetActiveTextureSlot(uint slot);

		BLAZE_API void SelectTexture(Graphics::Core::Texture2D*);
		BLAZE_API Graphics::Core::Texture2D* GetSelectedTexture2D();

		BLAZE_API void SelectTexture(Graphics::Core::Texture2DArray*);		
		BLAZE_API Graphics::Core::Texture2DArray* GetSelectedTexture2DArray();

		BLAZE_API void SelectTexture(Graphics::Core::TextureCubemap*);		
		BLAZE_API Graphics::Core::TextureCubemap* GetSelectedTextureCubemap();

		BLAZE_API void SelectVertexBuffer(Graphics::Core::GraphicsBuffer* buffer);		
		BLAZE_API Graphics::Core::GraphicsBuffer* GetSelectedVertexBuffer();

		BLAZE_API void SelectIndexBuffer(Graphics::Core::GraphicsBuffer* buffer);		
		BLAZE_API Graphics::Core::GraphicsBuffer* GetSelectedIndexBuffer();

		BLAZE_API void SelectUniformBuffer(Graphics::Core::GraphicsBuffer* buffer);		
		BLAZE_API Graphics::Core::GraphicsBuffer* GetSelectedUniformBuffer();

		BLAZE_API void SelectVertexArray(Graphics::Core::VertexArray* vertexArray);		
		BLAZE_API Graphics::Core::VertexArray* GetSelectedVertexArray();

		BLAZE_API void SelectProgram(Graphics::Core::Program* program);		
		BLAZE_API Graphics::Core::Program* GetSelectedProgram();

		BLAZE_API void SelectFramebuffer(Graphics::Core::Framebuffer* framebuffer);
		BLAZE_API Graphics::Core::Framebuffer* GetSelectedFramebuffer();


		BLAZE_API void SetClearColor(ColorRGBAf);
		BLAZE_API void SetViewport(Vec2i pos, Vec2i size);
		BLAZE_API void SetPatchSize(uint size);
		BLAZE_API void SetPointSize(uint size);
		BLAZE_API void SetScissorRect(Vec2i pos, Vec2i size);

		BLAZE_API uint GetMaxBoundTextures();
		BLAZE_API ColorRGBAf GetClearColor();
		BLAZE_API Vec2i GetViewportPos();
		BLAZE_API Vec2i GetViewportSize(); 
		BLAZE_API float GetViewportRatio();

		BLAZE_API void ClearTarget();
		BLAZE_API void ClearTargetColor();
		BLAZE_API void ClearTargetDepth();
		BLAZE_API void UpdateTarget();

		BLAZE_API void SetTarget(Window& win);
		BLAZE_API void SetPolygonMode(PolygonMode mode);

		BLAZE_API void EnableVSync(bool enable);
		BLAZE_API void EnableDepthBuffer(bool enable);
		BLAZE_API void EnableBlending(bool enable);		
		BLAZE_API void EnableProgramPointSize(bool enable);
		BLAZE_API void EnableFaceCulling(bool enable);		
		BLAZE_API void EnableDepthTest(bool enable);
		BLAZE_API void EnableScissorTest(bool enable);

		BLAZE_API void RenderTriangles(IndexType indexType, uint indexCount);
		BLAZE_API void RenderPatches(IndexType indexType, uint count);
		BLAZE_API void RenderTriangleArray(uint vertexCount, size_t vertexOffset);
		BLAZE_API void RenderPointArray(uint vertexCount, size_t vertexOffset);		
		BLAZE_API void RenderPatchArray(uint vertexCount, size_t vertexOffset);
	}
}