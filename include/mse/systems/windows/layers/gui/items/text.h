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
			Text(Layer* layer, const std::u32string& text, const glm::uvec4& area, const glm::uvec4& bgColor, const glm::uvec4& color, int pxSize = 1, bool border = false, const glm::uvec4& borderColor = {255, 255, 255, 255});
			void Init(Layer* layer,  const std::u32string& text, const glm::uvec4& area, const glm::uvec4& bgColor, const glm::uvec4& color, int pxSize = 1, bool border = false, const glm::uvec4& borderColor = {255, 255, 255, 255});
			virtual ~Text();
			
			// general GUIItem interface
			virtual void Display();
			virtual bool HandleEvent(EventTypes eventType, SDL_Event* event) override;

            // unique Text interface
            void ChangeText(const std::u32string& text);
            void Scroll(int x, int y);
            bool showBorder = false;
		protected:
			std::u32string m_text = U"";
            int m_pxSize = 1;
			glm::uvec4 m_backgroundColor = {0, 0, 0, 0};
            glm::uvec4 m_borderColor = {0, 0, 0, 0};
			glm::uvec4 m_textColor = {223, 223, 223, 255};
            glm::uvec2 m_scrollXY = {0, 0};
		};
	}
}

#endif
