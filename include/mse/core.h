#ifndef MSE_CORE_H
#define MSE_CORE_H

// basic includes required by all headers, modules and utilities
#include <mse/protocore.h>

// independent engine systems
#include <mse/utils/utils.h>
#include <mse/utils/logger.h>

// systems


// forward declarations
namespace mse
{
	class Platform; // static
	class Renderer; // static
	class WindowManager; // static
	class Window;
	class LayerManager; // static
	class Layer;
	class Texture;
	
	// unused forward declarations
	class Event; // unused
	class GUIItem;
	class ResourceManager; // static
	class Resource;
	class SceneManager; // static
	class Scene;
	class Camera2D;
}

#endif
