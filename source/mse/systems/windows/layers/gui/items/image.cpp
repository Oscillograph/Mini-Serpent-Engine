#include <mse/systems/windows/layers/gui/items/image.h>
#include <mse/systems/platform/platform.h>
#include <mse/systems/platform/rendererAPI.h>
#include <mse/systems/platform/renderer_base.h>
#include <mse/systems/platform/renderer/texture.h>
#include <mse/systems/platform/events/events.h>
#include <mse/systems/windows/window_manager.h>
#include <mse/systems/windows/window.h>
#include <mse/systems/windows/layers/layer.h>
#include <mse/systems/windows/layers/layer_manager.h>
#include <mse/systems/resources/resource_manager.h>
#include <mse/systems/ecs/components.h>

namespace mse
{
	namespace gui
	{
        /*========================================================================================*/
        /*                                     Image                                              */
        /*========================================================================================*/
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
            windowUser = layer->GetWindow();
			m_elementName = "Image";
			layerArea = area;
            m_spriteList = ResourceManager::UseTexture(spritelist, windowUser, {0, 0, 0});
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
					windowUser,
					windowUser->GetRenderer()->GetActiveRenderer(),
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
                
                sourceRect = {(int)sourceArea.x, (int)sourceArea.y, (int)sourceArea.z, (int)sourceArea.w};
                destRect = {0, 0, (float)layerArea.z, (float)layerArea.w};
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
		{
            if (m_texture != nullptr)
            {
                ResourceManager::DropResource(m_texture, windowUser);
                MSE_CORE_LOG("Image: texture dropped");
            }
        }
		
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
                
                SDL_Rect srcRect;

                if (m_animations != nullptr)
                {
                    srcRect.x = m_animations->frames[m_animations->currentAnimation]->begin.x +
                                m_animations->currentFrame * m_animations->frames[m_animations->currentAnimation]->width;
                    srcRect.y = m_animations->frames[m_animations->currentAnimation]->begin.y +
                                m_animations->currentFrame * m_animations->frames[m_animations->currentAnimation]->height;
                    srcRect.w = m_animations->frames[m_animations->currentAnimation]->width;
                    srcRect.h = m_animations->frames[m_animations->currentAnimation]->height;
                } else {
                    srcRect = {0, 0, layerArea.z, layerArea.w};
                }
                
    			Renderer::DrawTexture((Texture*)(m_texture->data), &destRect, &srcRect);
			}
		}
        
        
        /*========================================================================================*/
        /*                                     VImageTemplate                                     */
        /*========================================================================================*/
        // general initialization
        VImageTemplate::VImageTemplate()
        {
            Init(nullptr, {0, 0, 0, 0}, nullptr, {0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0});
        }
        
        VImageTemplate::VImageTemplate(Layer* layer, const glm::uvec4& area, const std::string& spritelist, const glm::uvec3& colorKey, const glm::uvec4& sourceTop, const glm::uvec4& sourceMid, const glm::uvec4& sourceBottom)
        {
            Init(layer, area, spritelist, colorKey, sourceTop, sourceMid, sourceBottom);
        }
        
        void VImageTemplate::Init(Layer* layer, const glm::uvec4& area, const std::string& spritelist, const glm::uvec3& colorKey, const glm::uvec4& sourceTop, const glm::uvec4& sourceMid, const glm::uvec4& sourceBottom)
        {
            // model
            parentLayer = layer;
            windowUser = layer->GetWindow();
            m_elementName = "VerticalImage";
            layerArea = area;
            m_spriteList = ResourceManager::UseTexture(spritelist, windowUser, colorKey);
            m_sourceTop = sourceTop;
            m_sourceMid = sourceMid;
            m_sourceBottom = sourceBottom;
            
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
                MSE_CORE_LOG("VerticalImage: requesting to create a texture");
                MSE_CORE_TRACE("VerticalImage_parentLayer = ", parentLayer);
                m_texture = ResourceManager::CreateTexture(
                                                           windowUser,
                                                           windowUser->GetRenderer()->GetActiveRenderer(),
                                                           layerArea.z,
                                                           layerArea.w,
                                                           0,
                                                           32,
                                                           {0, 0, 0, 0});
                
                // from where and where to draw
                SDL_Rect sourceRect;
                SDL_FRect destRect;
                
                // top part
                sourceRect = {(int)sourceTop.x, (int)sourceTop.y, (int)sourceTop.z, (int)sourceTop.w};
                destRect = {0, 0, (float)sourceTop.z, (float)sourceTop.w};
                Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                             (Texture*)(m_spriteList->data),
                                             &destRect,
                                             &sourceRect);
                
                // middle part
                sourceRect = {(int)sourceMid.x, (int)sourceMid.y, (int)sourceMid.z, 1};
                int middleHeight = layerArea.w - sourceTop.w - sourceBottom.w;
                for (int k = 0; k < middleHeight; ++k)
                {
                    destRect = {0, (float)(sourceTop.w + k), (float)sourceMid.z, 1};
                    Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                                 (Texture*)(m_spriteList->data),
                                                 &destRect,
                                                 &sourceRect);
                }
                
                // bottom part
                sourceRect = {(int)sourceBottom.x, (int)sourceBottom.y, (int)sourceBottom.z, (int)sourceBottom.w};
                destRect = {0, (float)(sourceTop.w + middleHeight), (float)sourceBottom.z, (float)sourceBottom.w};
                Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                             (Texture*)(m_spriteList->data),
                                             &destRect,
                                             &sourceRect);
                
                ((Texture*)(m_texture->data))->Update();
                
                MSE_CORE_LOG("VerticalImage: texture obtained");
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
        
        VImageTemplate::~VImageTemplate()
        {
            if (m_texture != nullptr)
            {
                ResourceManager::DropResource(m_texture, windowUser);
                MSE_CORE_LOG("VerticalImage: texture dropped");
            }
        }
        
        // general GUIItem interface
        void VImageTemplate::Display()
        {
            if (isEnabled && isVisible)
            {
                SDL_FRect destRect = {
                    (float)layerArea.x / WindowManager::GetCurrentWindow()->GetPrefs().width,
                    (float)layerArea.y / WindowManager::GetCurrentWindow()->GetPrefs().height,
                    (float)layerArea.z / WindowManager::GetCurrentWindow()->GetPrefs().width,
                    (float)layerArea.w / WindowManager::GetCurrentWindow()->GetPrefs().height,
                };
                
                SDL_Rect srcRect;

                if (m_animations != nullptr)
                {
                    srcRect.x = m_animations->frames[m_animations->currentAnimation]->begin.x +
                    m_animations->currentFrame * m_animations->frames[m_animations->currentAnimation]->width;
                    srcRect.y = m_animations->frames[m_animations->currentAnimation]->begin.y +
                    m_animations->currentFrame * m_animations->frames[m_animations->currentAnimation]->height;
                    srcRect.w = m_animations->frames[m_animations->currentAnimation]->width;
                    srcRect.h = m_animations->frames[m_animations->currentAnimation]->height;
                } else {
                    srcRect = {0, 0, layerArea.z, layerArea.w};
                }

                Renderer::DrawTexture((Texture*)(m_texture->data), &destRect, &srcRect);
            }
        }
        
        
        /*========================================================================================*/
        /*                                     HImageTemplate                                     */
        /*========================================================================================*/
        // general initialization
        HImageTemplate::HImageTemplate()
        {
            Init(nullptr, {0, 0, 0, 0}, nullptr, {0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0});
        }
        
        HImageTemplate::HImageTemplate(Layer* layer, const glm::uvec4& area, const std::string& spritelist, const glm::uvec3& colorKey, const glm::uvec4& sourceLeft, const glm::uvec4& sourceMid, const glm::uvec4& sourceRight)
        {
            Init(layer, area, spritelist, colorKey, sourceLeft, sourceMid, sourceRight);
        }
        
        void HImageTemplate::Init(Layer* layer, const glm::uvec4& area, const std::string& spritelist, const glm::uvec3& colorKey, const glm::uvec4& sourceLeft, const glm::uvec4& sourceMid, const glm::uvec4& sourceRight)
        {
            MSE_CORE_LOG("Got here");
            // model
            parentLayer = layer;
            windowUser = layer->GetWindow();
            m_elementName = "VerticalImage";
            layerArea = area;
            m_spriteList = ResourceManager::UseTexture(spritelist, windowUser, colorKey);
            m_sourceLeft = sourceLeft;
            m_sourceMid = sourceMid;
            m_sourceRight = sourceRight;
            MSE_CORE_LOG("1");
            MSE_CORE_LOG("Area: ", area.z, " x ", area.w);
            
            layerMask.resize(area.z * area.w);
            MSE_CORE_LOG("2");
            for (int x = 0; x < area.z; ++x)
            {
                for (int y = 0; y < area.w; ++y)
                {
                    layerMask[x + y*area.z] = id;
                }
            }
            MSE_CORE_LOG("3");
            
            // view
            if ((layer != nullptr) && (m_spriteList != nullptr))
            {
                // setup texture to draw on
                MSE_CORE_LOG("VerticalImage: requesting to create a texture");
                MSE_CORE_TRACE("VerticalImage_parentLayer = ", parentLayer);
                m_texture = ResourceManager::CreateTexture(
                                                           windowUser,
                                                           windowUser->GetRenderer(),
                                                           layerArea.z,
                                                           layerArea.w,
                                                           0,
                                                           32,
                                                           {0, 0, 0, 0});
                
                // from where and where to draw
                SDL_Rect sourceRect;
                SDL_FRect destRect;
                
                // left part
                sourceRect = {(int)sourceLeft.x, (int)sourceLeft.y, (int)sourceLeft.z, (int)sourceLeft.w};
                destRect = {0, 0, (float)sourceLeft.z, (float)sourceLeft.w};
                Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                             (Texture*)(m_spriteList->data),
                                             &destRect,
                                             &sourceRect);
                
                // middle part
                sourceRect = {(int)sourceMid.x, (int)sourceMid.y, 1, (int)sourceMid.w};
                int middleWidth = layerArea.z - sourceLeft.z - sourceRight.z;
                for (int k = 0; k < middleWidth; ++k)
                {
                    destRect = {(float)(sourceLeft.z + k), 0, 1, (float)sourceMid.w};
                    Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                                 (Texture*)(m_spriteList->data),
                                                 &destRect,
                                                 &sourceRect);
                }
                
                // right part
                sourceRect = {(int)sourceRight.x, (int)sourceRight.y, (int)sourceRight.z, (int)sourceRight.w};
                destRect = {(float)(sourceLeft.z + middleWidth), 0, (float)sourceRight.z, (float)sourceRight.w};
                Renderer::SurfaceDrawTexture((Texture*)(m_texture->data),
                                             (Texture*)(m_spriteList->data),
                                             &destRect,
                                             &sourceRect);
                
                ((Texture*)(m_texture->data))->Update();
                
                MSE_CORE_LOG("VerticalImage: texture obtained");
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
        
        HImageTemplate::~HImageTemplate()
        {
            if (m_texture != nullptr)
            {
                ResourceManager::DropResource(m_texture, windowUser);
                MSE_CORE_LOG("VerticalImage: texture dropped");
            }
        }
        
        // general GUIItem interface
        void HImageTemplate::Display()
        {
            if (isEnabled && isVisible)
            {
                SDL_FRect destRect = {
                    (float)layerArea.x / WindowManager::GetCurrentWindow()->GetPrefs().width,
                    (float)layerArea.y / WindowManager::GetCurrentWindow()->GetPrefs().height,
                    (float)layerArea.z / WindowManager::GetCurrentWindow()->GetPrefs().width,
                    (float)layerArea.w / WindowManager::GetCurrentWindow()->GetPrefs().height,
                };
                
                SDL_Rect srcRect;

                if (m_animations != nullptr)
                {
                    srcRect.x = m_animations->frames[m_animations->currentAnimation]->begin.x +
                    m_animations->currentFrame * m_animations->frames[m_animations->currentAnimation]->width;
                    srcRect.y = m_animations->frames[m_animations->currentAnimation]->begin.y +
                    m_animations->currentFrame * m_animations->frames[m_animations->currentAnimation]->height;
                    srcRect.w = m_animations->frames[m_animations->currentAnimation]->width;
                    srcRect.h = m_animations->frames[m_animations->currentAnimation]->height;
                } else {
                    srcRect = {0, 0, layerArea.z, layerArea.w};
                }
                
                Renderer::DrawTexture((Texture*)(m_texture->data), &destRect, &srcRect);
            }
        }
	}
}
