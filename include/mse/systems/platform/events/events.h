#ifndef MSE_SYSTEMS_PLATFORM_EVENTS_EVENTS_H
#define MSE_SYSTEMS_PLATFORM_EVENTS_EVENTS_H

// THIS SYSTEM IS UNUSED

#include <mse/systems/platform/platform.h>

namespace mse
{
	enum class EventTypes : Uint32
	{
		// General engine events
		None = 0,
		
		// Keyboard and mouse
		KeyDown,
		KeyUp,
		MouseMoved,
		MouseButtonDown,
		MouseButtonUp,
		MouseWheel,
		
		// Window
		WindowResized,
		WindowMoved,
		WindowFocusGained,
		WindowFocusLost,
		WindowHidden,
		WindowRestored,
		WindowMaximized,
		WindowMinimized,
		
		// GUI elements events
		GUIItemCreate,
		GUIItemDisplay,
		GUIItemShow,
		GUIItemHide,
		GUIItemDestroy,
		GUIItemFocusGain,
		GUIItemFocusLost,
		GUIItemMouseOver,
		GUIItemMouseMove,
		GUIItemMouseOut,
		GUIItemMouseButtonDown,
		GUIItemMouseButtonUp,
		GUIItemMouseWheel,
		GUIItemMoveStart,
		GUIItemMove,
		GUIItemMoveStop,
		
		/*
		// General engine events
		Silence = SDL_POLLSENTINEL, // < Signals the end of an event poll cycle 
		Quit = SDL_QUIT,
		Window = SDL_WINDOWEVENT,
		
		// Keyboard
		KeyDown = SDL_KEYDOWN, // < Key pressed 
		KeyUp = SDL_KEYUP, // < Key released 
		
		// Mouse
		MouseMoved = SDL_MOUSEMOTION, // < Mouse moved 
		MouseButtonDown = SDL_MOUSEBUTTONDOWN,        // < Mouse button pressed 
		MouseButtonUp = SDL_MOUSEBUTTONUP,          // < Mouse button released 
		MouseWheel = SDL_MOUSEWHEEL,             // < Mouse wheel motion 
		*/
	};
	
	/*
	enum class EventSubtypes : Uint32
	{
		None = 0,
		WindowFocusGained = SDL_WINDOWEVENT_FOCUS_GAINED,
		WindowFocusLost = SDL_WINDOWEVENT_FOCUS_LOST,
		WindowResized = SDL_WINDOWEVENT_RESIZED,
		WindowClose = SDL_WINDOWEVENT_CLOSE,
	};
	*/
	
	/*
	struct EventQueue
	{
		EventTypes type = EventTypes::None;
		void* receiver = nullptr;
		void* sender = nullptr;
		void* data = nullptr;
	};
	*/
	
	/*
	class Event
	{
	public:
		Event();
		Event(SDL_Event* e);
		~Event();
		
		void Process(SDL_Event* e);
		
	public:
		EventTypes type;
		EventSubtypes event;
		
		// window events
		uint32_t windowID;
		// mouse events
		int x, y;
		int button;
	};
	*/
}

#endif
