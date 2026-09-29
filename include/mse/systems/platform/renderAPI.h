#ifndef MSE_SYSTEMS_PLATFORM_RENDER_API_H
#define MSE_SYSTEMS_PLATFORM_RENDER_API_H

#include <mse/core.h>
#include <mse/systems/platform/platform.h>

// bricks of the renderer system

// TODO: Consider developing Render class into a per-scene object instead of a static global

namespace mse
{
	// class for different renderer implementations
	class RendererBase;

	class RendererAPI
	{
	public:
		// system setup and utilities
		static void SetActiveRenderer(void* renderer);
		static void* GetActiveRenderer();
		static void SetActiveScene(Scene* scene);
		static Scene* GetActiveScene();
		static void SetActiveCamera(Camera2D* camera);
		static Layer* GetActiveLayer();
		static void SetActiveLayer(Layer* layer);
		static Window* GetActiveWindow();
		static void SetActiveWindow(Window* window);
		static Camera2D* GetActiveCamera();
		static void SetActiveScreen(const glm::uvec4& screen);
		static void SetActiveScreenDefault();
		static glm::uvec4 GetActiveScreen();

		static void SetBackgroundColor(const glm::uvec4& color);
		static void ClearScreen();

		// low-level methods (draw pixels, primitives, operate with data)
		static uint32_t GetPixel(Texture* surface, int x, int y);
		static void DrawTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect);
		static void DrawTiledTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, const glm::vec2& tilingFactor);
		static void GeneralDrawTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, glm::vec2 tilingFactor, const glm::vec4& tintColor);

		static void DrawRect(SDL_FPoint center, SDL_FPoint size, SDL_Color color = {128, 255, 255, 255});
		static void DrawRect(SDL_FPoint p1, SDL_FPoint p2, SDL_FPoint p3, SDL_FPoint p4, SDL_Color color = {128, 255, 255, 255});
		static void DrawRectFilled(SDL_FPoint center, SDL_FPoint size, SDL_Color color = {128, 255, 255, 255});

		// low-level methods on surfaces (for software rendering)
		// "unsafe" methods mean that target surfaces are not locked during the drawing process
		static void SurfaceDrawPixel_unsafe(Texture* target, SDL_Point center, int pxSize, SDL_Color color = {128, 255, 255, 255});
		static void SurfaceDrawPixel(Texture* target, SDL_Point center, int pxSize, SDL_Color color = {128, 255, 255, 255});
		static void SurfaceDrawLine_unsafe(Texture* target, int x1, int y1, int x2, int y2, int pxSize, SDL_Color color = {128, 255, 255, 255});
		static void SurfaceDrawLine(Texture* target, int x1, int y1, int x2, int y2, int pxSize, SDL_Color color = {128, 255, 255, 255});
		static void SurfaceDrawRect_unsafe(Texture* target, SDL_Rect destRect, int pxSize, SDL_Color color = {128, 255, 255, 255});
		static void SurfaceDrawRect(Texture* target, SDL_Rect destRect, int pxSize, SDL_Color color = {128, 255, 255, 255});
		static void SurfaceDrawRectFilled_unsafe(Texture* target, SDL_Rect destRect, SDL_Color color = {128, 255, 255, 255});
		static void SurfaceDrawRectFilled(Texture* target, SDL_Rect destRect, SDL_Color color = {128, 255, 255, 255});
		static void SurfaceDrawCircle_unsafe(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color = {128, 255, 255, 255});
		static void SurfaceDrawCircle(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color = {128, 255, 255, 255});
		static void SurfaceDrawCircleFilled_unsafe(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color = {128, 255, 255, 255});
		static void SurfaceDrawCircleFilled(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color = {128, 255, 255, 255});

		// returns width and height of what was printed actually
		static std::pair<int, int> SurfaceDrawText_unsafe(Texture* target, const glm::ivec4& place, int pxSize, const std::u32string& text, Resource* textFont, const glm::uvec4& color = {128, 255, 255, 255}, int interval = 2);
		static std::pair<int, int> SurfaceDrawText(Texture* target, const glm::uvec4& place, int pxSize, const std::u32string& text, Resource* textFont, const glm::uvec4& color = {128, 255, 255, 255}, int interval = 2);

		static void SurfaceDrawTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect);
		static void SurfaceDrawTiledTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, const glm::vec2& tilingFactor);
		static void SurfaceGeneralDrawTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, glm::vec2 tilingFactor, const glm::vec4& tintColor);

		// mid-level methods (advanced renderer commands)
		static void NewFrame();
		static void ShowFrame();

		// high-level methods (complex graphics operations)
		static int Init(Window* window, WindowContextType windowContextType = WindowContextType::SDL);
		static int Shutdown();

	public:
		static RendererBase* m_Impl;
	};
}

#endif
