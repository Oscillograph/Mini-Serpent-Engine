#ifndef MSE_SYSTEMS_WINDOWS_WINDOW_H
#define MSE_SYSTEMS_WINDOWS_WINDOW_H

#include <mse/core.h>
#include <mse/systems/platform/platform.h>
#include <mse/systems/platform/events/events.h> // EventTypes

namespace mse
{
	// Window can be resized - thus, it's important to store its base values and use like:
	// 					realHeight = baseHeight * scalingFactor
	// This allows an easy way of graphics scaling mechanic that considers only base values.
	struct WindowPrefs
	{
		std::string title = "";
		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;
	};
	
	struct WindowScale
	{
		float x, y;
	};
	
	class Window
	{
	public:
		using func = std::function<bool(SDL_Event*)>;
		
		Window();
		Window(const WindowPrefs& prefs);
		Window(const std::string& title, int x, int y, int width, int height, int flags);
		virtual ~Window();
		
		// utility
		void NewWindow();
		void Rescale(int w, int h);
		
		// reactions
		void OnResize();
		bool EventProcessor(SDL_Event* event);
		
		// set private properties
		inline void SetRenderer(void* renderer) { m_renderer = renderer; }
		inline void SetNativeWindowID(uint32_t id) { m_windowNativeID = id; }
		void SetFocus(bool focus);
		void SetTitle(std::string& title);
		void ShowFPSInTitle(uint64_t fps);
		
		// just get private properties 
		inline void* GetNativeWindow() { return m_windowNative; }
		inline uint32_t GetNativeWindowID() { return m_windowNativeID; }
		inline void* GetRenderer() { return m_renderer; }
		inline WindowPrefs& GetPrefs() { return m_basePrefs; }
		inline WindowScale& GetScale() { return m_scale; }
		inline std::string& GetBaseTitle() { return m_basePrefs.title; }
		inline std::string& GetTitle() { return m_title; }
		inline int GetX() { return m_x; }
		inline int GetY() { return m_y; }
		inline int GetWidth() { return m_width; }
		inline int GetHeight() { return m_height; }
		inline int GetFlags() { return m_flags; }
		inline bool IsFocused() { return m_isFocused; }
		inline LayerManager* GetLayerManager() { return m_layerManager; }
		
		std::unordered_map<EventTypes, func> callbacks = {};
		
	protected:
		friend Renderer;
		friend LayerManager;
		friend Layer;
		
		void* m_windowNative = nullptr;
		uint32_t m_windowNativeID = 0;
		void* m_renderer = nullptr;
		WindowPrefs m_basePrefs;
		WindowScale m_scale = {1.0, 1.0};
		std::string m_title = "";
		int m_x = 0;
		int m_y = 0;
		int m_width = 0;
		int m_height = 0;
		int m_flags = 0;
		
		bool m_isFocused = false;
		char m_newTitle[512];
		
		LayerManager* m_layerManager = nullptr;
	};
}

#endif
