#ifndef MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_TEXT_H
#define MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_TEXT_H

#include <mse/core.h>
#include <mse/systems/windows/layers/gui/guiitem.h>

namespace mse
{
	namespace gui
	{
		class Text : public GUIItem
		{
		public:
			// general initialization
			Text();
			Text(Layer* layer, const std::u32string& text, const glm::uvec4& area, const glm::uvec4& bgColor, const glm::uvec4& color);
			void Init(Layer* layer,  const std::u32string& text, const glm::uvec4& area, const glm::uvec4& bgColor, const glm::uvec4& color);
			virtual ~Text();
			
			// general GUIItem interface
			virtual void Display();
			virtual bool HandleEvent(EventTypes eventType, SDL_Event* event) override;

            // unique Text interface
            void ChangeText(const std::u32string& text) { m_text = text; }
            bool showBorder = false;
		protected:
			std::u32string m_text = U"";
			glm::uvec4 m_backgroundColor = {0, 0, 0, 0};
			glm::uvec4 m_textColor = {223, 223, 223, 255};
		};
	}
}

#endif
