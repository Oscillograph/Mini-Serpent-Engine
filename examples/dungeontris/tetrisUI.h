#ifndef DUNGEONTRIS_TETRISUI_H
#define DUNGEONTRIS_TETRISUI_H

#include <mse/core.h>
#include <mse/systems/windows/layers/gui/guiitem.h>

namespace mse
{
	namespace gui
	{
		// a canvas to draw upon
		class TetrisCanvas : public GUIItem
		{
		public:
			// general initialization
			TetrisCanvas();
			TetrisCanvas(Layer* layer, const glm::uvec4& area);
			void Init(Layer* layer, const glm::uvec4& area);
			virtual ~TetrisCanvas();

			// general GUIItem interface
			virtual void Display() override;

			// TetrisCanvas interface
		};
	}
}

#endif
