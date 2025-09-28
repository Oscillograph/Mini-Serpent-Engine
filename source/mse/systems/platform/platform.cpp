#include <mse/core.h>
#include <mse/systems/platform/platform.h>

namespace mse
{
	// default settings
	uint32_t Platform::m_platformFlags = 0;
	uint32_t Platform::m_windowFlags = 0;
	uint32_t Platform::m_rendererFlags = 0;
	uint32_t Platform::m_imgFlags = 0;
	SDL_Event* Platform::m_eventListener = nullptr;
	
	int Platform::InitDefault()
	{
		m_platformFlags = SDL_INIT_EVERYTHING;
		m_windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
		// m_rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE;
		// m_rendererFlags = SDL_RENDERER_SOFTWARE | SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
		m_rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
		m_imgFlags = IMG_INIT_PNG + IMG_INIT_JPG;
		
		return Init(m_platformFlags, m_rendererFlags, m_windowFlags, m_imgFlags);
	}
	
	int Platform::Init(uint32_t platformFlags, uint32_t rendererFlags, uint32_t winFlags, uint32_t imgFlags)
	{
		// MSE_CORE_LOG("Renderer flags on Init: ", m_rendererFlags);
		m_platformFlags = platformFlags;
		m_windowFlags = rendererFlags;
		m_rendererFlags = winFlags;
		m_imgFlags = imgFlags;
		
		/*
		* - `SDL_INIT_TIMER`: timer subsystem
		* - `SDL_INIT_AUDIO`: audio subsystem
		* - `SDL_INIT_VIDEO`: video subsystem; automatically initializes the events
		*   subsystem
		* - `SDL_INIT_JOYSTICK`: joystick subsystem; automatically initializes the
		*   events subsystem
		* - `SDL_INIT_HAPTIC`: haptic (force feedback) subsystem
		* - `SDL_INIT_GAMECONTROLLER`: controller subsystem; automatically
		*   initializes the joystick subsystem
		* - `SDL_INIT_EVENTS`: events subsystem
		* - `SDL_INIT_EVERYTHING`: all of the above subsystems
		* - `SDL_INIT_NOPARACHUTE`: compatibility; this flag is ignored
		*/
		
		// Initialize SDL Core
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			MSE_CORE_LOG("Platform: Error initializing SDL");
			std::exit(-1);
		}
		MSE_CORE_LOG("Platform: SDL initialized.");
		
		// Initialize SDL Image
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			MSE_CORE_LOG("Platform: SDL_image could not initialize! SDL_image Error: ", IMG_GetError());
			SDL_Quit();
			std::exit(-2);
		}
		MSE_CORE_LOG("Platform: SDL_image online.");
		
		// Initialize SDL_mixer 
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) 
		{ 
			MSE_CORE_LOG( "Platform: SDL_mixer could not initialize! SDL_mixer Error: ", Mix_GetError() );
			IMG_Quit();
			SDL_Quit();
			std::exit(-3);
		}
		MSE_CORE_LOG("Platform: SDL_mixer online.");
		
		m_eventListener = new SDL_Event;
		MSE_CORE_LOG("Platform: SDL events listener online.");
		
		/*
		int index = SDL_GetNumRenderDrivers();
		SDL_RendererInfo* info;
		CSE_CORE_LOG("Total video drivers availiable: ", index);
		for (int i = 0; i < index; i++)
	{
		if (SDL_GetRenderDriverInfo(i, info) > 0)
	{
		CSE_CORE_LOG("Render driver info: #", i);
		CSE_CORE_LOG("Name: ", info->name);
		CSE_CORE_LOG(" ");
		}
		}*/
		
		
		return 0;
	}
	
	void* Platform::CreateWindow(const char* title, int x, int y, int width, int height, uint32_t winFlags)
	{
		SDL_Window* window = nullptr;
		if (winFlags < 0)
		{
			window = SDL_CreateWindow(title, x, y, width, height, winFlags);
		} else {
			window = SDL_CreateWindow(title, x, y, width, height, m_windowFlags);
		}
		
		MSE_CORE_ASSERT(window != NULL, "Error creating window: ", SDL_GetError());
		
		MSE_CORE_LOG("Platform: created a window \"", title, "\"");
		return (void*)window;
	}
	
	int Platform::DestroyWindow(void* window)
	{
		SDL_Window* sdl_window = (SDL_Window*)window;
		if (sdl_window != nullptr)
		{
			SDL_DestroyWindow(sdl_window);
			MSE_CORE_LOG("Platform: destroyed a window");
		} else {
			MSE_CORE_ERROR("Platform: cannot destroy a window if the pointer is nullptr");
			return -1;
		}
		
		return 0;
	}
	
	int Platform::SetWindowTitle(void* window, std::string& title)
	{
		if (window != nullptr)
		{
			SDL_SetWindowTitle((SDL_Window*)window, title.c_str());
			return 1;
		} else {
			MSE_CORE_LOG("Platform: cannot change a title of a window if the pointer is nullptr");
		}
		return 0;
	}
    
    void Platform::ToggleWindowFullScreen(void* window, int mode)
    {
        if (window != nullptr)
        {
            switch (mode)
            {
            case 1:
                {
                    if (SDL_SetWindowFullscreen((SDL_Window*)window, SDL_WINDOW_FULLSCREEN))
                    {
                        MSE_CORE_ERROR("SDL Fullscreen Error: ", SDL_GetError())
                    }
                    break;
                }
            case 2:
                {
                    if (SDL_SetWindowFullscreen((SDL_Window*)window, SDL_WINDOW_FULLSCREEN_DESKTOP))
                    {
                        MSE_CORE_ERROR("SDL Fullscreen Error: ", SDL_GetError())
                    }
                    break;
                }
            default:
                {
                    if (SDL_SetWindowFullscreen((SDL_Window*)window, 0))
                    {
                        MSE_CORE_ERROR("SDL Fullscreen Error: ", SDL_GetError())
                    }
                    break;
                }
            }
        } else {
            MSE_CORE_LOG("Platform: cannot set window fullscreen if the pointer is nullptr");
        }
    }
	
	glm::ivec2 Platform::GetWindowSize(void* window)
	{
		glm::ivec2 windowSize(1);
		if (window != nullptr)
		{
			SDL_GetWindowSize((SDL_Window*)window, &(windowSize.x), &(windowSize.y));
		} else {
			MSE_CORE_LOG("Platform: cannot read window properties if the pointer is nullptr");
		}
		return windowSize;
	}
	
	int Platform::CheckEventListener(bool waitForEvent)
	{
		if (waitForEvent)
		{
			return SDL_WaitEvent(m_eventListener);
		} else {
			return SDL_PollEvent(m_eventListener);
		}
		return 0;
	}
	
	SDL_Event* Platform::PeekEvent()
	{
		return m_eventListener;
	}
	
	SDL_Surface* Platform::GetWindowSurface(void* window)
	{
		SDL_Surface* surface = SDL_GetWindowSurface((SDL_Window*)window);
		MSE_CORE_ASSERT(surface != NULL, "Platform: Error getting surface: ", SDL_GetError());
		
		MSE_CORE_LOG("Platform: Window surface access check passed.");
		
		return surface;
	}
	
	void* Platform::InitRenderer(void* window)
	{
		SDL_Renderer* renderer = SDL_CreateRenderer((SDL_Window*)window, -1, m_rendererFlags);
		MSE_CORE_ASSERT(renderer != NULL, "Renderer could not be created! SDL Error: ", SDL_GetError());
		
		MSE_CORE_LOG("Platform: Renderer initialized.");
		return (void*)renderer;
	}
	
	void Platform::DestroyRenderer(void* renderer)
	{
		SDL_DestroyRenderer((SDL_Renderer*)renderer);
		MSE_CORE_LOG("Platform: Renderer destroyed.");
	}
	
	int Platform::Shutdown()
	{
		delete m_eventListener;
		m_eventListener = nullptr;
		
		// quit systems
		Mix_Quit();
		MSE_CORE_LOG("Platform: SDL_mixer offline.");
		IMG_Quit();
		MSE_CORE_LOG("Platform: SDL_image offline.");
		SDL_Quit();
		MSE_CORE_LOG("Platform: SDL offline.");
		
		return 0;
	}
}
