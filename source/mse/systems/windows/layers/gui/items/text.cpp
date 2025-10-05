#include <mse/systems/windows/layers/gui/items/text.h>
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
		Text::Text()
		: GUIItem()
		{
			Init(nullptr, U"", {0, 0, 1, 1}, {0, 0, 0, 0}, {255, 255, 255, 255}, 1, false, {0, 0, 0, 0});
		}
		
		Text::Text(Layer* layer,  const std::u32string& text, const glm::uvec4& area, const glm::uvec4& bgColor, const glm::uvec4& color, int pxSize, bool border, const glm::uvec4& borderColor)
		: GUIItem()
		{
			Init(layer, text, area, bgColor, color, pxSize, border, borderColor);
		}
		
		void Text::Init(Layer* layer,  const std::u32string& text, const glm::uvec4& area, const glm::uvec4& bgColor, const glm::uvec4& color, int pxSize, bool border, const glm::uvec4& borderColor)
		{
			// model
			parentLayer = layer;
            windowUser = layer->GetWindow();
			m_elementName = "Text_" + utf8::utf32to8(text);
			m_text = text;
            m_pxSize = pxSize;
			layerArea = area;
			m_backgroundColor = bgColor;
            m_borderColor = borderColor;
			m_textColor = color;
            showBorder = border;
			
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
				MSE_CORE_LOG("Text: requesting to create a texture");
				MSE_CORE_TRACE("Text_parentLayer = ", parentLayer);
				m_texture = ResourceManager::CreateTexture(
					windowUser,
					windowUser->GetRenderer(),
					layerArea.z,
					layerArea.w,
					0,
					32,
					{0, 0, 0, 0});
				MSE_CORE_LOG("Text: texture obtained");
				
				mse::Resource* bmpFont = mse::ResourceManager::UseResource(mse::ResourceType::FontBitmap, "./data/fonts/my8bit2.bmp", parentLayer->GetWindow());
				
				// text with border
                if (showBorder && (layerArea.z > 0) && (layerArea.w > 0))
                {
                    Renderer::SurfaceDrawRect(
                        (Texture*)(m_texture->data),
                        {0, 0, layerArea.z-1, layerArea.w-1},
                        m_pxSize,
                        {m_borderColor.x, m_borderColor.y, m_borderColor.z, m_borderColor.w}
                        );
                }
                if ((m_backgroundColor.w > 0) && (layerArea.z > 2) && (layerArea.w > 2))
                {
                    Renderer::SurfaceDrawRectFilled(
                                                    (Texture*)(m_texture->data),
                                                    {0 + 1, 1, layerArea.z - 2, layerArea.w - 2}, 
                                                    {m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, m_backgroundColor.w}
                                                    );
                }
				mse::Renderer::SurfaceDrawText(
					(Texture*)(m_texture->data), 
					{m_scrollXY.x + 2, m_scrollXY.y + 2, layerArea.z, layerArea.w}, 	// where to
					m_pxSize,             // pixel size
					m_text, 			// text content
					bmpFont, 			// font
					{color.x, color.y, color.z, color.w}, // color
					0); 				// interval between rows
				
				((Texture*)(m_texture->data))->Update();
				
				MSE_CORE_LOG("Text: texture edited");
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
		
		Text::~Text()
		{
            if (m_texture != nullptr)
            {
                ResourceManager::DropResource(m_texture, windowUser);
                MSE_CORE_LOG("Text: texture dropped");
            }
        }
		
		// general GUIItem interface
		void Text::Display()
		{
			SDL_FRect destRect = {
				(float)layerArea.x / WindowManager::GetCurrentWindow()->GetPrefs().width,
				(float)layerArea.y / WindowManager::GetCurrentWindow()->GetPrefs().height,
				(float)layerArea.z / WindowManager::GetCurrentWindow()->GetPrefs().width,
				(float)layerArea.w / WindowManager::GetCurrentWindow()->GetPrefs().height,
			};
			
			SDL_Rect srcRect = {
                0, 0, layerArea.z, layerArea.w
            };
			
			Renderer::DrawTexture((Texture*)(m_texture->data), &destRect, &srcRect);
		}
        
        bool Text::HandleEvent(EventTypes eventType, SDL_Event* event)
        {
            return false;
        }
        
        void Text::ChangeText(const std::u32string& text)
        {
            m_text = text;
            
            mse::Resource* bmpFont = mse::ResourceManager::UseResource(mse::ResourceType::FontBitmap, "./data/fonts/my8bit2.bmp", parentLayer->GetWindow());
            
            // text with border
            if (showBorder && (layerArea.z > 0) && (layerArea.w > 0))
            {
                Renderer::SurfaceDrawRect(
                                          (Texture*)(m_texture->data),
                                          {0, 0, layerArea.z-1, layerArea.w-1},
                                          m_pxSize,
                                          {m_borderColor.x, m_borderColor.y, m_borderColor.z, m_borderColor.w}
                                          );
            }
            if ((m_backgroundColor.w > 0) && (layerArea.z > 2) && (layerArea.w > 2))
            {
                Renderer::SurfaceDrawRectFilled(
                                                (Texture*)(m_texture->data),
                                                {0 + 1, 1, layerArea.z - 2, layerArea.w - 2}, 
                                                {m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, m_backgroundColor.w}
                                                );
            }
            mse::Renderer::SurfaceDrawText(
                (Texture*)(m_texture->data), 
                {m_scrollXY.x + 2, m_scrollXY.y + 2, layerArea.z, layerArea.w}, 	// where to
                m_pxSize,			// pixel size
                m_text, 			// text content
                bmpFont, 			// font
                {m_textColor.x, m_textColor.y, m_textColor.z, m_textColor.w}, // color
                0); 				// interval between rows
            
            ((Texture*)(m_texture->data))->Update();
            
            MSE_CORE_LOG("Text: texture edited");
        }
        
        void Text::Scroll(int x, int y)
        {
            m_scrollXY.x -= x;
            if (m_scrollXY.x < 0)
            {
                m_scrollXY.x = 0;
            }
            m_scrollXY.y -= y;
            if (m_scrollXY.y < 0)
            {
                m_scrollXY.y = 0;
            }
            
            ChangeText(m_text);
        }
	}
}
