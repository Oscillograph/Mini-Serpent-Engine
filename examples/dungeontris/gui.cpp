#include <mse/systems/windows/window.h>
#include <mse/systems/windows/window_manager.h>
#include <mse/systems/windows/layers/layer.h>
#include <mse/systems/resources/resource_manager.h>
#include <mse/systems/platform/renderer/texture.h>
#include <mse/systems/platform/rendererAPI.h>
#include <mse/systems/platform/renderer_base.h>
#include <mse/systems/platform/audio/soundman.h>
#include <dungeontris/gui.h>
#include <dungeontris/game-fwd.h>

extern DTetris::GameDB gameDB;
extern DTetris::Game game;

namespace mse
{
	namespace gui
	{
		// general initialization
		StoreItem::StoreItem()
		{}

		StoreItem::StoreItem(Layer* layer, const glm::uvec4& area, const std::u32string& cost_text)
		{}

		void StoreItem::Init(Layer* layer, const glm::uvec4& area, const std::u32string& cost_text)
		{
			// model
			parentLayer = layer;
			windowUser = layer->GetWindow();
			m_elementName = "StoreItem";
			layerArea = area;

			layerMask.resize(area.z * area.w);
			for (unsigned int x = 0; x < area.z; ++x)
			{
				for (unsigned int y = 0; y < area.w; ++y)
				{
					layerMask[x + y*area.z] = id;
				}
			}

			// view
			if (layer != nullptr)
			{
				// setup texture to draw on
				MSE_CORE_LOG("Button: requesting to create a texture");
				MSE_CORE_TRACE("Button_parentLayer = ", parentLayer);
				m_texture = ResourceManager::CreateTexture(
					windowUser,
					windowUser->GetRenderer()->GetActiveRenderer(),
					layerArea.z * 4,
					layerArea.w,
					0,
					32,
					{0, 0, 0, 0});
				MSE_CORE_LOG("Button: texture obtained");

				mse::Resource* bmpFont = mse::ResourceManager::UseResource(mse::ResourceType::FontBitmap, "./data/fonts/my8bit2.bmp", parentLayer->GetWindow());

				// still button
				Renderer::SurfaceDrawRectFilled(
					(Texture*)(m_texture->data),
					{0, 0, layerArea.z, layerArea.w},
					{0, 0, 0, 255}
				);
				mse::Renderer::SurfaceDrawText(
					(Texture*)(m_texture->data),
					{2, 2, layerArea.z, layerArea.w}, 	// where to
					1, 					// pixel size
					m_text, 			// text content
					bmpFont, 			// font
					{196, 196, 0, 255}, // color
					0); 				// interval between rows

				// hover button
				Renderer::SurfaceDrawRectFilled(
					(Texture*)(m_texture->data),
					{layerArea.z, 0, layerArea.z, layerArea.w},
					{255, 255, 255, 255}
				);
				Renderer::SurfaceDrawRectFilled(
					(Texture*)(m_texture->data),
					{layerArea.z + 1, 1, layerArea.z - 2, layerArea.w - 2},
					{0, 0, 0, 0}
				);
				mse::Renderer::SurfaceDrawText(
					(Texture*)(m_texture->data),
					{layerArea.z + 2, 2, layerArea.z, layerArea.w}, 	// where to
					1, 					// pixel size
					m_text, 			// text content
					bmpFont, 			// font
					{196, 196, 0, 255}, // color
					0); 				// interval between rows

				// pushed button
				Renderer::SurfaceDrawRectFilled(
					(Texture*)(m_texture->data),
					{layerArea.z * 2, 0, layerArea.z, layerArea.w},
					{255, 255, 255, 255}
				);
				mse::Renderer::SurfaceDrawText(
					(Texture*)(m_texture->data),
					{layerArea.z * 2 + 2, 2, layerArea.z, layerArea.w}, 	// where to
					1, 					// pixel size
					m_text, 			// text content
					bmpFont, 			// font
					{255, 255, 255, 255}, // color
					0); 				// interval between rows

				// disabled button
				Renderer::SurfaceDrawRectFilled(
					(Texture*)(m_texture->data),
					{layerArea.z * 3, 0, layerArea.z, layerArea.w},
					{0, 0, 0, 255}
				);
				mse::Renderer::SurfaceDrawText(
					(Texture*)(m_texture->data),
					{layerArea.z * 3 + 2, 2, layerArea.z, layerArea.w}, 	// where to
					1, 					// pixel size
					m_text, 			// text content
					bmpFont, 			// font
					{255, 255, 255, 255}, // color
					0); 				// interval between rows

				((Texture*)(m_texture->data))->Update();

				MSE_CORE_LOG("Button: texture edited");
			}

			// controller
			// setup interaction
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
		}

		StoreItem::~StoreItem()
		{}

		// general GUIItem interface
		void StoreItem::Display()
		{
			glm::vec4 scaled = {
				(float)layerArea.x / WindowManager::GetCurrentWindow()->GetPrefs().width,
				(float)layerArea.y / WindowManager::GetCurrentWindow()->GetPrefs().height,
				(float)layerArea.z / WindowManager::GetCurrentWindow()->GetPrefs().width,
				(float)layerArea.w / WindowManager::GetCurrentWindow()->GetPrefs().height,
			};

			SDL_FRect destRect = {
				(float)layerArea.x / WindowManager::GetCurrentWindow()->GetPrefs().width,
				(float)layerArea.y / WindowManager::GetCurrentWindow()->GetPrefs().height,
				(float)layerArea.z / WindowManager::GetCurrentWindow()->GetPrefs().width,
				(float)layerArea.w / WindowManager::GetCurrentWindow()->GetPrefs().height,
			};

			SDL_Rect srcRect;
			if (isEnabled)
			{
				if (!isHover && !isPushed)
				{
					srcRect = {
						0, 0, layerArea.z, layerArea.w
					};
				} else {
					if (isHover)
					{
						srcRect = {
							layerArea.z, 0, layerArea.z, layerArea.w
						};
					}

					if (isPushed)
					{
						srcRect = {
							layerArea.z * 2, 0, layerArea.z, layerArea.w
						};
					}
				}
			} else {
				srcRect = {
					layerArea.z * 3, 0, layerArea.z, layerArea.w
				};
			}

			//			Renderer::DrawTiledTexture((Texture*)(m_texture->data), &destRect, &srcRect, {0, 0});
			Renderer::DrawTexture((Texture*)(m_texture->data), &destRect, &srcRect);
		}

		bool StoreItem::HandleEvent(EventTypes eventType, SDL_Event* event)
		{
			// only one callback per event type
			if (callbacks.find(eventType) != callbacks.end())
			{
				switch (eventType)
				{
					case EventTypes::GUIItemMouseOver:
					{
						if (isEnabled)
						{
							isHover = true;
							callbacks[eventType](event);
						}
						return true;
					}
					case EventTypes::GUIItemMouseButtonDown:
					{
						if (isEnabled)
						{
							state = StoreItemStates::Pressed;
							isPushed = true;
							callbacks[eventType](event);
						}
						return true;
					}
					case EventTypes::GUIItemMouseButtonUp:
					{
						if (isEnabled)
						{
							state = StoreItemStates::Idle;
							isPushed = false;
							callbacks[eventType](event);
						}
						return true;
					}
					case EventTypes::GUIItemMouseOut:
					{
						if (isEnabled)
						{
							isPushed = false;
							isHover = false;
							callbacks[eventType](event);
						}
						return true;
					}
					default:
					{
						callbacks[eventType](event);
						return true;
					}
				}
			}
			return false;
		}
	}
}
