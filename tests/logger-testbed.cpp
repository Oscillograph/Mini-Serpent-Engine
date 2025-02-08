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
		// Game GUI
		gameCanvas = (mse::gui::Canvas*)(AddElement(new mse::gui::Canvas(this, {100, 10, 210, 220}, {32, 64, 48, 255})));
		
		gameCanvas->callbacks[mse::EventTypes::KeyDown] = [&](SDL_Event* event){
		};

		mse::gui::Button* playBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Играть", {10, 10, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
		playBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
			mode = 1;
		};
		mse::gui::Button* settingsBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Настройки", {10, 20, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
		settingsBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
			mode = 2;
		};
		mse::gui::Button* scoresBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Рекорды", {10, 30, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
		scoresBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
			mode = 3;
		};
		mse::gui::Button* aboutBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Титры", {10, 40, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
		aboutBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
			mode = 4;
		};
		mse::gui::Button* exitBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Выйти", {10, 50, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
		exitBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
			mode = 5;
		};
	}
	
	virtual void OnUpdate() override
	{
		switch (mode)
		{
			// idle
			case 0:
			{
				break;
			}
			// game
			case 1:
			{
				break;
			}
			// settings
			case 2:
			{
				break;
			}
			// scores
			case 3:
			{
				break;
			}
			// about
			case 4:
			{
				break;
			}
			// exit
			case 5:
			{
				MSE_LOG("Commanding to stop the application");
				mse::Application::GetApplication()->Stop();
				break;
			}
		}
	}
	
	~SimpleUILayer()
	{
		MSE_LOG("Destroyed a simpleUI layer");
	}
	
	int mode = 0; // 0 - nothing, 1 - game, 2 - settings, 3 - scores, 4 - about, 5 - exit
	mse::gui::Canvas* gameCanvas = nullptr;
};

class App : public mse::Application
{
public:
	App() : mse::Application()
	{
		MSE_LOG("Hello, world!");
		MSE_ERROR("Joke");
		
		MSE_LOG("Commanding to open a window");
		m_window = mse::WindowManager::CreateWindow(u8"Test window", 50, 50, 320, 240);
		m_window->callbacks[mse::EventTypes::KeyDown] = [&](SDL_Event* event){
			MSE_LOG("Key pressed: ", event->key.keysym.sym);
			return true;
		};
		
		MSE_LOG("Commanding to create a scene");
		m_scene = new mse::Scene();

		mse::Arcade::Unit* unit = new mse::Arcade::Unit(
			m_scene,
			m_window,
			"Volleyballist",
			"./data/img/Volleyballist.png",
			{
				{mse::Commands::KBCommand_Left, mse::ScanCode::A}, 
				{mse::Commands::KBCommand_Right, mse::ScanCode::D},
				{mse::Commands::KBCommand_Jump, mse::ScanCode::W},
				{mse::Commands::KBCommand_Down, mse::ScanCode::S}
			},
			{0, 0},
			{1, 1},
			{1.0f, 1.0f},
			{0, 0, 0},
			{0, 0}
			);
		
		unit->SetAnimations(
			{
				mse::EntityStates::STAND1,
				mse::EntityStates::STAND2,
				mse::EntityStates::WALK1,
				mse::EntityStates::WALK2,
				mse::EntityStates::JUMP1,
				mse::EntityStates::JUMP2,
				mse::EntityStates::STAND3,
				mse::EntityStates::STAND,
			},
			{20, 30},
			15.0f,
			true
			);
		
		unit->ChangeDirection(1);

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
	mse::Scene* m_scene = nullptr;
	mse::Window* m_window = nullptr;
	mse::Texture* cse_texture = nullptr;
};

mse::Application* mse::CreateApplication()
{
	return new App();
}
