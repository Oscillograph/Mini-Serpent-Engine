#include <mse/systems/windows/layers/gui/items/checkbox.h>
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
        /*========================================================================================*/
        /*                                     Image                                              */
        /*========================================================================================*/
        // general initialization
		Checkbox::Checkbox()
		: GUIItem()
		{
			Init(nullptr, {0, 0, 1, 1}, "", {0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0});
		}
		
		Checkbox::Checkbox(Layer* layer, const glm::uvec4& area, const std::string& spritelist, const glm::uvec3& colorKey, const glm::uvec4& sourceUnchecked, const glm::uvec4& sourceChecked, const glm::uvec4& sourceDisabledUnchecked, const glm::uvec4& sourceDisabledChecked)
		: GUIItem()
		{
			Init(layer, area, spritelist, colorKey, sourceUnchecked, sourceChecked, sourceDisabledUnchecked, sourceDisabledChecked);
		}
		
        // generic button
		void Checkbox::Init(Layer* layer, const glm::uvec4& area, const std::string& spritelist, const glm::uvec3& colorKey, const glm::uvec4& sourceUnchecked, const glm::uvec4& sourceChecked, const glm::uvec4& sourceDisabledUnchecked, const glm::uvec4& sourceDisabledChecked)
        {
            parentLayer = layer;
            windowUser = layer->GetWindow();
            layerArea = area;
            m_elementName = "Checkbox";
            
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
                MSE_CORE_LOG("Checkbox: requesting to create a texture");
                MSE_CORE_TRACE("Checkbox_parentLayer = ", parentLayer);
                m_texture = ResourceManager::CreateTexture(
                                                           windowUser,
                                                           windowUser->GetRenderer(),
                                                           textureWidth,
                                                           textureHeight,
                                                           0,
                                                           32,
                                                           {0, 0, 0, 0});
                MSE_CORE_LOG("Checkbox: texture obtained");
                
                // unchecked
                SDL_FRect destRect = {0, 0, layerArea.z, layerArea.w};
                SDL_Rect sourceRect = {sourceUnchecked.x, sourceUnchecked.y, sourceUnchecked.z, sourceUnchecked.w};
                Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                             (Texture*)(m_spriteList->data),
                                             &destRect,
                                             &sourceRect);
                // checked
                destRect = {layerArea.z, 0, layerArea.z, layerArea.w};
                sourceRect = {sourceChecked.x, sourceChecked.y, sourceChecked.z, sourceChecked.w};
                Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                             (Texture*)(m_spriteList->data),
                                             &destRect,
                                             &sourceRect);
                // disabled checked
                destRect = {2*layerArea.z, 0, layerArea.z, layerArea.w};
                sourceRect = {sourceDisabledUnchecked.x, sourceDisabledUnchecked.y, sourceDisabledUnchecked.z, sourceDisabledUnchecked.w};
                Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                             (Texture*)(m_spriteList->data),
                                             &destRect,
                                             &sourceRect);
                
                destRect = {3*layerArea.z, 0, layerArea.z, layerArea.w};
                sourceRect = {sourceDisabledChecked.x, sourceDisabledChecked.y, sourceDisabledChecked.z, sourceDisabledChecked.w};
                Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                             (Texture*)(m_spriteList->data),
                                             &destRect,
                                             &sourceRect);
                
                ((Texture*)(m_texture->data))->Update();
                
                MSE_CORE_LOG("Checkbox: texture edited");
            }
            
            // controller
            callbacks[EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
                SoundMan::PlaySound("data/audio/sounds/click.wav");
            };
            
            callbacks[EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
            };
            
            callbacks[EventTypes::GUIItemMouseOver] = [&](SDL_Event* event){
            };
            
            callbacks[EventTypes::GUIItemMouseOut] = [&](SDL_Event* event){
            };
        }
		
		Checkbox::~Checkbox()
		{
            if (m_texture != nullptr)
            {
                ResourceManager::DropResource(m_texture, windowUser);
                MSE_CORE_LOG("Image: texture dropped");
            }
        }
		
		// general GUIItem interface
		void Checkbox::Display()
		{
			if (isEnabled && isVisible)
			{
                SDL_FRect destRect = {
                    (float)layerArea.x / WindowManager::GetCurrentWindow()->GetPrefs().width,
                    (float)layerArea.y / WindowManager::GetCurrentWindow()->GetPrefs().height,
                    (float)layerArea.z / WindowManager::GetCurrentWindow()->GetPrefs().width,
                    (float)layerArea.w / WindowManager::GetCurrentWindow()->GetPrefs().height,
                };
                
                SDL_Rect srcRect = {0, 0, layerArea.z, layerArea.w};
                
    			Renderer::DrawTexture((Texture*)(m_texture->data), &destRect, &srcRect);
			}
		}
        
        bool Checkbox::HandleEvent(EventTypes eventType, SDL_Event* event)
        {	
            // only one callback per event type
            if (callbacks.find(eventType) != callbacks.end())
            {
                switch (eventType)
                {
                case EventTypes::GUIItemMouseButtonDown:
                    {
                        isPushed = true;
                        break;
                    }
                case EventTypes::GUIItemMouseButtonUp:
                    {
                        if (isEnabled && isPushed)
                        {
                            state = (state == CheckboxStates::Checked) ? CheckboxStates::Unchecked : CheckboxStates::Checked;
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
