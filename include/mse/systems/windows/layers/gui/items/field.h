#ifndef MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_FIELD_H
#define MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_FIELD_H

#include <mse/core.h>
#include <mse/systems/windows/layers/gui/guiitem.h>

namespace mse
{
	namespace gui
	{
		class Field : public GUIItem
		{
		public:
			// general initialization
			Field();
			Field(Layer* layer, const glm::uvec4& area, const glm::uvec4& bgcolor, bool border = false, int borderSize = 1, const glm::uvec4& borderColor = {255, 255, 255, 255});
			void Init(Layer* layer, const glm::uvec4& area, const glm::uvec4& bgcolor, bool border = false, int borderSize = 1, const glm::uvec4& borderColor = {255, 255, 255, 255});
			virtual ~Field();

			// general GUIItem interface
			virtual void Display() override;

			// unique Field interface
			void ChangeSourceArea(const glm::uvec4& sourceArea);

		protected:
			// generic
			glm::uvec4 m_backgroundColor = {0, 0, 0, 0};
			int m_border = false;
			int m_borderSize = 1;
			glm::uvec4 m_borderColor = {255, 255, 255, 255};

			// sprite-based
			Resource* m_spriteList = nullptr;
			glm::uvec4 m_sourceArea = {0, 0, 0, 0};
		};
	}
}

#endif
