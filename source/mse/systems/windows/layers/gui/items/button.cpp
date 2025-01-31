#include <mse/systems/windows/layers/gui/items/button.h>
#include <mse/systems/platform/platform.h>
#include <mse/systems/platform/renderer/renderer.h>
#include <mse/systems/platform/renderer/texture.h>
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
		// general initialization
		Button::Button()
		: GUIItem()
		{
			Init(nullptr, U"", {0, 0, 1, 1}, {0, 0, 0, 0}, {255, 255, 255, 255});
		}
		
		Button::Button(Layer* layer,  const std::u32string& text, const glm::uvec4& area, const glm::uvec4& bgColor, const glm::uvec4& color)
		: GUIItem()
		{
			Init(layer, text, area, bgColor, color);
		}
		
		void Button::Init(Layer* layer,  const std::u32string& text, const glm::uvec4& area, const glm::uvec4& bgColor, const glm::uvec4& color)
		{
			// model
			parentLayer = layer;
			m_elementName = "Button_" + utf8::utf32to8(text);
			m_text = text;
			layerArea = area;
			m_backgroundColor = bgColor;
			m_textColor = color;
			
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
				MSE_CORE_LOG("Button: requesting to create a texture");
				MSE_CORE_TRACE("Button_parentLayer = ", parentLayer);
				m_texture = ResourceManager::CreateTexture(
					parentLayer->GetWindow(),
					parentLayer->GetWindow()->GetRenderer(),
					layerArea.z * 4,
					layerArea.w,
					0,
					32,
					{0, 0, 0, 0});
				MSE_CORE_LOG("Button: texture obtained");
				
				mse::Resource* bmpFont = mse::ResourceManager::UseResource(mse::ResourceType::FontBitmap, "./data/fonts/my8bit3.bmp", parentLayer->GetWindow());
				
				// still button
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
				
				// hover button
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
				
				// pushed button
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
				
				// disabled button
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
			callbacks[EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
//				MSE_CORE_LOG(m_elementName, ": ...");
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
		
		Button::~Button()
		{}
		
		// general GUIItem interface
		void Button::Display()
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
		
		// unique Button interface
		bool Button::HandleEvent(EventTypes eventType, SDL_Event* event)
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
							isPushed = true;
							callbacks[eventType](event);
						}
						return true;
					}
				case EventTypes::GUIItemMouseButtonUp:
					{
						if (isEnabled)
						{
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
