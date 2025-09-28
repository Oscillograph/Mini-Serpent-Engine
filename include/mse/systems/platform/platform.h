#ifndef MSE_SYSTEMS_PLATFORM_PLATFORM_H
#define MSE_SYSTEMS_PLATFORM_PLATFORM_H

#include <mse/core.h>

// Simple Direct media Library
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

namespace mse
{
	// arguments involving SDL-specific types should be void* or engine-specific to allow other systems stay platform-agnostic
	// the comment above doesn't restrict SDL_Event. This is a decision.
	class Platform
	{
	public:
		static int InitDefault();
		static int Init(uint32_t platformFlags, uint32_t rendererFlags, uint32_t winFlags, uint32_t imgFlags);
		static int Shutdown();
		
		// Windowing
		static void* CreateWindow(const char* title, int x, int y, int width, int height, uint32_t winFlags);
		static Uint32 GetWindowID(void* window) { return SDL_GetWindowID((SDL_Window*)window); }
		static int DestroyWindow(void* window);
		static int SetWindowTitle(void* window, std::string& title);
        static void ToggleWindowFullScreen(void* window, int mode = 0); // 0 - fullscreen, 1 - window mode fullscreen, 2 - just window mode
		static glm::ivec2 GetWindowSize(void* window);
		
		// Events
		static int CheckEventListener(bool waitForEvent = false);
		static SDL_Event* PeekEvent();
		
		// Renderer
		static void* InitRenderer(void* window);
		static SDL_Surface* GetWindowSurface(void* window);
		static void DestroyRenderer(void* renderer);
		
		// Utils
		inline static uint64_t GetTimeMs() { return SDL_GetTicks64(); }
		
	private:
		static uint32_t m_platformFlags;
		static uint32_t m_rendererFlags;
		static uint32_t m_windowFlags;
		static uint32_t m_imgFlags;
		static SDL_Event* m_eventListener;
	};
}

#endif
