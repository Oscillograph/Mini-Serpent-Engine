# ! WORK IN PROGRESS ! #
# MINI SERPENT ENGINE #
Mini Serpent Engine (MSE) is a simple game engine for scientific research and entertainment applications. It is based on SDL v2.28.4 and at some point will replace with itself a version of another project of mine called Crystal Serpent Engine.
Currently, MSE is meant to be windows-only with main compatibility focus on Windows 7 and OpenGL 3.3+.

Upd. (3.02.2026): Due to personal circumstances, the project's attention is being shifted towards Linux compatibility with portable approach in mind.

## Features ##
The game engine heavily relies on the vision briefly discussed on page: https://kvk-tech.ru/post/devlog-cse-001

Currently MSE has:
+ Logging system that sends messages to console window;
+ Platform system which consists of a simple 2D Renderer, Window management and events listener which all are bound to SDL;
+ Resource management system which doesn't plot allocated memory structure yet;
+ Window management system to provide access to windows created by the app in runtime;
+ Individual Layer manegement system per window;
+ Layers which are attached to windows by the engine user's design and can have graphical user interface (GUI) items which have their own editable callbacks;
+ GUI items (Canvas and a generic Button);
+ Application class itself;
+ Bitmap fonts support (if the font was not developed specifically for MSE, the alphabet might need to be hardcoded though).

## Project files organization ##
+ /bin - for binaries meant to be dynamically linked
+ /build - for binaries during the build process
+ /data - various assets required by the engine systems
+ /docs - documentation (TODO: use doxygen)
+ /external - external libraries developed by someone else
+ /examples - example apps using MSE
+ /include - header files
+ /src - source files
+ /out - output .exe here
+ /tests - tests (TODO: implement them)
+ /tools - various MSE-specific tools, including scene editor, asset editor, etc. (TODO: develop them)
+ LICENSE.md - current license is creative commons
+ NEWS.md - recent updates
+ README.md - this document

Repository development strategy is yet to be devised.

## Usage ##
The engine is designed to be used through a header file. The common way to use it looks like this:
```
#include <mse/mse.h>

// Set up layers:
class Layer1 : public CSE::Layer
{
public:
	Layer1()
	: mse::Layer()
	{
		MSE_LOG("Constructed a simple layer");
	};

	~Layer1() 
	{};
};

// Set up the app class:
class App : public mse::Application
{
public:
	App() : mse::Application()
	{
		MSE_LOG("Hello, world!");

		MSE_LOG("Commanding to open a window");
		m_window = mse::WindowManager::CreateWindow(u8"Test window", 50, 50, 320, 240);
		mse::Renderer::SetActiveWindow(m_window);

		m_window->GetLayerManager()->Attach(new Layer1());
	}
	~App()
	{
		MSE_LOG("Commanding to close a window");
		mse::WindowManager::DestroyWindow(m_window);
		m_window = nullptr;
		MSE_LOG("Goodbye, world!");
	}
	
private:
	mse::Window* m_window = nullptr;
};

// Define a pre-declared function to create the application:
mse::Application* mse::CreateApplication()
{
	return new App();
}
```
That's it -- you're ready to have fun!

## Build notes ##
MSE is built using TDM-GCC via Red Panda C++ IDE.

Linker options:
```
-lmingw32_@@_-lSDL2main_@@_-lSDL2_@@_-lSDL2_image_@@_-lSDL2_mixer
```
(Obviously, there should be compiled libraries for SDL2, SDL_image and SDL_mixer)

Includes directories should contain:
- SDL/include
- Mini Serpent Engine/include
- Mini Serpent Engine/external

A precompiled header can dramatically decrease compilation time:
mse/common.h

## Tech Stack ##
+ **C++17** (almost everything)
+ **[UTF8-CPP](https://github.com/nemtrif/utfcpp)** (unicode strings support)
+ **[SDL2](https://github.com/libsdl-org/SDL)** (windowing, events listening, rendering, sounds)
+ **[GLM](https://github.com/g-truc/glm)** (math library)
+ **[Color Console](https://github.com/aafulei/color-console)** (Windows-only)
+ **[EnTT](https://github.com/skypjack/entt)** (entities and components management in scenes)
