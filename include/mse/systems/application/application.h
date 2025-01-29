#ifndef MSE_SYSTEMS_APPLICATION_APPLICATION_H
#define MSE_SYSTEMS_APPLICATION_APPLICATION_H

#include <mse/core.h>

namespace mse
{
	class Application
	{
	public:
		Application();
		virtual ~Application();
		
		virtual void Run();
		virtual void ManualGraphics();
		
		static Application* GetApplication() { return m_singleton; }
		
	protected:
		static Application* m_singleton;
		
		// focused elements
		Window* m_currentWindow = nullptr;
		
		// statuses
		bool m_shouldExit = false;
		
		// utility
		bool m_TaskToDo = false;
		bool m_showFPS = true;
		
		// time and fps measurement
		// a "frame" is a synonym of the game cycle iteration
		uint64_t m_TimeThisFrame = 0; // when does this frame happen
		uint64_t m_TimeLastFrame = 0; // when the last frame happened
		float m_TimeDelta = 0; // difference between those two ^^
		uint64_t m_fpsCount = 0; // frames per second ratio
		uint64_t m_frameCounter = 0; // to know how many frames passed
//		float m_TimeDeltaLimit = 0.0f;
		float m_TimeDeltaLimit = MSE_FPS60;
	};
	
	Application* CreateApplication();
}

#endif
