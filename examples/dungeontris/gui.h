#ifndef DUNGEONTRIS_GUI_H
#define DUNGEONTRIS_GUI_H

#include <mse/systems/windows/layers/gui/guiitem.h>
#include <mse/systems/windows/layers/gui/items/button.h>

namespace mse
{
	namespace gui
	{
		namespace StoreItemStates
		{
			enum StoreItemStatesEnum
			{
				Disabled	= 0b00000001,	// 1
				Idle		= 0b00000010,	// 2
				Hover		= 0b00000100,	// 4
				Pressed		= 0b00001000,	// 8
				Bought		= 0b00010000,	// 16
				Equipped	= 0b00100000	// 32
			};
		}

		// basically, a different kind of button
		class StoreItem : public GUIItem
		{
		public:
			// general initialization
			StoreItem();
			StoreItem(
				Layer* layer,
				const glm::uvec4& area, // where to place the item on layer
				const glm::uvec4& image,// what to place on layer (from screen-images.png)
				int cost 				// item cost
			);
			void Init(Layer* layer, const glm::uvec4& area, const glm::uvec4& image, int cost);
			virtual ~StoreItem();

			// general GUIItem interface
			virtual void Display() override;
			virtual bool HandleEvent(EventTypes eventType, SDL_Event* event) override;

			// unique StoreItem interface
			void SetImage(const glm::uvec4& area); // set icon of what we buy
			void ChangeCost(int cost);

			int state = StoreItemStates::Idle;

		protected:
			// sprite-based
			Resource* m_spriteList = nullptr;
			glm::uvec4 m_imageSource = {0, 0, 0, 0}; // icon set with SetImage
			std::u32string m_text = U"";
		};
	}
}

#endif
