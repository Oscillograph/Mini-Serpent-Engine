#ifndef MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_IMAGE_H
#define MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_IMAGE_H

#include <mse/core.h>
#include <mse/systems/windows/layers/gui/guiitem.h>

namespace mse
{
	namespace gui
	{
		class Image : public GUIItem
		{
		public:
			// general initialization
			Image();
            Image(Layer* layer, const glm::uvec4& area, const std::string& spritelist, const glm::uvec4& sourceArea, const glm::uvec4& bgcolor);
			void Init(Layer* layer, const glm::uvec4& area, const std::string& spritelist, const glm::uvec4& sourceArea, const glm::uvec4& bgcolor);
			virtual ~Image();
			
			// general GUIItem interface
			virtual void Display();

		protected:
            // generic
			glm::uvec4 m_backgroundColor = {0, 0, 0, 0};
            
            // sprite-based
            Resource* m_spriteList = nullptr;
            glm::uvec4 m_sourceArea = {0, 0, 0, 0};
		};
	}
}

#endif
