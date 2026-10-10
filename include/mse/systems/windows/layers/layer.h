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
		virtual void OnUpdate(TimeType t = 0);
		void Update(TimeType t = 0);
		virtual void OnDisplay();
		void Display();
		virtual void OnDetach();
		void Detach();
		
		bool HandleEvent(EventTypes eventType, SDL_Event* event, int elementId = -1);
		Window* GetWindow();
		GUIItem* AddElement(GUIItem* element);
		
		inline std::unordered_map<int, GUIItem*>& GetElements() { return m_elements; }
		inline int GetElementId(GUIItem* pointer) {
			for (auto element : m_elements)
			{
				if (element.second == pointer)
				{
					return element.first;
				}
			}
			return -1;
		}
		inline GUIItem* GetElementPointer(int id)
		{
			if (m_elements.find(id) != m_elements.end())
			{
				return m_elements[id];
			}
			return nullptr;
		}
		
		int elementInFocus = -1;
		bool enabled = true; // a layer can only be shown (thus, enabled) or hidden (disabled)
		
	protected:
		Window* m_window = nullptr;
		std::unordered_map<int, GUIItem*> m_elements = {}; // this int should be a hash or a unique id
		int m_screenWidth, m_screenHeight;
	};
}

#endif
