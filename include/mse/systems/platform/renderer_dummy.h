#ifndef MSE_SYSTEMS_PLATFORM_RENDER_DUMMY_H
#define MSE_SYSTEMS_PLATFORM_RENDER_DUMMY_H

#include <mse/core.h>

// bricks of the renderer system
#include <mse/systems/platform/renderer_base.h>

namespace mse
{
	// pure virtual
	class RendererDummy : public RendererBase
	{
	public:
		// system setup and utilities
		virtual void SetActiveRenderer(void* renderer) override;
		virtual void* GetActiveRenderer() override;
		virtual void SetActiveScene(Scene* scene) override;
		virtual Scene* GetActiveScene() override;
		virtual void SetActiveCamera(Camera2D* camera) override;
		virtual Layer* GetActiveLayer() override;
		virtual void SetActiveLayer(Layer* layer) override;
		virtual Window* GetActiveWindow() override;
		virtual void SetActiveWindow(Window* window) override;
		virtual Camera2D* GetActiveCamera() override;
		virtual void SetActiveScreen(const glm::uvec4& screen) override;
		virtual void SetActiveScreenDefault() override;
		virtual glm::uvec4 GetActiveScreen() override;

		virtual void SetBackgroundColor(const glm::uvec4& color) override;
		virtual void ClearScreen() override;

		// low-level methods (draw pixels, primitives, operate with data)
		virtual uint32_t GetPixel(Texture* surface, int x, int y) override;
		virtual void DrawTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect) override;
		virtual void DrawTiledTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, const glm::vec2& tilingFactor) override;
		virtual void GeneralDrawTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, glm::vec2 tilingFactor, const glm::vec4& tintColor) override;

		virtual void DrawRect(SDL_FPoint center, SDL_FPoint size, SDL_Color color = {128, 255, 255, 255}) override;
		virtual void DrawRect(SDL_FPoint p1, SDL_FPoint p2, SDL_FPoint p3, SDL_FPoint p4, SDL_Color color = {128, 255, 255, 255}) override;
		virtual void DrawRectFilled(SDL_FPoint center, SDL_FPoint size, SDL_Color color = {128, 255, 255, 255}) override;

		// low-level methods on surfaces (for software rendering)
		// "unsafe" methods mean that target surfaces are not locked during the drawing process
		virtual void SurfaceDrawPixel_unsafe(Texture* target, SDL_Point center, int pxSize, SDL_Color color = {128, 255, 255, 255}) override;
		virtual void SurfaceDrawPixel(Texture* target, SDL_Point center, int pxSize, SDL_Color color = {128, 255, 255, 255}) override;
		virtual void SurfaceDrawLine_unsafe(Texture* target, int x1, int y1, int x2, int y2, int pxSize, SDL_Color color = {128, 255, 255, 255}) override;
		virtual void SurfaceDrawLine(Texture* target, int x1, int y1, int x2, int y2, int pxSize, SDL_Color color = {128, 255, 255, 255}) override;
		virtual void SurfaceDrawRect_unsafe(Texture* target, SDL_Rect destRect, int pxSize, SDL_Color color = {128, 255, 255, 255}) override;
		virtual void SurfaceDrawRect(Texture* target, SDL_Rect destRect, int pxSize, SDL_Color color = {128, 255, 255, 255}) override;
		virtual void SurfaceDrawRectFilled_unsafe(Texture* target, SDL_Rect destRect, SDL_Color color = {128, 255, 255, 255}) override;
		virtual void SurfaceDrawRectFilled(Texture* target, SDL_Rect destRect, SDL_Color color = {128, 255, 255, 255}) override;
		virtual void SurfaceDrawCircle_unsafe(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color = {128, 255, 255, 255}) override;
		virtual void SurfaceDrawCircle(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color = {128, 255, 255, 255}) override;
		virtual void SurfaceDrawCircleFilled_unsafe(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color = {128, 255, 255, 255}) override;
		virtual void SurfaceDrawCircleFilled(Texture* target, SDL_Point center, int r, int pxSize, SDL_Color color = {128, 255, 255, 255}) override;

		// returns width and height of what was printed actually
		virtual std::pair<int, int> SurfaceDrawText_unsafe(Texture* target, const glm::ivec4& place, int pxSize, const std::u32string& text, Resource* textFont, const glm::uvec4& color = {128, 255, 255, 255}, int interval = 2) override;
		virtual std::pair<int, int> SurfaceDrawText(Texture* target, const glm::uvec4& place, int pxSize, const std::u32string& text, Resource* textFont, const glm::uvec4& color = {128, 255, 255, 255}, int interval = 2) override;

		virtual void SurfaceDrawTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect) override;
		virtual void SurfaceDrawTiledTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, const glm::vec2& tilingFactor) override;
		virtual void SurfaceGeneralDrawTexture(Texture* target, Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, glm::vec2 tilingFactor, const glm::vec4& tintColor) override;

		// mid-level methods (advanced renderer commands)
		virtual void NewFrame() override;
		virtual void ShowFrame() override;

		// high-level methods (complex graphics operations)
		virtual int Init(Window* window, WindowContextType windowContextType = WindowContextType::SDL) override;
		virtual int Shutdown() override;
	};
}

#endif
