#ifndef LAYERS_CPP
#define LAYERS_CPP

#include <lesta-autobattler/game-fwd.h>
#include <lesta-autobattler/layers.h>
#include <lesta-autobattler/gamestates.h>

extern LAutobattler::GameDB gameDB;
extern LAutobattler::Game game;
extern GameStateMachine gsm;

IntroUILayer::IntroUILayer() : mse::Layer()
{
    MSE_LOG("Constructed IntroUILayer");
}

IntroUILayer::~IntroUILayer()
{
    MSE_LOG("Destroyed IntroUILayer");
    text = nullptr;
}

void IntroUILayer::OnInit()
{
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"LESTA АВТОБАТЛЕР", 
                                  {40, 10, 225, 20}, 
                                  {64, 0, 0, 255}, 
                                  {0, 255, 255, 255},
                                  2));
    
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"by Broscillograph", 
                                  {40, 30, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    
    text = (mse::gui::Text*)(AddElement(new mse::gui::Text(
                                                           this, 
                                                           U"Загрузится через: ", 
                                                           {120, 50, 200, 10}, 
                                                           {0, 0, 0, 255}, 
                                                           {0, 255, 255, 255})));
}

void IntroUILayer::OnUpdate()
{
//    if (counterBackup != ((IntroPageState*)gsm.current)->counter)
//    {
//        int counterBackup = ((IntroPageState*)gsm.current)->counter;
//        
//        std::stringstream strstream;
//        strstream << "Загрузится через: " << counterBackup;
//        std::u32string newText = utf8::utf8to32(strstream.str());
//        
//        text->ChangeText(newText);
//        
//        if (counterBackup < 0)
//        {
//            gsm.ChangeStateTo(LAutobattler::GamePages::MainMenu);
//        }
//    }
    gsm.ChangeStateTo(LAutobattler::GamePages::MainMenu);
}

MainMenuUILayer::MainMenuUILayer() : mse::Layer()
{
    MSE_LOG("Constructed MainMenuUILayer");
}
MainMenuUILayer::~MainMenuUILayer()
{
    MSE_LOG("Destroyed MainMenuUILayer");
}

void MainMenuUILayer::OnInit()
{
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"LESTA АВТОБАТЛЕР", 
                                  {40, 10, 225, 20}, 
                                  {64, 0, 0, 255}, 
                                  {0, 255, 255, 255},
                                  2));
    
    mse::gui::Button* playerCreateBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(
                                                                                            this,
                                                                                            U" Новая  игра",
                                                                                            {0, 0, 0, 255},
                                                                                            {120, 40, 100, 13},
                                                                                            "./data/img/screen-images.png",
                                                                                            {122, 101, 4, 13},
                                                                                            {138, 101, 4, 13},
                                                                                            {154, 101, 4, 13}
                                                                                            )));
    playerCreateBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        gsm.ChangeStateTo(LAutobattler::GamePages::CharacterCreation);
    };
    
    mse::gui::Button* battleBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(
                                                                                      this,
                                                                                      U"  Загрузить",
                                                                                      {0, 0, 0, 255},
                                                                                      {120, 54, 100, 13},
                                                                                      "./data/img/screen-images.png",
                                                                                      {122, 101, 4, 13},
                                                                                      {138, 101, 4, 13},
                                                                                      {154, 101, 4, 13}
                                                                                      )));
    battleBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        gsm.ChangeStateTo(LAutobattler::GamePages::CharacterLoad);
    };
    
    mse::gui::Button* scoresBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(
                                                                                      this,
                                                                                      U"   Рекорды",
                                                                                      {0, 0, 0, 255},
                                                                                      {120, 68, 100, 13},
                                                                                      "./data/img/screen-images.png",
                                                                                      {122, 101, 4, 13},
                                                                                      {138, 101, 4, 13},
                                                                                      {154, 101, 4, 13}
                                                                                      )));
    scoresBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        gsm.ChangeStateTo(LAutobattler::GamePages::Highscores);
    };

    mse::gui::Button* creditsBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(
                                                                                       this,
                                                                                       U"    Титры",
                                                                                       {0, 0, 0, 255},
                                                                                       {120, 82, 100, 13},
                                                                                       "./data/img/screen-images.png",
                                                                                       {122, 101, 4, 13},
                                                                                       {138, 101, 4, 13},
                                                                                       {154, 101, 4, 13}
                                                                                       )));
    creditsBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        gsm.ChangeStateTo(LAutobattler::GamePages::Credits);
    };
    
    mse::gui::Button* exitBtn = (mse::gui::Button*)(AddElement(
                                                               new mse::gui::Button(
                                                                                    this,
                                                                                    U"    Выход",
                                                                                    {0, 0, 0, 255},
                                                                                    {120, 96, 100, 13},
                                                                                    "./data/img/screen-images.png",
                                                                                    {122, 101, 4, 13},
                                                                                    {138, 101, 4, 13},
                                                                                    {154, 101, 4, 13}
                                                                                    )));
    exitBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        gsm.ChangeStateTo(LAutobattler::GamePages::Exit);
    };
    
    mse::gui::Image* backgroundImage = (mse::gui::Image*)(AddElement(new mse::gui::Image(this, {0, 0, 320, 240}, "./data/img/background.png", {0, 0, 320, 240}, {0, 0, 0, 255})));
}

void MainMenuUILayer::OnUpdate()
{}

CharacterCreateUILayer::CharacterCreateUILayer() : mse::Layer()
{
    MSE_LOG("Constructed CharacterCreateUILayer");
}
CharacterCreateUILayer::~CharacterCreateUILayer()
{
    MSE_LOG("Destroyed CharacterCreateUILayer");
}

void CharacterCreateUILayer::OnInit()
{
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"LESTA АВТОБАТЛЕР", 
                                  {40, 10, 225, 20}, 
                                  {64, 0, 0, 255}, 
                                  {0, 255, 255, 255},
                                  2));
    
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Выбор класса:", 
                                  {80, 40, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 0, 255},
                                  1));
    
    AddElement(new mse::gui::Image(this, {40, 100, 34, 64}, "./data/img/heroes.png", {102, 0, 34, 64}, {0, 0, 0, 255}));
    
    mse::gui::Button* ClassRogueBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U" Разбойник", {32, 32, 32, 255}, {20, 170, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    ClassRogueBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.inputClass = LAutobattler::Classes::Rogue;
    };
    
    AddElement(new mse::gui::Image(this, {140, 100, 34, 64}, "./data/img/heroes.png", {0, 128, 34, 64}, {0, 0, 0, 255}));
    
    mse::gui::Button* ClassWarriorBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"   Воин", {32, 32, 32, 255}, {120, 170, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    ClassWarriorBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.inputClass = LAutobattler::Classes::Warrior;
    };
    
    AddElement(new mse::gui::Image(this, {240, 100, 34, 64}, "./data/img/heroes.png", {68, 64, 34, 64}, {0, 0, 0, 255}));
    
    mse::gui::Button* ClassBarbarianBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"  Варвар", {32, 32, 32, 255}, {220, 170, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    ClassBarbarianBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.inputClass = LAutobattler::Classes::Barbarian;
    };
    
    mse::gui::Button* BackToMainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"<< Назад", {32, 32, 32, 255}, {120, 190, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    BackToMainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        gsm.ChangeStateTo(LAutobattler::GamePages::MainMenu);
    };
    
    mse::gui::Image* backgroundImage = (mse::gui::Image*)(AddElement(new mse::gui::Image(this, {0, 0, 320, 240}, "./data/img/background.png", {0, 0, 320, 240}, {0, 0, 0, 255})));
}

void CharacterCreateUILayer::OnUpdate()
{}

CharacterLoadUILayer::CharacterLoadUILayer() : mse::Layer()
{
    MSE_LOG("Constructed CharacterLoadUILayer");
}
CharacterLoadUILayer::~CharacterLoadUILayer()
{
    MSE_LOG("Destroyed CharacterLoadUILayer");
}

void CharacterLoadUILayer::OnInit()
{}

void CharacterLoadUILayer::OnUpdate()
{}

CharacterSaveUILayer::CharacterSaveUILayer() : mse::Layer()
{
    MSE_LOG("Constructed CharacterSaveUILayer");
}
CharacterSaveUILayer::~CharacterSaveUILayer()
{
    MSE_LOG("Destroyed CharacterSaveUILayer");
}

void CharacterSaveUILayer::OnInit()
{}

void CharacterSaveUILayer::OnUpdate()
{}

WinnerUILayer::WinnerUILayer() : mse::Layer()
{
    MSE_LOG("Constructed WinnerUILayer");
}
WinnerUILayer::~WinnerUILayer()
{
    MSE_LOG("Destroyed WinnerUILayer");
}

void WinnerUILayer::OnInit()
{
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"LESTA АВТОБАТЛЕР", 
                                  {40, 10, 225, 20}, 
                                  {64, 0, 0, 255}, 
                                  {0, 255, 255, 255},
                                  2));
    
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Победа!", 
                                  {110, 80, 120, 40}, 
                                  {0, 0, 0, 255}, 
                                  {64, 255, 64, 255},
                                  2));
    if (game.battleCounter < 5)
    {
        AddElement(new mse::gui::Text(
                                      this, 
                                      U"В битве было нелегко, но герой справился и одолел противника.\nВпереди новое испытание!", 
                                      {10, 160, 220, 40}, 
                                      {0, 0, 0, 255}, 
                                      {196, 196, 196, 255},
                                      1));
        
        mse::gui::Button* mainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"К наградам", {120, 220, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
        mainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
            gsm.ChangeStateTo(LAutobattler::GamePages::CharacterUpdate);
        };
    } else {
        AddElement(new mse::gui::Text(
                                      this, 
                                      U"Герой покорил арену, одолев \nвсе трудности и опасности. \nЕго удали нет равных, и теперь, по праву чемпиона, он войдёт \nв Зал Славы как живая легенда.\nЭто было великолепно!", 
                                      {10, 140, 220, 60}, 
                                      {0, 0, 0, 255}, 
                                      {196, 196, 196, 255},
                                      1));
        
        mse::gui::Button* mainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"В Зал Славы", {120, 220, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
        mainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
            gsm.ChangeStateTo(LAutobattler::GamePages::Highscores);
        };
    }
    
}

void WinnerUILayer::OnUpdate()
{}

GameOverUILayer::GameOverUILayer() : mse::Layer()
{
    MSE_LOG("Constructed GameOverUILayer");
}
GameOverUILayer::~GameOverUILayer()
{
    MSE_LOG("Destroyed GameOverUILayer");
}

void GameOverUILayer::OnInit()
{
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"LESTA АВТОБАТЛЕР", 
                                  {40, 10, 225, 20}, 
                                  {64, 0, 0, 255}, 
                                  {0, 255, 255, 255},
                                  2));
    
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Проигрыш", 
                                  {110, 80, 120, 40}, 
                                  {0, 0, 0, 255}, 
                                  {255, 64, 64, 255},
                                  2));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Герой старался изо всех сил, но противник превзошёл его.\nУдачи в следующий раз!", 
                                  {10, 160, 220, 40}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    
    mse::gui::Button* mainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"В меню", {120, 220, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
    mainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        gsm.ChangeStateTo(LAutobattler::GamePages::MainMenu);
    };
}

void GameOverUILayer::OnUpdate()
{}

HighscoresUILayer::HighscoresUILayer() : mse::Layer()
{
    MSE_LOG("Constructed HighscoresUILayer");
}
HighscoresUILayer::~HighscoresUILayer()
{
    MSE_LOG("Destroyed HighscoresUILayer");
}

void HighscoresUILayer::OnInit()
{}

void HighscoresUILayer::OnUpdate()
{}

CreditsUILayer::CreditsUILayer() : mse::Layer()
{
    MSE_LOG("Constructed CreditsUILayer");
}
CreditsUILayer::~CreditsUILayer()
{
    MSE_LOG("Destroyed CreditsUILayer");
}

void CreditsUILayer::OnInit()
{
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"LESTA АВТОБАТЛЕР", 
                                  {40, 10, 225, 20}, 
                                  {64, 0, 0, 255}, 
                                  {0, 255, 255, 255},
                                  2));
    
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Программирование:", 
                                  {40, 30, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 0, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Broscillograph", 
                                  {80, 40, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Графика:", 
                                  {40, 50, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 0, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Broscillograph", 
                                  {80, 60, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Компилятор:", 
                                  {40, 70, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 0, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"TDM-GCC", 
                                  {80, 80, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Библиотеки:", 
                                  {40, 90, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 0, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"SDL2, utf-cpp, Color Console", 
                                  {80, 100, 200, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Среда разработки:", 
                                  {40, 110, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 0, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Red Panda C++", 
                                  {80, 120, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Музыка:", 
                                  {40, 130, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 0, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Трио из Удио", 
                                  {80, 140, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Место для вашей рекламы:", 
                                  {40, 150, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 0, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"", 
                                  {80, 160, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Юля, я люблю тебя:", 
                                  {40, 170, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 128, 196, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Broscillograph", 
                                  {80, 180, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Оригинальный диздок:", 
                                  {40, 190, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 0, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Lesta Игры", 
                                  {80, 200, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    
    mse::gui::Button* mainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"<< Назад", {32, 32, 32, 255}, {120, 220, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    mainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        gsm.ChangeStateTo(LAutobattler::GamePages::MainMenu);
    };
    
    mse::gui::Image* backgroundImage = (mse::gui::Image*)(AddElement(new mse::gui::Image(this, {0, 0, 320, 240}, "./data/img/background.png", {0, 0, 320, 240}, {0, 0, 0, 255})));
}

void CreditsUILayer::OnUpdate()
{}

ExitUILayer::ExitUILayer() : mse::Layer()
{
    MSE_LOG("Constructed ExitUILayer");
}
ExitUILayer::~ExitUILayer()
{
    MSE_LOG("Destroyed ExitUILayer");
}

void ExitUILayer::OnInit()
{}

void ExitUILayer::OnUpdate()
{}

ArenaUILayer::ArenaUILayer() : mse::Layer()
{
    MSE_LOG("Constructed ArenaUILayer");
}
ArenaUILayer::~ArenaUILayer()
{
    messageLog = nullptr;
    
    int i = 0;
    game.UILogger.Clear();
    
    MSE_LOG("Destroyed ArenaUILayer");
}

void ArenaUILayer::OnInit()
{
    messageLog = (mse::gui::Text*)(AddElement(new mse::gui::Text(
                                                                 this, 
                                                                 U"", 
                                                                 {10, 90, 300, 100}, 
                                                                 {0, 64, 0, 255}, 
                                                                 {255, 255, 0, 255})));
    messageLog->showBorder = true;
}

void ArenaUILayer::OnUpdate()
{
    std::u32string messages = U"";
    LAutobattler::MessageLogItem* item = game.UILogger.stack;
    static bool needToUpdateText = false;
    while (item != nullptr)
    {
        needToUpdateText = true;
        messages += item->text;
        item = item->next;
    }
    item = nullptr;
    if (needToUpdateText)
    {
        messageLog->ChangeText(messages);
        MSE_LOG("Message Log text changed!")
    }
    needToUpdateText = false;
}

CharacterUpdateUILayer::CharacterUpdateUILayer() : mse::Layer()
{
    MSE_LOG("Constructed CharacterUpdateUILayer");
}
CharacterUpdateUILayer::~CharacterUpdateUILayer()
{
    MSE_LOG("Destroyed CharacterUpdateUILayer");
}

void CharacterUpdateUILayer::OnInit()
{
    mse::gui::Text* text1 = (mse::gui::Text*)(AddElement(new mse::gui::Text(this, U"Победа! Выберите награду.", {60, 10, 200, 10}, {0, 0, 0, 255}, {255, 255, 0, 255})));
    mse::gui::Text* text2 = (mse::gui::Text*)(AddElement(new mse::gui::Text(this, U"Развить класс:", {60, 20, 120, 10}, {0, 0, 0, 255}, {255, 255, 255, 255})));
    
    // pick a subclass
    mse::gui::Button* ClassRogueBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Разбойник", {60, 30, 80, 10}, {196, 100, 100, 255}, {32, 32, 32, 255})));
    ClassRogueBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.inputClass = LAutobattler::Classes::Rogue;
        changeMade = true;
    };
    
    mse::gui::Button* ClassWarriorBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Воин", {150, 30, 80, 10}, {100, 196, 196, 255}, {32, 32, 32, 255})));
    ClassWarriorBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.inputClass = LAutobattler::Classes::Warrior;
        changeMade = true;
    };
    
    mse::gui::Button* ClassBarbarianBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Варвар", {240, 30, 80, 10}, {196, 196, 100, 255}, {32, 32, 32, 255})));
    ClassBarbarianBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.inputClass = LAutobattler::Classes::Barbarian;
        changeMade = true;
    };
    
    // pick a weapon
    mse::gui::Text* text3 = (mse::gui::Text*)(AddElement(new mse::gui::Text(this, U"Выбрать оружие:", {100, 40, 120, 10}, {0, 0, 0, 255}, {255, 255, 255, 255})));
    mse::gui::Button* KeepSameWeaponBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, game.playerCharacter.weapon.name, {60, 50, 120, 10}, {196, 100, 100, 255}, {32, 32, 32, 255})));
    KeepSameWeaponBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.inputWeapon = game.playerCharacter.weapon;
        changeMade = true;
    };
    
    mse::gui::Button* PickDroppedWeaponBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, (*(LAutobattler::Weapon*)(&game.npcCharacter.drop_list[0])).name, {180, 50, 120, 10}, {100, 196, 196, 255}, {32, 32, 32, 255})));
    PickDroppedWeaponBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        LAutobattler::Weapon* weapon = &(game.npcCharacter.drop_list[0]);
        game.inputWeapon.name = weapon->name;
        game.inputWeapon.type = weapon->type;
        game.inputWeapon.damage = weapon->damage;
        changeMade = true;
    };
    
    // confirm changes
    mse::gui::Button* PlayerCharacterUpdateBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Подтвердить", {160, 70, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
    PlayerCharacterUpdateBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.playerCharacterUpdated = true;
    };
    
    // defaults
    game.inputClass = game.playerCharacter.main_class.type;
    game.inputWeapon = game.playerCharacter.weapon;
    
    // show stats
    statsBox = (mse::gui::Text*)(AddElement(new mse::gui::Text(this, U"", {010, 90, 300, 100}, {0, 64, 0, 255}, {255, 255, 128, 255})));
    changeMade = true;
}

void CharacterUpdateUILayer::OnUpdate()
{
    if (changeMade)
    {
        std::u32string stats = U"";
        std::u32string main_class = U"";
        std::u32string sub_class = U"";
        
        switch (game.playerCharacter.main_class.type)
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
        
        if (game.inputClass != game.playerCharacter.main_class.type)
        {
            switch (game.inputClass)
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
        }
        
        // calculate stats for display
        game.playerCharacter.stats = 
        {
            game.playerCharacter.level * (game.playerCharacter.stats_max.health + game.playerCharacter.stats_max.endurance),
            game.playerCharacter.stats_max.strength,
            game.playerCharacter.stats_max.agility,
            game.playerCharacter.stats_max.endurance,
        };
        
        for (LAutobattler::Traits trait : game.playerCharacter.traits)
        {
            if (trait == LAutobattler::Traits::Strong)
            {
                game.playerCharacter.stats.strength++;
            }
            if (trait == LAutobattler::Traits::Agile)
            {
                game.playerCharacter.stats.agility++;
            }
            if (trait == LAutobattler::Traits::Survivor)
            {
                game.playerCharacter.stats.endurance++;
                game.playerCharacter.stats.health = game.playerCharacter.level * (game.playerCharacter.stats_max.health + game.playerCharacter.stats.endurance);
            }
        }
        
        std::stringstream strstream;
        strstream << 
        "Уровень:    " << game.playerCharacter.level <<
        "\nКласс:      " << utf8::utf32to8(main_class.c_str()) << 
        "\nПодкласс: " << utf8::utf32to8(sub_class.c_str()) <<
        "\nЗдоровье: " << game.playerCharacter.stats.health <<
        "\nСила:     " << game.playerCharacter.stats.strength <<
        "\nЛовкость: " << game.playerCharacter.stats.agility <<
        "\nВыносливость: " << game.playerCharacter.stats.endurance <<
        "\nОружие: " << utf8::utf32to8(game.inputWeapon.name.c_str()) << 
                   " (" << (int)(roundf(game.inputWeapon.damage)) << ")";
        stats = utf8::utf8to32(strstream.str());
        statsBox->ChangeText(stats);
        
        changeMade = false;
    }
}

SimpleUILayer::SimpleUILayer() : mse::Layer()
{
    MSE_LOG("Constructed a simpleUI layer");
}

SimpleUILayer::~SimpleUILayer()
{
    MSE_LOG("Destroyed a simpleUI layer");
}

void SimpleUILayer::OnInit()
{
}

void SimpleUILayer::OnUpdate()
{}

#endif
