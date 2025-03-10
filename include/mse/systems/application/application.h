#ifndef MSE_SYSTEMS_APPLICATION_APPLICATION_H
#define MSE_SYSTEMS_APPLICATION_APPLICATION_H

#include <mse/core.h>
#include <mse/systems/platform/events/events.h>

namespace mse
{
	class Application
	{
	public:
		using func = std::function<bool(SDL_Event*)>;
		
		Application();
		virtual ~Application();
		
		virtual void Run();
		virtual void ManualGraphics();
		 
		virtual void BackendStep(); // user defines only a step which happens at some rate
		void BackendStepSetRate(float fps) { m_BackendTimeDeltaLimit = fps; }
		virtual void BackendRaw(); // user defines the whole routine under the thread 
		void BackendStepWrapper();
		void BackendRawWrapper();
		void BackendProcessing();
		
		inline virtual void Stop() { m_shouldExit = true; }
		inline virtual void SetFPS(float fps) { m_TimeDeltaLimit = fps; }
		
		static Application* GetApplication() { return m_singleton; }
		
	protected:
		static Application* m_singleton;
		
		// threads
		std::thread m_backendThread;
//		std::thread m_eventsThread;
//		std::thread m_audioThread;
//		std::thread m_graphicsThread;
		
		// focused elements
		Window* m_currentWindow = nullptr;
		
		// statuses
		bool m_shouldExit = false;
		bool m_backendShouldStop = false;
		
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
		
		uint64_t m_BackendTimeThisFrame = 0; // when does this frame happen
		uint64_t m_BackendTimeLastFrame = 0; // when the last frame happened
		float m_BackendTimeDelta = 0; // difference between those two ^^
		float m_BackendTimeDeltaLimit = MSE_FPS60;
		
		std::unordered_map<EventTypes, func> callbacks = {};
	};
	
	Application* CreateApplication();
}

#endif
