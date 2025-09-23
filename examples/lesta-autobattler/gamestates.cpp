#include <lesta-autobattler/gamestates.h>
#include <lesta-autobattler/layers.h>
//#include <mse/mse.h>
#include <mse/systems/platform/renderer/renderer.h>
#include <mse/systems/application/application.h>
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
    MSE_LOG((int)(t));
    MSE_LOG((int)(1.0/MSE_FPS60));
    if ((timeLast - t) > 10*(int)(1.0/MSE_FPS60))
    {
        timeLast = 0;
        --counter;
    }
    timeLast += t;
    
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
    layer = new MainMenuUILayer();
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

bool CharacterCreatePageState::OnEnter()
{
    layer = new CharacterCreateUILayer();
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    return true;
}

bool CharacterCreatePageState::OnExit()
{
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
    layer = nullptr;
    return true;
}

bool CharacterCreatePageState::OnUpdate(mse::TimeType t)
{
    return true;
}

bool CharacterLoadPageState::OnEnter()
{
    layer = new CharacterLoadUILayer();
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    return true;
}

bool CharacterLoadPageState::OnExit()
{
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
    layer = nullptr;
    return true;
}

bool CharacterLoadPageState::OnUpdate(mse::TimeType t)
{
    return true;
}

bool CharacterSavePageState::OnEnter()
{
    layer = new CharacterSaveUILayer();
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    return true;
}

bool CharacterSavePageState::OnExit()
{
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
    layer = nullptr;
    return true;
}

bool CharacterSavePageState::OnUpdate(mse::TimeType t)
{
    return true;
}

bool CharacterUpdatePageState::OnEnter()
{
    layer = new CharacterUpdateUILayer();
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    return true;
}

bool CharacterUpdatePageState::OnExit()
{
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
    layer = nullptr;
    return true;
}

bool CharacterUpdatePageState::OnUpdate(mse::TimeType t)
{
    return true;
}

bool ArenaSetupPageState::OnEnter()
{
    layer = new ArenaUILayer();
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    return true;
}

bool ArenaSetupPageState::OnExit()
{
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
    layer = nullptr;
    return true;
}

bool ArenaSetupPageState::OnUpdate(mse::TimeType t)
{
    return true;
}

bool ArenaBattlePageState::OnEnter()
{
    layer = new ArenaUILayer();
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    return true;
}

bool ArenaBattlePageState::OnExit()
{
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
    layer = nullptr;
    return true;
}

bool ArenaBattlePageState::OnUpdate(mse::TimeType t)
{
    return true;
}

bool ArenaAftermathPageState::OnEnter()
{
    layer = new ArenaUILayer();
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    return true;
}

bool ArenaAftermathPageState::OnExit()
{
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
    layer = nullptr;
    return true;
}

bool ArenaAftermathPageState::OnUpdate(mse::TimeType t)
{
    return true;
}

bool WinnerPageState::OnEnter()
{
    layer = new WinnerUILayer();
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    return true;
}

bool WinnerPageState::OnExit()
{
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
    layer = nullptr;
    return true;
}

bool WinnerPageState::OnUpdate(mse::TimeType t)
{
    return true;
}

bool GameOverPageState::OnEnter()
{
    layer = new GameOverUILayer();
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    return true;
}

bool GameOverPageState::OnExit()
{
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
    layer = nullptr;
    return true;
}

bool GameOverPageState::OnUpdate(mse::TimeType t)
{
    return true;
}

bool HighscoresPageState::OnEnter()
{
    layer = new HighscoresUILayer();
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    return true;
}

bool HighscoresPageState::OnExit()
{
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
    layer = nullptr;
    return true;
}

bool HighscoresPageState::OnUpdate(mse::TimeType t)
{
    return true;
}

bool CreditsPageState::OnEnter()
{
    layer = new CreditsUILayer();
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    return true;
}

bool CreditsPageState::OnExit()
{
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
    layer = nullptr;
    return true;
}

bool CreditsPageState::OnUpdate(mse::TimeType t)
{
    return true;
}

bool ExitPageState::OnEnter()
{
    layer = new ExitUILayer();
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    return true;
}

bool ExitPageState::OnExit()
{
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
    layer = nullptr;
    return true;
}

bool ExitPageState::OnUpdate(mse::TimeType t)
{
    mse::Application::GetApplication()->Stop();
    return true;
}
