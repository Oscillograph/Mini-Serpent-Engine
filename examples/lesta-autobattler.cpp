#include <mse/mse.h>
#include <lesta-autobattler/game-fwd.h>
#include <lesta-autobattler/game-data.h>

#include <lesta-autobattler/gamestates.h>
#include <lesta-autobattler/layers.h>

// ********************************************************************************************** //
//                                    SCENES (do I need one?)
// ********************************************************************************************** //

LAutobattler::GameDB gameDB = LAutobattler::InitGameData();
LAutobattler::Game game;
GameStateMachine gsm;

class GameScene : public mse::Scene
{
public:
    GameScene()
    {
        MSE_LOG("GameScene constructor...");
        // register game states
        gsm.states[LAutobattler::GamePages::Intro] = new IntroPageState();
        gsm.states[LAutobattler::GamePages::MainMenu] = new MainPageState();
//        gsm.states[LAutobattler::GamePages::CharacterCreation];
//        gsm.states[LAutobattler::GamePages::CharacterLoad];
//        gsm.states[LAutobattler::GamePages::CharacterSave];
//        gsm.states[LAutobattler::GamePages::CharacterUpdate];
//        gsm.states[LAutobattler::GamePages::ArenaSetup];
//        gsm.states[LAutobattler::GamePages::ArenaBattle];
//        gsm.states[LAutobattler::GamePages::ArenaAftermath];
//        gsm.states[LAutobattler::GamePages::Winner];
//        gsm.states[LAutobattler::GamePages::GameOver];
//        gsm.states[LAutobattler::GamePages::Highscores];
//        gsm.states[LAutobattler::GamePages::Credits];
        gsm.states[LAutobattler::GamePages::Exit] = new ExitPageState();
        MSE_LOG("GameScene: gsm states initiated and stored.");
        
        // setup initial game state
        gsm.ChangeStateTo(LAutobattler::GamePages::Intro);
        MSE_LOG("GameScene constructor...done");
    };
    
    ~GameScene()
    {
        delete player;
        player = nullptr;
        delete npc;
        npc = nullptr;
        MSE_LOG("GameScene destructor...done");
    };
    
    virtual void OnUpdate(mse::TimeType time)
    {
        gsm.OnUpdate(time);
    }
    
    mse::Entity* player = nullptr;
    mse::Entity* npc = nullptr;
};

// ********************************************************************************************** //
//                                        Application
// ********************************************************************************************** //

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
            game.keyPressed = true;
			return true;
		};
		mse::Renderer::SetActiveWindow(m_window);
		
		MSE_LOG("Commanding to create and load scene");
        m_scene = new GameScene();
        mse::SceneManager::Load(m_scene);
        m_scene->Start();
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
	
public:
	mse::Scene* m_scene = nullptr;
	mse::Window* m_window = nullptr;
	mse::Window* m_window2 = nullptr;
	mse::Texture* cse_texture = nullptr;
};

mse::Application* mse::CreateApplication()
{
	return new App();
}
