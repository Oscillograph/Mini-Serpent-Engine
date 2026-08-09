#include <mse/mse.h>

/*
 * GUI Simple is an app to develop and test gui capabilities of Mini Serpent Engine
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
		MSE_LOG("GUISimpleApp/MainLayer: Constructed a simple layer");
	};

	virtual ~MainLayer()
	{
		MSE_LOG("GUISimpleApp/MainLayer: Deconstructed a simple layer");
	};

	virtual void OnInit() override
	{
		mse::gui::InputInt* inputInt = (mse::gui::InputInt*)(AddElement(new mse::gui::InputInt(this, 0, {2, 10, 15, 10}, {196, 64, 64, 255}, {196, 196, 32, 255})));
		inputInt->callbacks[mse::EventTypes::GUIItemMouseButtonDown] = [&, inputInt](SDL_Event* event){
			inputInt->isFocused = true;
		};

		// mse::gui::Button* redPencil = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Red Pencil", {2, 10, 15, 10}, {196, 64, 64, 255}, {196, 196, 32, 255})));
		// redPencil->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&, canvas](SDL_Event* event){
		// 	canvas->SetDrawColor({228, 64, 64, 255});
		// };
	}
};

// Set up the app class:
class GUISimpleApp : public mse::Application
{
public:
	GUISimpleApp() : mse::Application()
	{
		MSE_LOG("GUISimpleApp: Hello, world!");

		MSE_LOG("GUISimpleApp: Commanding to open a window");
		m_window = mse::WindowManager::CreateWindow(u8"GUISimple: испытания графического интерфейса", 50, 50, 640, 480);
		mse::Renderer::SetActiveWindow(m_window);

		m_window->GetLayerManager()->Attach(new MainLayer());
	}

	virtual ~GUISimpleApp()
	{
		MSE_LOG("GUISimpleApp: Commanding to close a window");
		mse::WindowManager::DestroyWindow(m_window);
		m_window = nullptr;
		MSE_LOG("GUISimpleApp: Goodbye, world!");
	}

private:
	mse::Window* m_window = nullptr;
};

// Define a pre-declared function to create the application:
mse::Application* mse::CreateApplication()
{
	return new GUISimpleApp();
}
