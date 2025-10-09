#include <mse/systems/windows/layers/gui/items/slider.h>
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
        HSlider::HSlider()
        {
            Init(nullptr, {0, 0, 0, 0}, 0, 0, 0, "", {0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0});
        }
        
        HSlider::HSlider(Layer* layer, const glm::uvec4& area, float min, float max, float step, const std::string&  spritelist, const glm::uvec3& colorKey, const glm::uvec4& btnUp, const glm::uvec4& btnBall, const glm::uvec4& btnDown, const glm::uvec4& sliderImgTop, const glm::uvec4& sliderImgMid, const glm::uvec4& sliderImgBottom)
        {
            
        }
        
        void HSlider::Init(Layer* layer, const glm::uvec4& area, float min, float max, float step, const std::string&  spritelist, const glm::uvec3& colorKey, const glm::uvec4& btnUp, const glm::uvec4& btnBall, const glm::uvec4& btnDown, const glm::uvec4& sliderImgTop, const glm::uvec4& sliderImgMid, const glm::uvec4& sliderImgBottom)
        {
            
        }
        
        HSlider::~HSlider()
        {
            
        }
        
        void HSlider::Display()
        {}
        
        bool HSlider::HandleEvent(EventTypes eventType, SDL_Event* event)
        {}
	}
}
