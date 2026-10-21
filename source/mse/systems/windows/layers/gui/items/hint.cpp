#include <mse/systems/platform/events/events.h>
#include <mse/systems/windows/layers/gui/items/text.h>
#include <mse/systems/windows/layers/gui/items/hint.h>
#include <mse/systems/windows/layers/layer.h>
#include <mse/systems/windows/window.h>
#include <mse/systems/windows/window_manager.h>
#include <mse/systems/platform/rendererAPI.h>
#include <mse/systems/platform/renderer_base.h>
#include <mse/systems/platform/renderer/texture.h>
#include <mse/systems/resources/resource_manager.h>


namespace mse
{
	namespace gui
	{
		// general initialization
		Hint::Hint()
		{}

		Hint::Hint(Layer* layer, GUIItem* parent, const std::u32string& text, const glm::ivec4& area, const glm::uvec4& bgColor, const glm::uvec4& textColor, Texture* bgImage, int pxSize, bool border, const glm::uvec4& borderColor)
		{
			Init(layer,
				 parent,
				 text,
				 area,
				 bgColor,
				 textColor,
				 bgImage,
				 pxSize,
				 border,
				 borderColor);
		}

		void Hint::Init(Layer* layer, GUIItem* parent, const std::u32string& text, const glm::ivec4& area, const glm::uvec4& bgColor, const glm::uvec4& textColor, Texture* bgImage, int pxSize, bool border, const glm::uvec4& borderColor)
		{
			parentLayer = layer;
			windowUser = layer->GetWindow();
			layerArea = area;
			m_elementName = "Hint";

			// view
			// hintText = (Text*)(layer->AddElement(new Text(
			// 	layer,
			// 	text,
			// 	area,
			// 	bgColor,
			// 	textColor,
			// 	pxSize,
			// 	border,
			// 	borderColor)));
			hintText = new Text(
				layer,
				text,
				area,
				bgColor,
				textColor,
				pxSize,
				border,
				borderColor);
			hintText->isVisible = false;

			BindTo(parent);

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

		Hint::~Hint()
		{
			UnBind(true);

			delete hintText;
			hintText = nullptr;
		}

		// general GUIItem interface
		void Hint::Display()
		{
			if (hintText->isVisible)
			{
				SDL_FRect destRect = {
					(float)layerArea.x / WindowManager::GetCurrentWindow()->GetPrefs().width,
					(float)layerArea.y / WindowManager::GetCurrentWindow()->GetPrefs().height,
					(float)layerArea.z / WindowManager::GetCurrentWindow()->GetPrefs().width,
					(float)layerArea.w / WindowManager::GetCurrentWindow()->GetPrefs().height,
				};

				SDL_Rect srcRect = {0, 0, layerArea.z, layerArea.w};

				Renderer::DrawTexture((Texture*)(hintText->GetTexture()->data), &destRect, &srcRect);
			}
		}

		bool Hint::HandleEvent(EventTypes eventType, SDL_Event* event)
		{
			return false;
		}

		// unique Hint interface
		void Hint::ChangeText(const std::u32string& text)
		{
			hintText->ChangeText(text);
		}

		void Hint::BindTo(GUIItem* parent)
		{
			UnBind();

			if (parent)
			{
				m_parent = parent;

				if (m_parent->callbacks.find(mse::EventTypes::GUIItemMouseOver) != m_parent->callbacks.end())
				{
					parent_callbacks[mse::EventTypes::GUIItemMouseOver] = m_parent->callbacks[mse::EventTypes::GUIItemMouseOver];
				}
				if (m_parent->callbacks.find(mse::EventTypes::GUIItemMouseMove) != m_parent->callbacks.end())
				{
					parent_callbacks[mse::EventTypes::GUIItemMouseMove] = m_parent->callbacks[mse::EventTypes::GUIItemMouseMove];
				}
				if (m_parent->callbacks.find(mse::EventTypes::GUIItemMouseOut) != m_parent->callbacks.end())
				{
					parent_callbacks[mse::EventTypes::GUIItemMouseOut] = m_parent->callbacks[mse::EventTypes::GUIItemMouseOut];
				}

				parent->callbacks[mse::EventTypes::GUIItemMouseOver] = [&](SDL_Event* event)
				{
					MoveHintText(event);

					if (!hintText->isVisible)
					{
						hintText->isVisible = true;
					}

					if (parent_callbacks.find(mse::EventTypes::GUIItemMouseOver) != parent_callbacks.end())
					{
						parent_callbacks[mse::EventTypes::GUIItemMouseOver](event);
					}
				};

				parent->callbacks[mse::EventTypes::GUIItemMouseMove] = [&](SDL_Event* event)
				{
					MoveHintText(event);

					if (!hintText->isVisible)
					{
						hintText->isVisible = true;
					}

					if (parent_callbacks.find(mse::EventTypes::GUIItemMouseMove) != parent_callbacks.end())
					{
						parent_callbacks[mse::EventTypes::GUIItemMouseMove](event);
					}
				};

				parent->callbacks[mse::EventTypes::GUIItemMouseOut] = [&](SDL_Event* event)
				{
					MoveHintText(event);

					if (hintText->isVisible)
					{
						hintText->isVisible = false;
					}

					if (parent_callbacks.find(mse::EventTypes::GUIItemMouseOut) != parent_callbacks.end())
					{
						parent_callbacks[mse::EventTypes::GUIItemMouseOut](event);
					}
				};
			}
		}

		void Hint::UnBind(bool die)
		{
			if (m_parent && !die)
			{
				if (parent_callbacks.find(mse::EventTypes::GUIItemMouseOver) != parent_callbacks.end())
				{
					m_parent->callbacks[mse::EventTypes::GUIItemMouseOver] = parent_callbacks[mse::EventTypes::GUIItemMouseOver];
					parent_callbacks.erase(mse::EventTypes::GUIItemMouseOver);
				}
				if (parent_callbacks.find(mse::EventTypes::GUIItemMouseMove) != parent_callbacks.end())
				{
					m_parent->callbacks[mse::EventTypes::GUIItemMouseMove] = parent_callbacks[mse::EventTypes::GUIItemMouseMove];
					parent_callbacks.erase(mse::EventTypes::GUIItemMouseMove);
				}
				if (parent_callbacks.find(mse::EventTypes::GUIItemMouseOut) != parent_callbacks.end())
				{
					m_parent->callbacks[mse::EventTypes::GUIItemMouseOut] = parent_callbacks[mse::EventTypes::GUIItemMouseOut];
					parent_callbacks.erase(mse::EventTypes::GUIItemMouseOut);
				}
			}

			parent_callbacks.clear();

			m_parent = nullptr;
		}

		void Hint::MoveHintText(SDL_Event* event)
		{
			SDL_Point correctMousePos = {
				(int)roundf(event->motion.x / windowUser->GetScale().x),
				(int)roundf(event->motion.y / windowUser->GetScale().y)
			};

			if (windowUser->GetPrefs().width > correctMousePos.x + m_parent->layerArea.z + 2)
			{
				hintText->layerArea.x = correctMousePos.x + 2;
			} else {
				hintText->layerArea.x = correctMousePos.x - m_parent->layerArea.z - 2;
			}
			if (hintText->layerArea.x < 0)
			{
				hintText->layerArea.x = 0;
			}
			layerArea.x = hintText->layerArea.x;

			if (windowUser->GetPrefs().height > correctMousePos.y + m_parent->layerArea.w + 2)
			{
				hintText->layerArea.y = correctMousePos.y + 2;
			} else {
				hintText->layerArea.y = correctMousePos.y - m_parent->layerArea.w - 2;
			}
			if (hintText->layerArea.y < 0)
			{
				hintText->layerArea.y = 0;
			}
			layerArea.y = hintText->layerArea.y;
		}
	}
}
