#include <mse/systems/windows/layers/gui/items/input_int.h>
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
		// default constructor
		InputInt::InputInt()
		{
			Init(nullptr, 0, {0, 0, 1, 1}, {0, 0, 0, 0}, {255, 255, 255, 255});
		}

		// generic constructor
		InputInt::InputInt(
			Layer* layer,
			int defaultValue,
			const glm::uvec4& area,
			const glm::uvec4& bgColor,
			const glm::uvec4& color
		)
		{
			Init(layer, defaultValue, area, bgColor, color);
		}

		// constructor from spritelist
		InputInt::InputInt(
			Layer* layer,
			int defaultValue,
			const glm::uvec4& color,
			const glm::uvec4& area,
			const std::string& spritelist,
			const glm::uvec4& leftSource,
			const glm::uvec4& midSource,
			const glm::uvec4& rightSource
		)
		{
			Init(layer, defaultValue, color, area, spritelist, leftSource, midSource, rightSource);
		}

		// generic
		void InputInt::Init(
			Layer* layer,
			int defaultValue,
			const glm::uvec4& area,
			const glm::uvec4& bgColor,
			const glm::uvec4& color
		)
		{
			parentLayer = layer;
			windowUser = layer->GetWindow();
			m_elementName = "InputInt";
			layerArea = area;
			m_backgroundColor = bgColor;
			m_textColor = color;
			m_value = defaultValue;

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
			}

			// controller
			// setup interaction


		}

		// sprite-based
		void InputInt::Init(
			Layer* layer,
			int defaultValue,
			const glm::uvec4& color,
			const glm::uvec4& area,
			const std::string& spritelist,
			const glm::uvec4& leftSource,
			const glm::uvec4& midSource,
			const glm::uvec4& rightSource
		)
		{}

		InputInt::~InputInt()
		{
		}

		// general GUIItem interface
		void InputInt::Display()
		{
		}

		bool InputInt::HandleEvent(EventTypes eventType, SDL_Event* event)
		{
			return false;
		}
	}
}
