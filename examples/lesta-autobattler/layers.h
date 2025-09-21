#ifndef LESTA_AUTOBATTLER_LAYERS_H
#define LESTA_AUTOBATTLER_LAYERS_H

#include <mse/mse.h>

#include <lesta-autobattler/fwd.h>

// ********************************************************************************************** //
//                                    LAYERS (to render things)
// ********************************************************************************************** //

class IntroUILayer : public mse::Layer
{
public:
    IntroUILayer();
    ~IntroUILayer();
    
    virtual void OnInit() override;
    virtual void OnUpdate() override;
    
    mse::gui::Text* text = nullptr;
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
};

class SimpleUILayer : public mse::Layer
{
public:
    SimpleUILayer();
    ~SimpleUILayer();
    
    virtual void OnInit() override;
    virtual void OnUpdate() override;
    
    mse::gui::Canvas* gameCanvas = nullptr;
};

#endif
