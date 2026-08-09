#ifndef MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_INPUT_INT_H
#define MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_INPUT_INT_H

#include <mse/core.h>
#include <mse/systems/windows/layers/gui/guiitem.h>
#include <string>

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
				const glm::uvec4& color,
				size_t length = 10
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
				const glm::uvec4& rightSource,
				size_t length = 10
				);

			void Init(
				Layer* layer,
				int defaultValue,
				const glm::uvec4& area,
				const glm::uvec4& bgColor,
				const glm::uvec4& color,
				size_t length = 10
				);
			void Init(
				Layer* layer,
				int defaultValue,
				const glm::uvec4& color,
				const glm::uvec4& area,
				const std::string& spritelist,
				const glm::uvec4& leftSource,
				const glm::uvec4& midSource,
				const glm::uvec4& rightSource,
				size_t length = 10
				);

			void RegisterCallbacks();

			virtual ~InputInt();

			// general GUIItem interface
			virtual void Display();
			virtual bool HandleEvent(EventTypes eventType, SDL_Event* event) override;

			// unique InputInt interface
			int value = 0;
			bool isHover = false;
			bool isPushed = false;
			bool isFocused = false;

			void UpdateDigits();
			void UpdateText();
			void UpdateTexture();

		protected:
			std::u32string m_text = U""; // text representation of the value

			size_t m_length = 10;
			SymbolsContainer<int, 10> m_digits = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			size_t m_cursorPosition = 0;
			bool m_negative = false;

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
