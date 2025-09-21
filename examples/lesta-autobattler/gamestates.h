#ifndef LESTA_AUTOBATTLER_GAMESTATES_H
#define LESTA_AUTOBATTLER_GAMESTATES_H

#include <mse/mse.h>
#include <lesta-autobattler/fwd.h>

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
    void ChangeStateTo(GameState* state);
    void ChangeStateTo(LAutobattler::GamePages gamePage);
    void OnUpdate(mse::TimeType t = 0);
    
    std::map<LAutobattler::GamePages, GameState*> states = {};
    GameState* current;
};

GameStateMachine gsm;

// ********************************************************************************************** //
//                                    STATES (to control gameplay)
// ********************************************************************************************** //

struct IntroPageState : GameState
{
    virtual bool OnEnter() override;
    virtual bool OnExit() override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
    
    mse::Layer* layer = nullptr;
    int counter = 10;
    mse::TimeType timeLast = 0;
};

struct MainPageState : GameState
{
    virtual bool OnEnter() override;
    virtual bool OnExit() override;
    virtual bool OnUpdate(mse::TimeType t = 0) override;
    
    mse::Layer* layer = nullptr;
};


#endif
