#ifndef MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_HINT_H
#define MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_HINT_H

#include <mse/core.h>
#include <mse/systems/windows/layers/gui/guiitem.h>

namespace mse
{
	namespace gui
	{
		class Text;

		class Hint : public GUIItem
		{
		public:
			using func = std::function<void(SDL_Event*)>;

			// general initialization
			Hint();
			Hint(Layer* layer,
				GUIItem* parent,
				const std::u32string& text,
				const glm::ivec4& area,
				const glm::uvec4& bgColor,
				const glm::uvec4& textColor,
				Texture* bgImage = nullptr,
				int pxSize = 1,
				bool border = false,
				const glm::uvec4& borderColor = {255, 255, 255, 255});
			void Init(Layer* layer,
					GUIItem* parent,
					const std::u32string& text,
					const glm::ivec4& area,
					const glm::uvec4& bgColor,
					const glm::uvec4& textColor,
					Texture* bgImage = nullptr,
					int pxSize = 1,
					bool border = false,
					const glm::uvec4& borderColor = {255, 255, 255, 255});
			virtual ~Hint();

			// general GUIItem interface
			virtual void Display() override;
			virtual bool HandleEvent(EventTypes eventType, SDL_Event* event) override;

			// unique Hint interface
			// bool showBorder = false;
			// std::vector<GUIItem> elements = {};

			Text* hintText = nullptr;

			void BindTo(GUIItem* parent);
			void UnBind(bool die = false);

		protected:
			void MoveHintText(SDL_Event* event);

			std::unordered_map<EventTypes, func> parent_callbacks = {};
			GUIItem* m_parent = nullptr;
			Texture* m_backgroundImage = nullptr;
			glm::uvec4 m_backgroundColor = {0, 0, 0, 0};
			glm::uvec4 m_borderColor = {0, 0, 0, 0};
		};
	}
}

#endif
