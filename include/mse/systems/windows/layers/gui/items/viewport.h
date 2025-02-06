#ifndef MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_VIEWPORT_H
#define MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_VIEWPORT_H

#include <mse/core.h>
#include <mse/systems/windows/layers/gui/guiitem.h>

namespace mse
{
	namespace gui
	{
		class Viewport : public GUIItem
		{
		public:
			// general initialization
			Viewport();
			Viewport(Layer* layer, Scene* toScene, Camera2D* withCamera, const std::u32string& text, const glm::uvec4& area);
			void Init(Layer* layer, Scene* toScene, Camera2D* withCamera, const std::u32string& text, const glm::uvec4& area);
			virtual ~Viewport();
			
			// general GUIItem interface
			virtual void Display();
			virtual bool HandleEvent(EventTypes eventType, SDL_Event* event) override;
			
			// unique Viewport interface
			Scene* scene = nullptr;
			Camera2D* camera = nullptr;
		protected:
			std::u32string m_text = U"";
		};
	}
}

#endif
