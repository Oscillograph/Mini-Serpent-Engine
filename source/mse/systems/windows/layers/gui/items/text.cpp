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
			Init(nullptr, U"", {0, 0, 1, 1}, {0, 0, 0, 0}, {255, 255, 255, 255}, 1);
		}
		
		Text::Text(Layer* layer,  const std::u32string& text, const glm::uvec4& area, const glm::uvec4& bgColor, const glm::uvec4& color, int pxSize)
		: GUIItem()
		{
			Init(layer, text, area, bgColor, color, pxSize);
		}
		
		void Text::Init(Layer* layer,  const std::u32string& text, const glm::uvec4& area, const glm::uvec4& bgColor, const glm::uvec4& color, int pxSize)
		{
			// model
			parentLayer = layer;
			m_elementName = "Text_" + utf8::utf32to8(text);
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
				MSE_CORE_LOG("Text: requesting to create a texture");
				MSE_CORE_TRACE("Text_parentLayer = ", parentLayer);
				m_texture = ResourceManager::CreateTexture(
					parentLayer->GetWindow(),
					parentLayer->GetWindow()->GetRenderer(),
					layerArea.z,
					layerArea.w,
					0,
					32,
					{0, 0, 0, 0});
				MSE_CORE_LOG("Text: texture obtained");
				
				mse::Resource* bmpFont = mse::ResourceManager::UseResource(mse::ResourceType::FontBitmap, "./data/fonts/my8bit3.bmp", parentLayer->GetWindow());
				
				// text with border
                if (showBorder)
                {
                    Renderer::SurfaceDrawRectFilled(
                        (Texture*)(m_texture->data),
                        {0, 0, layerArea.z, layerArea.w}, 
                        {255 - m_backgroundColor.x, 255 - m_backgroundColor.y, 255 - m_backgroundColor.z, m_backgroundColor.w}
                        );
                }
				Renderer::SurfaceDrawRectFilled(
					(Texture*)(m_texture->data),
					{0 + 1, 1, layerArea.z - 2, layerArea.w - 2}, 
					{m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, m_backgroundColor.w}
					);
				mse::Renderer::SurfaceDrawText(
					(Texture*)(m_texture->data), 
					{0 + 2, 2, layerArea.z, layerArea.w}, 	// where to
					pxSize,             // pixel size
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
		{}
		
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
            
            mse::Resource* bmpFont = mse::ResourceManager::UseResource(mse::ResourceType::FontBitmap, "./data/fonts/my8bit3.bmp", parentLayer->GetWindow());
            
            // text with border
            if (showBorder)
            {
                Renderer::SurfaceDrawRectFilled(
                    (Texture*)(m_texture->data),
                    {0, 0, layerArea.z, layerArea.w}, 
                    {255 - m_backgroundColor.x, 255 - m_backgroundColor.y, 255 - m_backgroundColor.z, m_backgroundColor.w}
                    );
            }
            Renderer::SurfaceDrawRectFilled(
                (Texture*)(m_texture->data),
                {0 + 1, 1, layerArea.z - 2, layerArea.w - 2}, 
                {m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, m_backgroundColor.w}
                );
            mse::Renderer::SurfaceDrawText(
                (Texture*)(m_texture->data), 
                {0 + 2, 2, layerArea.z, layerArea.w}, 	// where to
                1, 					// pixel size
                m_text, 			// text content
                bmpFont, 			// font
                {m_textColor.x, m_textColor.y, m_textColor.z, m_textColor.w}, // color
                0); 				// interval between rows
            
            ((Texture*)(m_texture->data))->Update();
            
            MSE_CORE_LOG("Text: texture edited");
        }
	}
}
