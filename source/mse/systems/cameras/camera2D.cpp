#include <mse/systems/cameras/camera2D.h>

namespace mse
{
	Camera2D::Camera2D()
	{}
	
	Camera2D::Camera2D(Scene* toScene)
	: scene(toScene)
	{}
	
	Camera2D::~Camera2D()
	{}
}
