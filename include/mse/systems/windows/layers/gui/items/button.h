#ifndef MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_BUTTON_H
#define MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_BUTTON_H

#include <mse/core.h>
#include <mse/systems/windows/layers/gui/guiitem.h>

namespace mse
{
	namespace gui
	{
		enum class ButtonStates
		{
			Pressed,
			Released
		};
		
		class Button : public GUIItem
		{
		public:
			// general initialization
			Button();
			Button(Layer* layer, const std::u32string& text, const glm::uvec4& area, const glm::uvec4& bgColor, const glm::uvec4& color);
            Button(Layer* layer,  const std::u32string& text, const glm::uvec4& color, const glm::uvec4& area, const std::string& spritelist, const glm::uvec4& leftSource, const glm::uvec4& midSource, const glm::uvec4& rightSource);
			void Init(Layer* layer,  const std::u32string& text, const glm::uvec4& area, const glm::uvec4& bgColor, const glm::uvec4& color);
            void Init(Layer* layer,  const std::u32string& text, const glm::uvec4& color, const glm::uvec4& area, const std::string& spritelist, const glm::uvec4& leftSource, const glm::uvec4& midSource, const glm::uvec4& rightSource);
			virtual ~Button();
			
			// general GUIItem interface
			virtual void Display();
			virtual bool HandleEvent(EventTypes eventType, SDL_Event* event) override;

			// unique Button interface
			ButtonStates state = ButtonStates::Released;
		protected:
			std::u32string m_text = U"";
            
            // generic
			glm::uvec4 m_backgroundColor = {0, 0, 0, 0};
			glm::uvec4 m_textColor = {223, 223, 223, 255};
            
            // sprite-based
            Resource* m_spriteList = nullptr;
            glm::uvec4 m_leftSource = {0, 0, 0, 0};
            glm::uvec4 m_midSource = {0, 0, 0, 0};
            glm::uvec4 m_rightSource = {0, 0, 0, 0};
			
			bool isHover = false;
			bool isPushed = false;
		};
	}
}

#endif
