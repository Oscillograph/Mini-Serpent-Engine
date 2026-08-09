#include <mse/systems/windows/layers/gui/items/input_int.h>
#include <mse/systems/platform/platform.h>
#include <mse/systems/platform/renderer/renderer.h>
#include <mse/systems/platform/renderer/texture.h>
#include <mse/systems/platform/audio/soundman.h>
#include <mse/systems/platform/events/events.h>
#include <mse/systems/windows/window_manager.h>
#include <mse/systems/windows/window.h>
#include <mse/systems/windows/layers/layer.h>
#include <mse/systems/windows/layers/layer_manager.h>
#include <mse/systems/resources/resource_manager.h>


namespace mse
{
	namespace gui
	{
		// default constructor
		InputInt::InputInt()
		{
			Init(nullptr, 0, {0, 0, 1, 1}, {0, 0, 0, 0}, {255, 255, 255, 255});
		}

		// generic constructor
		InputInt::InputInt(
			Layer* layer,
			int defaultValue,
			const glm::uvec4& area,
			const glm::uvec4& bgColor,
			const glm::uvec4& color,
			size_t length
		)
		{
			Init(layer, defaultValue, area, bgColor, color, length);
		}

		// constructor from spritelist
		InputInt::InputInt(
			Layer* layer,
			int defaultValue,
			const glm::uvec4& color,
			const glm::uvec4& area,
			const std::string& spritelist,
			const glm::uvec4& leftSource,
			const glm::uvec4& midSource,
			const glm::uvec4& rightSource,
			size_t length
		)
		{
			Init(layer, defaultValue, color, area, spritelist, leftSource, midSource, rightSource, length);
		}

		// generic
		void InputInt::Init(
			Layer* layer,
			int defaultValue,
			const glm::uvec4& area,
			const glm::uvec4& bgColor,
			const glm::uvec4& color,
			size_t length
		)
		{
			parentLayer = layer;
			windowUser = layer->GetWindow();
			m_elementName = "InputInt";
			layerArea = area;
			value = defaultValue;
			m_backgroundColor = bgColor;
			m_textColor = color;
			m_length = length;
			MSE_LOG("m_length = ", m_length);

			// collect digits
			UpdateDigits();

			// prepare layer mask for the element
			layerMask.resize(area.z * area.w);
			for (int x = 0; x < area.z; ++x)
			{
				for (int y = 0; y < area.w; ++y)
				{
					layerMask[x + y*area.z] = id;
				}
			}

			// view
			if (layer != nullptr)
			{
				// setup texture to draw on
				MSE_CORE_LOG("InputInt: requesting to create a texture");
				MSE_CORE_TRACE("InputInt_parentLayer = ", parentLayer);
				m_texture = ResourceManager::CreateTexture(
					windowUser,
					windowUser->GetRenderer(),
					layerArea.z * 4,
					layerArea.w,
					0,
					32,
					{0, 0, 0, 0});
				MSE_CORE_LOG("InputInt: texture obtained");

				UpdateTexture();
			}

			// controller
			// setup interaction
			RegisterCallbacks();
		}

		// sprite-based
		void InputInt::Init(
			Layer* layer,
			int defaultValue,
			const glm::uvec4& color,
			const glm::uvec4& area,
			const std::string& spritelist,
			const glm::uvec4& leftSource,
			const glm::uvec4& midSource,
			const glm::uvec4& rightSource,
			size_t length
		)
		{}

		void InputInt::RegisterCallbacks()
		{
			callbacks[EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
				//				MSE_CORE_LOG(m_elementName, ": ...");
				SoundMan::PlaySound("data/audio/sounds/click.wav");
			};

			callbacks[EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
				//				MSE_CORE_LOG(m_elementName, ": Yay, you clicked me!");
			};

			callbacks[EventTypes::GUIItemMouseOver] = [&](SDL_Event* event){
				//				MSE_CORE_LOG(m_elementName, ": Hello, Mouse!");
			};

			callbacks[EventTypes::GUIItemMouseOut] = [&](SDL_Event* event){
				//				MSE_CORE_LOG(m_elementName, ": Goodbye, Mouse!");
			};

			callbacks[EventTypes::GUIItemKeyDown] = [&](SDL_Event* event){
				if (this->isFocused)
				{
					switch (event->key.key)
					{
						case SDLK_0:
						{
							m_digits.input(0, m_cursorPosition);
							m_cursorPosition++;
							UpdateDigits();
							break;
						}

						case SDLK_1:
						{
							m_digits.input(1, m_cursorPosition);
							m_cursorPosition++;
							UpdateDigits();
							break;
						}

						case SDLK_2:
						{
							m_digits.input(2, m_cursorPosition);
							m_cursorPosition++;
							UpdateDigits();
							break;
						}

						case SDLK_3:
						{
							m_digits.input(3, m_cursorPosition);
							m_cursorPosition++;
							UpdateDigits();
							break;
						}

						case SDLK_4:
						{
							m_digits.input(4, m_cursorPosition);
							m_cursorPosition++;
							UpdateDigits();
							break;
						}

						case SDLK_5:
						{
							m_digits.input(5, m_cursorPosition);
							m_cursorPosition++;
							UpdateDigits();
							break;
						}

						case SDLK_6:
						{
							m_digits.input(6, m_cursorPosition);
							m_cursorPosition++;
							UpdateDigits();
							break;
						}

						case SDLK_7:
						{
							m_digits.input(7, m_cursorPosition);
							m_cursorPosition++;
							UpdateDigits();
							break;
						}

						case SDLK_8:
						{
							m_digits.input(8, m_cursorPosition);
							m_cursorPosition++;
							UpdateDigits();
							break;
						}

						case SDLK_9:
						{
							m_digits.input(9, m_cursorPosition);
							m_cursorPosition++;
							UpdateDigits();
							break;
						}

						case SDLK_LEFT:
						{
							m_cursorPosition--;
							break;
						}

						case SDLK_RIGHT:
						{
							m_cursorPosition++;
							break;
						}

						case SDLK_UP:
						{
							value++;
							UpdateDigits();
							break;
						}

						case SDLK_DOWN:
						{
							value--;
							UpdateDigits();
							break;
						}

						case SDLK_DELETE:
						{
							m_digits.remove(m_cursorPosition);
							m_cursorPosition++;
							UpdateDigits();
							break;
						}

						case SDLK_BACKSPACE:
						{
							m_digits.backspace(m_cursorPosition);
							m_cursorPosition--;
							UpdateDigits();
							break;
						}

						default:
						{}
					}
				}
			};
		}

		InputInt::~InputInt()
		{
		}

		// general GUIItem interface
		void InputInt::Display()
		{
		}

		bool InputInt::HandleEvent(EventTypes eventType, SDL_Event* event)
		{
			return false;
		}

		void InputInt::UpdateDigits()
		{
			// MSE_LOG("UpdateDigits start");
			m_negative = (value < 0) ? true : false;

			int temp = value;
			int i = m_length - 1;
			for ( ; i >= 0; --i)
			{
				// MSE_LOG("UpdateDigits ", i, " of ", m_length);
				m_digits.input_in_place(temp % 10, i);
				temp = (temp - m_digits[i]) / 10;
			}

			if (i > 0)
			{
				for ( ; i >= 0; --i)
				{
					m_digits.container[i].empty = true;
				}
			}

			// MSE_LOG("UpdateDigits end");
			UpdateText();
		}

		void InputInt::UpdateText()
		{
			char32_t symbols[m_length];
			for (size_t i = 0; i < m_length; ++i)
			{
				symbols[i] = '0' + m_digits[i];
			}
			m_text = std::u32string(symbols);

			UpdateTexture();
		}

		void InputInt::UpdateTexture()
		{
			mse::Resource* bmpFont = mse::ResourceManager::UseResource(mse::ResourceType::FontBitmap, "./data/fonts/my8bit2.bmp", parentLayer->GetWindow());

			// general state
			Renderer::SurfaceDrawRectFilled(
				(Texture*)(m_texture->data),
				{0, 0, layerArea.z, layerArea.w},
				{m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, m_backgroundColor.w}
			);
			mse::Renderer::SurfaceDrawText(
				(Texture*)(m_texture->data),
				{2, 2, layerArea.z, layerArea.w}, 	// where to
				1, 					// pixel size
				m_text, 			// text content
				bmpFont, 			// font
				{m_textColor.x, m_textColor.y, m_textColor.z, m_textColor.w}, // color
				0); 				// interval between rows

			// hover state
			Renderer::SurfaceDrawRectFilled(
				(Texture*)(m_texture->data),
				{layerArea.z, 0, layerArea.z, layerArea.w},
				{255 - m_backgroundColor.x, 255 - m_backgroundColor.y, 255 - m_backgroundColor.z, m_backgroundColor.w}
			);
			Renderer::SurfaceDrawRectFilled(
				(Texture*)(m_texture->data),
				{layerArea.z + 1, 1, layerArea.z - 2, layerArea.w - 2},
				{m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, m_backgroundColor.w}
			);
			mse::Renderer::SurfaceDrawText(
				(Texture*)(m_texture->data),
				{layerArea.z + 2, 2, layerArea.z, layerArea.w}, 	// where to
				1, 					// pixel size
				m_text, 			// text content
				bmpFont, 			// font
				{m_textColor.x, m_textColor.y, m_textColor.z, m_textColor.w}, // color
				0); 				// interval between rows

			// focused state
			Renderer::SurfaceDrawRectFilled(
				(Texture*)(m_texture->data),
				{layerArea.z * 2, 0, layerArea.z, layerArea.w},
				{255 - m_backgroundColor.x, 255 - m_backgroundColor.y, 255 - m_backgroundColor.z, m_backgroundColor.w}
			);
			mse::Renderer::SurfaceDrawText(
				(Texture*)(m_texture->data),
				{layerArea.z * 2 + 2, 2, layerArea.z, layerArea.w}, 	// where to
				1, 					// pixel size
				m_text, 			// text content
				bmpFont, 			// font
				{255 - m_textColor.x, 255 - m_textColor.y, 255 - m_textColor.z, 255 - m_textColor.w}, // color
				0); 				// interval between rows

			// disabled state
			Renderer::SurfaceDrawRectFilled(
				(Texture*)(m_texture->data),
				{layerArea.z * 3, 0, layerArea.z, layerArea.w},
				{m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, m_backgroundColor.w / 2}
			);
			mse::Renderer::SurfaceDrawText(
				(Texture*)(m_texture->data),
				{layerArea.z * 3 + 2, 2, layerArea.z, layerArea.w}, 	// where to
				1, 					// pixel size
				m_text, 			// text content
				bmpFont, 			// font
				{255 - m_textColor.x, 255 - m_textColor.y, 255 - m_textColor.z, m_textColor.w / 2}, // color
				0); 				// interval between rows

			((Texture*)(m_texture->data))->Update();

			MSE_CORE_LOG("InputInt: texture edited");
		}
	}
}
