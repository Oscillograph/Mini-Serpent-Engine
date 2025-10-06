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
        
        Button::Button(Layer* layer,  const std::u32string& text, const glm::uvec4& color, const glm::uvec4& area, const std::string& spritelist, const glm::uvec4& leftSource, const glm::uvec4& midSource, const glm::uvec4& rightSource)
        : GUIItem()
        {
            Init(layer, text, color, area, spritelist, leftSource, midSource, rightSource);
        }
        
        Button::Button(Layer* layer, const glm::uvec4& area, const std::string& spritelist, const glm::uvec3& colorKey, const glm::uvec4& sourceStill, const glm::uvec4& sourceHover, const glm::uvec4& sourcePressed, const glm::uvec4& sourceDisabled)
        : GUIItem()
        {
            parentLayer = layer;
            windowUser = layer->GetWindow();
            layerArea = area;
            m_elementName = "Button (image)";
            
            // view
            if (layer != nullptr)
            {
                layerMask.resize(area.z * area.w);
                for (int x = 0; x < area.z; ++x)
                {
                    for (int y = 0; y < area.w; ++y)
                    {
                        layerMask[x + y*area.z] = id;
                    }
                }
                
                m_spriteList = ResourceManager::UseTexture(spritelist, windowUser, colorKey);
                int textureWidth = layerArea.z * 4;
                int textureHeight = layerArea.w;
                
                // setup texture to draw on
                MSE_CORE_LOG("Button: requesting to create a texture");
                MSE_CORE_TRACE("Button_parentLayer = ", parentLayer);
                m_texture = ResourceManager::CreateTexture(
                   windowUser,
                   windowUser->GetRenderer(),
                   textureWidth,
                   textureHeight,
                   0,
                   32,
                   {0, 0, 0, 0});
                MSE_CORE_LOG("Button: texture obtained");
                
                // still button
                SDL_FRect destRect = {0, 0, layerArea.z, layerArea.w};
                SDL_Rect sourceRect = {sourceStill.x, sourceStill.y, sourceStill.z, sourceStill.w};
                Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                             (Texture*)(m_spriteList->data),
                                             &destRect,
                                             &sourceRect);
                // hover button
                destRect = {layerArea.z, 0, layerArea.z, layerArea.w};
                sourceRect = {sourceHover.x, sourceHover.y, sourceHover.z, sourceHover.w};
                Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                             (Texture*)(m_spriteList->data),
                                             &destRect,
                                             &sourceRect);
                // pressed button
                destRect = {2*layerArea.z, 0, layerArea.z, layerArea.w};
                sourceRect = {sourcePressed.x, sourcePressed.y, sourcePressed.z, sourcePressed.w};
                Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                             (Texture*)(m_spriteList->data),
                                             &destRect,
                                             &sourceRect);
                // disabled button
                destRect = {3*layerArea.z, 0, layerArea.z, layerArea.w};
                sourceRect = {sourceDisabled.x, sourceDisabled.y, sourceDisabled.z, sourceDisabled.w};
                Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                             (Texture*)(m_spriteList->data),
                                             &destRect,
                                             &sourceRect);
                
                ((Texture*)(m_texture->data))->Update();
                
                MSE_CORE_LOG("Button: texture edited");
            }
            
            // controller
            callbacks[EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
            };
            
            callbacks[EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
            };
            
            callbacks[EventTypes::GUIItemMouseOver] = [&](SDL_Event* event){
            };
            
            callbacks[EventTypes::GUIItemMouseOut] = [&](SDL_Event* event){
            };
        }
		
        // generic button
		void Button::Init(Layer* layer,  const std::u32string& text, const glm::uvec4& area, const glm::uvec4& bgColor, const glm::uvec4& color)
		{
			// model
			parentLayer = layer;
            windowUser = layer->GetWindow();
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
					windowUser,
					windowUser->GetRenderer(),
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
		
        // sprite-based button
        void Button::Init(Layer* layer,  const std::u32string& text, const glm::uvec4& color, const glm::uvec4& area, const std::string& spritelist, const glm::uvec4& leftSource, const glm::uvec4& midSource, const glm::uvec4& rightSource)
        {
            // model
            parentLayer = layer;
            windowUser = layer->GetWindow();
            m_elementName = "Button_" + utf8::utf32to8(text);
            m_text = text;
            layerArea = area;
            m_spriteList = ResourceManager::UseTexture(spritelist, windowUser, {0, 0, 0});
            m_leftSource = leftSource;
            m_midSource = midSource;
            m_rightSource = rightSource;
            
            layerMask.resize(area.z * area.w);
            for (int x = 0; x < area.z; ++x)
            {
                for (int y = 0; y < area.w; ++y)
                {
                    layerMask[x + y*area.z] = id;
                }
            }
            
            // view
            if ((layer != nullptr) && (m_spriteList != nullptr))
            {
                // setup texture to draw on
                MSE_CORE_LOG("Button: requesting to create a texture");
                MSE_CORE_TRACE("Button_parentLayer = ", parentLayer);
                m_texture = ResourceManager::CreateTexture(
                                                           windowUser,
                                                           windowUser->GetRenderer(),
                                                           layerArea.z * 4,
                                                           layerArea.w,
                                                           0,
                                                           32,
                                                           {0, 0, 0, 0});
                MSE_CORE_LOG("Button: texture obtained");
                Renderer::SurfaceDrawRectFilled(
                                                (Texture*)(m_texture->data),
                                                {0, 0, layerArea.z * 4, layerArea.w}, 
//                                                {m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, m_backgroundColor.w}
                                                {0, 0, 0, 255}
                                                );
                
                Resource* bmpFont = ResourceManager::UseResource(mse::ResourceType::FontBitmap, "./data/fonts/my8bit2.bmp", parentLayer->GetWindow());
                
                // number of middle pixels
                int middles = layerArea.z - leftSource.z - rightSource.z;
                
                // from where and where to draw
                SDL_Rect sourceRect;
                SDL_FRect destRect;
                
                // still button
                destRect = {0, 0, leftSource.z, leftSource.w};
                sourceRect = {leftSource.x, leftSource.y, leftSource.z, leftSource.w};
                Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                             (Texture*)(m_spriteList->data),
                                             &destRect,
                                             &sourceRect);
                sourceRect = {midSource.x, midSource.y, 1, midSource.w};
                for (int i = 0; i < middles; i++)
                {
                    destRect = {leftSource.z + i, 0, 1, midSource.w};
                    Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                                 (Texture*)(m_spriteList->data),
                                                 &destRect,
                                                 &sourceRect);
                }
                sourceRect = {rightSource.x, rightSource.y, rightSource.z, rightSource.w};
                destRect = {leftSource.z + middles, 0, rightSource.z, rightSource.w};
                Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                             (Texture*)(m_spriteList->data),
                                             &destRect,
                                             &sourceRect);
                
                Renderer::SurfaceDrawText(
                                               (Texture*)(m_texture->data), 
                                               {leftSource.z + 2, 3, layerArea.z, layerArea.w}, 	// where to
                                               1, 					// pixel size
                                               m_text, 			// text content
                                               bmpFont, 			// font
                                               {color.x, color.y, color.z, color.w}, // color
                                               0); 				// interval between rows
                
                // hover button
                destRect = {layerArea.z, 0, leftSource.z, leftSource.w};
                sourceRect = {leftSource.x + leftSource.z, leftSource.y, leftSource.z, leftSource.w};
                Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                             (Texture*)(m_spriteList->data),
                                             &destRect,
                                             &sourceRect);
                sourceRect = {midSource.x + midSource.z, midSource.y, 1, midSource.w};
                for (int i = 0; i < middles; i++)
                {
                    destRect = {layerArea.z + leftSource.z + i, 0, 1, midSource.w};
                    Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                                 (Texture*)(m_spriteList->data),
                                                 &destRect,
                                                 &sourceRect);
                }
                sourceRect = {rightSource.x + rightSource.z, rightSource.y, rightSource.z, rightSource.w};
                destRect = {layerArea.z + leftSource.z + middles, 0, rightSource.z, rightSource.w};
                Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                             (Texture*)(m_spriteList->data),
                                             &destRect,
                                             &sourceRect);
                
                Renderer::SurfaceDrawText(
                                          (Texture*)(m_texture->data), 
                                          {layerArea.z + leftSource.z + 2, 3, layerArea.z, layerArea.w}, 	// where to
                                          1, 					// pixel size
                                          m_text, 			// text content
                                          bmpFont, 			// font
                                          {color.x, color.y, color.z, color.w}, // color
                                          0); 				// interval between rows
                
                // pushed button
                destRect = {layerArea.z * 2, 0, leftSource.z, leftSource.w};
                sourceRect = {leftSource.x + leftSource.z * 2, leftSource.y, leftSource.z, leftSource.w};
                Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                             (Texture*)(m_spriteList->data),
                                             &destRect,
                                             &sourceRect);
                sourceRect = {midSource.x + midSource.z * 2, midSource.y, 1, midSource.w};
                for (int i = 0; i < middles; i++)
                {
                    destRect.x = layerArea.z * 2 + leftSource.z + i;
                    destRect.y = 0;
                    destRect.w = 1;
                    destRect.h = midSource.w;
                    Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                                 (Texture*)(m_spriteList->data),
                                                 &destRect,
                                                 &sourceRect);
                }
                sourceRect = {rightSource.x + rightSource.z * 2, rightSource.y, rightSource.z, rightSource.w};
                destRect = {layerArea.z * 2 + leftSource.z + middles, 0, rightSource.z, rightSource.w};
                Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                             (Texture*)(m_spriteList->data),
                                             &destRect,
                                             &sourceRect);
                
                Renderer::SurfaceDrawText(
                                          (Texture*)(m_texture->data), 
                                          {layerArea.z * 2 + leftSource.z + 2, 3, layerArea.z, layerArea.w}, 	// where to
                                          1, 					// pixel size
                                          m_text, 			// text content
                                          bmpFont, 			// font
                                          {color.x, color.y, color.z, color.w}, // color
                                          0); 				// interval between rows
                
                // disabled button
                destRect = {layerArea.z * 3, 0, leftSource.z, leftSource.w};
                sourceRect = {leftSource.x + leftSource.z * 3, leftSource.y, leftSource.z, leftSource.w};
                Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                             (Texture*)(m_spriteList->data),
                                             &destRect,
                                             &sourceRect);
                sourceRect = {midSource.x + midSource.z * 3, midSource.y, 1, midSource.w};
                for (int i = 0; i < middles; i++)
                {
                    destRect = {layerArea.z * 3 + leftSource.z + i, 0, 1, midSource.w};
                    Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                                 (Texture*)(m_spriteList->data),
                                                 &destRect,
                                                 &sourceRect);
                }
                sourceRect = {rightSource.x + rightSource.z * 3, rightSource.y, rightSource.z, rightSource.w};
                destRect = {layerArea.z * 3 + leftSource.z + middles, 0, rightSource.z, rightSource.w};
                Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                      (Texture*)(m_spriteList->data),
                                      &destRect,
                                      &sourceRect);
                
                Renderer::SurfaceDrawText(
                                          (Texture*)(m_texture->data), 
                                          {layerArea.z * 3 + leftSource.z + 2, 3, layerArea.z, layerArea.w}, 	// where to
                                          1, 					// pixel size
                                          m_text, 			// text content
                                          bmpFont, 			// font
                                          {color.x, color.y, color.z, color.w}, // color
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
		{
            if (m_texture != nullptr)
            {
                ResourceManager::DropResource(m_texture, windowUser);
                MSE_CORE_LOG("Button: texture dropped");
            }
        }
		
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
                            state = ButtonStates::Pressed;
							isPushed = true;
							callbacks[eventType](event);
						}
						return true;
					}
				case EventTypes::GUIItemMouseButtonUp:
					{
						if (isEnabled)
						{
                            state = ButtonStates::Released;
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
