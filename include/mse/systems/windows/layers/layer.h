#ifndef MSE_SYSTEMS_WINDOWS_LAYERS_LAYER_H
#define MSE_SYSTEMS_WINDOWS_LAYERS_LAYER_H

#include <mse/core.h>
#include <mse/systems/platform/events/events.h>
#include <mse/systems/platform/platform.h>

namespace mse
{	
	class Layer
	{
	public:
		Layer();
		virtual ~Layer();
		
		virtual void OnAttach();
		void Attach(Window* window);
		virtual void OnInit();
		void Init();
		virtual void OnUpdate();
		void Update();
		virtual void OnDisplay();
		void Display();
		virtual void OnDetach();
		void Detach();
		
		bool HandleEvent(EventTypes eventType, SDL_Event* event, int elementId = -1);
		Window* GetWindow();
		GUIItem* AddElement(GUIItem* element);
		inline std::unordered_map<int, GUIItem*>& GetElements() { return m_elements; }
		
		bool enabled = true; // a layer can only be shown (thus, enabled) or hidden (disabled)
		
	protected:
		Window* m_window = nullptr;
		std::unordered_map<int, GUIItem*> m_elements = {}; // this int should be a hash or a unique id
		int m_screenWidth, m_screenHeight;
	};
}

#endif
