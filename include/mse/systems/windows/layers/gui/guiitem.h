#ifndef MSE_SYSTEMS_WINDOWS_LAYERS_GUI_GUIITEM_H
#define MSE_SYSTEMS_WINDOWS_LAYERS_GUI_GUIITEM_H

#include <mse/core.h>
#include <mse/systems/platform/events/events.h>

namespace mse
{	
	// base class for all GUI elements
	class GUIItem
	{
	public:
		using func = std::function<void(SDL_Event*)>;
		
		GUIItem();
		virtual ~GUIItem();
		
		virtual bool HandleEvent(EventTypes eventType, SDL_Event* event);
		
		void OnDisplay();
		
		virtual void Display();
		
		inline void Enable() { isEnabled = true; }
		inline void Disable() { isEnabled = false; }
		inline void ToggleEnabled() { isEnabled = !isEnabled; }
		inline Resource* GetTexture() { return m_texture; }
	
	public:
		Layer* parentLayer = nullptr;
		int id = -1; // should be generated in respect to the overall number of indexed elements
		
		// display data for layers; all numbers respect to pixels bound to Window::basePrefs
		glm::ivec4 layerArea = {0, 0, 0, 0}; // x, y - coordinates of upper left corner; z - width, w - height
		std::vector<int> layerMask = {}; // rectangular of size (layerArea.z * layerArea.w) filled with unique id of the gui element
		
		// event callbacks
		std::unordered_map<EventTypes, func> callbacks = {};
		
		// flags
		bool isEnabled = true;
		bool isVisible = true;
		
	private:
		static int m_idNext; // all elements registered in program runtime
		
	protected:
		std::string m_elementName = "";
		Resource* m_texture = nullptr;
//		Resource* m_font = nullptr;
//		Resource* m_text = nullptr;
	};
	
	
	// Elements tied to Renderer by their essence
	
	
}

#endif
