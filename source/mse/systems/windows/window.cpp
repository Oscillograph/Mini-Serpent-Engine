#include <mse/systems/windows/window_manager.h>
#include <mse/systems/windows/window.h>
#include <mse/systems/platform/platform.h>
#include <mse/systems/platform/renderer/renderer.h>
#include <mse/systems/windows/layers/layer_manager.h>

namespace mse
{
	Window::Window()
	: m_title("Новое окно"), m_x(0), m_y(0), m_width(320), m_height(200), m_flags(0)
	{
		NewWindow();
	}
	
	Window::Window(const WindowPrefs& prefs)
	: m_title(prefs.title), m_x(prefs.x), m_y(prefs.y), m_width(prefs.width), m_height(prefs.height)
	{
		NewWindow();
	}
	
	Window::Window(const std::string& title, int x, int y, int width, int height, int flags)
	: m_title(title), m_x(x), m_y(y), m_width(width), m_height(height), m_flags(flags)
	{
		NewWindow();
	}
	
	void Window::NewWindow()
	{
		m_basePrefs = {m_title, m_x, m_y, m_width, m_height};
		m_windowNative = Platform::CreateWindow(m_title.c_str(), m_x, m_y, m_width, m_height, m_flags);
		m_windowNativeID = Platform::GetWindowID(m_windowNative);
		m_renderer = Platform::InitRenderer(m_windowNative);
		Renderer::SetActiveRenderer(m_renderer);
		m_layerManager = new LayerManager(this);
		
		callbacks[EventTypes::None] = [&](SDL_Event* event) { return false; };
		callbacks[EventTypes::KeyDown] = [&](SDL_Event* event) { return false; };
		callbacks[EventTypes::KeyUp] = [&](SDL_Event* event) { return false; };
		callbacks[EventTypes::MouseButtonDown] = [&](SDL_Event* event) { return false; };
		callbacks[EventTypes::MouseButtonUp] = [&](SDL_Event* event) { return false; };
		callbacks[EventTypes::MouseMoved] = [&](SDL_Event* event) { return false; };
		callbacks[EventTypes::MouseWheel] = [&](SDL_Event* event) { return false; };
		callbacks[EventTypes::WindowFocusGained] = [&](SDL_Event* event) { return false; };
		callbacks[EventTypes::WindowFocusLost] = [&](SDL_Event* event) { return false; };
		callbacks[EventTypes::WindowResized] = [&](SDL_Event* event) { return false; };
		callbacks[EventTypes::WindowMoved] = [&](SDL_Event* event) { return false; };
		callbacks[EventTypes::WindowMinimized] = [&](SDL_Event* event) { return false; };
		callbacks[EventTypes::WindowMaximized] = [&](SDL_Event* event) { return false; };
		callbacks[EventTypes::WindowHidden] = [&](SDL_Event* event) { return false; };
		callbacks[EventTypes::WindowRestored] = [&](SDL_Event* event) { return false; };
		
		MSE_CORE_LOG("Window: window object for \"", m_title, "\" created");
	}
	
	void Window::Rescale(int w, int h)
	{
		m_width = w;
		m_height = h;
		m_scale.x = (float)m_width / m_basePrefs.width;
		m_scale.y = (float)m_height / m_basePrefs.height;
	}
    
    void Window::ToggleFullscreen(int mode)
    {
        if (!m_isFullscreen)
        {
            Platform::ToggleWindowFullScreen(m_windowNative, 2);
            m_isFullscreen = true;
        } else {
            Platform::ToggleWindowFullScreen(m_windowNative, 0);
            m_isFullscreen = false;
        }
        glm::ivec2 windowSize = Platform::GetWindowSize(m_windowNative);
        MSE_CORE_LOG("Window: ToggleFullscreen() event handler (w=", windowSize.x, "; h=", windowSize.y, ")");
        Rescale(windowSize.x, windowSize.y);
    }
	
	void Window::OnResize()
	{
		glm::ivec2 windowSize = Platform::GetWindowSize(m_windowNative);
		MSE_CORE_LOG("Window: OnResize() event handler (w=", windowSize.x, "; h=", windowSize.y, ")");
		Rescale(windowSize.x, windowSize.y);
	}
	
	bool Window::EventProcessor(SDL_Event* event)
	{
		switch (event->type)
		{
			case SDL_WINDOWEVENT:
			{
				if (event->window.windowID == m_windowNativeID)
				{
					switch (event->window.event)
					{
					case SDL_WINDOWEVENT_MOVED:
						{
							MSE_CORE_LOG("Window: Window moved to: ", event->window.data1, ", ", event->window.data2);
							return true;
							break;
						}
						
					case SDL_WINDOWEVENT_RESIZED:
						{
							static int count = 0;
							count++;
							MSE_CORE_LOG("Window: Resize Event count: ", count);
							OnResize();
							return true;
							break;
						}
						
					case SDL_WINDOWEVENT_FOCUS_GAINED:
						{
							MSE_CORE_LOG("Window: Window \"", m_basePrefs.title, "\" gained focus");
							WindowManager::SetCurrentWindow(this);
							SetFocus(true);
							return true;
							break;
						}
						
					case SDL_WINDOWEVENT_FOCUS_LOST:
						{
							MSE_CORE_LOG("Window: Window \"", m_basePrefs.title, "\" lost focus");
							SetFocus(false);
							return true;
							break;
						}
					}
				}
				break;
			}

			default:
			{
				if (GetLayerManager() != nullptr)
				{
					switch (event->type)
					{
						case SDL_MOUSEMOTION:
						{
							if (event->motion.windowID == m_windowNativeID)
							{
//								MSE_CORE_LOG("Window: Mouse moved to: (", Platform::PeekEvent()->motion.x, "; ", Platform::PeekEvent()->motion.y, ")");
								if (m_layerManager->HandleEvent(EventTypes::MouseMoved, event))
								{
									return true;
								} else {
									return callbacks[EventTypes::MouseMoved](event);
								}
							} else {
								return false;
							}
							break;
						}
						
						case SDL_MOUSEBUTTONDOWN:
						{
							if (event->button.windowID == m_windowNativeID)
							{
//								MSE_CORE_LOG("Window: Mouse ", Platform::PeekEvent()->button.button, " button is down");
								if (m_layerManager->HandleEvent(EventTypes::MouseButtonDown, event))
								{
									return true;
								} else {
									return callbacks[EventTypes::MouseButtonDown](event);
								}
							} else {
								return false;
							}
							break;
						}
						
						case SDL_MOUSEBUTTONUP:
						{
							if (event->button.windowID == m_windowNativeID)
							{
//								MSE_CORE_LOG("Window: Mouse ", Platform::PeekEvent()->button.button, " button is up");
								if (m_layerManager->HandleEvent(EventTypes::MouseButtonUp, event))
								{
									return true;
								} else {
									return callbacks[EventTypes::MouseButtonUp](event);
								}
							} else {
								return false;
							}
							break;
						}
						
						case SDL_MOUSEWHEEL:
						{
							if (event->wheel.windowID == m_windowNativeID)
							{
//								MSE_CORE_LOG("Window: Mouse wheel moved in ", Platform::PeekEvent()->wheel.y, " direction");
								if (m_layerManager->HandleEvent(EventTypes::MouseWheel, event))
								{
									return true;
								} else {
									return callbacks[EventTypes::MouseWheel](event);
								}
							} else {
								return false;
							}
							break;
						}			
						
						case SDL_KEYDOWN:
						{
							if (event->key.windowID == m_windowNativeID)
							{
//								MSE_CORE_LOG("Window: Key pressed: ", event->key.keysym.sym);
								if (m_layerManager->HandleEvent(EventTypes::KeyDown, event))
								{
									return true;
								} else {
									return callbacks[EventTypes::KeyDown](event);
								}
							} else {
								return false;
							}
							break;
						}
						
						case SDL_KEYUP:
						{
							if (event->key.windowID == m_windowNativeID)
							{
//								MSE_CORE_LOG("Window: Key released: ", event->key.keysym.sym);
								if (m_layerManager->HandleEvent(EventTypes::KeyUp, event))
								{
									return true;
								} else {
									return callbacks[EventTypes::KeyUp](event);
								}
							} else {
								return false;
							}
							break;
						}
					}
				} else {
					MSE_CORE_LOG("Window: An undefined event coudld not be processed due to the lack of layer manager in the window ", GetPrefs().title);
				}
				return false;
			}
		}
		return false;
	}
	
	void Window::SetFocus(bool focus)
	{
		m_isFocused = focus;
		MSE_CORE_LOG("Window: focus status set to ", focus);
	}
	
	void Window::SetTitle(std::string& title)
	{
		m_title = title;
		Platform::SetWindowTitle(m_windowNative, title);
	}
	
	void Window::ShowFPSInTitle(uint64_t fps)
	{
		// TODO: find a way around stringstream leaking memory per << operator.
		// std::stringstream newTitle;
		// newTitle.str(std::string());
		// newTitle << GetBaseTitle() << " - FPS: " << fpsCount;
		// SetTitle(newTitle.str());
		
		sprintf(m_newTitle, u8"%s - FPS: %d", m_basePrefs.title.c_str(), fps);
		std::string fpsString(m_newTitle);
		SetTitle(fpsString);
	}
	
	Window::~Window()
	{
		Platform::DestroyRenderer(m_renderer);
		m_renderer = nullptr;
		Platform::DestroyWindow(m_windowNative);
		m_windowNative = nullptr;
		delete m_layerManager;
		m_layerManager = nullptr;
		MSE_CORE_LOG("Window: window object for \"", m_basePrefs.title, "\" desroyed");
	}
}
