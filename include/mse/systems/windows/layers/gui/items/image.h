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
        
        
        class VImageTemplate : public GUIItem
        {
        public:
            // general initialization
            VImageTemplate();
            VImageTemplate(Layer* layer, const glm::uvec4& area, const std::string& spritelist, const glm::uvec3& colorKey, const glm::uvec4& sourceTop, const glm::uvec4& sourceMid, const glm::uvec4& sourceBottom);
            void Init(Layer* layer, const glm::uvec4& area, const std::string& spritelist, const glm::uvec3& colorKey, const glm::uvec4& sourceTop, const glm::uvec4& sourceMid, const glm::uvec4& sourceBottom);
            virtual ~VImageTemplate();
            
            // general GUIItem interface
            virtual void Display();
            
        protected:
            // generic
            glm::uvec4 m_backgroundColor = {0, 0, 0, 0};
            
            // sprite-based
            Resource* m_spriteList = nullptr;
            glm::uvec4 m_sourceTop = {0, 0, 0, 0};
            glm::uvec4 m_sourceMid = {0, 0, 0, 0};
            glm::uvec4 m_sourceBottom = {0, 0, 0, 0};
        };
	}
}

#endif
