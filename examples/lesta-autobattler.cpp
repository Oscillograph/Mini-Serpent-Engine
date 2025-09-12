#include <mse/mse.h>

#include <lesta-autobattler/fwd.h>

LAutobattler::GamePages mode = LAutobattler::GamePages::None;

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
		mse::gui::Button* playBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Новая игра", {10, 10, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
		playBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
			mode = LAutobattler::GamePages::CharacterCreation;
			mse::Canban::PutTask(mse::CanbanEvents::Backend_Create, {mse::CanbanEvents::Backend_Create, mse::CanbanReceiver::Backend, nullptr});
			mse::Canban::PutTask(mse::CanbanEvents::Backend_Run, {mse::CanbanEvents::Backend_Run, mse::CanbanReceiver::Backend, nullptr});
            mse::Canban::PutTask(mse::CanbanEvents::Backend_Stop, {mse::CanbanEvents::Backend_Stop, mse::CanbanReceiver::Backend, nullptr});
		};
		mse::gui::Button* settingsBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Загрузить", {10, 20, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
		settingsBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
			mode = LAutobattler::GamePages::CharacterLoad;
		};
		mse::gui::Button* scoresBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Рекорды", {10, 30, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
		scoresBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
			mode = LAutobattler::GamePages::Highscores;
		};
		mse::gui::Button* aboutBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Титры", {10, 40, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
		aboutBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
			mode = LAutobattler::GamePages::Credits;
		};
		mse::gui::Button* exitBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Выйти", {10, 50, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
		exitBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
			mse::Canban::PutTask(mse::CanbanEvents::Backend_Stop, {mse::CanbanEvents::Backend_Stop, mse::CanbanReceiver::Backend, nullptr});
			mode = LAutobattler::GamePages::Exit;
		};
	}
	
	virtual void OnUpdate() override
	{
		switch (mode)
		{
			// idle
            case LAutobattler::GamePages::MainMenu:
			{
//				MSE_LOG("Initial screen mode");
				break;
			}
			// game
			case LAutobattler::GamePages::CharacterCreation:
			{
//				MSE_LOG("Game mode");
				break;
			}
			// settings
			case LAutobattler::GamePages::CharacterLoad:
			{
//				MSE_LOG("Settings mode");
				break;
			}
			// scores
			case LAutobattler::GamePages::Highscores:
			{
//				MSE_LOG("Scores mode");
				break;
			}
			// about
			case LAutobattler::GamePages::Credits:
			{
//				MSE_LOG("About mode");
				break;
			}
			// exit
			case LAutobattler::GamePages::Exit:
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
	
	mse::gui::Canvas* gameCanvas = nullptr;
};

class GameScene : mse::Scene
{
    GameScene(){};
    ~GameScene()
    {
        delete player;
        player = nullptr;
        delete npc;
        npc = nullptr;
    };
    
    virtual void Start()
    {
        player = CreateEntity("PlayerCharacter");
        npc = CreateEntity("NPC");
        
        player->AddComponent<LAutobattler::CharacterStats>();
        npc->AddComponent<LAutobattler::CharacterStats>();
    }
    
    mse::Entity* player = nullptr;
    mse::Entity* npc = nullptr;
};

class App : public mse::Application
{
public:
	App() : mse::Application()
	{
		MSE_LOG("Hello, world!");
		MSE_ERROR("Joke");
		
		MSE_LOG("Commanding to open a window");
		m_window = mse::WindowManager::CreateWindow(u8"Горка", 50, 50, 320, 240);
		m_window->callbacks[mse::EventTypes::KeyDown] = [&](SDL_Event* event){
			MSE_LOG("Key pressed: ", event->key.keysym.sym);
			return true;
		};
//		m_window2 = mse::WindowManager::CreateWindow(u8"Морка", 450, 50, 320, 240);
//		m_window2->callbacks[mse::EventTypes::KeyDown] = [&](SDL_Event* event){
//			MSE_LOG("Key pressed 2: ", event->key.keysym.sym);
//			return true;
//		};
		
		MSE_LOG("Commanding to create a scene");
		m_scene = new mse::Scene();

//		mse::Arcade::Unit* unit = new mse::Arcade::Unit(
//			m_scene,
//			m_window,
//			"Volleyballist",
//			"./data/img/Volleyballist.png",
//			{
//				{mse::Commands::KBCommand_Left, mse::ScanCode::A}, 
//				{mse::Commands::KBCommand_Right, mse::ScanCode::D},
//				{mse::Commands::KBCommand_Jump, mse::ScanCode::W},
//				{mse::Commands::KBCommand_Down, mse::ScanCode::S}
//			},
//			{0, 0},
//			{1, 1},
//			{1.0f, 1.0f},
//			{0, 0, 0},
//			{0, 0}
//			);
//		
//		unit->SetAnimations(
//			{
//				mse::EntityStates::STAND1,
//				mse::EntityStates::STAND2,
//				mse::EntityStates::WALK1,
//				mse::EntityStates::WALK2,
//				mse::EntityStates::JUMP1,
//				mse::EntityStates::JUMP2,
//				mse::EntityStates::STAND3,
//				mse::EntityStates::STAND,
//			},
//			{20, 30},
//			15.0f,
//			true
//			);
//		
//		unit->ChangeDirection(1);

		mse::Renderer::SetActiveWindow(m_window);
		m_window->GetLayerManager()->Attach(new SimpleUILayer());
//		m_window2->GetLayerManager()->Attach(new SimpleUILayer());
	}
	
	~App()
	{
		MSE_LOG("Commanding to destroy a window");
		delete cse_texture;
		cse_texture = nullptr;
		mse::WindowManager::DestroyWindow(m_window);
		m_window = nullptr;
		mse::WindowManager::DestroyWindow(m_window2);
		m_window2 = nullptr;
		MSE_LOG("Goodbye, world!");
	}
	
	virtual void BackendStep() override
	{
		static int i = 0;
		MSE_LOG("Application: my BackendStep() method call.");
		++i;
		MSE_LOG("Application: i = ", i);
		return;
	}
	
private:
	mse::Scene* m_scene = nullptr;
	mse::Window* m_window = nullptr;
	mse::Window* m_window2 = nullptr;
	mse::Texture* cse_texture = nullptr;
};

mse::Application* mse::CreateApplication()
{
	return new App();
}
