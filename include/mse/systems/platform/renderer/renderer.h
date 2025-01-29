#ifndef MSE_SYSTEMS_PLATFORM_RENDERER_RENDERER_H
#define MSE_SYSTEMS_PLATFORM_RENDERER_RENDERER_H

#include <mse/core.h>
#include <mse/systems/platform/platform.h>

// bricks of the renderer system
#include <mse/systems/platform/renderer/texture.h>
// #include <CSE/systems/renderer/camera2d.h>

// TODO: Consider developing Render class into a per-scene object instead of a static global

namespace mse
{
	// TODO: Draw graphic primitives
	// TODO: Draw text messages using raster fonts
	// TODO: Draw text messages using TTF (ex.: FreeType)
	// TODO: Optimization of draw calls using batch rendering
	
	class Renderer
	{
	public:
		// system setup and utilities
		static void SetActiveRenderer(void* renderer);
		inline static SDL_Renderer* GetActiveRenderer() { return m_Renderer; }
		static void SetActiveScene(Scene* scene);
		inline static Scene* GetActiveScene() { return m_ActiveScene; }
		static void SetActiveCamera(Camera2D* camera);
		inline static Layer* GetActiveLayer() { return m_ActiveLayer; }
		static void SetActiveLayer(Layer* layer);
		inline static Window* GetActiveWindow() { return m_ActiveWindow; }
		static void SetActiveWindow(Window* window);
		inline static Camera2D* GetActiveCamera() { return m_ActiveCamera; };
		static void SetActiveScreen(const glm::uvec4& screen);
		static void SetActiveScreenDefault();
		inline static glm::uvec4 GetActiveScreen() { return m_CurrentScreen; }
		
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
		
		// mid-level methods (advanced renderer commands)
		static void NewFrame(); 
		static void ShowFrame(); 
		
		// high-level methods (complex graphics operations)
		
	private:
		static SDL_Renderer* m_Renderer;
		static Scene* m_ActiveScene;
		static Layer* m_ActiveLayer;
		static Window* m_ActiveWindow;
		static Camera2D* m_ActiveCamera;
		
		static glm::uvec4 m_CurrentScreen; // contains rect properties of a current viewport
		static glm::vec2 m_PixelSize;
		static glm::uvec4 m_BackgroundColor;
		
		static glm::vec2 m_FrameSize;
		static glm::vec2 m_FrameScale;
		static glm::vec2 m_CameraPosition;
	};
}

#endif

