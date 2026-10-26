#ifndef DUNGEONTRIS_GUI_H
#define DUNGEONTRIS_GUI_H

#include <mse/systems/windows/layers/gui/guiitem.h>
#include <mse/systems/windows/layers/gui/items/button.h>

namespace mse
{
	namespace gui
	{
		enum class StoreItemStates
		{
			Idle,
			Pressed,
			Bought,
			Equipped
		};

		// basically, a different kind of button
		class StoreItem : public GUIItem
		{
		public:
			// general initialization
			StoreItem();
			StoreItem(Layer* layer, const glm::uvec4& area, const std::u32string& cost_text);
			void Init(Layer* layer, const glm::uvec4& area, const std::u32string& cost_text);
			virtual ~StoreItem();

			// general GUIItem interface
			virtual void Display() override;
			virtual bool HandleEvent(EventTypes eventType, SDL_Event* event) override;

			// unique StoreItem interface
			StoreItemStates state = StoreItemStates::Idle;
			bool isHover = false;
			bool isPushed = false;

		protected:
			// sprite-based
			Resource* m_spriteList = nullptr;
			glm::uvec4 m_imageSource = {0, 0, 0, 0};
			glm::uvec4 m_costSource = {0, 0, 0, 0};
			std::u32string m_text = U"";
		};
	}
}

#endif
