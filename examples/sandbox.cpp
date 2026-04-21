#include <mse/mse.h>

/*
 * Sandbox is an app to test some possibilities of SDL3, Compute Shaders and many other things
 *
 * Isn't really an example or anything
*/

// Set up layers:
class MainLayer : public mse::Layer
{
public:
	MainLayer()
	: mse::Layer()
	{
		MSE_LOG("SandboxApp/MainLayer: Constructed a simple layer");
	};

	~MainLayer()
	{
		MSE_LOG("SandboxApp/MainLayer: Deconstructed a simple layer");
	};
};

// Set up the app class:
class SandboxApp : public mse::Application
{
public:
	SandboxApp() : mse::Application()
	{
		MSE_LOG("SandboxApp: Hello, world!");

		MSE_LOG("SandboxApp: Commanding to open a window");
		m_window = mse::WindowManager::CreateWindow(u8"MSE Sandbox window", 50, 50, 320, 240);
		mse::Renderer::SetActiveWindow(m_window);

		m_window->GetLayerManager()->Attach(new MainLayer());
	}
	~SandboxApp()
	{
		MSE_LOG("SandboxApp: Commanding to close a window");
		mse::WindowManager::DestroyWindow(m_window);
		m_window = nullptr;
		MSE_LOG("SandboxApp: Goodbye, world!");
	}

private:
	mse::Window* m_window = nullptr;
};

// Define a pre-declared function to create the application:
mse::Application* mse::CreateApplication()
{
	return new SandboxApp();
}
