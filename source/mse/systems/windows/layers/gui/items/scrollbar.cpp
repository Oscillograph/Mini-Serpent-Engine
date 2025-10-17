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
        /******************************************************************************************/
        /*                                Vertical Scrollbar                                      */
        /******************************************************************************************/
		// general initialization
		VScrollbar::VScrollbar()
		: GUIItem()
		{
			Init(nullptr, {0, 0, 0, 0}, nullptr, nullptr, {0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0});
		}
		
		VScrollbar::VScrollbar(Layer* layer, const glm::uvec4& area, Text* textItem, const std::string& spritelist, const glm::uvec3& colorKey, const glm::uvec4& btnUp, const glm::uvec4& body, const glm::uvec4& btnDown, const glm::uvec4& sliderImgTop, const glm::uvec4& sliderImgMid, const glm::uvec4& sliderImgBottom)
		: GUIItem()
		{
			Init(layer, area, textItem, spritelist, colorKey, btnUp, body, btnDown, sliderImgTop, sliderImgMid, sliderImgBottom);
		}
		
		void VScrollbar::Init(Layer* layer, const glm::uvec4& area, Text* textItem, const std::string& spritelist, const glm::uvec3& colorKey, const glm::uvec4& btnUp, const glm::uvec4& btnBall, const glm::uvec4& btnDown, const glm::uvec4& sliderImgTop, const glm::uvec4& sliderImgMid, const glm::uvec4& sliderImgBottom)
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
				
                // draw btn to scroll up
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
                    m_textItem->Scroll(0, stepY);
                    
                    float areaPercentage = (float)stepY / (m_textItem->m_scrollXY.w);
                    int yrelPixels = (int)roundf((float)areaPercentage * m_sliderPanel->layerArea.w);
                    
                    m_BtnBall->layerArea.y += yrelPixels;
                    if (m_BtnBall->layerArea.y < (area.y + btnUp.w + 1))
                    {
                        m_BtnBall->layerArea.y = area.y + btnUp.w + 1;
                    }
                    if (m_BtnBall->layerArea.y > (area.y + area.w - btnDown.w - btnUp.w + 1))
                    {
                        m_BtnBall->layerArea.y = area.y + area.w - btnDown.w - btnUp.w + 1;
                    }
                };
         
                
                // draw ball btn
                int btnBallWidth = btnBall.z / 4;
                int btnBallHeight = btnBall.w;
                m_BtnBall = (Button*)(layer->AddElement(new Button(
                               layer,
                               {area.x + 1, area.y + btnUpHeight, btnBallWidth, btnBallHeight},
                               spritelist,
                               colorKey,
                               {btnBall.x, btnBall.y, btnBallWidth, btnBallHeight},
                               {btnBall.x + btnBallWidth, btnBall.y, btnBallWidth, btnBallHeight},
                               {btnBall.x + 2*btnBallWidth, btnBall.y, btnBallWidth, btnBallHeight},
                               {btnBall.x + 3*btnBallWidth, btnBall.y, btnBallWidth, btnBallHeight}
                               )));
                m_BtnBall->callbacks[EventTypes::GUIItemMouseMove] = [=](SDL_Event* event){
                    if (!correctingMousePosition)
                    {
                        if (m_BtnBall->isPushed)
                        {
                            int yrel = event->motion.yrel;
                            int yrelPixels = (int)roundf((float)yrel / windowUser->GetScale().y);
//                        float linesPercentage = (float)yrelPixels / m_textItem->m_scrollXY.w;
                            float areaPercentage = (float)yrelPixels / (area.w - btnUp.w - btnDown.w - btnBall.w + 1);
                            int linesToScroll = (int)roundf(m_textItem->m_scrollXY.w * areaPercentage);
                            
                            if (yrel < 0)
                            {
                                textItem->Scroll(0, linesToScroll);
                            } else {
                                if (yrel > 0)
                                {
                                    textItem->Scroll(0, linesToScroll);
                                }
                            }
                            
                            m_BtnBall->layerArea.y += yrelPixels;
                            
                            if (m_BtnBall->layerArea.y < (area.y + btnUp.w + 1))
                            {
                                m_BtnBall->layerArea.y = area.y + btnUp.w + 1;
                            }
                            if (m_BtnBall->layerArea.y > (area.y + area.w - btnDown.w - btnUp.w + 1))
                            {
                                m_BtnBall->layerArea.y = area.y + area.w - btnDown.w - btnUp.w + 1;
                            }
                            
                            correctingMousePosition = true;
                            SDL_WarpMouseInWindow((SDL_Window*)(windowUser->GetNativeWindow()), 
                                                  windowUser->GetScale().x * (2*m_BtnBall->layerArea.x + m_BtnBall->layerArea.z)/2,
                                                  windowUser->GetScale().y * (2*m_BtnBall->layerArea.y + m_BtnBall->layerArea.w)/2);
                        }
                    } else {
                        correctingMousePosition = false;
                    }
                };
                
                m_BtnBall->callbacks[EventTypes::GUIItemMouseOut] = [=](SDL_Event* event){
                    if (m_BtnBall->state == ButtonStates::Pressed)
                    {
                        m_BtnBall->isPushed = true;
                        correctingMousePosition = true;
                        SDL_WarpMouseInWindow((SDL_Window*)(windowUser->GetNativeWindow()), 
                                              windowUser->GetScale().x * (2*m_BtnBall->layerArea.x + m_BtnBall->layerArea.z)/2,
                                              windowUser->GetScale().y * (2*m_BtnBall->layerArea.y + m_BtnBall->layerArea.w)/2);
                    }
                };
                
                // draw slider panel
                m_sliderPanel = (VImageTemplate*)(layer->AddElement(new VImageTemplate(
                               layer,
                               {area.x, area.y + btnUpHeight, area.z, area.w - btnUp.w - btnDown.w},
                               spritelist,
                               colorKey,
                               sliderImgTop,
                               sliderImgMid,
                               sliderImgBottom
                               )));
                
                // draw btn to scroll down
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
                    m_textItem->Scroll(0, -stepY);
                        
                    float areaPercentage = -(float)stepY / (m_textItem->m_scrollXY.w);
                    int yrelPixels = (int)roundf((float)areaPercentage * m_sliderPanel->layerArea.w);
                    
                    m_BtnBall->layerArea.y += yrelPixels;
                    if (m_BtnBall->layerArea.y < (area.y + btnUp.w + 1))
                    {
                        m_BtnBall->layerArea.y = area.y + btnUp.w + 1;
                    }
                    if (m_BtnBall->layerArea.y > (area.y + area.w - btnDown.w - btnUp.w + 1))
                    {
                        m_BtnBall->layerArea.y = area.y + area.w - btnDown.w - btnUp.w + 1;
                    }
                };
                
                ((Texture*)(m_texture->data))->Update();
				
				MSE_CORE_LOG("VScrollbar: items set up");
			}
		}
		
		VScrollbar::~VScrollbar()
		{
            MSE_CORE_LOG("VScrollbar destroyed");
            delete m_BtnUp;
            m_BtnUp = nullptr;
            delete m_BtnDown;
            m_BtnDown = nullptr;
            delete m_BtnBall;
            m_BtnBall = nullptr;
        }
		
		// general GUIItem interface
		void VScrollbar::Display()
		{
//            m_BtnUp->Display();
//            m_BtnDown->Display();
		}
        
        bool VScrollbar::HandleEvent(EventTypes eventType, SDL_Event* event)
        {
            return false;
        }
        
        /******************************************************************************************/
        /*                                Horizontal Scrollbar                                    */
        /******************************************************************************************/
        // general initialization
        HScrollbar::HScrollbar()
        : GUIItem()
        {
            Init(nullptr, {0, 0, 0, 0}, nullptr, nullptr, {0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0});
        }
        
        HScrollbar::HScrollbar(Layer* layer, const glm::uvec4& area, Text* textItem, const std::string& spritelist, const glm::uvec3& colorKey, const glm::uvec4& btnUp, const glm::uvec4& body, const glm::uvec4& btnDown, const glm::uvec4& sliderImgLeft, const glm::uvec4& sliderImgMid, const glm::uvec4& sliderImgRight)
        : GUIItem()
        {
            Init(layer, area, textItem, spritelist, colorKey, btnUp, body, btnDown, sliderImgLeft, sliderImgMid, sliderImgRight);
        }
        
        void HScrollbar::Init(Layer* layer, const glm::uvec4& area, Text* textItem, const std::string& spritelist, const glm::uvec3& colorKey, const glm::uvec4& btnUp, const glm::uvec4& btnBall, const glm::uvec4& btnDown, const glm::uvec4& sliderImgLeft, const glm::uvec4& sliderImgMid, const glm::uvec4& sliderImgRight)
        {
            // model
            parentLayer = layer;
            windowUser = layer->GetWindow();
            m_elementName = "HScrollbar";
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
                MSE_CORE_LOG("HScrollbar: requesting to create a texture");
                MSE_CORE_TRACE("HScrollbar_parentLayer = ", parentLayer);
                m_texture = ResourceManager::CreateTexture(
                                                           windowUser,
                                                           windowUser->GetRenderer(),
                                                           layerArea.z,
                                                           layerArea.w,
                                                           0,
                                                           32,
                                                           {0, 0, 0, 0});
                MSE_CORE_LOG("HScrollbar: texture obtained");
                
                // draw btn to scroll up
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
                MSE_CORE_LOG("HScrollbar: BtnUp constructed");
                m_BtnUp->callbacks[EventTypes::GUIItemMouseButtonUp] = [=](SDL_Event* event){
                    m_textItem->Scroll(stepX, 0);
                    
                    float areaPercentage = (float)stepX / (m_textItem->m_scrollXY.z);
                    MSE_CORE_LOG("m_scrollXY: ", m_textItem->m_scrollXY.x, ", ", m_textItem->m_scrollXY.y, ", ", m_textItem->m_scrollXY.z, ", ", m_textItem->m_scrollXY.w, ", ");
                    int xrelPixels = (int)roundf((float)areaPercentage * m_sliderPanel->layerArea.z);
                    
                    m_BtnBall->layerArea.x += xrelPixels;
                    if (m_BtnBall->layerArea.x < (area.x + btnUpWidth + 1))
                    {
                        m_BtnBall->layerArea.x = area.x + btnUpWidth + 1;
                    }
                    if (m_BtnBall->layerArea.x > (area.x + area.z - btnDown.z / 4 - btnUpWidth + 1))
                    {
                        m_BtnBall->layerArea.x = area.x + area.z - btnDown.z / 4 - btnUpWidth + 1;
                    }
                };
                MSE_CORE_LOG("HScrollbar: BtnUp callback set up");
                
                
                // draw ball btn
                int btnBallWidth = btnBall.z / 4;
                int btnBallHeight = btnBall.w;
                m_BtnBall = (Button*)(layer->AddElement(new Button(
                                                                   layer,
                                                                   {area.x + btnUpWidth + 1, area.y + 1, btnBallWidth, btnBallHeight},
                                                                   spritelist,
                                                                   colorKey,
                                                                   {btnBall.x, btnBall.y, btnBallWidth, btnBallHeight},
                                                                   {btnBall.x + btnBallWidth, btnBall.y, btnBallWidth, btnBallHeight},
                                                                   {btnBall.x + 2*btnBallWidth, btnBall.y, btnBallWidth, btnBallHeight},
                                                                   {btnBall.x + 3*btnBallWidth, btnBall.y, btnBallWidth, btnBallHeight}
                                                                   )));
                MSE_CORE_LOG("HScrollbar: BtnBall constructed");
                m_BtnBall->callbacks[EventTypes::GUIItemMouseMove] = [=](SDL_Event* event){
                    if (!correctingMousePosition)
                    {
                        if (m_BtnBall->isPushed)
                        {
                            int xrel = event->motion.xrel;
                            int xrelPixels = (int)roundf((float)xrel / windowUser->GetScale().x);
//                        float linesPercentage = (float)yrelPixels / m_textItem->m_scrollXY.w;
                            float areaPercentage = (float)xrelPixels / (area.z - btnUpWidth - btnDown.z / 4 - btnBallWidth + 1);
                            int linesToScroll = (int)roundf(m_textItem->m_scrollXY.z * areaPercentage);
                            
                            if (xrel < 0)
                            {
                                textItem->Scroll(linesToScroll, 0);
                            } else {
                                if (xrel > 0)
                                {
                                    textItem->Scroll(linesToScroll, 0);
                                }
                            }
                            
                            m_BtnBall->layerArea.x += xrelPixels;
                            
                            if (m_BtnBall->layerArea.x < (area.x + btnUpWidth + 1))
                            {
                                m_BtnBall->layerArea.x = area.x + btnUpWidth + 1;
                            }
                            if (m_BtnBall->layerArea.x > (area.x + area.z - btnDown.z / 4 - btnUpWidth + 1))
                            {
                                m_BtnBall->layerArea.x = area.x + area.z - btnDown.z / 4 - btnUpWidth + 1;
                            }
                            
                            correctingMousePosition = true;
                            SDL_WarpMouseInWindow((SDL_Window*)(windowUser->GetNativeWindow()), 
                                                  windowUser->GetScale().x * (2*m_BtnBall->layerArea.x + m_BtnBall->layerArea.z)/2,
                                                  windowUser->GetScale().y * (2*m_BtnBall->layerArea.y + m_BtnBall->layerArea.w)/2);
                        }
                    } else {
                        correctingMousePosition = false;
                    }
                };
                MSE_CORE_LOG("HScrollbar: BtnBall callback set up");
                
                m_BtnBall->callbacks[EventTypes::GUIItemMouseOut] = [=](SDL_Event* event){
                    if (m_BtnBall->state == ButtonStates::Pressed)
                    {
                        m_BtnBall->isPushed = true;
                        correctingMousePosition = true;
                        SDL_WarpMouseInWindow((SDL_Window*)(windowUser->GetNativeWindow()), 
                                              windowUser->GetScale().x * (2*m_BtnBall->layerArea.x + m_BtnBall->layerArea.z)/2,
                                              windowUser->GetScale().y * (2*m_BtnBall->layerArea.y + m_BtnBall->layerArea.w)/2);
                    }
                };
                MSE_CORE_LOG("HScrollbar: BtnBall callback set up");
                
                // draw slider panel
                m_sliderPanel = (HImageTemplate*)(layer->AddElement(new HImageTemplate(
                                                                                       layer,
                                                                                       {area.x + btnUpWidth, area.y, area.z - btnUpWidth - btnDown.z / 4, area.w},
                                                                                       spritelist,
                                                                                       colorKey,
                                                                                       sliderImgLeft,
                                                                                       sliderImgMid,
                                                                                       sliderImgRight
                                                                                       )));
                MSE_CORE_LOG("HScrollbar: HImage Template");
                
                // draw btn to scroll down
                int btnDownWidth = btnDown.z / 4;
                int btnDownHeight = btnDown.w;
                m_BtnDown = (Button*)(layer->AddElement(new Button(
                                                                   layer, 
                                                                   {area.x + area.z - btnDownWidth, area.y, btnUpWidth, btnUpHeight}, 
                                                                   spritelist, 
                                                                   colorKey, 
                                                                   {btnDown.x, btnDown.y, btnDownWidth, btnDownHeight},
                                                                   {btnDown.x + btnDownWidth, btnDown.y, btnDownWidth, btnDownHeight},
                                                                   {btnDown.x + 2*btnDownWidth, btnDown.y, btnDownWidth, btnDownHeight},
                                                                   {btnDown.x + 3*btnDownWidth, btnDown.y, btnDownWidth, btnDownHeight})));
                MSE_CORE_LOG("HScrollbar: BtnDown constructed");
                m_BtnDown->callbacks[EventTypes::GUIItemMouseButtonUp] = [=](SDL_Event* event){
                    m_textItem->Scroll(-stepX, 0);
                    
                    float areaPercentage = -(float)stepX / (m_textItem->m_scrollXY.z);
                    int xrelPixels = (int)roundf((float)areaPercentage * m_sliderPanel->layerArea.z);
                    
                    m_BtnBall->layerArea.x += xrelPixels;
                    if (m_BtnBall->layerArea.x < (area.x + btnUpWidth + 1))
                    {
                        m_BtnBall->layerArea.x = area.x + btnUpWidth + 1;
                    }
                    if (m_BtnBall->layerArea.x > (area.x + area.z - btnDownWidth - btnUpWidth + 1))
                    {
                        m_BtnBall->layerArea.x = area.x + area.z - btnDownWidth - btnUpWidth + 1;
                    }
                };
                MSE_CORE_LOG("HScrollbar: BtnDown callback set up");
                
                ((Texture*)(m_texture->data))->Update();
                
                MSE_CORE_LOG("HScrollbar: items set up");
            }
        }
        
        HScrollbar::~HScrollbar()
        {
            MSE_CORE_LOG("HScrollbar destroyed");
            delete m_BtnUp;
            m_BtnUp = nullptr;
            delete m_BtnDown;
            m_BtnDown = nullptr;
            delete m_BtnBall;
            m_BtnBall = nullptr;
        }
        
        // general GUIItem interface
        void HScrollbar::Display()
        {
//            m_BtnUp->Display();
//            m_BtnDown->Display();
        }
        
        bool HScrollbar::HandleEvent(EventTypes eventType, SDL_Event* event)
        {
            return false;
        }
	}
}
