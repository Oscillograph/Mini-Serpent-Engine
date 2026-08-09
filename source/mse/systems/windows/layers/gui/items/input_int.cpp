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
			m_negative = (value < 0) ? true : false;
			m_length = length;

			// collect digits
			int temp = value;
			for (size_t i = m_length - 1; i >= 0; --i)
			{
				m_digits.input(temp % 10, i);
				temp = (temp - m_digits[i]) / 10;
			}

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
					{color.x, color.y, color.z, color.w}, // color
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
					{color.x, color.y, color.z, color.w}, // color
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
					{255 - color.x, 255 - color.y, 255 - color.z, 255 - color.w}, // color
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
					{255 - color.x, 255 - color.y, 255 - color.z, color.w / 2}, // color
					0); 				// interval between rows

				((Texture*)(m_texture->data))->Update();

				MSE_CORE_LOG("Button: texture edited");
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
							break;
						}

						case SDLK_1:
						{
							break;
						}

						case SDLK_2:
						{
							break;
						}

						case SDLK_3:
						{
							break;
						}

						case SDLK_4:
						{
							break;
						}

						case SDLK_5:
						{
							break;
						}

						case SDLK_6:
						{
							break;
						}

						case SDLK_7:
						{
							break;
						}

						case SDLK_8:
						{
							break;
						}

						case SDLK_9:
						{
							break;
						}

						case SDLK_DELETE:
						{
							break;
						}

						case SDLK_BACKSPACE:
						{
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
	}
}
