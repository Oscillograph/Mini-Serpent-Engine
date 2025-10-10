#ifndef MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_CHECKBOX_H
#define MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_CHECKBOX_H

#include <mse/core.h>
#include <mse/systems/windows/layers/gui/guiitem.h>

namespace mse
{
	namespace gui
	{
        enum class CheckboxStates
        {
            Unchecked,
            Checked,
        };
        
		class Checkbox : public GUIItem
		{
		public:
			// general initialization
			Checkbox();
            Checkbox(Layer* layer, const glm::uvec4& area, const std::string& spritelist, const glm::uvec3& colorKey, bool* var, const glm::uvec4& sourceUnchecked, const glm::uvec4& sourceChecked, const glm::uvec4& sourceDisabledUnchecked, const glm::uvec4& sourceDisabledChecked);
			void Init(Layer* layer, const glm::uvec4& area, const std::string& spritelist, const glm::uvec3& colorKey, bool* var, const glm::uvec4& sourceUnchecked, const glm::uvec4& sourceChecked, const glm::uvec4& sourceDisabledUnchecked, const glm::uvec4& sourceDisabledChecked);
			virtual ~Checkbox();
			
			// general GUIItem interface
			virtual void Display();
            virtual bool HandleEvent(EventTypes eventType, SDL_Event* event) override;

            // unique Checkbox interface
            CheckboxStates state = CheckboxStates::Unchecked;
            bool isPushed = false;
		protected:
            // generic
            bool* m_var = nullptr;
            // sprite-based
            Resource* m_spriteList = nullptr;
		};
	}
}

#endif
