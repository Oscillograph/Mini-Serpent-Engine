#include <utf8/cpp17.h>
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

#include <sstream>

extern DTetris::GameDB gameDB;
extern DTetris::Game game;

namespace mse
{
	namespace gui
	{
		// general initialization
		StoreItem::StoreItem()
		{}

		StoreItem::StoreItem(Layer* layer, const glm::uvec4& area, const glm::uvec4& image, int cost)
		{
			Init(layer, area, image, cost);
		}

		void StoreItem::Init(Layer* layer, const glm::uvec4& area, const glm::uvec4& image, int cost)
		{
			// model
			parentLayer = layer;
			windowUser = layer->GetWindow();
			m_elementName = "StoreItem";
			layerArea = area;

			ChangeCost(cost);

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
				m_spriteList = ResourceManager::UseTexture("./data/img/screen-images.png", windowUser, {0, 0, 0});
				m_imageSource = image;
				SetupTexture();
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
		{
			if (m_texture != nullptr)
			{
				ResourceManager::DropResource(m_texture, windowUser);
				MSE_CORE_LOG("StoreItem: texture dropped");
			}
		}

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

			// item icon
			SDL_Rect srcRect;
			if (!(state & StoreItemStates::Disabled))
			{
				if (!(state & StoreItemStates::Hover) && !(state & StoreItemStates::Pressed))
				{
					srcRect = {
						0, 0, layerArea.z, layerArea.w
					};
				} else {
					if (state & StoreItemStates::Hover)
					{
						srcRect = {
							layerArea.z, 0, layerArea.z, layerArea.w
						};
					}

					if (state & StoreItemStates::Pressed)
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

			Renderer::DrawTexture((Texture*)(m_texture->data), &destRect, &srcRect);

			// buy/equip button
			if (state & StoreItemStates::Bought)
			{
				// always show when equipped
				if (state & StoreItemStates::Equipped)
				{
					srcRect = {520, 29, 40, 11};
				} else {
					// show only when hovered/pressed
					if ((state & StoreItemStates::Hover) ||
						(state & StoreItemStates::Pressed))
					{
						srcRect = {480, 29, 40, 11};
					} else {
						srcRect = {0, 0, 0, 0};
					}
				}
			} else {
				srcRect = {440, 29, 40, 11};
			}

			destRect = {
				(float)layerArea.x 		  / WindowManager::GetCurrentWindow()->GetPrefs().width,
				(float)(layerArea.y + 16) / WindowManager::GetCurrentWindow()->GetPrefs().height,
				(float)layerArea.z 		  / WindowManager::GetCurrentWindow()->GetPrefs().width,
				(float)(layerArea.w - 16) / WindowManager::GetCurrentWindow()->GetPrefs().height,
			};

			Renderer::DrawTexture((Texture*)(m_texture->data), &destRect, &srcRect);
		}

		bool StoreItem::HandleEvent(EventTypes eventType, SDL_Event* event)
		{
			if (!(state & StoreItemStates::Disabled))
			{
				// only one callback per event type
				if (callbacks.find(eventType) != callbacks.end())
				{
					switch (eventType)
					{
						case EventTypes::GUIItemMouseOver:
						{
							if (!(state & StoreItemStates::Hover))
							{
								state = state + StoreItemStates::Hover;
								callbacks[eventType](event);
							}
							return true;
						}
						case EventTypes::GUIItemMouseButtonDown:
						{
							if (!(state & StoreItemStates::Pressed))
							{
								state = state + StoreItemStates::Pressed;
								callbacks[eventType](event);
							}
							return true;
						}
						case EventTypes::GUIItemMouseButtonUp:
						{
							if (state & StoreItemStates::Pressed)
							{
								state = state - StoreItemStates::Pressed;
								callbacks[eventType](event);
							}
							return true;
						}
						case EventTypes::GUIItemMouseOut:
						{
							if (isEnabled)
							{
								if (state & StoreItemStates::Hover)
								{
									state = state - StoreItemStates::Hover;
								}
								if (state & StoreItemStates::Pressed)
								{
									state = state - StoreItemStates::Pressed;
								}
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
			}

			return false;
		}

		void StoreItem::SetImage(const glm::uvec4& area) // set icon of what we buy
		{
			m_imageSource = area;
		}

		void StoreItem::ChangeCost(int cost)
		{
			std::stringstream stream;
			stream << cost;
			m_text = utf8::utf8to32(stream.str().c_str());
			stream.str("");
			stream.clear();
		}

		void StoreItem::SetupTexture()
		{
			SDL_Rect imageRect = {(int)m_imageSource.x, (int)m_imageSource.y, (int)m_imageSource.z, (int)m_imageSource.w};
			int textureWidth = layerArea.z * 4;
			int textureHeight = layerArea.w;

			// setup texture to draw on
			MSE_CORE_LOG("Button: requesting to create a texture");
			MSE_CORE_TRACE("Button_parentLayer = ", parentLayer);

			delete m_texture;

			m_texture = ResourceManager::CreateTexture(
				windowUser,
				windowUser->GetRenderer()->GetActiveRenderer(),
				textureWidth,
				textureHeight,
				0,
				32,
				{0, 0, 0, 0});
			MSE_CORE_LOG("Button: texture obtained");

			// still button
			SDL_FRect destRect = {0, 0, (float)layerArea.z, (float)layerArea.w};
			glm::uvec4 sourceStill = {600, 0, 40, 27};
			SDL_Rect sourceRect = {(int)sourceStill.x, (int)sourceStill.y, (int)sourceStill.z, (int)sourceStill.w};
			Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
										 (Texture*)(m_spriteList->data),
										 &destRect,
										 &imageRect);
			Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
										 (Texture*)(m_spriteList->data),
										 &destRect,
										 &sourceRect);
			// hover button
			glm::uvec4 sourceHover = {440, 0, 40, 27};
			destRect = {(float)layerArea.z, 0, (float)layerArea.z, (float)layerArea.w};
			sourceRect = {(int)sourceHover.x, (int)sourceHover.y, (int)sourceHover.z, (int)sourceHover.w};
			Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
										 (Texture*)(m_spriteList->data),
										 &destRect,
										 &imageRect);
			Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
										 (Texture*)(m_spriteList->data),
										 &destRect,
										 &sourceRect);
			// pressed button
			glm::uvec4 sourcePressed = {480, 0, 40, 27};
			destRect = {2*(float)layerArea.z, 0, (float)layerArea.z, (float)layerArea.w};
			sourceRect = {(int)sourcePressed.x, (int)sourcePressed.y, (int)sourcePressed.z, (int)sourcePressed.w};
			Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
										 (Texture*)(m_spriteList->data),
										 &destRect,
										 &imageRect);
			Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
										 (Texture*)(m_spriteList->data),
										 &destRect,
										 &sourceRect);
			// disabled button
			glm::uvec4 sourceDisabled = {600, 0, 40, 27};
			destRect = {3*(float)layerArea.z, 0, (float)layerArea.z, (float)layerArea.w};
			sourceRect = {(int)sourceDisabled.x, (int)sourceDisabled.y, (int)sourceDisabled.z, (int)sourceDisabled.w};
			Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
										 (Texture*)(m_spriteList->data),
										 &destRect,
										 &imageRect);
			Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
										 (Texture*)(m_spriteList->data),
										 &destRect,
										 &sourceRect);

			((Texture*)(m_texture->data))->Update();

			MSE_CORE_LOG("StoreItem: texture edited");
		}
	}
}
