#include <mse/systems/platform/renderer_dummy.h>
#include <mse/systems/platform/platform.h>

#include <mse/utils/logger.h>

namespace mse
{
	void RendererDummy::SetActiveRenderer(void* renderer)
	{
	}

	void* RendererDummy::GetActiveRenderer()
	{
		return nullptr;
	}

	void RendererDummy::SetActiveScene(Scene* scene)
	{
	}

	Scene* RendererDummy::GetActiveScene()
	{
		return nullptr;
	}

	void RendererDummy::SetActiveLayer(Layer* layer)
	{
	}

	Layer* RendererDummy::GetActiveLayer()
	{
		return nullptr;
	}

	void RendererDummy::SetActiveWindow(Window* window)
	{
	}

	Window* RendererDummy::GetActiveWindow()
	{
		return nullptr;
	}

	void RendererDummy::SetActiveCamera(Camera2D* camera)
	{
	}

	Camera2D* RendererDummy::GetActiveCamera()
	{
		return nullptr;
	}

	void RendererDummy::SetActiveScreen(const glm::uvec4& screen)
	{
	}

	void RendererDummy::SetActiveScreenDefault()
	{
	}

	glm::uvec4 RendererDummy::GetActiveScreen()
	{
		return {0, 0, 0, 0};
	}

	void RendererDummy::SetBackgroundColor(const glm::uvec4& color)
	{
	}

	void RendererDummy::ClearScreen()
	{
	}


	// low-level methods (draw pixels, primitives, operate with data)
	uint32_t RendererDummy::GetPixel(Texture* surface, int x, int y)
	{
		return 0;
	}

	void RendererDummy::DrawTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect)
	{
	}

	void RendererDummy::DrawTiledTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, const glm::vec2& tilingFactor)
	{
	}

	void RendererDummy::GeneralDrawTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, glm::vec2 tilingFactor, const glm::vec4& tintColor)
	{
	}

	void RendererDummy::DrawRect(SDL_FPoint center, SDL_FPoint size, SDL_Color color)
	{
	}

	void RendererDummy::DrawRect(SDL_FPoint p1, SDL_FPoint p2, SDL_FPoint p3, SDL_FPoint p4, SDL_Color color)
	{
	}

	void RendererDummy::DrawRectFilled(SDL_FPoint center, SDL_FPoint size, SDL_Color color)
	{
	}

	// low-level methods on surfaces (for software rendering)
	void RendererDummy::SurfaceDrawPixel_unsafe(Texture* target, SDL_Point center, int pxSize, SDL_Color color)
	{
	}

	void RendererDummy::SurfaceDrawPixel(Texture* target, SDL_Point center, int pxSize, SDL_Color color)
	{
	}

	void RendererDummy::SurfaceDrawLine_unsafe(Texture* target, int x1, int y1, int x2, int y2, int pxSize, SDL_Color color)
	{
	}

	void RendererDummy::SurfaceDrawLine(Texture* target, int x1, int y1, int x2, int y2, int pxSize, SDL_Color color)
	{
	}

	void RendererDummy::SurfaceDrawRect_unsafe(Texture* target, SDL_Rect destRect, int pxSize, SDL_Color color)
	{
	}

	void RendererDummy::SurfaceDrawRect(Texture* target, SDL_Rect destRect, int pxSize, SDL_Color color)
	{
	}

	void RendererDummy::SurfaceDrawRectFilled_unsafe(Texture* target, SDL_Rect destRect, SDL_Color color)
	{
	}

	void RendererDummy::SurfaceDrawRectFilled(Texture* target, SDL_Rect destRect, SDL_Color color)
	{
	}

	void RendererDummy::SurfaceDrawCircle_unsafe(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
	}

	void RendererDummy::SurfaceDrawCircle(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
	}

	void RendererDummy::SurfaceDrawCircleFilled_unsafe(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
	}

	void RendererDummy::SurfaceDrawCircleFilled(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
	}

	std::pair<int, int> RendererDummy::SurfaceDrawText_unsafe(
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

	std::pair<int, int> RendererDummy::SurfaceDrawText(
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

	void RendererDummy::SurfaceDrawTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect)
	{
	}

	void RendererDummy::SurfaceDrawTiledTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, const glm::vec2& tilingFactor)
	{
	}

	void RendererDummy::SurfaceGeneralDrawTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, glm::vec2 tilingFactor, const glm::vec4& tintColor)
	{
	}

	// mid-level methods (advanced renderer commands)
	void RendererDummy::NewFrame()
	{
	}

	void RendererDummy::ShowFrame()
	{
	}

	// high-level methods (complex graphics operations)
	int RendererDummy::Init(Window* window, WindowContextType windowContextType)
	{
		return 0;
	}

	int RendererDummy::Shutdown()
	{
		return 0;
	}
}
