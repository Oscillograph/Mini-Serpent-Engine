#include <mse/systems/windows/layers/gui/items/field.h>
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
		/*                                     Field                                              */
		/*========================================================================================*/
		// general initialization
		Field::Field()
		: GUIItem()
		{
			Init(nullptr, {0, 0, 1, 1}, {0, 0, 0, 255}, true, 1, {255, 255, 255, 255});
		}

		Field::Field(Layer* layer, const glm::uvec4& area, const glm::uvec4& bgcolor, bool border, int borderSize, const glm::uvec4& borderColor)
		: GUIItem()
		{
			Init(layer, area, bgcolor, border, borderSize, borderColor);
		}

		// generic button
		void Field::Init(Layer* layer, const glm::uvec4& area, const glm::uvec4& bgcolor, bool border, int borderSize, const glm::uvec4& borderColor)
		{
			// model
			parentLayer = layer;
			windowUser = layer->GetWindow();
			m_elementName = "Image";
			layerArea = area;

			m_border = border;
			m_borderSize = borderSize;
			m_borderColor = borderColor;
			m_backgroundColor = bgcolor;

			m_texture = ResourceManager::CreateTexture(
				windowUser,
				windowUser->GetRenderer()->GetActiveRenderer(),
				layerArea.z,
				layerArea.w,
				0,
				32,
				{0, 0, 0, 0});


			layerMask.resize(area.z * area.w);
			for (size_t x = 0; x < area.z; ++x)
			{
				for (size_t y = 0; y < area.w; ++y)
				{
					layerMask[x + y*area.z] = id;
				}
			}

			// view
			if (layer != nullptr)
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
					{
						(Uint8)m_backgroundColor.x,
						(Uint8)m_backgroundColor.y,
						(Uint8)m_backgroundColor.z,
						(Uint8)m_backgroundColor.w
					}
				);

				if (m_border)
				{
					Renderer::SurfaceDrawRect(
						(Texture*)(m_texture->data),
						{0, 0, layerArea.z, layerArea.w},
						borderSize,
						{
							(Uint8)m_borderColor.x,
							(Uint8)m_borderColor.y,
							(Uint8)m_borderColor.z,
							(Uint8)m_borderColor.w
						}
					);
				}

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

		Field::~Field()
		{
			if (m_texture != nullptr)
			{
				ResourceManager::DropResource(m_texture, windowUser);
				MSE_CORE_LOG("Image: texture dropped");
			}
		}

		// general GUIItem interface
		void Field::Display()
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
				srcRect = {0, 0, layerArea.z, layerArea.w};

				Renderer::DrawTexture((Texture*)(m_texture->data), &destRect, &srcRect);
			}
		}

		// unique Image interface
		void Field::ChangeSourceArea(const glm::uvec4& sourceArea)
		{
			m_sourceArea.x = sourceArea.x;
			m_sourceArea.y = sourceArea.y;
			m_sourceArea.z = sourceArea.z;
			m_sourceArea.w = sourceArea.w;

			// clear texture if exists
			if (m_texture)
			{
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
			}
		}
	}
}
