/* A chalkboard to draw on */

#include <mse/mse.h>

class SimpleUILayer : public mse::Layer
{
public:
	SimpleUILayer()
	: mse::Layer()
	{
		MSE_LOG("Constructed a simpleUI layer");
	}
	
	virtual void OnInit() override
	{
		mse::gui::Canvas* canvas = (mse::gui::Canvas*)(AddElement(new mse::gui::Canvas(this, {20, 10, 290, 220}, {32, 64, 48, 255})));
		canvas->SetDrawColor({228, 228, 228, 255});
		
		mse::gui::Button* redPencil = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Red Pencil", {2, 10, 15, 10}, {196, 64, 64, 255}, {196, 196, 32, 255})));
		redPencil->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&, canvas](SDL_Event* event){
			canvas->SetDrawColor({228, 64, 64, 255});
		};
		mse::gui::Button* greenPencil = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Green Pencil", {2, 20, 15, 10}, {64, 196, 64, 255}, {196, 196, 32, 255})));
		greenPencil->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&, canvas](SDL_Event* event){
			canvas->SetDrawColor({64, 228, 64, 255});
		};
		mse::gui::Button* bluePencil = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Blue Pencil", {2, 30, 15, 10}, {64, 64, 196, 255}, {196, 196, 32, 255})));
		bluePencil->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&, canvas](SDL_Event* event){
			canvas->SetDrawColor({64, 64, 228, 255});
		};
		mse::gui::Button* yellowPencil = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Yellow Pencil", {2, 40, 15, 10}, {196, 196, 64, 255}, {196, 196, 32, 255})));
		yellowPencil->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&, canvas](SDL_Event* event){
			canvas->SetDrawColor({228, 228, 64, 255});
		};
		mse::gui::Button* pinkPencil = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Pink Pencil", {2, 50, 15, 10}, {196, 64, 196, 255}, {196, 196, 32, 255})));
		pinkPencil->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&, canvas](SDL_Event* event){
			canvas->SetDrawColor({228, 64, 228, 255});
		};
		mse::gui::Button* cyanPencil = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Cyan Pencil", {2, 60, 15, 10}, {64, 196, 196, 255}, {196, 196, 32, 255})));
		cyanPencil->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&, canvas](SDL_Event* event){
			canvas->SetDrawColor({64, 228, 228, 255});
		};
		mse::gui::Button* whitePencil = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"White Pencil", {2, 70, 15, 10}, {228, 228, 228, 255}, {32, 32, 32, 255})));
		whitePencil->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&, canvas](SDL_Event* event){
			canvas->SetDrawColor({228, 228, 228, 255});
		};
	}
	
	~SimpleUILayer()
	{
		MSE_LOG("Destroyed a simpleUI layer");
	}
};

class App : public mse::Application
{
public:
	App() : mse::Application()
	{
		MSE_LOG("Hello, world!");
		MSE_CORE_ERROR("Joke");
		
		MSE_LOG("Commanding to open a window");
		m_window = mse::WindowManager::CreateWindow(u8"Test window", 50, 50, 320, 240);

		mse::Renderer::SetActiveWindow(m_window);
		m_window->GetLayerManager()->Attach(new SimpleUILayer());
	}
	
	~App()
	{
		MSE_LOG("Commanding to destroy a window");
		delete cse_texture;
		cse_texture = nullptr;
		mse::WindowManager::DestroyWindow(m_window);
		m_window = nullptr;
		MSE_LOG("Goodbye, world!");
	}
	
private:
	mse::Window* m_window = nullptr;
	mse::Texture* cse_texture = nullptr;
};

mse::Application* mse::CreateApplication()
{
	return new App();
}
