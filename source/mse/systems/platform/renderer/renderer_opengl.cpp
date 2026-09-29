#include <mse/systems/platform/platform.h>
#include <mse/systems/platform/renderer/renderer_opengl.h>

#include <mse/utils/logger.h>

namespace mse
{
	RendererOpenGL::RendererOpenGL()
	{}

	RendererOpenGL::RendererOpenGL(Window* window)
	{}

	RendererOpenGL::~RendererOpenGL()
	{}

	void RendererOpenGL::SetActiveRenderer(void* renderer)
	{
	}

	void* RendererOpenGL::GetActiveRenderer()
	{
		return nullptr;
	}

	void RendererOpenGL::SetActiveScene(Scene* scene)
	{
	}

	Scene* RendererOpenGL::GetActiveScene()
	{
		return nullptr;
	}

	void RendererOpenGL::SetActiveLayer(Layer* layer)
	{
	}

	Layer* RendererOpenGL::GetActiveLayer()
	{
		return nullptr;
	}

	void RendererOpenGL::SetActiveWindow(Window* window)
	{
	}

	Window* RendererOpenGL::GetActiveWindow()
	{
		return nullptr;
	}

	void RendererOpenGL::SetActiveCamera(Camera2D* camera)
	{
	}

	Camera2D* RendererOpenGL::GetActiveCamera()
	{
		return nullptr;
	}

	void RendererOpenGL::SetActiveScreen(const glm::uvec4& screen)
	{
	}

	void RendererOpenGL::SetActiveScreenDefault()
	{
	}

	glm::uvec4 RendererOpenGL::GetActiveScreen()
	{
		return {0, 0, 0, 0};
	}

	void RendererOpenGL::SetBackgroundColor(const glm::uvec4& color)
	{
	}

	void RendererOpenGL::ClearScreen()
	{
	}


	// low-level methods (draw pixels, primitives, operate with data)
	uint32_t RendererOpenGL::GetPixel(Texture* surface, int x, int y)
	{
		return 0;
	}

	void RendererOpenGL::DrawTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect)
	{
	}

	void RendererOpenGL::DrawTiledTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, const glm::vec2& tilingFactor)
	{
	}

	void RendererOpenGL::GeneralDrawTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, glm::vec2 tilingFactor, const glm::vec4& tintColor)
	{
	}

	void RendererOpenGL::DrawRect(SDL_FPoint center, SDL_FPoint size, SDL_Color color)
	{
	}

	void RendererOpenGL::DrawRect(SDL_FPoint p1, SDL_FPoint p2, SDL_FPoint p3, SDL_FPoint p4, SDL_Color color)
	{
	}

	void RendererOpenGL::DrawRectFilled(SDL_FPoint center, SDL_FPoint size, SDL_Color color)
	{
	}

	// low-level methods on surfaces (for software rendering)
	void RendererOpenGL::SurfaceDrawPixel_unsafe(Texture* target, SDL_Point center, int pxSize, SDL_Color color)
	{
	}

	void RendererOpenGL::SurfaceDrawPixel(Texture* target, SDL_Point center, int pxSize, SDL_Color color)
	{
	}

	void RendererOpenGL::SurfaceDrawLine_unsafe(Texture* target, int x1, int y1, int x2, int y2, int pxSize, SDL_Color color)
	{
	}

	void RendererOpenGL::SurfaceDrawLine(Texture* target, int x1, int y1, int x2, int y2, int pxSize, SDL_Color color)
	{
	}

	void RendererOpenGL::SurfaceDrawRect_unsafe(Texture* target, SDL_Rect destRect, int pxSize, SDL_Color color)
	{
	}

	void RendererOpenGL::SurfaceDrawRect(Texture* target, SDL_Rect destRect, int pxSize, SDL_Color color)
	{
	}

	void RendererOpenGL::SurfaceDrawRectFilled_unsafe(Texture* target, SDL_Rect destRect, SDL_Color color)
	{
	}

	void RendererOpenGL::SurfaceDrawRectFilled(Texture* target, SDL_Rect destRect, SDL_Color color)
	{
	}

	void RendererOpenGL::SurfaceDrawCircle_unsafe(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
	}

	void RendererOpenGL::SurfaceDrawCircle(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
	}

	void RendererOpenGL::SurfaceDrawCircleFilled_unsafe(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
	}

	void RendererOpenGL::SurfaceDrawCircleFilled(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
	}

	std::pair<int, int> RendererOpenGL::SurfaceDrawText_unsafe(
		Texture* target,
		const glm::ivec4& place,	// where to draw
		int pxSize, 				// size of the pen
		const std::u32string& text, 		// what to draw
		Resource* textFont,			// how is should look like
		const glm::uvec4& color,	// color
		int interval)				// interval between lines
	{
		return {0, 0};
	}

	std::pair<int, int> RendererOpenGL::SurfaceDrawText(
		Texture* target,
		const glm::uvec4& place,	// where to draw
		int pxSize, 				// size of the pen
		const std::u32string& text, 		// what to draw
		Resource* textFont,			// how is should look like
		const glm::uvec4& color,	// color
		int interval)				// interval between lines
	{
		return {0, 0};
	}

	void RendererOpenGL::SurfaceDrawTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect)
	{
	}

	void RendererOpenGL::SurfaceDrawTiledTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, const glm::vec2& tilingFactor)
	{
	}

	void RendererOpenGL::SurfaceGeneralDrawTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, glm::vec2 tilingFactor, const glm::vec4& tintColor)
	{
	}

	// mid-level methods (advanced renderer commands)
	void RendererOpenGL::NewFrame()
	{
	}

	void RendererOpenGL::ShowFrame()
	{
	}

	// high-level methods (complex graphics operations)
	int RendererOpenGL::Init(Window* window, WindowContextType windowContextType)
	{
		return 0;
	}

	int RendererOpenGL::Shutdown()
	{
		return 0;
	}
}
