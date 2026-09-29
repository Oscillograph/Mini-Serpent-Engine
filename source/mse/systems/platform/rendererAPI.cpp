#include "mse/systems/platform/renderer_dummy.h"
#include <mse/systems/platform/renderAPI.h>
#include <mse/systems/platform/renderer_base.h>
#include <mse/systems/platform/renderer/renderer_opengl.h>
#include <mse/systems/platform/renderer/renderer_sdl.h>

#include <mse/utils/logger.h>

namespace mse
{
	RendererBase* RendererAPI::m_Impl = nullptr;

	void RendererAPI::SetActiveRenderer(void* renderer)
	{
		m_Impl->SetActiveRenderer(renderer);
	}

	void* RendererAPI::GetActiveRenderer()
	{
		return m_Impl->GetActiveRenderer();
	}

	void RendererAPI::SetActiveScene(Scene* scene)
	{
		m_Impl->SetActiveScene(scene);
	}

	Scene* RendererAPI::GetActiveScene()
	{
		return m_Impl->GetActiveScene();
	}

	void RendererAPI::SetActiveLayer(Layer* layer)
	{
		m_Impl->SetActiveLayer(layer);
	}

	Layer* RendererAPI::GetActiveLayer()
	{
		return m_Impl->GetActiveLayer();
	}

	void RendererAPI::SetActiveWindow(Window* window)
	{
		m_Impl->SetActiveWindow(window);
	}

	Window* RendererAPI::GetActiveWindow()
	{
		return m_Impl->GetActiveWindow();
	}

	void RendererAPI::SetActiveCamera(Camera2D* camera)
	{
		m_Impl->SetActiveCamera(camera);
	}

	Camera2D* RendererAPI::GetActiveCamera()
	{
		return m_Impl->GetActiveCamera();
	};

	void RendererAPI::SetActiveScreen(const glm::uvec4& screen)
	{
		m_Impl->SetActiveScreen(screen);
	}

	void RendererAPI::SetActiveScreenDefault()
	{
		m_Impl->SetActiveScreenDefault();
	}

	glm::uvec4 RendererAPI::GetActiveScreen()
	{
		return m_Impl->GetActiveScreen();
	}

	void RendererAPI::SetBackgroundColor(const glm::uvec4& color)
	{
		m_Impl->SetBackgroundColor(color);
	}

	void RendererAPI::ClearScreen()
	{
		m_Impl->ClearScreen();
	}


	// low-level methods (draw pixels, primitives, operate with data)
	uint32_t RendererAPI::GetPixel(Texture* surface, int x, int y)
	{
		return m_Impl->GetPixel(surface, x, y);
	}

	void RendererAPI::DrawTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect)
	{
		m_Impl->DrawTexture(texture, destRect, srcRect);
	}

	void RendererAPI::DrawTiledTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, const glm::vec2& tilingFactor)
	{
		m_Impl->DrawTiledTexture(texture, destRect, srcRect, tilingFactor);
	}

	void RendererAPI::GeneralDrawTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, glm::vec2 tilingFactor, const glm::vec4& tintColor)
	{
		m_Impl->GeneralDrawTexture(texture, destRect, srcRect, tilingFactor, tintColor);
	}

	void RendererAPI::DrawRect(SDL_FPoint center, SDL_FPoint size, SDL_Color color)
	{
		m_Impl->DrawRect(center, size, color);
	}

	void RendererAPI::DrawRect(SDL_FPoint p1, SDL_FPoint p2, SDL_FPoint p3, SDL_FPoint p4, SDL_Color color)
	{
		m_Impl->DrawRect(p1, p2, p3, p4, color);
	}

	void RendererAPI::DrawRectFilled(SDL_FPoint center, SDL_FPoint size, SDL_Color color)
	{
		m_Impl->DrawRectFilled(center, size, color);
	}

	// low-level methods on surfaces (for software rendering)
	void RendererAPI::SurfaceDrawPixel_unsafe(Texture* target, SDL_Point center, int pxSize, SDL_Color color)
	{
		m_Impl->SurfaceDrawPixel_unsafe(target, center, pxSize, color);
	}

	void RendererAPI::SurfaceDrawPixel(Texture* target, SDL_Point center, int pxSize, SDL_Color color)
	{
		m_Impl->SurfaceDrawPixel(target, center, pxSize, color);
	}

	void RendererAPI::SurfaceDrawLine_unsafe(Texture* target, int x1, int y1, int x2, int y2, int pxSize, SDL_Color color)
	{
		m_Impl->SurfaceDrawLine_unsafe(target, x1, y1, x2, y2, pxSize, color);
	}

	void RendererAPI::SurfaceDrawLine(Texture* target, int x1, int y1, int x2, int y2, int pxSize, SDL_Color color)
	{
		m_Impl->SurfaceDrawLine(target, x1, y1, x2, y2, pxSize, color);
	}

	void RendererAPI::SurfaceDrawRect_unsafe(Texture* target, SDL_Rect destRect, int pxSize, SDL_Color color)
	{
		m_Impl->SurfaceDrawRect_unsafe(target, destRect, pxSize, color);
	}

	void RendererAPI::SurfaceDrawRect(Texture* target, SDL_Rect destRect, int pxSize, SDL_Color color)
	{
		m_Impl->SurfaceDrawRect(target, destRect, pxSize, color);
	}

	void RendererAPI::SurfaceDrawRectFilled_unsafe(Texture* target, SDL_Rect destRect, SDL_Color color)
	{
		m_Impl->SurfaceDrawRectFilled_unsafe(target, destRect, color);
	}

	void RendererAPI::SurfaceDrawRectFilled(Texture* target, SDL_Rect destRect, SDL_Color color)
	{
		m_Impl->SurfaceDrawRectFilled(target, destRect, color);
	}

	void RendererAPI::SurfaceDrawCircle_unsafe(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
		m_Impl->SurfaceDrawCircle_unsafe(target, center, r, pxSize, color);
	}

	void RendererAPI::SurfaceDrawCircle(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
		m_Impl->SurfaceDrawCircle(target, center, r, pxSize, color);
	}

	void RendererAPI::SurfaceDrawCircleFilled_unsafe(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
		m_Impl->SurfaceDrawCircleFilled_unsafe(target, center, r, pxSize, color);
	}

	void RendererAPI::SurfaceDrawCircleFilled(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
		m_Impl->SurfaceDrawCircleFilled(target, center, r, pxSize, color);
	}

	std::pair<int, int> RendererAPI::SurfaceDrawText_unsafe(
		Texture* target,
		const glm::ivec4& place,	// where to draw
		int pxSize, 				// size of the pen
		const std::u32string& text, 		// what to draw
		Resource* textFont,			// how is should look like
		const glm::uvec4& color,	// color
		int interval)				// interval between lines
	{
		return m_Impl->SurfaceDrawText_unsafe(target, place, pxSize, text, textFont, color, interval);
	}

	std::pair<int, int> RendererAPI::SurfaceDrawText(
		Texture* target,
		const glm::uvec4& place,	// where to draw
		int pxSize, 				// size of the pen
		const std::u32string& text, 		// what to draw
		Resource* textFont,			// how is should look like
		const glm::uvec4& color,	// color
		int interval)				// interval between lines
	{
		return m_Impl->SurfaceDrawText(target, place, pxSize, text, textFont, color, interval);
	}

	void RendererAPI::SurfaceDrawTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect)
	{
		m_Impl->SurfaceDrawTexture(target, texture, destRect, srcRect);
	}

	void RendererAPI::SurfaceDrawTiledTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, const glm::vec2& tilingFactor)
	{
		m_Impl->SurfaceDrawTiledTexture(target, texture, destRect, srcRect, tilingFactor);
	}

	void RendererAPI::SurfaceGeneralDrawTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, glm::vec2 tilingFactor, const glm::vec4& tintColor)
	{
		m_Impl->SurfaceGeneralDrawTexture(target, texture, destRect, srcRect, tilingFactor, tintColor);
	}

	// mid-level methods (advanced renderer commands)
	void RendererAPI::NewFrame()
	{
		m_Impl->NewFrame();
	}

	void RendererAPI::ShowFrame()
	{
		m_Impl->ShowFrame();
	}

	// high-level methods (complex graphics operations)
	int RendererAPI::Init(Window* window, WindowContextType windowContextType)
	{
		if (window != nullptr)
		{
			switch (windowContextType)
			{
				case mse::WindowContextType::None:
				{
					m_Impl = new RendererDummy(window);
					return 0;
					break;
				}
				case WindowContextType::OpenGL:
				{
					m_Impl = new RendererOpenGL(window);
					return 0;
					break;
				}
				case WindowContextType::SDL:
				{
					m_Impl = new RendererSDL(window);
					return 0;
					break;
				}
				default:
				{
					MSE_CORE_ERROR("RendererAPI: Unknown context type");
					return 0;
				}
			}
		} else {
			return 0;
		}
	}

	int RendererAPI::Shutdown()
	{
		delete m_Impl;
		return 0;
	}
}

