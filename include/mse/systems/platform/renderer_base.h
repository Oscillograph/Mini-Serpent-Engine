#ifndef MSE_SYSTEMS_PLATFORM_RENDER_BASE_H
#define MSE_SYSTEMS_PLATFORM_RENDER_BASE_H

#include <mse/core.h>
#include <mse/systems/platform/platform.h>

// bricks of the renderer system

namespace mse
{
	// pure virtual
	class RendererBase
	{
	public:
		// system setup and utilities
		virtual void SetActiveRenderer(void* renderer) = 0;
		virtual void* GetActiveRenderer() = 0;
		virtual void SetActiveScene(Scene* scene) = 0;
		virtual Scene* GetActiveScene() = 0;
		virtual void SetActiveCamera(Camera2D* camera) = 0;
		virtual Layer* GetActiveLayer() = 0;
		virtual void SetActiveLayer(Layer* layer) = 0;
		virtual Window* GetActiveWindow() = 0;
		virtual void SetActiveWindow(Window* window) = 0;
		virtual Camera2D* GetActiveCamera() = 0;
		virtual void SetActiveScreen(const glm::uvec4& screen) = 0;
		virtual void SetActiveScreenDefault() = 0;
		virtual glm::uvec4 GetActiveScreen() = 0;

		virtual void SetBackgroundColor(const glm::uvec4& color) = 0;
		virtual void ClearScreen() = 0;

		// low-level methods (draw pixels, primitives, operate with data)
		virtual uint32_t GetPixel(Texture* surface, int x, int y) = 0;
		virtual void DrawTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect) = 0;
		virtual void DrawTiledTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, const glm::vec2& tilingFactor) = 0;
		virtual void GeneralDrawTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, glm::vec2 tilingFactor, const glm::vec4& tintColor) = 0;

		virtual void DrawRect(SDL_FPoint center, SDL_FPoint size, SDL_Color color = {128, 255, 255, 255}) = 0;
		virtual void DrawRect(SDL_FPoint p1, SDL_FPoint p2, SDL_FPoint p3, SDL_FPoint p4, SDL_Color color = {128, 255, 255, 255}) = 0;
		virtual void DrawRectFilled(SDL_FPoint center, SDL_FPoint size, SDL_Color color = {128, 255, 255, 255}) = 0;

		// low-level methods on surfaces (for software rendering)
		// "unsafe" methods mean that target surfaces are not locked during the drawing process
		virtual void SurfaceDrawPixel_unsafe(Texture* target, SDL_Point center, int pxSize, SDL_Color color = {128, 255, 255, 255}) = 0;
		virtual void SurfaceDrawPixel(Texture* target, SDL_Point center, int pxSize, SDL_Color color = {128, 255, 255, 255}) = 0;
		virtual void SurfaceDrawLine_unsafe(Texture* target, int x1, int y1, int x2, int y2, int pxSize, SDL_Color color = {128, 255, 255, 255}) = 0;
		virtual void SurfaceDrawLine(Texture* target, int x1, int y1, int x2, int y2, int pxSize, SDL_Color color = {128, 255, 255, 255}) = 0;
		virtual void SurfaceDrawRect_unsafe(Texture* target, SDL_Rect destRect, int pxSize, SDL_Color color = {128, 255, 255, 255}) = 0;
		virtual void SurfaceDrawRect(Texture* target, SDL_Rect destRect, int pxSize, SDL_Color color = {128, 255, 255, 255}) = 0;
		virtual void SurfaceDrawRectFilled_unsafe(Texture* target, SDL_Rect destRect, SDL_Color color = {128, 255, 255, 255}) = 0;
		virtual void SurfaceDrawRectFilled(Texture* target, SDL_Rect destRect, SDL_Color color = {128, 255, 255, 255}) = 0;
		virtual void SurfaceDrawCircle_unsafe(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color = {128, 255, 255, 255}) = 0;
		virtual void SurfaceDrawCircle(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color = {128, 255, 255, 255}) = 0;
		virtual void SurfaceDrawCircleFilled_unsafe(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color = {128, 255, 255, 255}) = 0;
		virtual void SurfaceDrawCircleFilled(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color = {128, 255, 255, 255}) = 0;

		// returns width and height of what was printed actually
		virtual std::pair<int, int> SurfaceDrawText_unsafe(Texture* target, const glm::ivec4& place, int pxSize, const std::u32string& text, Resource* textFont, const glm::uvec4& color = {128, 255, 255, 255}, int interval = 2) = 0;
		virtual std::pair<int, int> SurfaceDrawText(Texture* target, const glm::uvec4& place, int pxSize, const std::u32string& text, Resource* textFont, const glm::uvec4& color = {128, 255, 255, 255}, int interval = 2) = 0;

		virtual void SurfaceDrawTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect) = 0;
		virtual void SurfaceDrawTiledTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, const glm::vec2& tilingFactor) = 0;
		virtual void SurfaceGeneralDrawTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, glm::vec2 tilingFactor, const glm::vec4& tintColor) = 0;

		// mid-level methods (advanced renderer commands)
		virtual void NewFrame() = 0;
		virtual void ShowFrame() = 0;

		// high-level methods (complex graphics operations)
		virtual int Init(Window* window, WindowContextType windowContextType = WindowContextType::SDL) = 0;
		virtual int Shutdown() = 0;
	};
}

#endif
