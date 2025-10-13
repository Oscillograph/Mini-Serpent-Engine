#ifndef DUNGEONTRIS_LAYERS_CPP
#define DUNGEONTRIS_LAYERS_CPP

#include <mse/systems/platform/audio/soundman.h>
#include <dungeontris/game-fwd.h>
#include <dungeontris/layers.h>
#include <dungeontris/gamestates.h>

extern DTetris::GameDB gameDB;
extern DTetris::Game game;
extern GameStateMachine gsm;

void GameLogo(mse::Layer* layer)
{
    layer->AddElement(new mse::gui::Text(
                                  layer, 
                                  U"ПОДЗЕМЕТРИС", 
                                  {80, 12, 180, 20}, 
                                  {0, 0, 0, 0}, 
                                  {255, 255, 255, 255},
                                  2));
    layer->AddElement(new mse::gui::Text(
                                  layer, 
                                  U"ПОДЗЕМЕТРИС", 
                                  {80, 10, 180, 20}, 
                                  {0, 0, 0, 0}, 
                                  {255, 196, 0, 255},
                                  2));
    layer->AddElement(new mse::gui::Text(
                                  layer, 
                                  U"ПОДЗЕМЕТРИС", 
                                  {80, 9, 180, 20}, 
                                  {0, 0, 0, 0}, 
                                  {255, 128, 0, 255},
                                  2));
}

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
    GameLogo(this);
    
    text = (mse::gui::Text*)(AddElement(new mse::gui::Text(
                                                           this, 
                                                           U"Загрузится через: ", 
                                                           {120, 50, 200, 10}, 
                                                           {0, 0, 0, 255}, 
                                                           {0, 255, 255, 255})));
    
//    mse::SoundMan::PlayTrack("data/audio/tracks/01_towards_neon_shadows.mp3");
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
//            gsm.ChangeStateTo(DTetris::GamePages::MainMenu);
//        }
//    }
    gsm.ChangeStateTo(DTetris::GamePages::MainMenu);
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
    GameLogo(this);
    
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
        gsm.ChangeStateTo(DTetris::GamePages::CharacterCreation);
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
//        gsm.ChangeStateTo(DTetris::GamePages::CharacterLoad);
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
//        gsm.ChangeStateTo(DTetris::GamePages::Highscores);
//    };

    mse::gui::Button* settingsBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(
                                                                                       this,
                                                                                       U"  Настройки",
                                                                                       {0, 0, 0, 255},
                                                                                       {110, 54, 100, 13},
                                                                                       "./data/img/screen-images.png",
                                                                                       {122, 101, 4, 13},
                                                                                       {138, 101, 4, 13},
                                                                                       {154, 101, 4, 13}
                                                                                       )));
    settingsBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        gsm.ChangeStateTo(DTetris::GamePages::Settings);
    };
    
    mse::gui::Button* creditsBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(
                                                                                       this,
                                                                                       U"    Титры",
                                                                                       {0, 0, 0, 255},
                                                                                       {110, 68, 100, 13},
                                                                                       "./data/img/screen-images.png",
                                                                                       {122, 101, 4, 13},
                                                                                       {138, 101, 4, 13},
                                                                                       {154, 101, 4, 13}
                                                                                       )));
    creditsBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        gsm.ChangeStateTo(DTetris::GamePages::Credits);
    };
    
    mse::gui::Button* exitBtn = (mse::gui::Button*)(AddElement(
                                                               new mse::gui::Button(
                                                                                    this,
                                                                                    U"    Выход",
                                                                                    {0, 0, 0, 255},
                                                                                    {110, 82, 100, 13},
                                                                                    "./data/img/screen-images.png",
                                                                                    {122, 101, 4, 13},
                                                                                    {138, 101, 4, 13},
                                                                                    {154, 101, 4, 13}
                                                                                    )));
    exitBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        gsm.ChangeStateTo(DTetris::GamePages::Exit);
    };
    
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Перед началом игры характеристики \nперсонажей генерируются случайным образом.\n\nЕсли выпавшие не нравятся, можно вернуться в главное меню и начать игру снова.\n\nБой считается выигранным, если \nперсонаж игрока нокаутировал соперника\nили продержался 30 ходов.\n\nЧтобы выйти из игры в любое время, \nдостаточно нажать клавишу ESC.", 
                                  {10, 110, 300, 110}, 
                                  {0, 0, 0, 255}, 
                                  {255, 255, 255, 255},
                                  1));
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
    GameLogo(this);
    
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
    "\n" << utf8::utf32to8(gameDB.traits[DTetris::Traits::HiddenStrike].name);
    stats = utf8::utf8to32(strstream.str());
    rogueStatsBox->ChangeText(stats);
    strstream.str("");
    strstream.clear();
    mse::gui::Button* ClassRogueBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Разбойник", {32, 32, 32, 255}, {20, 180, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    ClassRogueBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [=](SDL_Event* event){
        game.inputClass = DTetris::Classes::Rogue;
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
    "\n" << utf8::utf32to8(gameDB.traits[DTetris::Traits::Rush].name);
    stats = utf8::utf8to32(strstream.str());
    warriorStatsBox->ChangeText(stats);
    strstream.str("");
    strstream.clear();
    mse::gui::Button* ClassWarriorBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"   Воин", {32, 32, 32, 255}, {120, 180, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    ClassWarriorBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [=](SDL_Event* event){
        game.inputClass = DTetris::Classes::Warrior;
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
    "\n" << utf8::utf32to8(gameDB.traits[DTetris::Traits::Rage].name);
    stats = utf8::utf8to32(strstream.str());
    barbarianStatsBox->ChangeText(stats);
    strstream.str("");
    strstream.clear();
    mse::gui::Button* ClassBarbarianBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"  Варвар", {32, 32, 32, 255}, {220, 180, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    ClassBarbarianBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [=](SDL_Event* event){
        game.inputClass = DTetris::Classes::Barbarian;
        game.inputStats = {6, str, agi, end};
    };
    
    mse::gui::Button* mainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"<< Назад", {32, 32, 32, 255}, {120, 215, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    mainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        gsm.ChangeStateTo(DTetris::GamePages::MainMenu);
    };
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
    GameLogo(this);
    
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
                                      U"В битве было нелегко, но герой\nсправился и одолел противника.\nВпереди новое испытание!", 
                                      {50, 160, 220, 40}, 
                                      {0, 0, 0, 255}, 
                                      {196, 196, 196, 255},
                                      1));
        
        mse::gui::Button* mainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"К наградам", {32, 32, 32, 255}, {120, 215, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
        mainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
            gsm.ChangeStateTo(DTetris::GamePages::CharacterUpdate);
        };
    } else {
        AddElement(new mse::gui::Text(
                                      this, 
                                      U"Герой покорил арену, преодолев\nвсе трудности и опасности. \nЕго удали нет равных, и теперь,по праву чемпиона, он войдёт \nв Зал Славы как живая легенда.\nЭто было великолепно!", 
                                      {50, 140, 220, 60}, 
                                      {0, 0, 0, 255}, 
                                      {196, 196, 196, 255},
                                      1));
        
        mse::gui::Button* mainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"К началу", {32, 32, 32, 255}, {120, 215, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
        mainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
            gsm.ChangeStateTo(DTetris::GamePages::MainMenu);
        };
//        mse::gui::Button* mainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"В Зал Славы", {120, 220, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
//        mainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
//            gsm.ChangeStateTo(DTetris::GamePages::Highscores);
//        };
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
    GameLogo(this);
    
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Проигрыш", 
                                  {105, 80, 120, 40}, 
                                  {0, 0, 0, 255}, 
                                  {255, 64, 64, 255},
                                  2));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Герой старался изо всех сил,\nно противник превзошёл его.\nУдачи в следующий раз!", 
                                  {50, 160, 220, 40}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    
    mse::gui::Button* mainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"К началу", {32, 32, 32, 255}, {120, 215, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    mainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        gsm.ChangeStateTo(DTetris::GamePages::MainMenu);
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
    GameLogo(this);
    
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Программирование:", 
                                  {20, 30, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 0, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Broscillograph", 
                                  {60, 40, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Графика:", 
                                  {20, 50, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 0, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Broscillograph", 
                                  {60, 60, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Музыка:", 
                                  {20, 70, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 0, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Трио из Удио\n\"Наша Таня громко плачет\" (2025)", 
                                  {60, 80, 240, 20}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Библиотеки:", 
                                  {20, 100, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 0, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"SDL2, utf8-cpp, Color Console\nYAML-cpp, GLM, EnTT, SDL-Mixer", 
                                  {60, 110, 240, 20}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Моральная поддержка:", 
                                  {20, 130, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 0, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Сообщество Nau Engine", 
                                  {60, 140, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Место для вашей рекламы:", 
                                  {20, 150, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 0, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"", 
                                  {60, 160, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Оригинальный диздок:", 
                                  {20, 170, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 0, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Леста Игры", 
                                  {60, 180, 220, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    
    mse::gui::Button* mainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"<< Назад", {32, 32, 32, 255}, {120, 220, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    mainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        gsm.ChangeStateTo(DTetris::GamePages::MainMenu);
    };
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
        case DTetris::Races::Goblin:
            {
                src = {222, 95, 30, 64};
                break;
            }
        case DTetris::Races::Skeleton:
            {
                src = {257, 27, 34, 64};
                break;
            }
        case DTetris::Races::Slime:
            {
                src = {225, 43, 30, 48};
                break;
            }
        case DTetris::Races::Ghost:
            {
                src = {252, 99, 32, 54};
                break;
            }
        case DTetris::Races::Golem:
            {
                src = {200, 172, 30, 51};
                break;
            }
        case DTetris::Races::Dragon:
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
        case DTetris::Classes::Rogue:
            {
                src.y = 0;
                break;
            }
        case DTetris::Classes::Warrior:
            {
                src.y = 128;
                break;
            }
        case DTetris::Classes::Barbarian:
            {
                src.y = 64;
                break;
            }
        }
        
        switch (game.playerCharacter.weapon.sprite)
        {
        case DTetris::WeaponSprite::Dagger:
            {
                src.x = 102;
                break;
            }
        case DTetris::WeaponSprite::Sword:
            {
                src.x = 0;
                break;
            }
        case DTetris::WeaponSprite::Spear:
            {
                src.x = 34;
                break;
            }
        case DTetris::WeaponSprite::Club:
            {
                src.x = 68;
                break;
            }
        case DTetris::WeaponSprite::Axe:
            {
                src.x = 136;
                break;
            }
        case DTetris::WeaponSprite::LegendarySword:
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
                                                                 {10, 90, 280, 120}, 
                                                                 {0, 64, 0, 255}, 
                                                                 {255, 255, 128, 255},
                                                                 1,
                                                                 true,
                                                                 {255, 196, 0, 255})));
    AddElement(new mse::gui::VScrollbar(
                                        this, 
                                        {290, 90, 13, 120}, 
                                        messageLog, 
                                        "./data/img/screen-images.png", 
                                        {0, 0, 0},
                                        {24, 49, 52, 12},
                                        {24, 74, 44, 10},
                                        {24, 61, 52, 12},
                                        {63, 61, 13, 4},
                                        {63, 65, 13, 4},
                                        {63, 69, 13, 4}));
    
    mse::gui::Checkbox* fastBattleBtn = (mse::gui::Checkbox*)AddElement(new mse::gui::Checkbox(
                                        this, 
                                        {30, 215, 14, 13}, 
                                        "data/img/screen-images.png", 
                                        {0, 0, 0}, 
                                        &(game.config.fastBattle),
                                        {24, 88, 14, 13}, 
                                        {38, 88, 14, 13}, 
                                        {52, 88, 14, 13}, 
                                        {66, 88, 14, 13}));
    fastBattleBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [=] (SDL_Event* event)
    {
        game.config.turnLength = (game.config.fastBattle) ? 0 : 500;
    };
    
    AddElement(new mse::gui::Text(
                                        this, 
                                        U"Быстрый бой", 
                                        {45, 215, 100, 10}, 
                                        {0, 0, 0, 255}, 
                                        {255, 255, 255, 255},
                                        1
                                        ));

    nextBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(
                  this, U"Продолжить", {32, 32, 32, 255}, {150, 215, 100, 13}, 
                  "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    nextBtn->Disable();
    nextBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.gamePageHasToChange = true;
    };
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
    DTetris::MessageLogItem* item = game.UILogger.stack;
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
        game.inputClass = DTetris::Classes::Rogue;
        changeMade = true;
        classRogueBtn->Disable();
        if (game.playerCharacter.sub_class.type != DTetris::Classes::None)
        {
            if ((game.playerCharacter.main_class.type == DTetris::Classes::Warrior) ||
                (game.playerCharacter.sub_class.type == DTetris::Classes::Warrior))
            {
                classWarriorBtn->Enable();
            }
            
            if ((game.playerCharacter.main_class.type == DTetris::Classes::Barbarian) ||
                (game.playerCharacter.sub_class.type == DTetris::Classes::Barbarian))
            {
                classBarbarianBtn->Enable();
            }
        } else {
            classWarriorBtn->Enable();
            classBarbarianBtn->Enable();
        }
    };
    // disable by default as the main class is levelling up by default
    if (game.playerCharacter.main_class.type == DTetris::Classes::Rogue)
    {
        classRogueBtn->Disable();
    }
    // disable if not picked as a subclass or main class
    if (game.playerCharacter.sub_class.type != DTetris::Classes::None)
    {
        if ((game.playerCharacter.sub_class.type != DTetris::Classes::Rogue) &&
            (game.playerCharacter.main_class.type != DTetris::Classes::Rogue))
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
        game.inputClass = DTetris::Classes::Warrior;
        changeMade = true;
        classWarriorBtn->Disable();
        if (game.playerCharacter.sub_class.type != DTetris::Classes::None)
        {
            if ((game.playerCharacter.main_class.type == DTetris::Classes::Rogue) ||
                (game.playerCharacter.sub_class.type == DTetris::Classes::Rogue))
            {
                classRogueBtn->Enable();
            }
            
            if ((game.playerCharacter.main_class.type == DTetris::Classes::Barbarian) ||
                (game.playerCharacter.sub_class.type == DTetris::Classes::Barbarian))
            {
                classBarbarianBtn->Enable();
            }
        } else {
            classRogueBtn->Enable();
            classBarbarianBtn->Enable();
        }
    };
    // disable by default as the main class is levelling up by default
    if (game.playerCharacter.main_class.type == DTetris::Classes::Warrior)
    {
        classWarriorBtn->Disable();
    }
    // disable if not picked as a subclass or main class
    if (game.playerCharacter.sub_class.type != DTetris::Classes::None)
    {
        if ((game.playerCharacter.sub_class.type != DTetris::Classes::Warrior) &&
            (game.playerCharacter.main_class.type != DTetris::Classes::Warrior))
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
        game.inputClass = DTetris::Classes::Barbarian;
        changeMade = true;
        classBarbarianBtn->Disable();
        if (game.playerCharacter.sub_class.type != DTetris::Classes::None)
        {
            if ((game.playerCharacter.main_class.type == DTetris::Classes::Rogue) ||
                (game.playerCharacter.sub_class.type == DTetris::Classes::Rogue))
            {
                classRogueBtn->Enable();
            }
            
            if ((game.playerCharacter.main_class.type == DTetris::Classes::Warrior) ||
                (game.playerCharacter.sub_class.type == DTetris::Classes::Warrior))
            {
                classWarriorBtn->Enable();
            }
        } else {
            classRogueBtn->Enable();
            classWarriorBtn->Enable();
        }
    };
    // disable by default as the main class is levelling up by default
    if (game.playerCharacter.main_class.type == DTetris::Classes::Barbarian)
    {
        classBarbarianBtn->Disable();
    }
    // disable if not picked as a subclass or main class
    if (game.playerCharacter.sub_class.type != DTetris::Classes::None)
    {
        if ((game.playerCharacter.sub_class.type != DTetris::Classes::Barbarian) &&
            (game.playerCharacter.main_class.type != DTetris::Classes::Barbarian))
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
    case DTetris::Classes::Rogue:
        {
            srcKeep.y = 0;
            srcNew.y = 0;
            break;
        }
    case DTetris::Classes::Warrior:
        {
            srcKeep.y = 128;
            srcNew.y = 128;
            break;
        }
    case DTetris::Classes::Barbarian:
        {
            srcKeep.y = 64;
            srcNew.y = 64;
            break;
        }
    }
    
    switch (game.playerCharacter.weapon.sprite)
    {
    case DTetris::WeaponSprite::Dagger:
        {
            srcKeep.x = 102;
            break;
        }
    case DTetris::WeaponSprite::Sword:
        {
            srcKeep.x = 0;
            break;
        }
    case DTetris::WeaponSprite::Spear:
        {
            srcKeep.x = 34;
            break;
        }
    case DTetris::WeaponSprite::Club:
        {
            srcKeep.x = 68;
            break;
        }
    case DTetris::WeaponSprite::Axe:
        {
            srcKeep.x = 136;
            break;
        }
    case DTetris::WeaponSprite::LegendarySword:
        {
            srcKeep.x = 170;
            break;
        }
    }
    
    switch ((&(game.npcCharacter.drop_list[0]))->sprite)
    {
        case DTetris::WeaponSprite::Dagger:
        {
            srcNew.x = 102;
            break;
        }
    case DTetris::WeaponSprite::Sword:
        {
            srcNew.x = 0;
            break;
        }
    case DTetris::WeaponSprite::Spear:
        {
            srcNew.x = 34;
            break;
        }
    case DTetris::WeaponSprite::Club:
        {
            srcNew.x = 68;
            break;
        }
    case DTetris::WeaponSprite::Axe:
        {
            srcNew.x = 136;
            break;
        }
    case DTetris::WeaponSprite::LegendarySword:
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
        DTetris::Weapon* weapon = &(game.npcCharacter.drop_list[0]);
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
    statsBox = (mse::gui::Text*)(AddElement(new mse::gui::Text(
                   this, 
                   U"", 
                   {10, 109, 287, 100}, 
                   {0, 64, 0, 255}, 
                   {255, 255, 128, 255},
                   1,
                   true,
                   {255, 196, 0, 255})));
    changeMade = true;
    
    AddElement(new mse::gui::VScrollbar(
                    this, 
                    {297, 109, 13, 100}, 
                    statsBox, 
                    "./data/img/screen-images.png", 
                    {0, 0, 0},
                    {24, 49, 52, 12},
                    {24, 74, 44, 10},
                    {24, 61, 52, 12},
                    {63, 61, 13, 4},
                    {63, 65, 13, 4},
                    {63, 69, 13, 4}));
    
    // confirm changes
    mse::gui::Button* PlayerCharacterUpdateBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Подтвердить", {32, 32, 32, 255}, {110, 215, 100, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    PlayerCharacterUpdateBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.playerCharacterUpdated = true;
    };
}

void CharacterUpdateUILayer::OnUpdate()
{
    if (changeMade)
    {
        std::u32string stats = U"";
        std::u32string main_class = U"";
        std::u32string sub_class = U"";
        
        main_class = gameDB.classes[game.playerCharacter.main_class.type].name;
        
        if (game.playerCharacter.sub_class.type != DTetris::Classes::None)
        {
            sub_class = gameDB.classes[game.playerCharacter.sub_class.type].name;
        } else {
            if (game.inputClass != game.playerCharacter.main_class.type)
            {
                sub_class = gameDB.classes[game.inputClass].name;
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
        
        for (DTetris::Traits trait : game.playerCharacter.traits)
        {
            if (trait == DTetris::Traits::Strong)
            {
                game.playerCharacter.stats.strength++;
            }
            if (trait == DTetris::Traits::Agile)
            {
                game.playerCharacter.stats.agility++;
            }
            if (trait == DTetris::Traits::Survivor)
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
                   "\n          (урон " << (int)(roundf(game.inputWeapon.damage));
        strstream << ", " << utf8::utf32to8(gameDB.damageTypes[game.inputWeapon.type].name);
        strstream << ")" <<
        "\nСпособности: ";
        for (int i = 0; i < game.playerCharacter.traits.size(); ++i)
        {
            if (i > 0)
            {
                strstream << ", ";
            }
            strstream << "\n  " << utf8::utf32to8(gameDB.traits[game.playerCharacter.traits[i]].name)
                      << "\n    " << utf8::utf32to8(gameDB.traits[game.playerCharacter.traits[i]].description);
        }
        strstream << "\n\nБитв пройдено: " << game.battleCounter << " из 5";
        stats = utf8::utf8to32(strstream.str());
        statsBox->ChangeText(stats);
        
        changeMade = false;
    }
}

SettingsUILayer::SettingsUILayer() : mse::Layer()
{
    MSE_LOG("Constructed a SettingsUI layer");
}

SettingsUILayer::~SettingsUILayer()
{
    MSE_LOG("Destroyed a SettingsUI layer");
}

void SettingsUILayer::OnInit()
{
    
    
    // Settings to control from here:
    // --Audio--
    // master volume
    // music volume
    // sounds volume
    // --Graphics--
    // fullscreen toggle on/off
    
    
    
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Звук:", 
                                  {20, 40, 140, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 0, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Музыка фоном", 
                                  {60, 54, 140, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    mse::gui::Button* playInBattlegroundBtn = (mse::gui::Button*)AddElement(new mse::gui::Checkbox(
                                  this, 
                                  {210, 54, 14, 13}, 
                                  "data/img/screen-images.png", 
                                  {0, 0, 0}, 
                                  &(game.config.playInBackground),
                                  {24, 88, 14, 13}, 
                                  {38, 88, 14, 13}, 
                                  {52, 88, 14, 13}, 
                                  {66, 88, 14, 13}));
    playInBattlegroundBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [=] (SDL_Event* event)
    {
        mse::Application::config.playInBackground = game.config.playInBackground;
    };
    
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Громкость музыки", 
                                  {60, 68, 140, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    musicVolumeSlider = (mse::gui::HSlider*)AddElement(new mse::gui::HSlider(
                                  this, 
                                  {210, 68, 80, 13}, 
                                  &(game.config.musicVolume),
                                  0,
                                  MIX_MAX_VOLUME,
                                  MIX_MAX_VOLUME / 16,
                                  "data/img/screen-images.png", 
                                  {0, 0, 0}, 
                                  {24, 23, 52, 13}, 
                                  {24, 11, 40, 11}, 
                                  {24, 36, 52, 13}, 
                                  {63, 36, 4, 14}, 
                                  {67, 36, 5, 14}, 
                                  {72, 36, 4, 14}));

    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Громкость эффектов", 
                                  {60, 82, 140, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    soundsVolumeSlider = (mse::gui::HSlider*)AddElement(new mse::gui::HSlider(
                                     this, 
                                     {210, 82, 80, 13}, 
                                     &(game.config.soundsVolume),
                                     0,
                                     MIX_MAX_VOLUME,
                                     MIX_MAX_VOLUME / 16,
                                     "data/img/screen-images.png", 
                                     {0, 0, 0}, 
                                     {24, 23, 52, 13}, 
                                     {24, 11, 40, 11}, 
                                     {24, 36, 52, 13}, 
                                     {63, 36, 4, 14}, 
                                     {67, 36, 5, 14}, 
                                     {72, 36, 4, 14}));
    
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Графика:", 
                                  {20, 96, 140, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 0, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Окно во весь экран", 
                                  {60, 110, 140, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1));
    mse::gui::Checkbox* fullscreenBtn = (mse::gui::Checkbox*)(AddElement(new mse::gui::Checkbox(
                                  this, 
                                  {210, 110, 14, 13}, 
                                  "data/img/screen-images.png", 
                                  {0, 0, 0}, 
                                  &(game.config.fullscreen),
                                  {24, 88, 14, 13}, 
                                  {38, 88, 14, 13}, 
                                  {52, 88, 14, 13}, 
                                  {66, 88, 14, 13})));
    fullscreenBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [=](SDL_Event* event)
    {
        mse::Application::config.fullscreen = game.config.fullscreen;
//        fullscreenBtn->state = mse::gui::CheckboxStates::Checked;
        m_window->ToggleFullscreen();
    };
    
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Игровой процесс:", 
                                  {20, 124, 140, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 0, 255},
                                  1));
    AddElement(new mse::gui::Text(
                                  this, 
                                  U"Быстрый бой", 
                                  {60, 138, 140, 10}, 
                                  {0, 0, 0, 255}, 
                                  {196, 196, 196, 255},
                                  1
                                  ));
    mse::gui::Checkbox* fastBattleBtn = (mse::gui::Checkbox*)AddElement(new mse::gui::Checkbox(
                                  this, 
                                  {210, 138, 14, 13}, 
                                  "data/img/screen-images.png", 
                                  {0, 0, 0}, 
                                  &(game.config.fastBattle),
                                  {24, 88, 14, 13}, 
                                  {38, 88, 14, 13}, 
                                  {52, 88, 14, 13}, 
                                  {66, 88, 14, 13}));
    fastBattleBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [=] (SDL_Event* event)
    {
        game.config.turnLength = (game.config.fastBattle) ? 0 : 500;
    };
    
    mse::gui::Button* mainMenuBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"<< Назад", {32, 32, 32, 255}, {120, 220, 80, 13}, "./data/img/screen-images.png", {122, 101, 4, 13}, {138, 101, 4, 13}, {154, 101, 4, 13})));
    mainMenuBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        gsm.ChangeStateTo(DTetris::GamePages::MainMenu);
    };
}

void SettingsUILayer::OnUpdate()
{
    if (musicVolumeSlider->valueChanged)
    {
        MSE_CORE_LOG("musicVolumeSlider");
        musicVolumeSlider->valueChanged = false;
        mse::Application::config.musicVolume = game.config.musicVolume;
        mse::SoundMan::AdjustTrackVolume((int)roundf(mse::Application::config.musicVolume));
    }
    if (soundsVolumeSlider->valueChanged)
    {
        MSE_CORE_LOG("soundsVolumeSlider");
        soundsVolumeSlider->valueChanged = false;
        mse::Application::config.soundsVolume = game.config.soundsVolume;
        mse::SoundMan::AdjustSoundsVolume((int)roundf(mse::Application::config.soundsVolume));
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
    mse::gui::Image* backgroundImage = (mse::gui::Image*)(AddElement(new mse::gui::Image(
                     this, 
                     {0, 0, 320, 240}, 
                     "./data/img/background.png", 
                     {0, 0, 320, 240}, 
                     {0, 0, 0, 255})));
}

void SimpleUILayer::OnUpdate()
{}

#endif
