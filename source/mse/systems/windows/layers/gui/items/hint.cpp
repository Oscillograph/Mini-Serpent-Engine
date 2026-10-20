#include <mse/systems/windows/layers/gui/items/hint.h>
#include <mse/systems/windows/layers/layer.h>


namespace mse
{
	namespace gui
	{
		// general initialization
		Hint::Hint()
		{}

		Hint::Hint(Layer* layer, GUIItem* parent, glm::ivec4& area, glm::uvec4& bgColor, Texture* bgImage, bool border, const glm::uvec4& borderColor)
		{}

		void Hint::Init(Layer* layer, GUIItem* parent, glm::ivec4& area, glm::uvec4& bgColor, Texture* bgImage, bool border, const glm::uvec4& borderColor)
		{
			parentLayer = layer;
			windowUser = layer->GetWindow();
			layerArea = area;
			m_elementName = "Hint";


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
		{}

		// general GUIItem interface
		void Hint::Display()
		{}

		bool Hint::HandleEvent(EventTypes eventType, SDL_Event* event)
		{
			return false;
		}

		// unique Hint interface
	}
}
