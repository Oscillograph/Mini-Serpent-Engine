#include <mse/systems/windows/layers/gui/items/canvas.h>
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
		Canvas::Canvas()
		: GUIItem()
		{
			Init(nullptr, {0, 0, 1, 1}, {255, 255, 255, 255});
		}
		
		Canvas::Canvas(Layer* layer, const glm::uvec4& area, const glm::uvec4& color)
		: GUIItem()
		{
			Init(layer, area, color);
		}
		
		void Canvas::Init(Layer* layer, const glm::uvec4& area, const glm::uvec4& color)
		{
			// model
			parentLayer = layer;
			m_elementName = "Canvas";
			layerArea = area;
			backgroundColor = color;
			
			layerMask.resize(area.z * area.w);
			for (int x = 0; x < area.z; ++x)
			{
				for (int y = 0; y < area.w; ++y)
				{
//				MSE_CORE_LOG("Canvas: mask filling at (", x, "; ", y, ")");
					layerMask[x + y*area.z] = id;
				}
			}
			
			// view
			if (layer != nullptr)
			{
				// setup texture to draw on
				MSE_CORE_LOG("Canvas: requesting to create a texture");
				MSE_CORE_TRACE("Canvas_parentLayer = ", parentLayer);
				m_texture = ResourceManager::CreateTexture(
					parentLayer->GetWindow(),
					parentLayer->GetWindow()->GetRenderer(),
					layerArea.z,
					layerArea.w,
					0,
					32,
					{0, 0, 0, 0});
				MSE_CORE_LOG("Canvas: texture obtained");
				DrawRect(0, 0, layerArea.z, layerArea.w, {backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w});
				MSE_CORE_LOG("Canvas: texture edited");
			}

			// controller
			// setup interaction
			callbacks[EventTypes::GUIItemMouseMove] = [&](SDL_Event* event){
				// map real mouse coordinates to window basePrefs
				int mouseX = 0;
				int mouseY = 0;
				
				mouseX = (int)floorf(event->motion.x / WindowManager::GetCurrentWindow()->GetScale().x);
				mouseY = (int)floorf(event->motion.y / WindowManager::GetCurrentWindow()->GetScale().y);
				
				if (m_MouseLeftButtonIsHeld)
				{
					if ((m_lastMouseXY.x != mouseX) || (m_lastMouseXY.y != mouseY))
					{
						DrawLine(
							{
								m_lastMouseXY.x - layerArea.x, 
								m_lastMouseXY.y - layerArea.y, 
								mouseX - layerArea.x, 
								mouseY - layerArea.y
							}, 
							m_lastColor
							);
					} else {
						DrawPixel(mouseX - layerArea.x, mouseY - layerArea.y, m_lastColor);
					}
				}

				m_lastMouseXY = {mouseX, mouseY};
			};
			callbacks[EventTypes::GUIItemMouseOver] = [&](SDL_Event* event){
				MSE_CORE_LOG("Canvas: I can see a mouse cursor! Yay!");
			};
			callbacks[EventTypes::GUIItemMouseOut] = [&](SDL_Event* event){
				MSE_CORE_LOG("Canvas: Mouse moved away from me. Goodbye, mouse!");
				m_MouseLeftButtonIsHeld = false;
				m_MouseRightButtonIsHeld = false;
				m_MouseMiddleButtonIsHeld = false;
			};
			callbacks[EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
				switch (event->button.button)
				{
					case SDL_BUTTON_LEFT:
					{
						MSE_CORE_LOG("Canvas: Left Mouse button is down");
						m_MouseLeftButtonIsHeld = true;
						break;
					}
					case SDL_BUTTON_RIGHT:
					{
						MSE_CORE_LOG("Canvas: Right Mouse button is down");
						m_MouseRightButtonIsHeld = true;
						break;
					}
					case SDL_BUTTON_MIDDLE:
					{
						MSE_CORE_LOG("Canvas: Middle Mouse button is down");
						m_MouseMiddleButtonIsHeld = true;
						break;
					}
					default:
					{
						MSE_CORE_LOG("Canvas: No idea what Mouse button is down");
					}
				}
				
				// map real mouse coordinates to window basePrefs
				int mouseX = 0;
				int mouseY = 0;
				
				mouseX = (int)floorf(event->motion.x / WindowManager::GetCurrentWindow()->GetScale().x);
				mouseY = (int)floorf(event->motion.y / WindowManager::GetCurrentWindow()->GetScale().y);
				m_lastMouseXY = {mouseX, mouseY};
				
				DrawPixel(mouseX - layerArea.x, mouseY - layerArea.y, m_lastColor);
			};
			callbacks[EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
				switch (event->button.button)
				{
					case SDL_BUTTON_LEFT:
					{
						MSE_CORE_LOG("Canvas: Left Mouse button is up");
						m_MouseLeftButtonIsHeld = false;
						break;
					}
					case SDL_BUTTON_RIGHT:
					{
						MSE_CORE_LOG("Canvas: Right Mouse button is up");
						m_MouseRightButtonIsHeld = false;
						break;
					}
					case SDL_BUTTON_MIDDLE:
					{
						MSE_CORE_LOG("Canvas: Middle Mouse button is up");
						m_MouseMiddleButtonIsHeld = false;
						break;
					}
					default:
					{
						MSE_CORE_LOG("Canvas: No idea what Mouse button is up");
					}
				}
			};
		}
		
		Canvas::~Canvas()
		{}
		
		
		// general GUIItem interface
		void Canvas::Display()
		{
//			MSE_CORE_LOG("Canvas: Display routine");
			
			glm::vec4 scaled(0.0f);
			scaled = {
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
		
		
		// unique Canvas interface
		bool Canvas::SetDrawColor(const glm::uvec4& color)
		{
			m_lastColor = color;
			return true;
		}
		
		bool Canvas::Clear()
		{
			DrawRect(0, 0, layerArea.w, layerArea.z, backgroundColor);
			return true;
		}
		
		bool Canvas::DrawPixel(int x, int y, const glm::uvec4& color)
		{
			Renderer::SurfaceDrawPixel(
				(Texture*)(m_texture->data),
				{x, y},
				1,
				{color.x, color.y, color.z, color.w}
				);
			((Texture*)(m_texture->data))->Update();
			return true;
		}
		
		bool Canvas::DrawLine(const glm::ivec4& AB, const glm::uvec4& color)
		{
			Renderer::SurfaceDrawLine(
				(Texture*)(m_texture->data),
				AB.x, AB.y,
				AB.z, AB.w,
				1,
				{color.x, color.y, color.z, color.w}
				);
			((Texture*)(m_texture->data))->Update();
			return true;
		}
		
		bool Canvas::DrawRect(int x1, int y1, int x2, int y2, const glm::uvec4& color)
		{
			Renderer::SurfaceDrawRectFilled(
				(Texture*)(m_texture->data),
				{x1, y1, (x2 - x1), (y2 - y1)}, 
				{color.x, color.y, color.z, color.w}
				);
			((Texture*)(m_texture->data))->Update();
			return true;
		}
		
		bool Canvas::FillRect(int x1, int y1, int x2, int y2, const glm::uvec4& color)
		{
			return true;
		}
		
		bool Canvas::DrawCircle(int x, int y, int radius, const glm::uvec4& color)
		{
			Renderer::SurfaceDrawCircle(
				(Texture*)(m_texture->data),
				{x, y},
				radius,
				3,
				{color.x, color.y, color.z, color.w}
				);
			((Texture*)(m_texture->data))->Update();
			return true;
		}
		
		bool Canvas::FillCircle(int x, int y, int radius, const glm::uvec4& color)
		{
			return true;
		}
		
		bool Canvas::DrawText(int x, int y, const std::string& text, int font, const glm::uvec4& color, const glm::uvec4& backgroundColor)
		{
			return true;
		}
	}
}
