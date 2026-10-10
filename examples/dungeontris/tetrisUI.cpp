#include <mse/systems/platform/input/input.h>
#include <mse/systems/windows/layers/gui/items/canvas.h>
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
#include <dungeontris/tetris.h>
#include <dungeontris/tetrisUI.h>

namespace mse
{
	namespace gui
	{
		// general initialization
		TetrisCanvas::TetrisCanvas()
		: GUIItem()
		{
			Init(nullptr, {0, 0, 1, 1});
		}

		TetrisCanvas::TetrisCanvas(Layer* layer, const glm::uvec4& area)
		: GUIItem()
		{
			Init(layer, area);
		}

		void TetrisCanvas::Init(Layer* layer, const glm::uvec4& area)
		{
			// model
			parentLayer = layer;
			m_elementName = "Canvas";
			layerArea = area;

			layerMask.resize(area.z * area.w);
			for (unsigned int x = 0; x < area.z; ++x)
			{
				for (unsigned int y = 0; y < area.w; ++y)
				{
					// MSE_CORE_LOG("Canvas: mask filling at (", x, "; ", y, ")");
					layerMask[x + y*area.z] = id;
				}
			}

			// view
			if (layer != nullptr)
			{
				// setup texture to draw on
				MSE_CORE_LOG("TetrisCanvas: requesting to create a texture");
				MSE_CORE_TRACE("TetrisCanvas_parentLayer = ", parentLayer);
				m_texture = ResourceManager::CreateTexture(
					parentLayer->GetWindow(),
					parentLayer->GetWindow()->GetRenderer()->GetActiveRenderer(),
					layerArea.z,
					layerArea.w,
					0,
					32,
					{0, 0, 0, 0});
				MSE_CORE_LOG("TetrisCanvas: texture obtained");
				Renderer::DrawRect(
					{0, 0},
					{(float)layerArea.z, (float)layerArea.w},
					{0, 0, 0, 0}
				);
				MSE_CORE_LOG("TetrisCanvas: texture edited");
			}

			// controller
			// setup interaction
			callbacks[EventTypes::GUIItemKeyDown] = [&](SDL_Event* event){
				switch (event->key.key)
				{
					case mse::KeyCode::Up:
					{
						MSE_CORE_LOG("Canvas: Left Mouse button is up");
						break;
					}
					case mse::KeyCode::Left:
					{
						MSE_CORE_LOG("Canvas: Right Mouse button is up");
						break;
					}
					case mse::KeyCode::Right:
					{
						MSE_CORE_LOG("Canvas: Middle Mouse button is up");
						break;
					}
					case mse::KeyCode::Down:
					{
						MSE_CORE_LOG("Canvas: Middle Mouse button is up");
						break;
					}
				}
			};
		}

		TetrisCanvas::~TetrisCanvas()
		{}


		// general GUIItem interface
		void TetrisCanvas::Display()
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
	}
}
