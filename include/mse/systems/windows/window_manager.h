#ifndef MSE_SYSTEMS_WINDOWS_WINDOW_MANAGER_H
#define MSE_SYSTEMS_WINDOWS_WINDOW_MANAGER_H

#include <mse/core.h>

namespace mse
{	
	class WindowManager
	{
	public:
		static void Init();
		static void Shutdown();
		
		// create and destroy
		static Window* CreateWindow(const std::string& title, int x, int y, int width, int height, int flags = -1);
		static int DestroyWindow(Window* window);
		
		// collection
		static std::vector<Window*>& GetWindows() { return m_windows; }
		static void Pop(Window* window);
		static Window* GetWindowByNativeID(uint32_t id);
		
		// current window - the one that got focus
		static Window* GetCurrentWindow();
		static int SetCurrentWindow(Window* window);
		static int SetCurrentWindow(int windowIndex);
		
	private:
		static std::vector<Window*> m_windows;
		static int m_windowInsertIndex;
		static Window* m_currentWindow;
	};
}

#endif
