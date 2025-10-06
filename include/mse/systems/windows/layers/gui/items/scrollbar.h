#ifndef MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_SCROLLBAR_H
#define MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_SCROLLBAR_H

#include <mse/core.h>
#include <mse/systems/windows/layers/gui/guiitem.h>
#include <mse/systems/windows/layers/gui/items/text.h>
#include <mse/systems/windows/layers/gui/items/button.h>
#include <mse/systems/windows/layers/gui/items/image.h>

namespace mse
{
	namespace gui
	{
        enum class VScrollbarStates
        {
            BtnUpHover,
            BtnUpPressed,
            BtnUpHold,
            BtnUpReleased,
            
            BtnDownHover,
            BtnDownPressed,
            BtnDownHold,
            BtnDownReleased,
            
            BtnBallHover,
            BtnBallPressed,
            BtnBallMoved,
            BtnBallReleased,
            
            BallPanelClicked,
            BallPanelReleased,
        };
        
		class VScrollbar : public GUIItem
		{
		public:
			// general initialization
			VScrollbar();
			VScrollbar(Layer* layer, const glm::uvec4& area, Text* textItem, const std::string&  spritelist, const glm::uvec3& colorKey, const glm::uvec4& btnUp, const glm::uvec4& btnBall, const glm::uvec4& btnDown, const glm::uvec4& sliderImgTop, const glm::uvec4& sliderImgMid, const glm::uvec4& sliderImgBottom);
			void Init(Layer* layer, const glm::uvec4& area, Text* textItem, const std::string& spritelist, const glm::uvec3& colorKey, const glm::uvec4& btnUp, const glm::uvec4& btnBall, const glm::uvec4& btnDown, const glm::uvec4& sliderImgTop, const glm::uvec4& sliderImgMid, const glm::uvec4& sliderImgBottom);
			virtual ~VScrollbar();
			
			// general GUIItem interface
			virtual void Display();
			virtual bool HandleEvent(EventTypes eventType, SDL_Event* event) override;

            int stepX = 10;
            int stepY = 10;
            int scrollMax = 0;
            
            // unique Text interface
            void StateProcessor();
            VScrollbarStates state = VScrollbarStates::BallPanelReleased;
            glm::uvec2 ballCoordinates = {0, 0};
            glm::uvec4 ballPanelArea = {0, 0, 0, 0};
            bool correctingMousePosition = false;
        protected:
            Text* m_textItem = nullptr;
            Button* m_BtnUp = nullptr;
            Button* m_BtnDown = nullptr;
            Button* m_BtnBall = nullptr;
            VImageTemplate* m_sliderPanel = nullptr;
		};
	}
}

#endif
