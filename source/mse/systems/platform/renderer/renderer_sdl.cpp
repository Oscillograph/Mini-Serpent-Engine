#include <mse/systems/platform/platform.h>
#include <mse/systems/platform/renderer/renderer_sdl.h>

#include <mse/utils/logger.h>

namespace mse
{
	RendererSDL::RendererSDL()
	{}

	RendererSDL::RendererSDL(Window* window)
	{}

	RendererSDL::~RendererSDL()
	{}

	void RendererSDL::SetActiveRenderer(void* renderer)
	{
	}

	void* RendererSDL::GetActiveRenderer()
	{
		return nullptr;
	}

	void RendererSDL::SetActiveScene(Scene* scene)
	{
	}

	Scene* RendererSDL::GetActiveScene()
	{
		return nullptr;
	}

	void RendererSDL::SetActiveLayer(Layer* layer)
	{
	}

	Layer* RendererSDL::GetActiveLayer()
	{
		return nullptr;
	}

	void RendererSDL::SetActiveWindow(Window* window)
	{
	}

	Window* RendererSDL::GetActiveWindow()
	{
		return nullptr;
	}

	void RendererSDL::SetActiveCamera(Camera2D* camera)
	{
	}

	Camera2D* RendererSDL::GetActiveCamera()
	{
		return nullptr;
	}

	void RendererSDL::SetActiveScreen(const glm::uvec4& screen)
	{
	}

	void RendererSDL::SetActiveScreenDefault()
	{
	}

	glm::uvec4 RendererSDL::GetActiveScreen()
	{
		return {0, 0, 0, 0};
	}

	void RendererSDL::SetBackgroundColor(const glm::uvec4& color)
	{
	}

	void RendererSDL::ClearScreen()
	{
	}


	// low-level methods (draw pixels, primitives, operate with data)
	uint32_t RendererSDL::GetPixel(Texture* surface, int x, int y)
	{
		return 0;
	}

	void RendererSDL::DrawTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect)
	{
	}

	void RendererSDL::DrawTiledTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, const glm::vec2& tilingFactor)
	{
	}

	void RendererSDL::GeneralDrawTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, glm::vec2 tilingFactor, const glm::vec4& tintColor)
	{
	}

	void RendererSDL::DrawRect(SDL_FPoint center, SDL_FPoint size, SDL_Color color)
	{
	}

	void RendererSDL::DrawRect(SDL_FPoint p1, SDL_FPoint p2, SDL_FPoint p3, SDL_FPoint p4, SDL_Color color)
	{
	}

	void RendererSDL::DrawRectFilled(SDL_FPoint center, SDL_FPoint size, SDL_Color color)
	{
	}

	// low-level methods on surfaces (for software rendering)
	void RendererSDL::SurfaceDrawPixel_unsafe(Texture* target, SDL_Point center, int pxSize, SDL_Color color)
	{
	}

	void RendererSDL::SurfaceDrawPixel(Texture* target, SDL_Point center, int pxSize, SDL_Color color)
	{
	}

	void RendererSDL::SurfaceDrawLine_unsafe(Texture* target, int x1, int y1, int x2, int y2, int pxSize, SDL_Color color)
	{
	}

	void RendererSDL::SurfaceDrawLine(Texture* target, int x1, int y1, int x2, int y2, int pxSize, SDL_Color color)
	{
	}

	void RendererSDL::SurfaceDrawRect_unsafe(Texture* target, SDL_Rect destRect, int pxSize, SDL_Color color)
	{
	}

	void RendererSDL::SurfaceDrawRect(Texture* target, SDL_Rect destRect, int pxSize, SDL_Color color)
	{
	}

	void RendererSDL::SurfaceDrawRectFilled_unsafe(Texture* target, SDL_Rect destRect, SDL_Color color)
	{
	}

	void RendererSDL::SurfaceDrawRectFilled(Texture* target, SDL_Rect destRect, SDL_Color color)
	{
	}

	void RendererSDL::SurfaceDrawCircle_unsafe(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
	}

	void RendererSDL::SurfaceDrawCircle(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
	}

	void RendererSDL::SurfaceDrawCircleFilled_unsafe(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
	}

	void RendererSDL::SurfaceDrawCircleFilled(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
	}

	std::pair<int, int> RendererSDL::SurfaceDrawText_unsafe(
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

	std::pair<int, int> RendererSDL::SurfaceDrawText(
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

	void RendererSDL::SurfaceDrawTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect)
	{
	}

	void RendererSDL::SurfaceDrawTiledTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, const glm::vec2& tilingFactor)
	{
	}

	void RendererSDL::SurfaceGeneralDrawTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, glm::vec2 tilingFactor, const glm::vec4& tintColor)
	{
	}

	// mid-level methods (advanced renderer commands)
	void RendererSDL::NewFrame()
	{
	}

	void RendererSDL::ShowFrame()
	{
	}

	// high-level methods (complex graphics operations)
	int RendererSDL::Init(Window* window, WindowContextType windowContextType)
	{
		return 0;
	}

	int RendererSDL::Shutdown()
	{
		return 0;
	}
}
