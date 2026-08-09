#ifndef MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_INPUT_INT_H
#define MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_INPUT_INT_H

#include <mse/core.h>
#include <mse/systems/windows/layers/gui/guiitem.h>

namespace mse
{
	namespace gui
	{
		class InputInt : public GUIItem
		{
		public:
			// default constructor
			InputInt();
			// generic constructor
			InputInt(
				Layer* layer,
				int defaultValue,
				const glm::uvec4& area,
				const glm::uvec4& bgColor,
				const glm::uvec4& color
				);
			// constructor from spritelist
			InputInt(
				Layer* layer,
				int defaultValue,
				const glm::uvec4& color,
				const glm::uvec4& area,
				const std::string& spritelist,
				const glm::uvec4& leftSource,
				const glm::uvec4& midSource,
				const glm::uvec4& rightSource
				);

			void Init(
				Layer* layer,
				int defaultValue,
				const glm::uvec4& area,
				const glm::uvec4& bgColor,
				const glm::uvec4& color
				);
			void Init(
				Layer* layer,
				int defaultValue,
				const glm::uvec4& color,
				const glm::uvec4& area,
				const std::string& spritelist,
				const glm::uvec4& leftSource,
				const glm::uvec4& midSource,
				const glm::uvec4& rightSource
				);

			virtual ~InputInt();

			// general GUIItem interface
			virtual void Display();
			virtual bool HandleEvent(EventTypes eventType, SDL_Event* event) override;

			// unique InputInt interface
			int value = 0;
			bool isHover = false;
			bool isPushed = false;
			bool isFocused = false;

		protected:
			int m_value = 0;

			// generic
			glm::uvec4 m_backgroundColor = {0, 0, 0, 0};
			glm::uvec4 m_textColor = {223, 223, 223, 255};

			// sprite-based
			Resource* m_spriteList = nullptr;
			glm::uvec4 m_leftSource = {0, 0, 0, 0};
			glm::uvec4 m_midSource = {0, 0, 0, 0};
			glm::uvec4 m_rightSource = {0, 0, 0, 0};
		};
	}
}

#endif
