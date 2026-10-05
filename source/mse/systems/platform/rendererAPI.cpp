#include "mse/systems/platform/renderer_dummy.h"
#include <mse/systems/platform/rendererAPI.h>
#include <mse/systems/platform/renderer_base.h>
#include <mse/systems/platform/renderer/renderer_opengl.h>
#include <mse/systems/platform/renderer/renderer_sdl.h>
#include <mse/systems/windows/window.h>

#include <mse/utils/logger.h>

namespace mse
{
	RendererBase* Renderer::m_Impl = nullptr;

	void Renderer::SetActiveRenderer(void* renderer)
	{
		m_Impl->SetActiveRenderer(renderer);
	}

	void* Renderer::GetActiveRenderer()
	{
		return m_Impl->GetActiveRenderer();
	}

	void Renderer::SetActiveScene(Scene* scene)
	{
		m_Impl->SetActiveScene(scene);
	}

	Scene* Renderer::GetActiveScene()
	{
		return m_Impl->GetActiveScene();
	}

	void Renderer::SetActiveLayer(Layer* layer)
	{
		m_Impl->SetActiveLayer(layer);
	}

	Layer* Renderer::GetActiveLayer()
	{
		return m_Impl->GetActiveLayer();
	}

	void Renderer::SetActiveWindow(Window* window)
	{
		m_Impl = window->GetRenderer();
		// m_Impl->SetActiveWindow(window);
	}

	Window* Renderer::GetActiveWindow()
	{
		return nullptr;
	}

	void Renderer::SetActiveCamera(Camera2D* camera)
	{
		m_Impl->SetActiveCamera(camera);
	}

	Camera2D* Renderer::GetActiveCamera()
	{
		return m_Impl->GetActiveCamera();
	};

	void Renderer::SetActiveScreen(const glm::uvec4& screen)
	{
		m_Impl->SetActiveScreen(screen);
	}

	void Renderer::SetActiveScreenDefault()
	{
		m_Impl->SetActiveScreenDefault();
	}

	glm::uvec4 Renderer::GetActiveScreen()
	{
		return m_Impl->GetActiveScreen();
	}

	void Renderer::SetBackgroundColor(const glm::uvec4& color)
	{
		m_Impl->SetBackgroundColor(color);
	}

	void Renderer::ClearScreen()
	{
		m_Impl->ClearScreen();
	}


	// low-level methods (draw pixels, primitives, operate with data)
	uint32_t Renderer::GetPixel(Texture* surface, int x, int y)
	{
		return m_Impl->GetPixel(surface, x, y);
	}

	void Renderer::DrawTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect)
	{
		m_Impl->DrawTexture(texture, destRect, srcRect);
	}

	void Renderer::DrawTiledTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, const glm::vec2& tilingFactor)
	{
		m_Impl->DrawTiledTexture(texture, destRect, srcRect, tilingFactor);
	}

	void Renderer::GeneralDrawTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, glm::vec2 tilingFactor, const glm::vec4& tintColor)
	{
		m_Impl->GeneralDrawTexture(texture, destRect, srcRect, tilingFactor, tintColor);
	}

	void Renderer::DrawRect(SDL_FPoint center, SDL_FPoint size, SDL_Color color)
	{
		m_Impl->DrawRect(center, size, color);
	}

	void Renderer::DrawRect(SDL_FPoint p1, SDL_FPoint p2, SDL_FPoint p3, SDL_FPoint p4, SDL_Color color)
	{
		m_Impl->DrawRect(p1, p2, p3, p4, color);
	}

	void Renderer::DrawRectFilled(SDL_FPoint center, SDL_FPoint size, SDL_Color color)
	{
		m_Impl->DrawRectFilled(center, size, color);
	}

	// low-level methods on surfaces (for software rendering)
	void Renderer::SurfaceDrawPixel_unsafe(Texture* target, SDL_Point center, int pxSize, SDL_Color color)
	{
		m_Impl->SurfaceDrawPixel_unsafe(target, center, pxSize, color);
	}

	void Renderer::SurfaceDrawPixel(Texture* target, SDL_Point center, int pxSize, SDL_Color color)
	{
		m_Impl->SurfaceDrawPixel(target, center, pxSize, color);
	}

	void Renderer::SurfaceDrawLine_unsafe(Texture* target, int x1, int y1, int x2, int y2, int pxSize, SDL_Color color)
	{
		m_Impl->SurfaceDrawLine_unsafe(target, x1, y1, x2, y2, pxSize, color);
	}

	void Renderer::SurfaceDrawLine(Texture* target, int x1, int y1, int x2, int y2, int pxSize, SDL_Color color)
	{
		m_Impl->SurfaceDrawLine(target, x1, y1, x2, y2, pxSize, color);
	}

	void Renderer::SurfaceDrawRect_unsafe(Texture* target, SDL_Rect destRect, int pxSize, SDL_Color color)
	{
		m_Impl->SurfaceDrawRect_unsafe(target, destRect, pxSize, color);
	}

	void Renderer::SurfaceDrawRect(Texture* target, SDL_Rect destRect, int pxSize, SDL_Color color)
	{
		m_Impl->SurfaceDrawRect(target, destRect, pxSize, color);
	}

	void Renderer::SurfaceDrawRectFilled_unsafe(Texture* target, SDL_Rect destRect, SDL_Color color)
	{
		m_Impl->SurfaceDrawRectFilled_unsafe(target, destRect, color);
	}

	void Renderer::SurfaceDrawRectFilled(Texture* target, SDL_Rect destRect, SDL_Color color)
	{
		m_Impl->SurfaceDrawRectFilled(target, destRect, color);
	}

	void Renderer::SurfaceDrawCircle_unsafe(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
		m_Impl->SurfaceDrawCircle_unsafe(target, center, r, pxSize, color);
	}

	void Renderer::SurfaceDrawCircle(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
		m_Impl->SurfaceDrawCircle(target, center, r, pxSize, color);
	}

	void Renderer::SurfaceDrawCircleFilled_unsafe(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
		m_Impl->SurfaceDrawCircleFilled_unsafe(target, center, r, pxSize, color);
	}

	void Renderer::SurfaceDrawCircleFilled(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color)
	{
		m_Impl->SurfaceDrawCircleFilled(target, center, r, pxSize, color);
	}

	std::pair<int, int> Renderer::SurfaceDrawText_unsafe(
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

	std::pair<int, int> Renderer::SurfaceDrawText(
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

	void Renderer::SurfaceDrawTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect)
	{
		m_Impl->SurfaceDrawTexture(target, texture, destRect, srcRect);
	}

	void Renderer::SurfaceDrawTiledTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, const glm::vec2& tilingFactor)
	{
		m_Impl->SurfaceDrawTiledTexture(target, texture, destRect, srcRect, tilingFactor);
	}

	void Renderer::SurfaceGeneralDrawTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, glm::vec2 tilingFactor, const glm::vec4& tintColor)
	{
		m_Impl->SurfaceGeneralDrawTexture(target, texture, destRect, srcRect, tilingFactor, tintColor);
	}

	// mid-level methods (advanced renderer commands)
	void Renderer::NewFrame(Window* window)
	{
		window->GetRenderer()->NewFrame();
	}

	void Renderer::ShowFrame(Window* window)
	{
		window->GetRenderer()->ShowFrame();
	}

	// high-level methods (complex graphics operations)
	RendererBase* Renderer::Init(Window* window, WindowContextType windowContextType)
	{
		if (window != nullptr)
		{
			switch (windowContextType)
			{
				case mse::WindowContextType::None:
				{
					m_Impl = new RendererDummy(window);
					return m_Impl;
					break;
				}
				case WindowContextType::OpenGL:
				{
					m_Impl = new RendererOpenGL(window);
					return m_Impl;
					break;
				}
				case WindowContextType::SDL:
				{
					m_Impl = new RendererSDL(window);
					return m_Impl;
					break;
				}
				default:
				{
					MSE_CORE_ERROR("Renderer: Unknown context type");
					return nullptr;
				}
			}
		} else {
			return nullptr;
		}
	}

	int Renderer::Shutdown()
	{
		delete m_Impl;
		m_Impl = nullptr;
		return 0;
	}
}

