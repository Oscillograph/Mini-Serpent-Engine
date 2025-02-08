#include <mse/core.h>
#include <mse/systems/application/application.h>
#include <mse/systems/resources/resource_manager.h>
#include <mse/systems/platform/platform.h>
#include <mse/systems/platform/renderer/renderer.h>
#include <mse/systems/windows/window_manager.h>
#include <mse/systems/windows/window.h>
#include <mse/systems/windows/layers/layer_manager.h>
#include <mse/systems/scenes/scene_manager.h>
#include <mse/systems/canban.h>

namespace mse
{
	Application* Application::m_singleton = nullptr;
	
	Application::Application()
	{
		MSE_CORE_LOG("Application: constructor called");
		Platform::Init(
			SDL_INIT_EVERYTHING,
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE,
			IMG_INIT_PNG | IMG_INIT_JPG
			);
		WindowManager::Init();
		ResourceManager::Init();
		
		// general callbacks
		callbacks[EventTypes::None] = [&](SDL_Event* event) { return false; };
		callbacks[EventTypes::KeyDown] = [&](SDL_Event* event) { return false; };
		callbacks[EventTypes::KeyUp] = [&](SDL_Event* event) { return false; };
		callbacks[EventTypes::MouseButtonDown] = [&](SDL_Event* event) { return false; };
		callbacks[EventTypes::MouseButtonUp] = [&](SDL_Event* event) { return false; };
		callbacks[EventTypes::MouseMoved] = [&](SDL_Event* event) { return false; };
		callbacks[EventTypes::MouseWheel] = [&](SDL_Event* event) { return false; };
		
		m_singleton = this;
	}
	
	Application::~Application()
	{
		MSE_CORE_LOG("Application: destructor called");
		Renderer::SetActiveWindow(nullptr);
		ResourceManager::ShutDown();
		WindowManager::Shutdown();
		Platform::Shutdown();
	}
	
	void Application::ManualGraphics()
	{
		return;
	}
	
	void Application::Run()
	{
		MSE_CORE_LOG("Application: run procedure called");
		Renderer::SetBackgroundColor({0, 0, 0, 255});
		while (!m_shouldExit)
		{
			m_TimeThisFrame = Platform::GetTimeMs();
			m_TimeDelta = (float)(m_TimeThisFrame - m_TimeLastFrame)/1000;
			
			if ((m_TimeDelta >= m_TimeDeltaLimit) || (m_TaskToDo))
			{
				// TODO: 0. Debug and profile system
				
				// TODO: 1. Input management system
				
				// 2. Events system
				while (Platform::CheckEventListener())
				{
					// process events crucial to the engine's work
					switch (Platform::PeekEvent()->type)
					{
						case SDL_QUIT:
						{
							m_shouldExit = true;
							break;
						}
						
						case SDL_WINDOWEVENT:
						{
							for (Window* window : WindowManager::GetWindows())
							{
								if (Platform::PeekEvent()->window.windowID == window->GetNativeWindowID())
								{
									switch (Platform::PeekEvent()->window.event)
									{
										case SDL_WINDOWEVENT_CLOSE:
										{
											if (WindowManager::GetWindows().size() == 1)
											{
												m_shouldExit = true;
											} else {
												WindowManager::DestroyWindow(window);
											}
											break; // this event should not go into OnEvent mechanism
										}
									}
								}
							}
							break;
						}
						
						case SDL_KEYDOWN:
						{
							switch (Platform::PeekEvent()->key.keysym.sym)
							{
								// turn fps counter on/off
								case SDL_KeyCode::SDLK_BACKQUOTE:
								{
									m_showFPS = !m_showFPS;
									if (!m_showFPS)
									{
										for (Window* window : WindowManager::GetWindows())
										{
											window->SetTitle(window->GetPrefs().title);
										}
									}
									break;
								}
								
								case SDL_KeyCode::SDLK_SPACE:
								{
									for (Window* window : WindowManager::GetWindows())
									{
										MSE_CORE_TRACE("Window \"", window->GetPrefs().title, "\" is ", (window == WindowManager::GetCurrentWindow())?"focused":"not focused");
									}
									break;
								}
							}
							break;
						}
					}
					
					// try to process the event
					bool handled = false;
					switch (Platform::PeekEvent()->type)
					{
						case SDL_KEYDOWN:
						{
							handled = callbacks[EventTypes::KeyDown](Platform::PeekEvent());
							break;
						}
						case SDL_KEYUP:
						{
							handled = callbacks[EventTypes::KeyUp](Platform::PeekEvent());
							break;
						}
						case SDL_MOUSEBUTTONDOWN:
						{
							handled = callbacks[EventTypes::MouseButtonDown](Platform::PeekEvent());
							break;
						}
						case SDL_MOUSEBUTTONUP:
						{
							handled = callbacks[EventTypes::MouseButtonUp](Platform::PeekEvent());
							break;
						}
						case SDL_MOUSEMOTION:
						{
							handled = callbacks[EventTypes::MouseMoved](Platform::PeekEvent());
							break;
						}
						case SDL_MOUSEWHEEL:
						{
							handled = callbacks[EventTypes::MouseWheel](Platform::PeekEvent());
							break;
						}
					}
					
					// send the event to windows and corresponding layers event processors
					if (!handled)
					{
						for (Window* window : WindowManager::GetWindows())
						{
							if (window->EventProcessor(Platform::PeekEvent()))
							{
								break;
							}
						}
					}
				}
				
				// TODO: 3. App Reaction system
				if (m_TimeDelta >= m_TimeDeltaLimit)
				{
					for (Window* window : WindowManager::GetWindows())
					{
						window->GetLayerManager()->UpdateScreen();
					}
				}
				
				// TODO: 4. Game Object Management System (GOMS)
				
				// TODO: 5. World progress system
				SceneManager::Update(m_TimeThisFrame - m_TimeLastFrame);
				
				// TODO: 6. Asset management system
				
				// TODO: Inter-scene assets storage mechanism to allow moving things between windows
				
				// TODO: 7. Sound system
				
				// TODO: 8. Show debug information as text over the screen
				
				// 9. Graphic system
				if (m_TimeDelta >= m_TimeDeltaLimit)
				{
					if (m_TimeDeltaLimit > 0.1f)
					{
						m_fpsCount = (uint64_t)round(1 / m_TimeDeltaLimit);
					} else {
						if (m_TimeDelta > 0.0f)
						{
							m_fpsCount = (uint64_t)round(1 / m_TimeDelta);
						} else {
							m_fpsCount = 9001; // it's over nine thousand!
						}
					}
					
					Window* window = nullptr;
					for (int i = 0; i < WindowManager::GetWindows().size(); ++i)
					{
						window = WindowManager::GetWindows()[i];
						
						if ((SDL_Renderer*)(window->GetRenderer()) != Renderer::GetActiveRenderer())
						{
							Renderer::SetActiveWindow(window);
							Renderer::SetActiveRenderer(window->GetRenderer());
							Renderer::SetActiveScreen({0, 0, window->GetPrefs().width, window->GetPrefs().height});
						}
						
						Renderer::NewFrame();
						
						if (window->IsFocused())
						{
							if (m_showFPS)
							{
								window->ShowFPSInTitle(m_fpsCount);
							}
						}
						
						window->GetLayerManager()->Display();
						
						Renderer::ShowFrame();
					}
				}
				
				// TODO: 10. File I/O system
				
				// TODO: 11. Log system
				
				// 12. FPS Count
				m_frameCounter++;
				m_TimeLastFrame = m_TimeThisFrame;
				
				// 13. Systems cleanups
			}
			
			if (WindowManager::GetWindows().size() == 0)
				m_shouldExit = false;
		}
		
		MSE_CORE_LOG("Application: run procedure finished");
		return;
	}
	
}

extern mse::Application* mse::CreateApplication();
