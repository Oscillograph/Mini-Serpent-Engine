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
                                  U"ЛЕСТА АВТОБАТЛЕР", 
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
                                  U"АВТОБАТЛЕР", 
                                  {90, 12, 160, 20}, 
                                  {0, 0, 0, 0}, 
                                  {255, 255, 255, 255},
                                  2));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"АВТОБАТЛЕР", 
                                  {90, 10, 160, 20}, 
                                  {0, 0, 0, 0}, 
                                  {255, 196, 0, 255},
                                  2));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"АВТОБАТЛЕР", 
                                  {90, 9, 160, 20}, 
                                  {0, 0, 0, 0}, 
                                  {255, 128, 0, 255},
                                  2));
    
    mse::gui::Button* playerCreateBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(
                                                                                            this,
                                                                                            U" Новая  игра",
                                                                                            {0, 0, 0, 255},
                                                                                            {110, 40, 100, 13},
                                                                                            "./data/img/screen-images.png",
                                                                                            {122, 101, 4, 13},
                                                                                            {138, 101, 4, 13},
                                                                                            {154, 101, 4, 13}
                                                                                            )));
    playerCreateBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        gsm.ChangeStateTo(LAutobattler::GamePages::CharacterCreation);
    };
    
//    mse::gui::Button* battleBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(
//                                                                                      this,
//                                                                                      U"  Загрузить",
//                                                                                      {0, 0, 0, 255},
//                                                                                      {110, 54, 100, 13},
//                                                                                      "./data/img/screen-images.png",
//                                                                                      {122, 101, 4, 13},
//                                                                                      {138, 101, 4, 13},
//                                                                                      {154, 101, 4, 13}
//                                                                                      )));
//    battleBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
//        gsm.ChangeStateTo(LAutobattler::GamePages::CharacterLoad);
//    };
//    
//    mse::gui::Button* scoresBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(
//                                                                                      this,
//                                                                                      U"   Рекорды",
//                                                                                      {0, 0, 0, 255},
//                                                                                      {110, 68, 100, 13},
//                                                                                      "./data/img/screen-images.png",
//                                                                                      {122, 101, 4, 13},
//                                                                                      {138, 101, 4, 13},
//                                                                                      {154, 101, 4, 13}
//                                                                                      )));
//    scoresBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
//        gsm.ChangeStateTo(LAutobattler::GamePages::Highscores);
//    };

    mse::gui::Button* creditsBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(
                                                                                       this,
                                                                                       U"    Титры",
                                                                                       {0, 0, 0, 255},
                                                                                       {110, 54, 100, 13},
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
                                                                                    {110, 68, 100, 13},
                                                                                    "./data/img/screen-images.png",
                                                                                    {122, 101, 4, 13},
                                                                                    {138, 101, 4, 13},
                                                                                    {154, 101, 4, 13}
                                                                                    )));
    exitBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        gsm.ChangeStateTo(LAutobattler::GamePages::Exit);
    };
    
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Перед началом игры характеристики \nперсонажей генерируются случайным образом.\n\nЕсли выпавшие не нравятся, можно вернуться в главное меню и начать игру снова.\n\nБой считается выигранным, если \nперсонаж игрока нокаутировал соперника\nили продержался 30 ходов.\n\nЧтобы выйти из игры в любое время, \nдостаточно нажать клавишу ESC.", 
                                  {10, 120, 300, 100}, 
                                  {0, 0, 0, 255}, 
                                  {255, 255, 255, 255},
                                  1));
    
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
                                  U"Выбор класса", 
                                  {80, 12, 180, 20}, 
                                  {0, 0, 0, 0}, 
                                  {255, 255, 255, 255},
                                  2));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Выбор класса", 
                                  {80, 10, 180, 20}, 
                                  {0, 0, 0, 0}, 
                                  {255, 196, 0, 255},
                                  2));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Выбор класса", 
                                  {80, 9, 180, 20}, 
                                  {0, 0, 0, 0}, 
                                  {255, 128, 0, 255},
                                  2));
    
    int str, agi, end = 0; // stats for characters
    std::u32string stats; // stats text gui content
    std::stringstream strstream; // buffer to add numbers
    
    AddElement(new mse::gui::Image(this, {40, 60, 34, 64}, "./data/img/heroes.png", {102, 0, 34, 64}, {0, 0, 0, 255}));
    str = std::rand() % 3 + 1;
    agi = std::rand() % 3 + 1;
    end = std::rand() % 3 + 1;
    rogueStatsBox = (mse::gui::Text*)(AddElement(new mse::gui::Text(this, U"", {10, 125, 100, 50}, {0, 0, 0, 255}, {255, 255, 255, 255})));
    strstream << 
    "Здоровье: " << 4 <<
    "\nСила:     " << str <<
    "\nЛовкость: " << agi <<
    "\nВыносл-ть: " << end <<
    "\nСкрытый удар";
    stats = utf8::utf8to32(strstream.str());
    rogueStatsBox->ChangeText(stats);
    strstream.str("");
    strstream.clear();
    mse::gui::Button* ClassRogueBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Разбойник", {32, 32, 32, 255}, {20, 180, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    ClassRogueBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [=](SDL_Event* event){
        game.inputClass = LAutobattler::Classes::Rogue;
        game.inputStats = {4, str, agi, end};
    };
    
    AddElement(new mse::gui::Image(this, {140, 60, 34, 64}, "./data/img/heroes.png", {0, 128, 34, 64}, {0, 0, 0, 255}));
    str = std::rand() % 3 + 1;
    agi = std::rand() % 3 + 1;
    end = std::rand() % 3 + 1;
    warriorStatsBox = (mse::gui::Text*)(AddElement(new mse::gui::Text(this, U"", {110, 125, 100, 50}, {0, 0, 0, 255}, {255, 255, 255, 255})));
    strstream << 
    "Здоровье: " << 5 <<
    "\nСила:     " << str <<
    "\nЛовкость: " << agi <<
    "\nВыносл-ть: " << end <<
    "\nРвение";
    stats = utf8::utf8to32(strstream.str());
    warriorStatsBox->ChangeText(stats);
    strstream.str("");
    strstream.clear();
    mse::gui::Button* ClassWarriorBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"   Воин", {32, 32, 32, 255}, {120, 180, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    ClassWarriorBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [=](SDL_Event* event){
        game.inputClass = LAutobattler::Classes::Warrior;
        game.inputStats = {5, str, agi, end};
    };
    
    AddElement(new mse::gui::Image(this, {240, 60, 34, 64}, "./data/img/heroes.png", {68, 64, 34, 64}, {0, 0, 0, 255}));
    str = std::rand() % 3 + 1;
    agi = std::rand() % 3 + 1;
    end = std::rand() % 3 + 1;
    barbarianStatsBox = (mse::gui::Text*)(AddElement(new mse::gui::Text(this, U"", {210, 125, 100, 50}, {0, 0, 0, 255}, {255, 255, 255, 255})));
    strstream << 
    "Здоровье: " << 6 <<
    "\nСила:     " << str <<
    "\nЛовкость: " << agi <<
    "\nВыносл-ть: " << end <<
    "\nЯрость";
    stats = utf8::utf8to32(strstream.str());
    barbarianStatsBox->ChangeText(stats);
    strstream.str("");
    strstream.clear();
    mse::gui::Button* ClassBarbarianBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"  Варвар", {32, 32, 32, 255}, {220, 180, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    ClassBarbarianBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [=](SDL_Event* event){
        game.inputClass = LAutobattler::Classes::Barbarian;
        game.inputStats = {6, str, agi, end};
    };
    
    mse::gui::Button* mainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"<< Назад", {32, 32, 32, 255}, {120, 215, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    mainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
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
                                  U"АВТОБАТЛЕР", 
                                  {90, 12, 160, 20}, 
                                  {0, 0, 0, 0}, 
                                  {255, 255, 255, 255},
                                  2));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"АВТОБАТЛЕР", 
                                  {90, 10, 160, 20}, 
                                  {0, 0, 0, 0}, 
                                  {255, 196, 0, 255},
                                  2));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"АВТОБАТЛЕР", 
                                  {90, 9, 160, 20}, 
                                  {0, 0, 0, 0}, 
                                  {255, 128, 0, 255},
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
                                      {50, 160, 220, 40}, 
                                      {0, 0, 0, 255}, 
                                      {196, 196, 196, 255},
                                      1));
        
        mse::gui::Button* mainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"К наградам", {32, 32, 32, 255}, {120, 215, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
        mainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
            gsm.ChangeStateTo(LAutobattler::GamePages::CharacterUpdate);
        };
    } else {
        AddElement(new mse::gui::Text(
                                      this, 
                                      U"Герой покорил арену, одолев \nвсе трудности и опасности. \nЕго удали нет равных, и теперь,по праву чемпиона, он войдёт \nв Зал Славы как живая легенда.\nЭто было великолепно!", 
                                      {50, 140, 220, 60}, 
                                      {0, 0, 0, 255}, 
                                      {196, 196, 196, 255},
                                      1));
        
        mse::gui::Button* mainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"К началу", {32, 32, 32, 255}, {120, 215, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
        mainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
            gsm.ChangeStateTo(LAutobattler::GamePages::MainMenu);
        };
//        mse::gui::Button* mainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"В Зал Славы", {120, 220, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
//        mainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
//            gsm.ChangeStateTo(LAutobattler::GamePages::Highscores);
//        };
    }
    
    mse::gui::Image* backgroundImage = (mse::gui::Image*)(AddElement(new mse::gui::Image(this, {0, 0, 320, 240}, "./data/img/background.png", {0, 0, 320, 240}, {0, 0, 0, 255})));
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
                                  U"АВТОБАТЛЕР", 
                                  {90, 12, 160, 20}, 
                                  {0, 0, 0, 0}, 
                                  {255, 255, 255, 255},
                                  2));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"АВТОБАТЛЕР", 
                                  {90, 10, 160, 20}, 
                                  {0, 0, 0, 0}, 
                                  {255, 196, 0, 255},
                                  2));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"АВТОБАТЛЕР", 
                                  {90, 9, 160, 20}, 
                                  {0, 0, 0, 0}, 
                                  {255, 128, 0, 255},
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
                                  {50, 160, 220, 40}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    
    mse::gui::Button* mainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"К началу", {32, 32, 32, 255}, {120, 215, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    mainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        gsm.ChangeStateTo(LAutobattler::GamePages::MainMenu);
    };
    
    mse::gui::Image* backgroundImage = (mse::gui::Image*)(AddElement(new mse::gui::Image(this, {0, 0, 320, 240}, "./data/img/background.png", {0, 0, 320, 240}, {0, 0, 0, 255})));
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
                                  U"АВТОБАТЛЕР", 
                                  {90, 12, 160, 20}, 
                                  {0, 0, 0, 0}, 
                                  {255, 255, 255, 255},
                                  2));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"АВТОБАТЛЕР", 
                                  {90, 10, 160, 20}, 
                                  {0, 0, 0, 0}, 
                                  {255, 196, 0, 255},
                                  2));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"АВТОБАТЛЕР", 
                                  {90, 9, 160, 20}, 
                                  {0, 0, 0, 0}, 
                                  {255, 128, 0, 255},
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
                                  U"Моральная поддержка:", 
                                  {40, 130, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 0, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Сообщество Nau Engine", 
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
                                  U"Леста Игры", 
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
    // npc life
    AddElement(new mse::gui::Image(this, {20, 15, 13, 13}, "./data/img/screen-images.png", {168, 119, 13, 13}, {0, 0, 0, 255}));
    npcLife = (mse::gui::Text*)(AddElement(new mse::gui::Text(
                                                              this, 
                                                              U"", 
                                                              {35, 15, 20, 10}, 
                                                              {0, 0, 0, 255}, 
                                                              {255, 196, 0, 255})));
    // npc character
    {
        glm::uvec4 src = {0, 0, 34, 64};
    
        switch (game.npcCharacter.race)
        {
        case LAutobattler::Races::Goblin:
            {
                src = {222, 95, 30, 64};
                break;
            }
        case LAutobattler::Races::Skeleton:
            {
                src = {257, 27, 34, 64};
                break;
            }
        case LAutobattler::Races::Slime:
            {
                src = {225, 43, 30, 48};
                break;
            }
        case LAutobattler::Races::Ghost:
            {
                src = {252, 99, 32, 54};
                break;
            }
        case LAutobattler::Races::Golem:
            {
                src = {200, 172, 30, 51};
                break;
            }
        case LAutobattler::Races::Dragon:
            {
                src = {232, 164, 34, 59};
                break;
            }
        }
        AddElement(new mse::gui::Image(this, {40, 90 - src.w, src.z, src.w}, "./data/img/screen-images.png", src, {0, 0, 0, 255}));
    }
    
    mse::gui::Text* battleNumber = (mse::gui::Text*)(AddElement(new mse::gui::Text(
                                                                        this, 
                                                                        U"", 
                                                                        {110, 15, 100, 10}, 
                                                                        {0, 0, 0, 255}, 
                                                                        {255, 255, 255, 255})));
    std::stringstream battleNumberContents;
    battleNumberContents << "Битва " << (game.battleCounter + 1) << " из 5";
    battleNumber->ChangeText(utf8::utf8to32(battleNumberContents.str()));
    battleNumberContents.str("");
    battleNumberContents.clear();
    
    // player life
    AddElement(new mse::gui::Image(this, {250, 15, 13, 13}, "./data/img/screen-images.png", {168, 119, 13, 13}, {0, 0, 0, 255}));
    playerLife = (mse::gui::Text*)(AddElement(new mse::gui::Text(
                                                                 this, 
                                                                 U"", 
                                                                 {265, 15, 20, 10}, 
                                                                 {0, 0, 0, 255}, 
                                                                 {255, 196, 0, 255})));
    // player character
    {
        glm::uvec4 src = {0, 0, 34, 64};
        switch (game.playerCharacter.main_class.type)
        {
        case LAutobattler::Classes::Rogue:
            {
                src.y = 0;
                break;
            }
        case LAutobattler::Classes::Warrior:
            {
                src.y = 128;
                break;
            }
        case LAutobattler::Classes::Barbarian:
            {
                src.y = 64;
                break;
            }
        }
        
        switch (game.playerCharacter.weapon.sprite)
        {
        case LAutobattler::WeaponSprite::Dagger:
            {
                src.x = 102;
                break;
            }
        case LAutobattler::WeaponSprite::Sword:
            {
                src.x = 0;
                break;
            }
        case LAutobattler::WeaponSprite::Spear:
            {
                src.x = 34;
                break;
            }
        case LAutobattler::WeaponSprite::Club:
            {
                src.x = 68;
                break;
            }
        case LAutobattler::WeaponSprite::Axe:
            {
                src.x = 136;
                break;
            }
        case LAutobattler::WeaponSprite::LegendarySword:
            {
                src.x = 170;
                break;
            }
        }
        
        AddElement(new mse::gui::Image(this, {240, 90 - src.w, src.z, src.w}, "./data/img/heroes.png", src, {0, 0, 0, 255}));
    }
    
    messageLog = (mse::gui::Text*)(AddElement(new mse::gui::Text(
                                                                 this, 
                                                                 U"", 
                                                                 {10, 90, 300, 120}, 
                                                                 {0, 64, 0, 255}, 
                                                                 {255, 255, 0, 255})));
    messageLog->showBorder = true;
    
    nextBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(
                  this, U"Продолжить", {32, 32, 32, 255}, {110, 215, 100, 13}, 
                  "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    nextBtn->Disable();
    nextBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.gamePageHasToChange = true;
    };
    
    mse::gui::Image* backgroundImage = (mse::gui::Image*)(AddElement(new mse::gui::Image(this, {0, 0, 320, 240}, "./data/img/background.png", {0, 0, 320, 240}, {0, 0, 0, 255})));
}

void ArenaUILayer::OnUpdate()
{
//    std::u32string npcLifeTextContent = U"";
    std::stringstream npcLifeTextStream;
    npcLifeTextStream << game.npcCharacter.stats.health;
    npcLife->ChangeText(utf8::utf8to32(npcLifeTextStream.str()));
    npcLifeTextStream.str("");
    npcLifeTextStream.clear();
    
    std::stringstream playerLifeTextStream;
    playerLifeTextStream << game.playerCharacter.stats.health;
    playerLife->ChangeText(utf8::utf8to32(playerLifeTextStream.str()));
    playerLifeTextStream.str("");
    playerLifeTextStream.clear();
    
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
    
    if ((game.battleFinished) && !(nextBtn->isEnabled))
    {
        nextBtn->Enable();
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
    mse::gui::Text* text2 = (mse::gui::Text*)(AddElement(new mse::gui::Text(this, U"Развить класс:", {20, 35, 100, 10}, {0, 0, 0, 255}, {255, 255, 255, 255})));
    
    // pick a subclass
    classRogueBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Разбойник", {32, 32, 32, 255}, {20, 55, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    classRogueBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.inputClass = LAutobattler::Classes::Rogue;
        changeMade = true;
        classRogueBtn->Disable();
        if (game.playerCharacter.sub_class.type != LAutobattler::Classes::None)
        {
            if ((game.playerCharacter.main_class.type == LAutobattler::Classes::Warrior) ||
                (game.playerCharacter.sub_class.type == LAutobattler::Classes::Warrior))
            {
                classWarriorBtn->Enable();
            }
            
            if ((game.playerCharacter.main_class.type == LAutobattler::Classes::Barbarian) ||
                (game.playerCharacter.sub_class.type == LAutobattler::Classes::Barbarian))
            {
                classBarbarianBtn->Enable();
            }
        } else {
            classWarriorBtn->Enable();
            classBarbarianBtn->Enable();
        }
    };
    // disable by default as the main class is levelling up by default
    if (game.playerCharacter.main_class.type == LAutobattler::Classes::Rogue)
    {
        classRogueBtn->Disable();
    }
    // disable if not picked as a subclass or main class
    if (game.playerCharacter.sub_class.type != LAutobattler::Classes::None)
    {
        if ((game.playerCharacter.sub_class.type != LAutobattler::Classes::Rogue) &&
            (game.playerCharacter.main_class.type != LAutobattler::Classes::Rogue))
        {
            classRogueBtn->Disable();
        }
    }
    // disable if level limit reached
    if ((game.playerCharacter.main_class.level + game.playerCharacter.sub_class.level) == gameDB.level_max)
    {
        classRogueBtn->Disable();
    }
    
    classWarriorBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Воин", {32, 32, 32, 255}, {20, 69, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    classWarriorBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.inputClass = LAutobattler::Classes::Warrior;
        changeMade = true;
        classWarriorBtn->Disable();
        if (game.playerCharacter.sub_class.type != LAutobattler::Classes::None)
        {
            if ((game.playerCharacter.main_class.type == LAutobattler::Classes::Rogue) ||
                (game.playerCharacter.sub_class.type == LAutobattler::Classes::Rogue))
            {
                classRogueBtn->Enable();
            }
            
            if ((game.playerCharacter.main_class.type == LAutobattler::Classes::Barbarian) ||
                (game.playerCharacter.sub_class.type == LAutobattler::Classes::Barbarian))
            {
                classBarbarianBtn->Enable();
            }
        } else {
            classRogueBtn->Enable();
            classBarbarianBtn->Enable();
        }
    };
    // disable by default as the main class is levelling up by default
    if (game.playerCharacter.main_class.type == LAutobattler::Classes::Warrior)
    {
        classWarriorBtn->Disable();
    }
    // disable if not picked as a subclass or main class
    if (game.playerCharacter.sub_class.type != LAutobattler::Classes::None)
    {
        if ((game.playerCharacter.sub_class.type != LAutobattler::Classes::Warrior) &&
            (game.playerCharacter.main_class.type != LAutobattler::Classes::Warrior))
        {
            classWarriorBtn->Disable();
        }
    }
    // disable if level limit reached
    if ((game.playerCharacter.main_class.level + game.playerCharacter.sub_class.level) == gameDB.level_max)
    {
        classWarriorBtn->Disable();
    }
    
    classBarbarianBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Варвар", {32, 32, 32, 255}, {20, 83, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    classBarbarianBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.inputClass = LAutobattler::Classes::Barbarian;
        changeMade = true;
        classBarbarianBtn->Disable();
        if (game.playerCharacter.sub_class.type != LAutobattler::Classes::None)
        {
            if ((game.playerCharacter.main_class.type == LAutobattler::Classes::Rogue) ||
                (game.playerCharacter.sub_class.type == LAutobattler::Classes::Rogue))
            {
                classRogueBtn->Enable();
            }
            
            if ((game.playerCharacter.main_class.type == LAutobattler::Classes::Warrior) ||
                (game.playerCharacter.sub_class.type == LAutobattler::Classes::Warrior))
            {
                classWarriorBtn->Enable();
            }
        } else {
            classRogueBtn->Enable();
            classWarriorBtn->Enable();
        }
    };
    // disable by default as the main class is levelling up by default
    if (game.playerCharacter.main_class.type == LAutobattler::Classes::Barbarian)
    {
        classBarbarianBtn->Disable();
    }
    // disable if not picked as a subclass or main class
    if (game.playerCharacter.sub_class.type != LAutobattler::Classes::None)
    {
        if ((game.playerCharacter.sub_class.type != LAutobattler::Classes::Barbarian) &&
            (game.playerCharacter.main_class.type != LAutobattler::Classes::Barbarian))
        {
            classBarbarianBtn->Disable();
        }
    }
    // disable if level limit reached
    if ((game.playerCharacter.main_class.level + game.playerCharacter.sub_class.level) == gameDB.level_max)
    {
        classBarbarianBtn->Disable();
    }
    
    // pick a weapon
    glm::uvec4 srcKeep = {0, 0, 34, 64};
    glm::uvec4 srcNew = {0, 0, 34, 64};
    switch (game.playerCharacter.main_class.type)
    {
    case LAutobattler::Classes::Rogue:
        {
            srcKeep.y = 0;
            srcNew.y = 0;
            break;
        }
    case LAutobattler::Classes::Warrior:
        {
            srcKeep.y = 128;
            srcNew.y = 128;
            break;
        }
    case LAutobattler::Classes::Barbarian:
        {
            srcKeep.y = 64;
            srcNew.y = 64;
            break;
        }
    }
    
    switch (game.playerCharacter.weapon.sprite)
    {
    case LAutobattler::WeaponSprite::Dagger:
        {
            srcKeep.x = 102;
            break;
        }
    case LAutobattler::WeaponSprite::Sword:
        {
            srcKeep.x = 0;
            break;
        }
    case LAutobattler::WeaponSprite::Spear:
        {
            srcKeep.x = 34;
            break;
        }
    case LAutobattler::WeaponSprite::Club:
        {
            srcKeep.x = 68;
            break;
        }
    case LAutobattler::WeaponSprite::Axe:
        {
            srcKeep.x = 136;
            break;
        }
    case LAutobattler::WeaponSprite::LegendarySword:
        {
            srcKeep.x = 170;
            break;
        }
    }
    
    switch ((&(game.npcCharacter.drop_list[0]))->sprite)
    {
        case LAutobattler::WeaponSprite::Dagger:
        {
            srcNew.x = 102;
            break;
        }
    case LAutobattler::WeaponSprite::Sword:
        {
            srcNew.x = 0;
            break;
        }
    case LAutobattler::WeaponSprite::Spear:
        {
            srcNew.x = 34;
            break;
        }
    case LAutobattler::WeaponSprite::Club:
        {
            srcNew.x = 68;
            break;
        }
    case LAutobattler::WeaponSprite::Axe:
        {
            srcNew.x = 136;
            break;
        }
    case LAutobattler::WeaponSprite::LegendarySword:
        {
            srcNew.x = 170;
            break;
        }
    }
    
    mse::gui::Text* text3 = (mse::gui::Text*)(AddElement(new mse::gui::Text(this, U"Выбрать оружие:", {155, 20, 120, 10}, {0, 0, 0, 255}, {255, 255, 255, 255})));
    AddElement(new mse::gui::Image(this, {155, 95 - srcKeep.w, srcKeep.z, srcKeep.w}, "./data/img/heroes.png", srcKeep, {0, 0, 0, 255}));
    keepSameWeaponBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Оставить", {32, 32, 32, 255}, {135, 95, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    keepSameWeaponBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.inputWeapon = game.playerCharacter.weapon;
        changeMade = true;
        
        keepSameWeaponBtn->Disable();
        pickDroppedWeaponBtn->Enable();
    };
    keepSameWeaponBtn->Disable();
    
    AddElement(new mse::gui::Image(this, {240, 95 - srcNew.w, srcNew.z, srcNew.w}, "./data/img/heroes.png", srcNew, {0, 0, 0, 255}));
    pickDroppedWeaponBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Подобрать", {32, 32, 32, 255}, {220, 95, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    pickDroppedWeaponBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        LAutobattler::Weapon* weapon = &(game.npcCharacter.drop_list[0]);
        game.inputWeapon.name = weapon->name;
        game.inputWeapon.type = weapon->type;
        game.inputWeapon.damage = weapon->damage;
        game.inputWeapon.sprite = weapon->sprite;
        changeMade = true;
        
        keepSameWeaponBtn->Enable();
        pickDroppedWeaponBtn->Disable();
    };
    
    // defaults
    game.inputClass = game.playerCharacter.main_class.type;
    game.inputWeapon = game.playerCharacter.weapon;
    
    // show stats
    statsBox = (mse::gui::Text*)(AddElement(new mse::gui::Text(this, U"", {010, 109, 305, 100}, {0, 64, 0, 255}, {255, 255, 128, 255})));
    changeMade = true;
    
    // confirm changes
    mse::gui::Button* PlayerCharacterUpdateBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Подтвердить", {32, 32, 32, 255}, {110, 215, 100, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    PlayerCharacterUpdateBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.playerCharacterUpdated = true;
    };
    
    mse::gui::Image* backgroundImage = (mse::gui::Image*)(AddElement(new mse::gui::Image(this, {0, 0, 320, 240}, "./data/img/background.png", {0, 0, 320, 240}, {0, 0, 0, 255})));
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
        
        if (game.playerCharacter.sub_class.type != LAutobattler::Classes::None)
        {
            switch (game.playerCharacter.sub_class.type)
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
        } else {
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
        "\nКласс:      " << utf8::utf32to8(main_class.c_str()) << " (" << game.playerCharacter.main_class.level << ")" << 
        "\nПодкласс: " << utf8::utf32to8(sub_class.c_str()) << " (" << game.playerCharacter.sub_class.level << ")" <<
        "\nЗдоровье: " << game.playerCharacter.stats.health <<
        "\nСила:     " << game.playerCharacter.stats.strength <<
        "\nЛовкость: " << game.playerCharacter.stats.agility <<
        "\nВыносливость: " << game.playerCharacter.stats.endurance <<
        "\nОружие: " << utf8::utf32to8(game.inputWeapon.name.c_str()) << 
                   " (" << (int)(roundf(game.inputWeapon.damage)) << ")" <<
        "\nСпособности: ";
        for (int i = 0; i < game.playerCharacter.traits.size(); ++i)
        {
            if (i > 0)
            {
                strstream << ", ";
            }
            
            switch (game.playerCharacter.traits[i])
            {
            case LAutobattler::Traits::HiddenStrike:
                {
                    strstream << "Скрытый удар";
                    break;
                }
            case LAutobattler::Traits::Rush:
                {
                    strstream << "Рвение";
                    break;
                }
            case LAutobattler::Traits::Rage:
                {
                    strstream << "Ярость";
                    break;
                }
            case LAutobattler::Traits::StoneSkin:
                {
                    strstream << "Каменная кожа";
                    break;
                }
            case LAutobattler::Traits::Strong:
                {
                    strstream << "Силач";
                    break;
                }
            case LAutobattler::Traits::Agile:
                {
                    strstream << "Ловкач";
                    break;
                }
            case LAutobattler::Traits::Survivor:
                {
                    strstream << "Выживальщик";
                    break;
                }
            case LAutobattler::Traits::Shield:
                {
                    strstream << "Щит";
                    break;
                }
            case LAutobattler::Traits::Poison:
                {
                    strstream << "Яд";
                    break;
                }
            case LAutobattler::Traits::CuttingImmune:
                {
                    strstream << "Не режется";
                    break;
                }
            case LAutobattler::Traits::CrushingWeakness:
                {
                    strstream << "Страх дубин";
                    break;
                }
            case LAutobattler::Traits::FireBreather:
                {
                    strstream << "Огненное дыхание";
                    break;
                }
            }
        }
        strstream << "\n\nБитв пройдено: " << game.battleCounter << " из 5";
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
//    mse::gui::Image* backgroundImage = (mse::gui::Image*)(AddElement(new mse::gui::Image(this, {0, 0, 320, 240}, "./data/img/background.png", {0, 0, 320, 240}, {0, 0, 0, 255})));
}

void SimpleUILayer::OnUpdate()
{}

#endif
