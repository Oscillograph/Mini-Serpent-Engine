#ifndef DUNGEONTRIS_AUTOBATTLER_GAMESTATES_H
#define DUNGEONTRIS_AUTOBATTLER_GAMESTATES_H

#include <dungeontris/game-fwd.h>

#include <mse/systems/windows/window.h>

// crude state machine to think about
struct GameState
{
    GameState();
    virtual ~GameState();
    
    // layers attachment for GUI and base logic initialization
    virtual bool OnEnter(mse::Layer* pass_layer = nullptr) = 0;
    // laters detachment for GUI, resetting data and cleaning memory
    virtual bool OnExit(bool pass_layer = false) = 0;
    // common logic step
    virtual bool OnUpdate(mse::TimeType t = 0) = 0;
    
    // so that the state knows what it is
    DTetris::GamePages page = DTetris::GamePages::None;
    // so that the state can tell where it should move to
    DTetris::GamePages changeTo = DTetris::GamePages::None;
    // so that the states can pass corresponding layers to one another
    mse::Layer* layer = nullptr;
};

struct GameStateMachine
{
    void ChangeStateTo(DTetris::GamePages gamePage, mse::Layer* pass_layer = nullptr);
    void OnUpdate(mse::TimeType t = 0);
    
    std::map<DTetris::GamePages, GameState*> states = {};
    GameState* current;
};

//GameStateMachine gsm;

// ********************************************************************************************** //
//                                    STATES (to control gameplay)
// ********************************************************************************************** //

struct IntroPageState : GameState
{
    IntroPageState();
    ~IntroPageState();
    virtual bool OnEnter(mse::Layer* pass_layer = nullptr) override;
    virtual bool OnExit(bool pass_layer = false) override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
    
    int counter = 10;
    mse::TimeType timeLast = 0;
};

struct MainPageState : GameState
{
    MainPageState();
    ~MainPageState();
    virtual bool OnEnter(mse::Layer* pass_layer = nullptr) override;
    virtual bool OnExit(bool pass_layer = false) override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
};

struct CharacterCreatePageState : GameState
{
    CharacterCreatePageState();
    ~CharacterCreatePageState();
    virtual bool OnEnter(mse::Layer* pass_layer = nullptr) override;
    virtual bool OnExit(bool pass_layer = false) override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
};

struct CharacterLoadPageState : GameState
{
    CharacterLoadPageState();
    ~CharacterLoadPageState();
    virtual bool OnEnter(mse::Layer* pass_layer = nullptr) override;
    virtual bool OnExit(bool pass_layer = false) override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
};

struct CharacterSavePageState : GameState
{
    CharacterSavePageState();
    ~CharacterSavePageState();
    virtual bool OnEnter(mse::Layer* pass_layer = nullptr) override;
    virtual bool OnExit(bool pass_layer = false) override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
};

struct CharacterUpdatePageState : GameState
{
    CharacterUpdatePageState();
    ~CharacterUpdatePageState();
    virtual bool OnEnter(mse::Layer* pass_layer = nullptr) override;
    virtual bool OnExit(bool pass_layer = false) override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
};

struct ArenaSetupPageState : GameState
{
    ArenaSetupPageState();
    ~ArenaSetupPageState();
    virtual bool OnEnter(mse::Layer* pass_layer = nullptr) override;
    virtual bool OnExit(bool pass_layer = false) override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
};

struct ArenaBattlePageState : GameState
{
    ArenaBattlePageState();
    ~ArenaBattlePageState();
    virtual bool OnEnter(mse::Layer* pass_layer = nullptr) override;
    virtual bool OnExit(bool pass_layer = false) override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
};

struct ArenaAftermathPageState : GameState
{
    ArenaAftermathPageState();
    ~ArenaAftermathPageState();
    virtual bool OnEnter(mse::Layer* pass_layer = nullptr) override;
    virtual bool OnExit(bool pass_layer = false) override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
    
    mse::TimeType localTime = 0;
};

struct WinnerPageState : GameState
{
    WinnerPageState();
    ~WinnerPageState();
    virtual bool OnEnter(mse::Layer* pass_layer = nullptr) override;
    virtual bool OnExit(bool pass_layer = false) override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
    
    mse::TimeType current_time = 0;
};

struct GameOverPageState : GameState
{
    GameOverPageState();
    ~GameOverPageState();
    virtual bool OnEnter(mse::Layer* pass_layer = nullptr) override;
    virtual bool OnExit(bool pass_layer = false) override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
};

struct HighscoresPageState : GameState
{
    HighscoresPageState();
    ~HighscoresPageState();
    virtual bool OnEnter(mse::Layer* pass_layer = nullptr) override;
    virtual bool OnExit(bool pass_layer = false) override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
};

struct SettingsPageState : GameState
{
    SettingsPageState();
    ~SettingsPageState();
    virtual bool OnEnter(mse::Layer* pass_layer = nullptr) override;
    virtual bool OnExit(bool pass_layer = false) override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
};

struct CreditsPageState : GameState
{
    CreditsPageState();
    ~CreditsPageState();
    virtual bool OnEnter(mse::Layer* pass_layer = nullptr) override;
    virtual bool OnExit(bool pass_layer = false) override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
};

struct ExitPageState : GameState
{
    ExitPageState();
    ~ExitPageState();
    virtual bool OnEnter(mse::Layer* pass_layer = nullptr) override;
    virtual bool OnExit(bool pass_layer = false) override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
};

#endif
