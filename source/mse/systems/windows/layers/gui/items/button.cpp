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
			Init(nullptr, "", {0, 0, 1, 1}, {0, 0, 0, 0}, {255, 255, 255, 255});
		}
		
		Button::Button(Layer* layer,  const std::string& text, const glm::uvec4& area, const glm::uvec4& bgColor, const glm::uvec4& color)
		: GUIItem()
		{
			Init(layer, text, area, bgColor, color);
		}
		
		void Button::Init(Layer* layer,  const std::string& text, const glm::uvec4& area, const glm::uvec4& bgColor, const glm::uvec4& color)
		{
			// model
			parentLayer = layer;
			m_elementName = "Button_" + text;
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
					layerArea.z,
					layerArea.w,
					0,
					32,
					{0, 0, 0, 0});
				MSE_CORE_LOG("Button: texture obtained");
				
				Renderer::SurfaceDrawRectFilled(
					(Texture*)(m_texture->data),
					{0, 0, layerArea.z, layerArea.w}, 
					{m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, m_backgroundColor.w}
					);
				((Texture*)(m_texture->data))->Update();
				MSE_CORE_LOG("Button: texture edited");
			}
			
			// controller
			// setup interaction
			callbacks[EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
				MSE_CORE_LOG(m_elementName, ": ...");
			};
			
			callbacks[EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
				MSE_CORE_LOG(m_elementName, ": Yay, you clicked me!");
			};
			
			callbacks[EventTypes::GUIItemMouseOver] = [&](SDL_Event* event){
				MSE_CORE_LOG(m_elementName, ": Hello, Mouse!");
			};
			
			callbacks[EventTypes::GUIItemMouseOut] = [&](SDL_Event* event){
				MSE_CORE_LOG(m_elementName, ": Goodbye, Mouse!");
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
			
			Renderer::DrawTexture((Texture*)(m_texture->data), &destRect, NULL);
		}
		
		// unique Button interface
	}
}
