#ifndef LESTA_AUTOBATTLER_GAMESTATES_H
#define LESTA_AUTOBATTLER_GAMESTATES_H

#include <lesta-autobattler/game-fwd.h>

#include <mse/systems/windows/window.h>

// crude state machine to think about
struct GameState
{
    GameState();
    virtual ~GameState();
    
    // layers attachment for GUI and base logic initialization
    virtual bool OnEnter() = 0;
    // laters detachment for GUI, resetting data and cleaning memory
    virtual bool OnExit() = 0;
    // common logic step
    virtual bool OnUpdate(mse::TimeType t = 0) = 0;
    
    // so that the state knows what it is
    LAutobattler::GamePages page = LAutobattler::GamePages::None;
    // so that the state can tell where it should move to
    LAutobattler::GamePages changeTo = LAutobattler::GamePages::None;
};

struct GameStateMachine
{
    void ChangeStateTo(LAutobattler::GamePages gamePage);
    void OnUpdate(mse::TimeType t = 0);
    
    std::map<LAutobattler::GamePages, GameState*> states = {};
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
    virtual bool OnEnter() override;
    virtual bool OnExit() override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
    
    mse::Layer* layer = nullptr;
    int counter = 10;
    mse::TimeType timeLast = 0;
};

struct MainPageState : GameState
{
    MainPageState();
    ~MainPageState();
    virtual bool OnEnter() override;
    virtual bool OnExit() override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
    
    mse::Layer* layer = nullptr;
};

struct CharacterCreatePageState : GameState
{
    CharacterCreatePageState();
    ~CharacterCreatePageState();
    virtual bool OnEnter() override;
    virtual bool OnExit() override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
    
    mse::Layer* layer = nullptr;
};

struct CharacterLoadPageState : GameState
{
    CharacterLoadPageState();
    ~CharacterLoadPageState();
    virtual bool OnEnter() override;
    virtual bool OnExit() override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
    
    mse::Layer* layer = nullptr;
};

struct CharacterSavePageState : GameState
{
    CharacterSavePageState();
    ~CharacterSavePageState();
    virtual bool OnEnter() override;
    virtual bool OnExit() override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
    
    mse::Layer* layer = nullptr;
};

struct CharacterUpdatePageState : GameState
{
    CharacterUpdatePageState();
    ~CharacterUpdatePageState();
    virtual bool OnEnter() override;
    virtual bool OnExit() override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
    
    mse::Layer* layer = nullptr;
};

struct ArenaSetupPageState : GameState
{
    ArenaSetupPageState();
    ~ArenaSetupPageState();
    virtual bool OnEnter() override;
    virtual bool OnExit() override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
    
    mse::Layer* layer = nullptr;
};

struct ArenaBattlePageState : GameState
{
    ArenaBattlePageState();
    ~ArenaBattlePageState();
    virtual bool OnEnter() override;
    virtual bool OnExit() override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
    
    mse::Layer* layer = nullptr;
};

struct ArenaAftermathPageState : GameState
{
    ArenaAftermathPageState();
    ~ArenaAftermathPageState();
    virtual bool OnEnter() override;
    virtual bool OnExit() override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
    
    mse::Layer* layer = nullptr;
};

struct WinnerPageState : GameState
{
    WinnerPageState();
    ~WinnerPageState();
    virtual bool OnEnter() override;
    virtual bool OnExit() override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
    
    mse::Layer* layer = nullptr;
};

struct GameOverPageState : GameState
{
    GameOverPageState();
    ~GameOverPageState();
    virtual bool OnEnter() override;
    virtual bool OnExit() override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
    
    mse::Layer* layer = nullptr;
};

struct HighscoresPageState : GameState
{
    HighscoresPageState();
    ~HighscoresPageState();
    virtual bool OnEnter() override;
    virtual bool OnExit() override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
    
    mse::Layer* layer = nullptr;
};

struct CreditsPageState : GameState
{
    CreditsPageState();
    ~CreditsPageState();
    virtual bool OnEnter() override;
    virtual bool OnExit() override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
    
    mse::Layer* layer = nullptr;
};

struct ExitPageState : GameState
{
    ExitPageState();
    ~ExitPageState();
    virtual bool OnEnter() override;
    virtual bool OnExit() override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
    
    mse::Layer* layer = nullptr;
};

#endif
