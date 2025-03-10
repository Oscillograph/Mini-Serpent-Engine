#include <mse/core.h>
#include <mse/systems/platform/platform.h>
#include <mse/systems/windows/window_manager.h>
#include <mse/systems/windows/window.h>

namespace mse
{
	std::vector<Window*> WindowManager::m_windows = {};
	int WindowManager::m_windowInsertIndex = 0;
	Window* WindowManager::m_currentWindow = nullptr;
	
	void WindowManager::Init()
	{
		MSE_CORE_LOG("Window manager initialization routine");
		m_windowInsertIndex = 0;
		m_currentWindow = nullptr;
	}
	
	void WindowManager::Shutdown()
	{
		MSE_CORE_LOG("Window manager shutdown routine");
		
		for (int i = 0; i < m_windows.size(); ++i)
		{
			delete m_windows[i];
			m_windows[i] = nullptr;
		}
	}
	
	// create and destroy
	Window* WindowManager::CreateWindow(const std::string& title, int x, int y, int width, int height, int flags)
	{
		MSE_CORE_LOG("Window manager: open a window");
		Window* window = new Window(title, x, y, width, height, flags);
		m_windows.emplace(m_windows.begin() + m_windowInsertIndex, window);
		m_windowInsertIndex++;
		MSE_CORE_LOG("Window manager: window opened");
		SetCurrentWindow(window);
		return window;
	}
	
	int WindowManager::DestroyWindow(Window* window)
	{
		if (window != nullptr)
		{
			auto it = std::find(m_windows.begin(), m_windows.end(), window);
			if (it != m_windows.end())
			{
				m_windows.erase(it);
				delete window;
				SetCurrentWindow(m_windowInsertIndex-1);
				m_windowInsertIndex--;
				MSE_CORE_LOG("Window manager: window destroyed");
				return 0;
			}
			MSE_CORE_LOG("Window manager: window not found");
		} else {
			MSE_CORE_LOG("Window manager: window is nullptr");
		}
		
		return 1;
	}
	
	Window* WindowManager::GetWindowByNativeID(uint32_t id)
	{
		for (Window* window : m_windows)
		{
			if (window->GetNativeWindowID() == id)
			{
				return window;
			}
		}
	}
	
	// current window - the one that got focus
	Window* WindowManager::GetCurrentWindow()
	{
		return m_currentWindow;
	}
	
	int WindowManager::SetCurrentWindow(Window* window)
	{
		m_currentWindow = window;
		return 0;
	}
	
	int WindowManager::SetCurrentWindow(int windowIndex)
	{
		if (windowIndex > 0)
		{
			if (m_windows[windowIndex] != nullptr)
			{
				m_currentWindow = m_windows[windowIndex];
				return 0;
			}
		}
		m_currentWindow = nullptr;
		return 0;
	}
}
