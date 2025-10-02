#include <mse/systems/windows/layers/gui/items/image.h>
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
		Image::Image()
		: GUIItem()
		{
			Init(nullptr, {0, 0, 1, 1}, nullptr, {0, 0, 0, 0}, {0, 0, 0, 0});
		}
		
		Image::Image(Layer* layer, const glm::uvec4& area, const std::string& spritelist, const glm::uvec4& sourceArea, const glm::uvec4& bgcolor)
		: GUIItem()
		{
			Init(layer, area, spritelist, sourceArea, bgcolor);
		}
		
        // generic button
		void Image::Init(Layer* layer, const glm::uvec4& area, const std::string& spritelist, const glm::uvec4& sourceArea, const glm::uvec4& bgcolor)
		{
			// model
			parentLayer = layer;
			m_elementName = "Image";
			layerArea = area;
            m_spriteList = ResourceManager::UseTexture(spritelist, parentLayer->GetWindow(), {0, 0, 0});
            m_sourceArea = sourceArea;
			m_backgroundColor = bgcolor;
			
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
				MSE_CORE_LOG("Image: requesting to create a texture");
				MSE_CORE_TRACE("Image_parentLayer = ", parentLayer);
				m_texture = ResourceManager::CreateTexture(
					parentLayer->GetWindow(),
					parentLayer->GetWindow()->GetRenderer(),
					layerArea.z,
					layerArea.w,
					0,
					32,
					{0, 0, 0, 0});
                Renderer::SurfaceDrawRectFilled(
                                                (Texture*)(m_texture->data),
                                                {0, 0, layerArea.z, layerArea.w}, 
                                                {0, 0, 0, 255}
                                                );
                
                // from where and where to draw
                SDL_Rect sourceRect;
                SDL_FRect destRect;
                
                sourceRect = {sourceArea.x, sourceArea.y, sourceArea.z, sourceArea.w};
                destRect = {0, 0, layerArea.z, layerArea.w};
                Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                             (Texture*)(m_spriteList->data),
                                             &destRect,
                                             &sourceRect);
                
                ((Texture*)(m_texture->data))->Update();
                
				MSE_CORE_LOG("Image: texture obtained");
			}
			
			// controller
			// setup interaction
			callbacks[EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
			};
			
			callbacks[EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
			};
			
			callbacks[EventTypes::GUIItemMouseOver] = [&](SDL_Event* event){
			};
			
			callbacks[EventTypes::GUIItemMouseOut] = [&](SDL_Event* event){
			};
		}
		
		Image::~Image()
		{}
		
		// general GUIItem interface
		void Image::Display()
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
	}
}
