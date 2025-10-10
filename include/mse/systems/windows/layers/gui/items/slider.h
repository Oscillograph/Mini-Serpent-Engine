#ifndef MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_SLIDER_H
#define MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_SLIDER_H

#include <mse/core.h>
#include <mse/systems/windows/layers/gui/guiitem.h>

namespace mse
{
	namespace gui
	{
        class Button;
        class HImageTemplate;
        
		class HSlider : public GUIItem
        {
        public:
            // general initialization
            HSlider();
            
            HSlider(Layer* layer, const glm::uvec4& area, float* var, float min, float max, float step, const std::string&  spritelist, const glm::uvec3& colorKey, const glm::uvec4& btnLeft, const glm::uvec4& btnBall, const glm::uvec4& btnRight, const glm::uvec4& sliderImgLeft, const glm::uvec4& sliderImgMid, const glm::uvec4& sliderImgRight);
            void Init(Layer* layer, const glm::uvec4& area, float* var, float min, float max, float step, const std::string& spritelist, const glm::uvec3& colorKey, const glm::uvec4& btnLeft, const glm::uvec4& btnBall, const glm::uvec4& btnRight, const glm::uvec4& sliderImgLeft, const glm::uvec4& sliderImgMid, const glm::uvec4& sliderImgRight);
            
            virtual ~HSlider();
            
            // general GUIItem interface
            virtual void Display();
            virtual bool HandleEvent(EventTypes eventType, SDL_Event* event) override;
            
            float varMin = 0;
            float varMax = 0;
            float varStep = 0;
            
            float* var = nullptr;
            
            // unique HSlider interface
            glm::uvec2 ballCoordinates = {0, 0};
            glm::uvec4 ballPanelArea = {0, 0, 0, 0};
            bool correctingMousePosition = false;
        protected:
            Button* m_btnLeft = nullptr;
            Button* m_btnRight = nullptr;
            Button* m_BtnBall = nullptr;
            HImageTemplate* m_sliderPanel = nullptr;
        };
	}
}

#endif
