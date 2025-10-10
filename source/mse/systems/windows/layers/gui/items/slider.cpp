#include <mse/systems/windows/layers/gui/items/slider.h>
#include <mse/systems/windows/layers/gui/items/button.h>
#include <mse/systems/windows/layers/gui/items/image.h>
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
        HSlider::HSlider()
        : GUIItem()
        {
            Init(nullptr, {0, 0, 0, 0}, nullptr, 0, 0, 0, "", {0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0});
        }
        
        HSlider::HSlider(Layer* layer, const glm::uvec4& area, float* var, float min, float max, float step, const std::string&  spritelist, const glm::uvec3& colorKey, const glm::uvec4& btnLeft, const glm::uvec4& btnBall, const glm::uvec4& btnRight, const glm::uvec4& sliderImgLeft, const glm::uvec4& sliderImgMid, const glm::uvec4& sliderImgRight)
        : GUIItem()
        {
            Init(layer, area, var, min, max, step, spritelist, colorKey, btnLeft, btnBall, btnRight, sliderImgLeft, sliderImgMid, sliderImgRight);
        }
        
        void HSlider::Init(Layer* layer, const glm::uvec4& area, float* var, float min, float max, float step, const std::string&  spritelist, const glm::uvec3& colorKey, const glm::uvec4& btnLeft, const glm::uvec4& btnBall, const glm::uvec4& btnRight, const glm::uvec4& sliderImgLeft, const glm::uvec4& sliderImgMid, const glm::uvec4& sliderImgRight)
        {
            // model
            parentLayer = layer;
            windowUser = layer->GetWindow();
            m_elementName = "HScrollbar";
            layerArea = area;
            
            varMin = min;
            varMax = max;
            varStep = step;
            
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
                int btnLeftWidth = btnLeft.z / 4;
                int btnLeftHeight = btnLeft.w;
                m_btnLeft = (Button*)(layer->AddElement(new Button(
                                                                 layer, 
                                                                 {area.x, area.y, btnLeftWidth, btnLeftHeight}, 
                                                                 spritelist, 
                                                                 colorKey, 
                                                                 {btnLeft.x, btnLeft.y, btnLeftWidth, btnLeftHeight},
                                                                 {btnLeft.x + btnLeftWidth, btnLeft.y, btnLeftWidth, btnLeftHeight},
                                                                 {btnLeft.x + 2*btnLeftWidth, btnLeft.y, btnLeftWidth, btnLeftHeight},
                                                                 {btnLeft.x + 3*btnLeftWidth, btnLeft.y, btnLeftWidth, btnLeftHeight})));
                MSE_CORE_LOG("HScrollbar: btnLeft constructed");
                m_btnLeft->callbacks[EventTypes::GUIItemMouseButtonUp] = [=](SDL_Event* event){
                    *var -= varStep;
                    if (*var < varMin)
                    {
                        *var = varMin;
                    }
                    valueChanged = true;
                    SDL_Event* e = new SDL_Event();
                    SDL_PushEvent(e);
                    
                    float areaPercentage = *var / varMax;
                    float areaPixels = layerArea.z - btnLeft.z / 4 - btnRight.z / 4 - btnBall.z / 4 - 2;
                    int xrelPixels = (int)roundf(areaPercentage * areaPixels);
                    MSE_CORE_LOG("Var = ", *var, "; area: ", areaPixels, "; xrel: ", xrelPixels, "; area.z: ", layerArea.z);
                    
                    m_BtnBall->layerArea.x = layerArea.x + btnLeftWidth + xrelPixels + 1;
                };
                MSE_CORE_LOG("HScrollbar: btnLeft callback set up");
                
                
                // draw ball btn
                int btnBallWidth = btnBall.z / 4;
                int btnBallHeight = btnBall.w;
                int where = (int)roundf((layerArea.z - btnBallWidth - btnLeftWidth - btnRight.z / 4 - 2) * (*var / varMax));
                
                m_BtnBall = (Button*)(layer->AddElement(new Button(
                                                                   layer,
                                                                   {area.x + where + btnLeftWidth + 1, area.y + 1, btnBallWidth, btnBallHeight},
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
                            float areaPercentage = (float)xrelPixels / (area.z - btnLeftWidth - btnBallWidth - btnRight.z / 4 + 2);
                            float linesToScroll = varMax * areaPercentage;
                            
                            *var += linesToScroll;
                            if (*var < varMin)
                            {
                                *var = varMin;
                            }
                            if (*var > varMax)
                            {
                                *var = varMax;
                            }
                            valueChanged = true;
                            SDL_Event* e = new SDL_Event();
                            SDL_PushEvent(e);
                            MSE_CORE_LOG("lines to scroll: ", linesToScroll, "; volume = ", *var);
                            
                            m_BtnBall->layerArea.x += xrelPixels;
                            
                            if (m_BtnBall->layerArea.x < (area.x + btnLeftWidth + 1))
                            {
                                m_BtnBall->layerArea.x = area.x + btnLeftWidth + 1;
                            }
                            if (m_BtnBall->layerArea.x > (area.x + area.z - btnRight.z / 4 - btnLeftWidth + 2))
                            {
                                m_BtnBall->layerArea.x = area.x + area.z - btnRight.z / 4 - btnLeftWidth + 2;
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
                                                                                       {area.x + btnLeftWidth, area.y, area.z - btnLeftWidth - btnRight.z / 4, area.w},
                                                                                       spritelist,
                                                                                       colorKey,
                                                                                       sliderImgLeft,
                                                                                       sliderImgMid,
                                                                                       sliderImgRight
                                                                                       )));
                MSE_CORE_LOG("HScrollbar: HImage Template");
                
                // draw btn to scroll down
                int btnRightWidth = btnRight.z / 4;
                int btnRightHeight = btnRight.w;
                m_btnRight = (Button*)(layer->AddElement(new Button(
                                                                   layer, 
                                                                   {area.x + area.z - btnRightWidth, area.y, btnLeftWidth, btnLeftHeight}, 
                                                                   spritelist, 
                                                                   colorKey, 
                                                                   {btnRight.x, btnRight.y, btnRightWidth, btnRightHeight},
                                                                   {btnRight.x + btnRightWidth, btnRight.y, btnRightWidth, btnRightHeight},
                                                                   {btnRight.x + 2*btnRightWidth, btnRight.y, btnRightWidth, btnRightHeight},
                                                                   {btnRight.x + 3*btnRightWidth, btnRight.y, btnRightWidth, btnRightHeight})));
                MSE_CORE_LOG("HScrollbar: btnRight constructed");
                m_btnRight->callbacks[EventTypes::GUIItemMouseButtonUp] = [=](SDL_Event* event){
                    *var += varStep;
                    if (*var > varMax)
                    {
                        *var = varMax;
                    }
                    valueChanged = true;
                    SDL_Event* e = new SDL_Event();
                    e->type = SDL_EventType::SDL_USEREVENT;
                    SDL_PushEvent(e);
                    
                    float areaPercentage = *var / varMax;
                    float areaPixels = layerArea.z - btnLeftWidth - btnRightWidth - btnBall.z / 4 - 2;
                    int xrelPixels = (int)roundf(areaPercentage * areaPixels);
                    
                    MSE_CORE_LOG("Var = ", *var, "; area: ", areaPixels, "; xrel: ", xrelPixels, "; area.z: ", layerArea.z);
                    
//                    m_BtnBall->layerArea.x = xrelPixels;
                    m_BtnBall->layerArea.x = layerArea.x + btnLeftWidth + xrelPixels + 1;
                };
                MSE_CORE_LOG("HScrollbar: btnRight callback set up");
                
                ((Texture*)(m_texture->data))->Update();
                
                MSE_CORE_LOG("HScrollbar: items set up");
            }
        }
        
        HSlider::~HSlider()
        {
            
        }
        
        void HSlider::Display()
        {}
        
        bool HSlider::HandleEvent(EventTypes eventType, SDL_Event* event)
        {
            if (valueChanged)
            {
                valueChanged = false;
                if (callbacks.find(EventTypes::GUIItemChangeValue) != callbacks.end())
                {
                    callbacks[EventTypes::GUIItemChangeValue](event);
                    return true;
                }
            }
            return false;
        }
	}
}
