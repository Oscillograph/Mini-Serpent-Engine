#include <mse/mse.h>

#include <lesta-autobattler/fwd.h>

// the usual
struct GameState;
struct GameStateMachine;
struct MainPageState;
// layers
class SimleUILayer;

// redundant?
LAutobattler::GamePages mode = LAutobattler::GamePages::None;

// crude state machine to think about
struct GameState
{
    // so that the state knows what it is
    LAutobattler::GamePages page = LAutobattler::GamePages::None;
    // so that the state can tell where it should move to
    LAutobattler::GamePages changeTo = LAutobattler::GamePages::None;
    
    // layers attachment for GUI and base logic initialization
    virtual bool OnEnter() = 0;
    // laters detachment for GUI, resetting data and cleaning memory
    virtual bool OnExit() = 0;
    // common logic step
    virtual bool OnUpdate(mse::TimeType t = 0) = 0;
    
    GameState(){}
    virtual ~GameState(){}
};

struct GameStateMachine
{
    std::map<LAutobattler::GamePages, GameState*> states = {};
    GameState* current;
    
    void ChangeStateTo(GameState* state)
    {
        while (!current->OnExit()){}
        delete current;
        current = state;
        while(!current->OnEnter()){}
    }
    
    void ChangeStateTo(LAutobattler::GamePages gamePage)
    {
        while (!current->OnExit()){}
        delete current;
        current = states[gamePage];
        while(!current->OnEnter()){}
    }
    
    void OnUpdate(mse::TimeType t = 0)
    {
        current->OnUpdate(t);
        if (current->changeTo != LAutobattler::GamePages::None)
        {
            ChangeStateTo(states[current->changeTo]);
        }
    }
};

GameStateMachine gsm;

// ********************************************************************************************** //
//                                    LAYERS (to render things)
// ********************************************************************************************** //

class IntroUILayer : public mse::Layer
{
public:
    IntroUILayer() : mse::Layer()
    {
        MSE_LOG("Constructed IntroUILayer");
    }
    ~IntroUILayer()
    {
        MSE_LOG("Destroyed IntroUILayer");
    }
    
    virtual void OnInit() override
    {}
    
    virtual void OnUpdate() override
    {}
};

class MainMenuUILayer : public mse::Layer
{
public:
    MainMenuUILayer() : mse::Layer()
    {
        MSE_LOG("Constructed MainMenuUILayer");
    }
    ~MainMenuUILayer()
    {
        MSE_LOG("Destroyed MainMenuUILayer");
    }
    
    virtual void OnInit() override
    {}
    
    virtual void OnUpdate() override
    {}
};

class CharacterCreateUILayer : public mse::Layer
{
public:
    CharacterCreateUILayer() : mse::Layer()
    {
        MSE_LOG("Constructed CharacterCreateUILayer");
    }
    ~CharacterCreateUILayer()
    {
        MSE_LOG("Destroyed CharacterCreateUILayer");
    }
    
    virtual void OnInit() override
    {
        mse::gui::Button* ClassRogueBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Разбойник", {100, 10, 80, 10}, {196, 100, 100, 255}, {32, 32, 32, 255})));
        ClassRogueBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
            LAutobattler::Game::inputClass = LAutobattler::Classes::Rogue;
            m_window->GetLayerManager()->Detach(this);
        };
        
        mse::gui::Button* ClassWarriorBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Воин", {100, 20, 80, 10}, {100, 196, 196, 255}, {32, 32, 32, 255})));
        ClassWarriorBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
            LAutobattler::Game::inputClass = LAutobattler::Classes::Warrior;
            m_window->GetLayerManager()->Detach(this);
        };
        
        mse::gui::Button* ClassBarbarianBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Варвар", {100, 30, 80, 10}, {196, 196, 100, 255}, {32, 32, 32, 255})));
        ClassBarbarianBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
            LAutobattler::Game::inputClass = LAutobattler::Classes::Barbarian;
            m_window->GetLayerManager()->Detach(this);
        };
    }
    
    virtual void OnUpdate() override
    {}
};

class CharacterLoadUILayer : public mse::Layer
{
public:
    CharacterLoadUILayer() : mse::Layer()
    {
        MSE_LOG("Constructed CharacterLoadUILayer");
    }
    ~CharacterLoadUILayer()
    {
        MSE_LOG("Destroyed CharacterLoadUILayer");
    }
    
    virtual void OnInit() override
    {}
    
    virtual void OnUpdate() override
    {}
};

class CharacterSaveUILayer : public mse::Layer
{
public:
    CharacterSaveUILayer() : mse::Layer()
    {
        MSE_LOG("Constructed CharacterSaveUILayer");
    }
    ~CharacterSaveUILayer()
    {
        MSE_LOG("Destroyed CharacterSaveUILayer");
    }
    
    virtual void OnInit() override
    {}
    
    virtual void OnUpdate() override
    {}
};

class WinnerUILayer : public mse::Layer
{
public:
    WinnerUILayer() : mse::Layer()
    {
        MSE_LOG("Constructed WinnerUILayer");
    }
    ~WinnerUILayer()
    {
        MSE_LOG("Destroyed WinnerUILayer");
    }
    
    virtual void OnInit() override
    {}
    
    virtual void OnUpdate() override
    {}
};

class GameOverUILayer : public mse::Layer
{
public:
    GameOverUILayer() : mse::Layer()
    {
        MSE_LOG("Constructed GameOverUILayer");
    }
    ~GameOverUILayer()
    {
        MSE_LOG("Destroyed GameOverUILayer");
    }
    
    virtual void OnInit() override
    {}
    
    virtual void OnUpdate() override
    {}
};

class HighscoresUILayer : public mse::Layer
{
public:
    HighscoresUILayer() : mse::Layer()
    {
        MSE_LOG("Constructed HighscoresUILayer");
    }
    ~HighscoresUILayer()
    {
        MSE_LOG("Destroyed HighscoresUILayer");
    }
    
    virtual void OnInit() override
    {}
    
    virtual void OnUpdate() override
    {}
};

class CreditsUILayer : public mse::Layer
{
public:
    CreditsUILayer() : mse::Layer()
    {
        MSE_LOG("Constructed CreditsUILayer");
    }
    ~CreditsUILayer()
    {
        MSE_LOG("Destroyed CreditsUILayer");
    }
    
    virtual void OnInit() override
    {}
    
    virtual void OnUpdate() override
    {}
};

class ExitUILayer : public mse::Layer
{
public:
    ExitUILayer() : mse::Layer()
    {
        MSE_LOG("Constructed ExitUILayer");
    }
    ~ExitUILayer()
    {
        MSE_LOG("Destroyed ExitUILayer");
    }
    
    virtual void OnInit() override
    {}
    
    virtual void OnUpdate() override
    {}
};

class ArenaUILayer : public mse::Layer
{
public:
    ArenaUILayer() : mse::Layer()
    {
        MSE_LOG("Constructed ArenaUILayer");
    }
    ~ArenaUILayer()
    {
        messageLog = nullptr;
        
        int i = 0;
        LAutobattler::Game::UILogger.Clear();
        
        MSE_LOG("Destroyed ArenaUILayer");
    }
    
    virtual void OnInit() override
    {
        messageLog = (mse::gui::Text*)(AddElement(new mse::gui::Text(
                       this, 
                       U"", 
                       {010, 90, 300, 100}, 
                       {0, 64, 0, 255}, 
                       {255, 255, 0, 255})));
        messageLog->showBorder = true;
    }
    
    virtual void OnUpdate() override
    {
        if ((LAutobattler::Game::gamePage != LAutobattler::GamePages::ArenaSetup) &&
            (LAutobattler::Game::gamePage != LAutobattler::GamePages::ArenaBattle) && 
            (LAutobattler::Game::gamePage != LAutobattler::GamePages::ArenaAftermath))
        {
            m_window->GetLayerManager()->Detach(this);
        } else {
            std::u32string messages = U"";
            LAutobattler::MessageLogItem* item = LAutobattler::Game::UILogger.stack;
            while (item != nullptr)
            {
                messages += item->text;
                item = item->next;
            }
            item = nullptr;
            
            messageLog->ChangeText(messages);
            MSE_LOG("Message Log text changed!")
        }
    }
    
    mse::gui::Text* messageLog = nullptr;
};

class CharacterUpdateUILayer : public mse::Layer
{
public:
    CharacterUpdateUILayer() : mse::Layer()
    {
        MSE_LOG("Constructed CharacterUpdateUILayer");
    }
    ~CharacterUpdateUILayer()
    {
        MSE_LOG("Destroyed CharacterUpdateUILayer");
    }
    
    virtual void OnInit() override
    {
        mse::gui::Text* text1 = (mse::gui::Text*)(AddElement(new mse::gui::Text(this, U"Победа! Выберите награду.", {100, 10, 200, 10}, {0, 0, 0, 255}, {255, 255, 0, 255})));
        mse::gui::Text* text2 = (mse::gui::Text*)(AddElement(new mse::gui::Text(this, U"Развить класс:", {100, 20, 120, 10}, {0, 0, 0, 255}, {255, 255, 255, 255})));
        
        // pick a subclass
        mse::gui::Button* ClassRogueBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Разбойник", {100, 30, 80, 10}, {196, 100, 100, 255}, {32, 32, 32, 255})));
        ClassRogueBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
            LAutobattler::Game::inputClass = LAutobattler::Classes::Rogue;
        };
        
        mse::gui::Button* ClassWarriorBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Воин", {180, 30, 80, 10}, {100, 196, 196, 255}, {32, 32, 32, 255})));
        ClassWarriorBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
            LAutobattler::Game::inputClass = LAutobattler::Classes::Warrior;
        };
        
        mse::gui::Button* ClassBarbarianBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Варвар", {260, 30, 80, 10}, {196, 196, 100, 255}, {32, 32, 32, 255})));
        ClassBarbarianBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
            LAutobattler::Game::inputClass = LAutobattler::Classes::Barbarian;
        };
        
        // pick a weapon
        mse::gui::Text* text3 = (mse::gui::Text*)(AddElement(new mse::gui::Text(this, U"Выбрать оружие:", {100, 40, 120, 10}, {0, 0, 0, 255}, {255, 255, 255, 255})));
        mse::gui::Button* KeepSameWeaponBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, LAutobattler::Game::playerCharacter.weapon.name, {100, 50, 80, 10}, {196, 100, 100, 255}, {32, 32, 32, 255})));
        KeepSameWeaponBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
            LAutobattler::Game::inputWeapon = LAutobattler::Game::playerCharacter.weapon;
        };
        
        mse::gui::Button* PickDroppedWeaponBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, (*(LAutobattler::Weapon*)(&LAutobattler::Game::npcCharacter.drop_list[0])).name, {180, 50, 80, 10}, {100, 196, 196, 255}, {32, 32, 32, 255})));
        PickDroppedWeaponBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
            LAutobattler::Game::inputWeapon = *(LAutobattler::Weapon*)(&LAutobattler::Game::npcCharacter.drop_list[0]);
        };
        
        // confirm changes
        mse::gui::Button* PlayerCharacterUpdateBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Подтвердить", {100, 70, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
        PlayerCharacterUpdateBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
            LAutobattler::Game::playerCharacterUpdated = true;
            m_window->GetLayerManager()->Detach(this);
        };
        
        // show stats
        std::u32string stats = U"";
        std::u32string main_class = U"";
        std::u32string sub_class = U"";
        switch (LAutobattler::Game::playerCharacter.main_class.type)
        {
        case LAutobattler::Classes::Rogue:
        {
            main_class = U"Разбойник";
            break;
        }
        case LAutobattler::Classes::Warrior:
            {
                main_class = U"Воин";
                break;
            }
        case LAutobattler::Classes::Barbarian:
            {
                main_class = U"Варвар";
                break;
            }
        }
        switch (LAutobattler::Game::playerCharacter.sub_class.type)
        {
        case LAutobattler::Classes::Rogue:
        {
            sub_class = U"Разбойник";
            break;
        }
        case LAutobattler::Classes::Warrior:
            {
                sub_class = U"Воин";
                break;
            }
        case LAutobattler::Classes::Barbarian:
            {
                sub_class = U"Варвар";
                break;
            }
        }
        
        std::stringstream strstream;
        strstream << 
        "Класс:      " << utf8::utf32to8(main_class.c_str()) << 
        "\nПодкласс: " << utf8::utf32to8(sub_class.c_str()) <<
        "\nЗдоровье: " << LAutobattler::Game::playerCharacter.stats.health <<
        "\nСила:     " << LAutobattler::Game::playerCharacter.stats.strength <<
        "\nЛовкость: " << LAutobattler::Game::playerCharacter.stats.agility <<
        "\nВыносливость: " << LAutobattler::Game::playerCharacter.stats.endurance <<
        "\nОружие: " << utf8::utf32to8(LAutobattler::Game::playerCharacter.weapon.name.c_str());
        stats = utf8::utf8to32(strstream.str());
        
        mse::gui::Text* text4 = (mse::gui::Text*)(AddElement(new mse::gui::Text(this, stats, {010, 90, 300, 100}, {0, 64, 0, 255}, {255, 255, 128, 255})));
    }
    
    virtual void OnUpdate() override
    {
        
    }
};

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
		mse::gui::Button* playerCreateBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Новая игра", {10, 10, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
		playerCreateBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
            LAutobattler::Game::gamePage = LAutobattler::GamePages::CharacterCreation;
//			mse::Canban::PutTask(mse::CanbanEvents::Backend_Create, {mse::CanbanEvents::Backend_Create, mse::CanbanReceiver::Backend, nullptr});
//			mse::Canban::PutTask(mse::CanbanEvents::Backend_Run, {mse::CanbanEvents::Backend_Run, mse::CanbanReceiver::Backend, nullptr});
//            mse::Canban::PutTask(mse::CanbanEvents::Backend_Stop, {mse::CanbanEvents::Backend_Stop, mse::CanbanReceiver::Backend, nullptr});
            m_window->GetLayerManager()->Attach(new ClassSelectUILayer());
		};
        
        mse::gui::Button* battleBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Начать бой", {10, 20, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
        battleBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
            LAutobattler::Game::gamePage = LAutobattler::GamePages::ArenaSetup;
            m_window->GetLayerManager()->Attach(new ArenaUILayer());
        };
        
		mse::gui::Button* playerLoadBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Загрузить", {10, 20, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
		playerLoadBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
			mode = LAutobattler::GamePages::Intro;\
            LAutobattler::Game::gamePage = LAutobattler::GamePages::CharacterLoad;
		};
		mse::gui::Button* scoresBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Рекорды", {10, 30, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
		scoresBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
			mode = LAutobattler::GamePages::Highscores;
		};
		mse::gui::Button* creditsBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Титры", {10, 40, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
		creditsBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
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
        LAutobattler::Game::GameLogic();
        
        if (LAutobattler::Game::gamePageHasToChange)
        {
            LAutobattler::Game::gamePageFrom = LAutobattler::Game::gamePage;
            switch (LAutobattler::Game::gamePageTo)
            {
            case LAutobattler::GamePages::MainMenu:
                {
                    LAutobattler::Game::gamePage = LAutobattler::Game::gamePageTo;
                    LAutobattler::Game::gamePageHasToChange = false;
                    break;
                }
            case LAutobattler::GamePages::CharacterUpdate:
                {
                    LAutobattler::Game::gamePage = LAutobattler::Game::gamePageTo;
                    LAutobattler::Game::gamePageHasToChange = false;
                    m_window->GetLayerManager()->Attach(new CharacterUpdateUILayer());
                    break;
                }
            case LAutobattler::GamePages::ArenaSetup:
                {
                    LAutobattler::Game::gamePage = LAutobattler::Game::gamePageTo;
                    LAutobattler::Game::gamePageHasToChange = false;
                    m_window->GetLayerManager()->Attach(new ArenaUILayer());
                    break;
                }
            }
        }
        
		switch (LAutobattler::Game::gamePage)
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

// ********************************************************************************************** //
//                                    STATES (to control gameplay)
// ********************************************************************************************** //

struct IntroPageState : GameState
{
    virtual bool OnEnter() override
    {
        layer = new IntroUILayer();
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
        return true;
    }
    
    virtual bool OnExit() override
    {
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
        layer = nullptr;
        return true;
    }
    
    virtual bool OnUpdate(mse::TimeType t = 0) override
    {
        return true;
    }
    
    mse::Layer* layer = nullptr;
};

struct MainPageState : GameState
{
    virtual bool OnEnter() override
    {
        layer = new SimpleUILayer();
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
        return true;
    }
    virtual bool OnExit() override
    {
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
        layer = nullptr;
        return true;
    }
    virtual bool OnUpdate(mse::TimeType t = 0) override
    {
        return true;
    }
    
    mse::Layer* layer = nullptr;
};

// ********************************************************************************************** //
//                                    SCENES (do I need one?)
// ********************************************************************************************** //

class GameScene : public mse::Scene
{
public:
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
    
    virtual void OnUpdate(mse::TimeType time)
    {
//        LAutobattler::Game::GameLogic();
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
            LAutobattler::Game::keyPressed = true;
			return true;
		};
		mse::Renderer::SetActiveWindow(m_window);
		
		MSE_LOG("Commanding to create a scene");
        
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
//        gsm.states[LAutobattler::GamePages::Exit];
        
        // setup initial game state
        gsm.ChangeStateTo(LAutobattler::GamePages::Intro);
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
