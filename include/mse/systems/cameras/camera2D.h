#ifndef MSE_SYSTEMS_CAMERAS_CAMERA2D_H
#define MSE_SYSTEMS_CAMERAS_CAMERA2D_H

#include <mse/core.h>

namespace mse
{
	// The only function of any camera is to be a mathematical argument to subtract from actual objects position and transforms
	class Camera2D
	{
	public:
		Camera2D();
		Camera2D(Scene* toScene);
		~Camera2D();
		
		glm::ivec4 properties = {0, 0, 0, 0}; // x,y - position; z,w - area
		Scene* scene = nullptr;
	};
}

#endif
