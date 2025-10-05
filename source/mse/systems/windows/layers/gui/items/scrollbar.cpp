#include <mse/systems/windows/layers/gui/items/scrollbar.h>
#include <mse/systems/windows/layers/gui/items/text.h>
#include <mse/systems/windows/layers/gui/items/button.h>
#include <mse/systems/windows/layers/gui/items/image.h>
#include <mse/systems/platform/platform.h>
#include <mse/systems/platform/renderer/renderer.h>
#include <mse/systems/platform/renderer/texture.h>
#include <mse/systems/platform/renderer/font.h>
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
		VScrollbar::VScrollbar()
		: GUIItem()
		{
			Init(nullptr, {0, 0, 0, 0}, nullptr, nullptr, {0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0});
		}
		
		VScrollbar::VScrollbar(Layer* layer, const glm::uvec4& area, Text* textItem, const std::string& spritelist, const glm::uvec3& colorKey, const glm::uvec4& btnUp, const glm::uvec4& body, const glm::uvec4& btnDown)
		: GUIItem()
		{
			Init(layer, area, textItem, spritelist, colorKey, btnUp, body, btnDown);
		}
		
		void VScrollbar::Init(Layer* layer, const glm::uvec4& area, Text* textItem, const std::string& spritelist, const glm::uvec3& colorKey, const glm::uvec4& btnUp, const glm::uvec4& body, const glm::uvec4& btnDown)
		{
			// model
			parentLayer = layer;
            windowUser = layer->GetWindow();
			m_elementName = "VScrollbar";
			layerArea = area;
            m_textItem = textItem;
			
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
				MSE_CORE_LOG("VScrollbar: requesting to create a texture");
				MSE_CORE_TRACE("VScrollbar_parentLayer = ", parentLayer);
				m_texture = ResourceManager::CreateTexture(
					windowUser,
					windowUser->GetRenderer(),
					layerArea.z,
					layerArea.w,
					0,
					32,
					{0, 0, 0, 0});
				MSE_CORE_LOG("VScrollbar: texture obtained");
				
                // draw element
                int btnUpWidth = btnUp.z / 4;
                int btnUpHeight = btnUp.w;
                m_BtnUp = (Button*)(layer->AddElement(new Button(
                             layer, 
                             {area.x, area.y, btnUpWidth, btnUpHeight}, 
                             spritelist, 
                             colorKey, 
                             {btnUp.x, btnUp.y, btnUpWidth, btnUpHeight},
                             {btnUp.x + btnUpWidth, btnUp.y, btnUpWidth, btnUpHeight},
                             {btnUp.x + 2*btnUpWidth, btnUp.y, btnUpWidth, btnUpHeight},
                             {btnUp.x + 3*btnUpWidth, btnUp.y, btnUpWidth, btnUpHeight})));
                m_BtnUp->callbacks[EventTypes::GUIItemMouseButtonUp] = [=](SDL_Event* event){
                    m_textItem->Scroll(0, 10);
                };
                
                int btnDownWidth = btnDown.z / 4;
                int btnDownHeight = btnDown.w;
                m_BtnDown = (Button*)(layer->AddElement(new Button(
                             layer, 
                             {area.x, area.y + area.w - btnDownHeight, btnUpWidth, btnUpHeight}, 
                             spritelist, 
                             colorKey, 
                             {btnDown.x, btnDown.y, btnDownWidth, btnDownHeight},
                             {btnDown.x + btnDownWidth, btnDown.y, btnDownWidth, btnDownHeight},
                             {btnDown.x + 2*btnDownWidth, btnDown.y, btnDownWidth, btnDownHeight},
                             {btnDown.x + 3*btnDownWidth, btnDown.y, btnDownWidth, btnDownHeight})));
                m_BtnDown->callbacks[EventTypes::GUIItemMouseButtonUp] = [=](SDL_Event* event){
                    m_textItem->Scroll(0, -10);
                };
                
                ((Texture*)(m_texture->data))->Update();
				
				MSE_CORE_LOG("VScrollbar: items set up");
			}
			
			// controller
			// setup interaction
//			callbacks[EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
////				MSE_CORE_LOG(m_elementName, ": ...");
//			};
//			
//			callbacks[EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
////				MSE_CORE_LOG(m_elementName, ": Yay, you clicked me!");
//			};
//			
//			callbacks[EventTypes::GUIItemMouseOver] = [&](SDL_Event* event){
////				MSE_CORE_LOG(m_elementName, ": Hello, Mouse!");
//			};
//			
//			callbacks[EventTypes::GUIItemMouseOut] = [&](SDL_Event* event){
////				MSE_CORE_LOG(m_elementName, ": Goodbye, Mouse!");
//			};
		}
		
		VScrollbar::~VScrollbar()
		{
            MSE_CORE_LOG("VScrollbar destroyed");
        }
		
		// general GUIItem interface
		void VScrollbar::Display()
		{
            m_BtnUp->Display();
            m_BtnDown->Display();
//			SDL_FRect destRect = {
//				(float)layerArea.x / WindowManager::GetCurrentWindow()->GetPrefs().width,
//				(float)layerArea.y / WindowManager::GetCurrentWindow()->GetPrefs().height,
//				(float)layerArea.z / WindowManager::GetCurrentWindow()->GetPrefs().width,
//				(float)layerArea.w / WindowManager::GetCurrentWindow()->GetPrefs().height,
//			};
//			
//			SDL_Rect srcRect = {
//                0, 0, layerArea.z, layerArea.w
//            };
//			
//			Renderer::DrawTexture((Texture*)(m_texture->data), &destRect, &srcRect);
		}
        
        bool VScrollbar::HandleEvent(EventTypes eventType, SDL_Event* event)
        {
            return false;
        }
        
        void VScrollbar::StateProcessor()
        {
            switch (state)
            {
            case mse::gui::VScrollbarStates::BallPanelReleased:
                {
                    break;
                }
            case mse::gui::VScrollbarStates::BtnUpHover:
                {
                    break;
                }
            case mse::gui::VScrollbarStates::BtnUpPressed:
                {
                    break;
                }
            case mse::gui::VScrollbarStates::BtnUpReleased:
                {
                    break;
                }
            case mse::gui::VScrollbarStates::BtnDownHover:
                {
                    break;
                }
            case mse::gui::VScrollbarStates::BtnDownPressed:
                {
                    break;
                }
            case mse::gui::VScrollbarStates::BtnDownReleased:
                {
                    break;
                }
            case mse::gui::VScrollbarStates::BtnBallHover:
                {
                    break;
                }
            case mse::gui::VScrollbarStates::BtnBallPressed:
                {
                    break;
                }
            case mse::gui::VScrollbarStates::BtnBallReleased:
                {
                    break;
                }
            case mse::gui::VScrollbarStates::BallPanelClicked:
                {
                    break;
                }
            }
        }
	}
}
