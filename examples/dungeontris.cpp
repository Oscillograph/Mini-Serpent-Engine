#include <mse/mse.h>
#include <dungeontris/game-fwd.h>
#include <dungeontris/game-data.h>
#include <dungeontris/data-loader.h>

#include <dungeontris/gamestates.h>
#include <dungeontris/layers.h>

// ********************************************************************************************** //
//                                    SCENES (do I need one?)
// ********************************************************************************************** //

DTetris::GameDB gameDB = DTetris::InitGameData();
DTetris::Game game;
GameStateMachine gsm;

class GameScene : public mse::Scene
{
public:
    GameScene()
    {
        MSE_LOG("GameScene constructor...");
        // register game states
        gsm.states[DTetris::GamePages::Intro] = new IntroPageState();
        gsm.states[DTetris::GamePages::MainMenu] = new MainPageState();
        gsm.states[DTetris::GamePages::CharacterCreation] = new CharacterCreatePageState();
//        gsm.states[DTetris::GamePages::CharacterLoad] = new CharacterLoadPageState();
//        gsm.states[DTetris::GamePages::CharacterSave] = new CharacterSavePageState();
        gsm.states[DTetris::GamePages::CharacterUpdate] = new CharacterUpdatePageState();
        gsm.states[DTetris::GamePages::ArenaSetup] = new ArenaSetupPageState();
        gsm.states[DTetris::GamePages::ArenaBattle] = new ArenaBattlePageState();
        gsm.states[DTetris::GamePages::ArenaAftermath] = new ArenaAftermathPageState;
        gsm.states[DTetris::GamePages::Winner] = new WinnerPageState();
        gsm.states[DTetris::GamePages::GameOver] = new GameOverPageState();
        gsm.states[DTetris::GamePages::Settings] = new SettingsPageState();
//        gsm.states[DTetris::GamePages::Highscores] = new HighscoresPageState();
        gsm.states[DTetris::GamePages::Credits] = new CreditsPageState();
        gsm.states[DTetris::GamePages::Exit] = new ExitPageState();
        MSE_LOG("GameScene: gsm states initiated and stored.");
        
        // setup initial game state
        gsm.ChangeStateTo(DTetris::GamePages::Intro);
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
        
        std::srand((unsigned int)(time(NULL)));
		
		MSE_LOG("Commanding to open a window");
		m_window = mse::WindowManager::CreateWindow(u8"Подземетрис", 50, 50, 320, 240);
		m_window->callbacks[mse::EventTypes::KeyDown] = [&](SDL_Event* event){
			MSE_LOG("Key pressed: ", event->key.keysym.sym);
            if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
            {
//                m_window->ToggleFullscreen();
                mse::SoundMan::PlayNext();
            }
            if (event->key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            {
                if (gsm.current->page == DTetris::GamePages::MainMenu)
                {
                    this->Stop();
                } else {
                    gsm.ChangeStateTo(DTetris::GamePages::MainMenu);
                }
            }
            game.keyPressed = true;
			return true;
		};
        
        // create a custom cursor
        mse::Resource* spriteList = mse::ResourceManager::UseTexture("data/img/screen-images.png", m_window, {0, 0, 0});
        glm::uvec4 spriteListRect = {109, 22, 32, 32};
        m_Cursor = mse::ResourceManager::CreateCursor(m_window, 0, 0, spriteList, spriteListRect, {0, 0, 0});
        mse::Cursor* cursorObj = (mse::Cursor*)(m_Cursor->data);
        SDL_SetCursor(cursorObj->GetNativeCursor());

		mse::Renderer::SetActiveWindow(m_window);
//        m_window->GetLayerManager()->Attach(new SimpleUILayer);
		
		MSE_LOG("Commanding to create and load scene");
        m_scene = new GameScene();
        mse::SceneManager::Load(m_scene);
        m_scene->Start();
        
        DTetris::LoadConfig(game.config, "data/config.yaml");
        config.fullscreen = game.config.fullscreen;
        config.musicVolume = game.config.musicVolume;
        config.soundsVolume = game.config.soundsVolume;
        config.playInBackground = game.config.playInBackground;
        mse::SoundMan::AdjustTrackVolume(game.config.musicVolume);
        mse::SoundMan::AdjustSoundsVolume(game.config.soundsVolume);
        if (config.fullscreen)
        {
            m_window->ToggleFullscreen(2);
        }
        
        mse::SoundMan::LoadSounds({
            "data/audio/sounds/click.wav",
            "data/audio/sounds/samplefocus.com-sword-cut-fx.wav",
            "data/audio/sounds/samplefocus.com-violent-sword-slice-sound-fx_D_minor.wav",
            "data/audio/sounds/samplefocus.com-short-knife-whoosh-fx.wav",
        });
        
        mse::SoundMan::LoadTracks({
            "data/audio/tracks/01_towards_neon_shadows.mp3",
            "data/audio/tracks/02_our_tanya_cries_loud.mp3",
            "data/audio/tracks/03_polar_star.mp3",
            "data/audio/tracks/04_fear_of_empty_pages.mp3",
            "data/audio/tracks/05_airplane_in_love.mp3",
            "data/audio/tracks/06_undina.mp3",
            "data/audio/tracks/07_caleydoscope.mp3",
            "data/audio/tracks/08_quest_for_glory.mp3",
        });
	}
	
	~App()
	{
        MSE_LOG("Saving config");
        DTetris::SaveConfig(game.config, "data/config.yaml");
        
		MSE_LOG("Commanding to destroy a window");
        mse::ResourceManager::DropResource(m_Cursor, m_window);
        m_Cursor = nullptr;
        SDL_SetCursor(NULL);
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
    mse::Resource* m_Cursor = nullptr;
};

mse::Application* mse::CreateApplication()
{
	return new App();
}
