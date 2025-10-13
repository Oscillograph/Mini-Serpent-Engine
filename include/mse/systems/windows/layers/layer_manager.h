#ifndef MSE_SYSTEMS_WINDOWS_LAYERS_LAYER_MANAGER_H
#define MSE_SYSTEMS_WINDOWS_LAYERS_LAYER_MANAGER_H

#include <mse/core.h>
#include <mse/systems/platform/platform.h>
#include <mse/systems/platform/events/events.h>

namespace mse
{
	class LayerManager
	{
	public:
		LayerManager() = delete;
		LayerManager(Window* window);
		virtual ~LayerManager();
		
		void Init();
		
		void Attach(Layer* layer);
		void Detach(Layer* layer);
		inline std::vector<Layer*>& GetLayers() { return m_layers; }
		
		bool HandleEvent(EventTypes eventType, SDL_Event* event, int elId = -1);
		
		void UpdateScreen();
		
		void Display();
		
		void Shutdown();
		
	private:
		friend Layer;
		
		std::vector<Layer*> m_layers;
		int m_layerInsertIndex;
		std::vector<int> m_screen = {};
		Window* m_window;
		int m_mouseOverElementID = -1;
		int m_focusElementID = -1;
	};
}

#endif
