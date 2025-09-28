#ifndef LESTA_AUTOBATTLER_LAYERS_H
#define LESTA_AUTOBATTLER_LAYERS_H

#include <lesta-autobattler/game-fwd.h>

#include <mse/systems/windows/layers/layer.h>
#include <mse/systems/windows/layers/layer_manager.h>
#include <mse/systems/windows/layers/gui/gui.h>
#include <mse/systems/canban.h>
#include <mse/systems/application/application.h>

// ********************************************************************************************** //
//                                    LAYERS (to render things)
// ********************************************************************************************** //

//namespace LAutobattler
//{
//    struct GameDB;
//    struct Game;
//}
//extern LAutobattler::GameDB gameDB;
//extern LAutobattler::Game game;
//
//struct GameStateMachine;
//extern GameStateMachine gsm;

class IntroUILayer : public mse::Layer
{
public:
    IntroUILayer();
    ~IntroUILayer();
    
    virtual void OnInit() override;
    virtual void OnUpdate() override;
    
    mse::gui::Text* text = nullptr;
    int counterBackup = 0;
};

class MainMenuUILayer : public mse::Layer
{
public:
    MainMenuUILayer();
    ~MainMenuUILayer();
    
    virtual void OnInit() override;
    virtual void OnUpdate() override;
};

class CharacterCreateUILayer : public mse::Layer
{
public:
    CharacterCreateUILayer();
    ~CharacterCreateUILayer();
    
    virtual void OnInit() override;
    virtual void OnUpdate() override;
};

class CharacterLoadUILayer : public mse::Layer
{
public:
    CharacterLoadUILayer();
    ~CharacterLoadUILayer();
    
    virtual void OnInit() override;
    virtual void OnUpdate() override;
};

class CharacterSaveUILayer : public mse::Layer
{
public:
    CharacterSaveUILayer();
    ~CharacterSaveUILayer();
    
    virtual void OnInit() override;
    virtual void OnUpdate() override;
};

class WinnerUILayer : public mse::Layer
{
public:
    WinnerUILayer();
    ~WinnerUILayer();
    
    virtual void OnInit() override;
    virtual void OnUpdate() override;
};

class GameOverUILayer : public mse::Layer
{
public:
    GameOverUILayer();
    ~GameOverUILayer();
    
    virtual void OnInit() override;
    virtual void OnUpdate() override;
};

class HighscoresUILayer : public mse::Layer
{
public:
    HighscoresUILayer();
    ~HighscoresUILayer();
    
    virtual void OnInit() override;
    virtual void OnUpdate() override;
};

class CreditsUILayer : public mse::Layer
{
public:
    CreditsUILayer();
    ~CreditsUILayer();
    
    virtual void OnInit() override;
    virtual void OnUpdate() override;
};

class ExitUILayer : public mse::Layer
{
public:
    ExitUILayer();
    ~ExitUILayer();
    
    virtual void OnInit() override;
    virtual void OnUpdate() override;
};

class ArenaUILayer : public mse::Layer
{
public:
    ArenaUILayer();
    ~ArenaUILayer();
    
    virtual void OnInit() override;
    virtual void OnUpdate() override;
    
    mse::gui::Text* messageLog = nullptr;
};

class CharacterUpdateUILayer : public mse::Layer
{
public:
    CharacterUpdateUILayer();
    ~CharacterUpdateUILayer();
    
    virtual void OnInit() override;
    virtual void OnUpdate() override;
    
    mse::gui::Text* statsBox = nullptr;
    bool changeMade = false;
};

class SimpleUILayer : public mse::Layer
{
public:
    SimpleUILayer();
    ~SimpleUILayer();
    
    virtual void OnInit() override;
    virtual void OnUpdate() override;
};

#endif
