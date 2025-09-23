#include <lesta-autobattler/gamestates.h>
#include <lesta-autobattler/layers.h>
//#include <mse/mse.h>
#include <mse/systems/platform/renderer/renderer.h>
#include <mse/systems/windows/window.h>
#include <mse/systems/windows/layers/layer.h>

GameState::GameState()
{}

GameState::~GameState()
{}

void GameStateMachine::ChangeStateTo(GameState* state)
{
    while (!current->OnExit()){}
    delete current;
    current = state;
    while(!current->OnEnter()){}
}

void GameStateMachine::ChangeStateTo(LAutobattler::GamePages gamePage)
{
    while (!current->OnExit()){}
    delete current;
    current = states[gamePage];
    while(!current->OnEnter()){}
}

void GameStateMachine::OnUpdate(mse::TimeType t)
{
    current->OnUpdate(t);
    if (current->changeTo != LAutobattler::GamePages::None)
    {
        ChangeStateTo(states[current->changeTo]);
    }
}

// ********************************************************************************************** //
//                                    STATES (to control gameplay)
// ********************************************************************************************** //

bool IntroPageState::OnEnter()
{
    layer = new IntroUILayer();
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    return true;
}

bool IntroPageState::OnExit()
{
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    layer = nullptr;
    return true;
}

bool IntroPageState::OnUpdate(mse::TimeType t)
{
    if ((timeLast - t) > 100)
    {
        timeLast = t;
        --counter;
    }
    
    if (counter < 0)
    {
        gsm.ChangeStateTo(LAutobattler::GamePages::MainMenu);
    }
    
    return true;
}

bool MainPageState::OnEnter()
{
    layer = new SimpleUILayer();
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    return true;
}

bool MainPageState::OnExit()
{
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    layer = nullptr;
    return true;
}
bool MainPageState::OnUpdate(mse::TimeType t)
{
    return true;
}
