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

void GameStateMachine::ChangeStateTo(LAutobattler::GamePages gamePage)
{
    MSE_LOG("GameStateMachine: change state to ", (int)gamePage);
    if (states.find(gamePage) != states.end())
    {
        MSE_LOG("GameStateMachine: state ", (int)gamePage, " found");
        if (current != nullptr)
        {
            while (!current->OnExit()){}
            MSE_LOG("GameStateMachine: state ", (int)(current->page), " exited");
            delete current;
        }
        current = states[gamePage];
        while(!current->OnEnter()){}
        MSE_LOG("GameStateMachine: state ", (int)(current->page), " entered");
    } else {
        MSE_LOG("GameStateMachine: cannot change to state ", (int)gamePage, " - none found");
    }
}

void GameStateMachine::OnUpdate(mse::TimeType t)
{
    current->OnUpdate(t);
    if (current->changeTo != LAutobattler::GamePages::None)
    {
        ChangeStateTo(current->changeTo);
    }
}

// ********************************************************************************************** //
//                                    STATES (to control gameplay)
// ********************************************************************************************** //

bool IntroPageState::OnEnter()
{
    MSE_LOG("IntroPageState OnEnter...");
    layer = new IntroUILayer();
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    MSE_LOG("IntroPageState OnEnter...done");
    return true;
}

bool IntroPageState::OnExit()
{
    MSE_LOG("IntroPageState OnExit...");
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
    layer = nullptr;
    MSE_LOG("IntroPageState OnExit...done");
    return true;
}

bool IntroPageState::OnUpdate(mse::TimeType t)
{
    MSE_LOG("IntroPageState OnUpdate...");
    MSE_LOG((int)(timeLast - t));
    if ((timeLast - t) > 100)
    {
        timeLast = t;
        --counter;
    }
    
    if (counter < 0)
    {
        gsm.ChangeStateTo(LAutobattler::GamePages::MainMenu);
    }
    
    MSE_LOG("IntroPageState OnUpdate...done");
    return true;
}

bool MainPageState::OnEnter()
{
    MSE_LOG("MainPageState OnEnter...");
    layer = new SimpleUILayer();
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    MSE_LOG("MainPageState OnEnter...done");
    return true;
}

bool MainPageState::OnExit()
{
    MSE_LOG("MainPageState OnExit...");
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
    layer = nullptr;
    MSE_LOG("MainPageState OnExit...done");
    return true;
}
bool MainPageState::OnUpdate(mse::TimeType t)
{
    MSE_LOG("MainPageState OnUpdate...");
    MSE_LOG("MainPageState OnUpdate...done");
    return true;
}
