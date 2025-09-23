#ifndef LAYERS_CPP
#define LAYERS_CPP

#include <lesta-autobattler/game-fwd.h>
#include <lesta-autobattler/layers.h>
#include <lesta-autobattler/gamestates.h>

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
    text = (mse::gui::Text*)(AddElement(new mse::gui::Text(
                                                           this, 
                                                           U"Победа! Выберите награду.", 
                                                           {100, 10, 200, 10}, 
                                                           {0, 0, 0, 255}, 
                                                           {0, 255, 255, 255})));
}

void IntroUILayer::OnUpdate()
{
    std::stringstream strstream;
    int m = ((IntroPageState*)gsm.current)->counter;
    strstream << "Обратный отсчёт: " << m;
    std::u32string newText = utf8::utf8to32(strstream.str());
    
    text->ChangeText(newText);
    
    if (m < 0)
    {
        gsm.ChangeStateTo(LAutobattler::GamePages::MainMenu);
    }
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
{}

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
    mse::gui::Button* ClassRogueBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Разбойник", {100, 10, 80, 10}, {196, 100, 100, 255}, {32, 32, 32, 255})));
    ClassRogueBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.inputClass = LAutobattler::Classes::Rogue;
        m_window->GetLayerManager()->Detach(this);
    };
    
    mse::gui::Button* ClassWarriorBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Воин", {100, 20, 80, 10}, {100, 196, 196, 255}, {32, 32, 32, 255})));
    ClassWarriorBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.inputClass = LAutobattler::Classes::Warrior;
        m_window->GetLayerManager()->Detach(this);
    };
    
    mse::gui::Button* ClassBarbarianBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Варвар", {100, 30, 80, 10}, {196, 196, 100, 255}, {32, 32, 32, 255})));
    ClassBarbarianBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.inputClass = LAutobattler::Classes::Barbarian;
        m_window->GetLayerManager()->Detach(this);
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
{}

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
{}

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
{}

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
                                                                 {010, 90, 300, 100}, 
                                                                 {0, 64, 0, 255}, 
                                                                 {255, 255, 0, 255})));
    messageLog->showBorder = true;
}

void ArenaUILayer::OnUpdate()
{
    if ((game.gamePage != LAutobattler::GamePages::ArenaSetup) &&
        (game.gamePage != LAutobattler::GamePages::ArenaBattle) && 
        (game.gamePage != LAutobattler::GamePages::ArenaAftermath))
    {
        m_window->GetLayerManager()->Detach(this);
    } else {
        std::u32string messages = U"";
        LAutobattler::MessageLogItem* item = game.UILogger.stack;
        while (item != nullptr)
        {
            messages += item->text;
            item = item->next;
        }
        item = nullptr;
        
        messageLog->ChangeText(messages);
        MSE_LOG("Message Log text changed!")
    }
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
    mse::gui::Text* text1 = (mse::gui::Text*)(AddElement(new mse::gui::Text(this, U"Победа! Выберите награду.", {100, 10, 200, 10}, {0, 0, 0, 255}, {255, 255, 0, 255})));
    mse::gui::Text* text2 = (mse::gui::Text*)(AddElement(new mse::gui::Text(this, U"Развить класс:", {100, 20, 120, 10}, {0, 0, 0, 255}, {255, 255, 255, 255})));
    
    // pick a subclass
    mse::gui::Button* ClassRogueBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Разбойник", {100, 30, 80, 10}, {196, 100, 100, 255}, {32, 32, 32, 255})));
    ClassRogueBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.inputClass = LAutobattler::Classes::Rogue;
    };
    
    mse::gui::Button* ClassWarriorBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Воин", {180, 30, 80, 10}, {100, 196, 196, 255}, {32, 32, 32, 255})));
    ClassWarriorBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.inputClass = LAutobattler::Classes::Warrior;
    };
    
    mse::gui::Button* ClassBarbarianBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Варвар", {260, 30, 80, 10}, {196, 196, 100, 255}, {32, 32, 32, 255})));
    ClassBarbarianBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.inputClass = LAutobattler::Classes::Barbarian;
    };
    
    // pick a weapon
    mse::gui::Text* text3 = (mse::gui::Text*)(AddElement(new mse::gui::Text(this, U"Выбрать оружие:", {100, 40, 120, 10}, {0, 0, 0, 255}, {255, 255, 255, 255})));
    mse::gui::Button* KeepSameWeaponBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, game.playerCharacter.weapon.name, {100, 50, 80, 10}, {196, 100, 100, 255}, {32, 32, 32, 255})));
    KeepSameWeaponBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.inputWeapon = game.playerCharacter.weapon;
    };
    
    mse::gui::Button* PickDroppedWeaponBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, (*(LAutobattler::Weapon*)(&game.npcCharacter.drop_list[0])).name, {180, 50, 80, 10}, {100, 196, 196, 255}, {32, 32, 32, 255})));
    PickDroppedWeaponBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.inputWeapon = *(LAutobattler::Weapon*)(&game.npcCharacter.drop_list[0]);
    };
    
    // confirm changes
    mse::gui::Button* PlayerCharacterUpdateBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Подтвердить", {100, 70, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
    PlayerCharacterUpdateBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.playerCharacterUpdated = true;
        m_window->GetLayerManager()->Detach(this);
    };
    
    // show stats
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
    
    std::stringstream strstream;
    strstream << 
    "Класс:      " << utf8::utf32to8(main_class.c_str()) << 
    "\nПодкласс: " << utf8::utf32to8(sub_class.c_str()) <<
    "\nЗдоровье: " << game.playerCharacter.stats.health <<
    "\nСила:     " << game.playerCharacter.stats.strength <<
    "\nЛовкость: " << game.playerCharacter.stats.agility <<
    "\nВыносливость: " << game.playerCharacter.stats.endurance <<
    "\nОружие: " << utf8::utf32to8(game.playerCharacter.weapon.name.c_str());
    stats = utf8::utf8to32(strstream.str());
    
    mse::gui::Text* text4 = (mse::gui::Text*)(AddElement(new mse::gui::Text(this, stats, {010, 90, 300, 100}, {0, 64, 0, 255}, {255, 255, 128, 255})));
}

void CharacterUpdateUILayer::OnUpdate()
{}

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
    // Game GUI
    mse::gui::Button* playerCreateBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Новая игра", {10, 10, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
    playerCreateBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.gamePage = LAutobattler::GamePages::CharacterCreation;
//			mse::Canban::PutTask(mse::CanbanEvents::Backend_Create, {mse::CanbanEvents::Backend_Create, mse::CanbanReceiver::Backend, nullptr});
//			mse::Canban::PutTask(mse::CanbanEvents::Backend_Run, {mse::CanbanEvents::Backend_Run, mse::CanbanReceiver::Backend, nullptr});
//            mse::Canban::PutTask(mse::CanbanEvents::Backend_Stop, {mse::CanbanEvents::Backend_Stop, mse::CanbanReceiver::Backend, nullptr});
        m_window->GetLayerManager()->Attach(new CharacterCreateUILayer());
    };
    
    mse::gui::Button* battleBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Начать бой", {10, 20, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
    battleBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.gamePage = LAutobattler::GamePages::ArenaSetup;
        m_window->GetLayerManager()->Attach(new ArenaUILayer());
    };
    
    mse::gui::Button* playerLoadBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Загрузить", {10, 20, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
    playerLoadBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        game.gamePage = LAutobattler::GamePages::CharacterLoad;
    };
    mse::gui::Button* scoresBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Рекорды", {10, 30, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
    scoresBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
    };
    mse::gui::Button* creditsBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Титры", {10, 40, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
    creditsBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
    };
    mse::gui::Button* exitBtn = (mse::gui::Button*)(AddElement(new mse::gui::Button(this, U"Выйти", {10, 50, 80, 10}, {196, 196, 196, 255}, {32, 32, 32, 255})));
    exitBtn->callbacks[mse::EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
        mse::Canban::PutTask(mse::CanbanEvents::Backend_Stop, {mse::CanbanEvents::Backend_Stop, mse::CanbanReceiver::Backend, nullptr});
    };
}

void SimpleUILayer::OnUpdate()
{
    if (game.gamePageHasToChange)
    {
        game.gamePageFrom = game.gamePage;
        switch (game.gamePageTo)
        {
        case LAutobattler::GamePages::MainMenu:
            {
                game.gamePage = game.gamePageTo;
                game.gamePageHasToChange = false;
                break;
            }
        case LAutobattler::GamePages::CharacterUpdate:
            {
                game.gamePage = game.gamePageTo;
                game.gamePageHasToChange = false;
                m_window->GetLayerManager()->Attach(new CharacterUpdateUILayer());
                break;
            }
        case LAutobattler::GamePages::ArenaSetup:
            {
                game.gamePage = game.gamePageTo;
                game.gamePageHasToChange = false;
                m_window->GetLayerManager()->Attach(new ArenaUILayer());
                break;
            }
        }
    }
    
    switch (game.gamePage)
    {
        // idle
    case LAutobattler::GamePages::MainMenu:
        {
//			MSE_LOG("Initial screen mode");
            break;
        }
        // game
    case LAutobattler::GamePages::CharacterCreation:
        {
//				MSE_LOG("Game mode");
            break;
        }
        // settings
    case LAutobattler::GamePages::CharacterLoad:
        {
//				MSE_LOG("Settings mode");
            break;
        }
        // scores
    case LAutobattler::GamePages::Highscores:
        {
//				MSE_LOG("Scores mode");
            break;
        }
        // about
    case LAutobattler::GamePages::Credits:
        {
//				MSE_LOG("About mode");
            break;
        }
        // exit
    case LAutobattler::GamePages::Exit:
        {
            MSE_LOG("Commanding to stop the application");
            mse::Application::GetApplication()->Stop();
            break;
        }
    }
}

#endif
