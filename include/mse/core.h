#ifndef MSE_CORE_H
#define MSE_CORE_H

// basic includes required by all headers, modules and utilities
#include <mse/protocore.h>

// independent engine systems
#include <mse/utils/utils.h>
#include <mse/utils/logger.h>

// systems

// custom types
namespace mse
{
	typedef uint32_t TimeType;
}

// important structs
namespace mse
{
    struct AppConfig
    {
        bool music = false;
        float musicVolume = 0;
        bool sounds = false;
        float soundsVolume = 0;
        bool playInBackground = false;
        bool fullscreen = true;
    };
}

// forward declarations
namespace mse
{
	class Platform; // static - a system to abstract away various platform calls from window management and event listening to renderer and audio system calls
	class Renderer; // static - a platform subsystem specifically for rendering routines
	class Texture;
    class Cursor;
    class SoundMan; // static - a platform subsystem specifically for audio management routines
    class Sound;
    class Track;
	class WindowManager; // static - a system to command windows
	class Window;
	class LayerManager; // static - a window subsystem which keeps track of all layers attached and allows to command them
	class Layer;
	class GUIItem;
	class ResourceManager; // static - a system that keeps track of all media resources loaded into memory and collects garbage occasionally
	class Resource;
	class Font;
	class FontBitmap;
	class FontTrueType;
	class SceneManager; // static - a system that keeps track of scenes and eases their management
	class Scene;
	class PhysicsProcessor; // a system that manages physics worlds and corresponding entities
	class PhysicsAPI; // virtual interface to physics processor implementations
	class World;
	class Entity;
	
	// unused forward declarations
	class Event; // unused at all - SDL_Event covers almost all needs of the engine; however, it is being kept for a future internal events management system to ease communication between the engine subsystems and architecture layers
	class Camera2D;
}

#endif
