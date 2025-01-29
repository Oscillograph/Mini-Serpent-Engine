#ifndef MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_CANVAS_H
#define MSE_SYSTEMS_WINDOWS_LAYERS_GUI_ITEMS_CANVAS_H

#include <mse/core.h>
#include <mse/systems/windows/layers/gui/guiitem.h>

namespace mse 
{
	namespace gui
	{
		// a canvas to draw upon
		class Canvas : public GUIItem
		{
		public:
			// general initialization
			Canvas();
			Canvas(Layer* layer, const glm::uvec4& area, const glm::uvec4& color);
			void Init(Layer* layer, const glm::uvec4& area, const glm::uvec4& color);
			virtual ~Canvas();

			// general GUIItem interface
			virtual void Display();
			
			// unique Canvas interface
			bool SetDrawColor(const glm::uvec4& color);
			bool Clear();
			bool DrawPixel(int x, int y, const glm::uvec4& color = {255, 255, 255, 255});
			bool DrawLine(const glm::ivec4& AB, const glm::uvec4& color = {255, 255, 255, 255});
			bool DrawRect(int x1, int y1, int x2, int y2, const glm::uvec4& color = {255, 255, 255, 255});
			bool FillRect(int x1, int y1, int x2, int y2, const glm::uvec4& color = {255, 255, 255, 255});
			bool DrawCircle(int x, int y, int radius, const glm::uvec4& color = {255, 255, 255, 255});
			bool FillCircle(int x, int y, int radius, const glm::uvec4& color = {255, 255, 255, 255});
			bool DrawText(int x, int y, const std::string& text, int font = 0, const glm::uvec4& color = {255, 255, 255, 255}, const glm::uvec4& backgroundColor = {0, 0, 0, 255});
			
		public:
			glm::uvec4 backgroundColor = {0, 0, 0, 0};
			
		protected:
			bool m_MouseLeftButtonIsHeld = false;
			bool m_MouseRightButtonIsHeld = false;
			bool m_MouseMiddleButtonIsHeld = false;
			glm::uvec4 m_lastColor = {223, 223, 223, 255};
			glm::uvec2 m_lastMouseXY = {0, 0};
		};
	}
}

#endif
