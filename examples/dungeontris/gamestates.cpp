#include "dungeontris/game-fwd.h"
#include "mse/core.h"
#include "mse/utils/logger.h"
#include <dungeontris/gamestates.h>
#include <dungeontris/layers.h>
#include <dungeontris/tetris.h>
//#include <mse/mse.h>
#include <mse/systems/platform/rendererAPI.h>
#include <mse/systems/platform/audio/soundman.h>
#include <mse/systems/application/application.h>
#include <mse/systems/windows/window.h>
#include <mse/systems/windows/layers/layer.h>

extern DTetris::GameDB gameDB;
extern DTetris::Game game;
extern DTetris::TetrisMap tetrisMap;
extern DTetris::TetriminoTemplatesDB tetriminoDB;
extern DTetris::Tetrimino tetrimino;
extern DTetris::TetrisEngineData DTetris::tetrisEngineData;
extern GameStateMachine gsm;

GameState::GameState()
{}

GameState::~GameState()
{}

void GameStateMachine::ChangeStateTo(DTetris::GamePages gamePage, mse::Layer* pass_layer)
{
    MSE_LOG("GameStateMachine: change state to ", (int)gamePage);
    if (states.find(gamePage) != states.end())
    {
        MSE_LOG("GameStateMachine: state ", (int)gamePage, " found");
        if (current != nullptr)
        {
            if (gamePage != current->page)
            {
                while (!current->OnExit((pass_layer != nullptr))){}
                MSE_LOG("GameStateMachine: state ", (int)(current->page), " exited");
                
                current = states[gamePage];
                while(!current->OnEnter(pass_layer)){}
                MSE_LOG("GameStateMachine: state ", (int)(current->page), " entered");
                
                return;
            } else {
                MSE_LOG("GameStateMachine: no change - current state is already ", (int)gamePage);
                return;
            }
        } else {
            // the program just started - there are no previous stages yet
            current = states[gamePage];
            while(!current->OnEnter(pass_layer)){}
            MSE_LOG("GameStateMachine: state ", (int)(current->page), " entered");
        }
    } else {
        MSE_LOG("GameStateMachine: cannot change to state ", (int)gamePage, " - none found");
    }
}

void GameStateMachine::OnUpdate(mse::TimeType t)
{
    current->OnUpdate(t);
    if (current->changeTo != DTetris::GamePages::None)
    {
        ChangeStateTo(current->changeTo);
    }
}

// ********************************************************************************************** //
//                                    STATES (to control gameplay)
// ********************************************************************************************** //

IntroPageState::IntroPageState()
{
    page = DTetris::GamePages::Intro;
}

IntroPageState::~IntroPageState()
{}

bool IntroPageState::OnEnter(mse::Layer* pass_layer)
{
    MSE_LOG("IntroPageState OnEnter...");
    if (pass_layer != nullptr)
    {
        if (layer != pass_layer)
        {
            layer = pass_layer;
        }
    } else {
        layer = new IntroUILayer();
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    }
    MSE_LOG("IntroPageState OnEnter...done");
    return true;
}

bool IntroPageState::OnExit(bool pass_layer)
{
    MSE_LOG("IntroPageState OnExit...");
    if (!pass_layer)
    {
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
        layer = nullptr;
    }
    MSE_LOG("IntroPageState OnExit...done");
    return true;
}

bool IntroPageState::OnUpdate(mse::TimeType t)
{
//    MSE_LOG("IntroPageState OnUpdate...");
//    MSE_LOG((int)(t));
//    MSE_LOG((int)(1.0/MSE_FPS60));
    if ((timeLast - t) > 10*(int)(1.0/MSE_FPS60))
    {
        timeLast = 0;
        --counter;
    }
    timeLast += t;
    
    if (counter < 0)
    {
        gsm.ChangeStateTo(DTetris::GamePages::MainMenu);
    }
    
//    MSE_LOG("IntroPageState OnUpdate...done");
    return true;
}

MainPageState::MainPageState()
{
    page = DTetris::GamePages::MainMenu;
}

MainPageState::~MainPageState()
{}

bool MainPageState::OnEnter(mse::Layer* pass_layer)
{
    MSE_LOG("MainPageState OnEnter...");
    if (pass_layer != nullptr)
    {
        if (layer != pass_layer)
        {
            layer = pass_layer;
        }
    } else {
        layer = new MainMenuUILayer();
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    }
    MSE_LOG("MainPageState OnEnter...done");
    return true;
}

bool MainPageState::OnExit(bool pass_layer)
{
    MSE_LOG("MainPageState OnExit...");
    if (!pass_layer)
    {
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
        layer = nullptr;
    }
    MSE_LOG("MainPageState OnExit...done");
    return true;
}

bool MainPageState::OnUpdate(mse::TimeType t)
{
//    MSE_LOG("MainPageState OnUpdate...");
//    MSE_LOG("MainPageState OnUpdate...done");
    return true;
}

CharacterCreatePageState::CharacterCreatePageState()
{
    page = DTetris::GamePages::CharacterCreation;
}

CharacterCreatePageState::~CharacterCreatePageState()
{}

bool CharacterCreatePageState::OnEnter(mse::Layer* pass_layer)
{
    MSE_LOG("CharacterCreatePageState OnEnter...");
    layer = new CharacterCreateUILayer();
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    MSE_LOG("CharacterCreatePageState OnEnter...done");
    
    // reset game
    game.battleCounter = 0;
    game.battleFinished = false;
    game.battleJustStarted = false;
    game.battleJustFinished = false;
    game.inputClass = DTetris::Classes::None;

    game.shop_traits.clear();
    game.shop_weapons.clear();
    
    return true;
}

bool CharacterCreatePageState::OnExit(bool pass_layer)
{
    MSE_LOG("CharacterCreatePageState OnExit..");
    if (!pass_layer)
    {
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
        layer = nullptr;
    }
    MSE_LOG("CharacterCreatePageState OnExit..done");
    return true;
}

bool CharacterCreatePageState::OnUpdate(mse::TimeType t)
{
    if (game.inputClass != DTetris::Classes::None)
    {
        switch (game.inputClass)
        {
        case DTetris::Classes::Rogue:
            {
                game.inputTrait = DTetris::Traits::HiddenStrike;
                game.inputWeapon = gameDB.weapons[2];
                break;
            }
        case DTetris::Classes::Warrior:
            {
                game.inputTrait = DTetris::Traits::Rush;
                game.inputWeapon = gameDB.weapons[3];
                break;
            }
        case DTetris::Classes::Barbarian:
            {
                game.inputTrait = DTetris::Traits::Rage;
                game.inputWeapon = gameDB.weapons[4];
                break;
            }
        case DTetris::Classes::Monk:
        {
            game.inputTrait = DTetris::Traits::Meditation;
            game.inputWeapon = gameDB.weapons[0];
            break;
        }
        case DTetris::Classes::Cultist:
        {
            game.inputTrait = DTetris::Traits::MarkOfChaos;
            game.inputWeapon = gameDB.weapons[0];
            break;
        }
        case DTetris::Classes::Witcher:
        {
            game.inputTrait = DTetris::Traits::SchoolOfBeaver;
            game.inputWeapon = gameDB.weapons[3];
            break;
        }
        default:
            {
                printf("can't create player character: undefined class\n");
            }
        }
        
        game.playerCharacter = 
        {
            1,                     // level
            U"Игрок",              // name
            game.inputRace,         // race
            game.inputStats,          // stats_max
            game.inputStats,          // stats
            {game.inputClass, 1},    // main class
            {DTetris::Classes::None, 0},    // sub class
            {game.inputTrait},                    // traits
            {},  // drop
            gameDB.classes[game.inputClass].sprite_whole_area, // sprite_whole_area
            gameDB.classes[game.inputClass].sprite_handle_area, // sprite_handle_area
            gameDB.classes[game.inputClass].sprite_head_area, // sprite_head_area
            gameDB.classes[game.inputClass].sprite_head_special_area, // sprite_head_special_area
            game.inputWeapon   // weapon
        };

        printf("Character %s created! (%.2f, %.2f, %.2f, %.2f)\n", 
               (char*)(game.playerCharacter.name.c_str()),
               game.playerCharacter.stats.health,
               game.playerCharacter.stats.strength,
               game.playerCharacter.stats.agility,
               game.playerCharacter.stats.endurance);
        printf("Attempted to create: (%.2f, %.2f, %.2f, %.2f)\n", 
               game.inputStats.health,
               game.inputStats.strength,
               game.inputStats.agility,
               game.inputStats.endurance);
        
        game.battleCounter = 0;
        
        game.inputClass = DTetris::Classes::None;
        
        // gsm.ChangeStateTo(DTetris::GamePages::ArenaSetup);
    }
    
    return true;
}

CharacterLoadPageState::CharacterLoadPageState()
{
    page = DTetris::GamePages::CharacterLoad;
}

CharacterLoadPageState::~CharacterLoadPageState()
{}

bool CharacterLoadPageState::OnEnter(mse::Layer* pass_layer)
{
    MSE_LOG("CharacterLoadPageState OnEnter...");
    if (pass_layer != nullptr)
    {
        if (layer != pass_layer)
        {
            layer = pass_layer;
        }
    } else {
        layer = new CharacterLoadUILayer();
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    }
    MSE_LOG("CharacterLoadPageState OnEnter...done");
    return true;
}

bool CharacterLoadPageState::OnExit(bool pass_layer)
{
    MSE_LOG("CharacterLoadPageState OnExit...");
    if (!pass_layer)
    {
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
        layer = nullptr;
    }
    MSE_LOG("CharacterLoadPageState OnExit...done");
    return true;
}

bool CharacterLoadPageState::OnUpdate(mse::TimeType t)
{
    return true;
}

CharacterSavePageState::CharacterSavePageState()
{
    page = DTetris::GamePages::CharacterSave;
}

CharacterSavePageState::~CharacterSavePageState()
{}

bool CharacterSavePageState::OnEnter(mse::Layer* pass_layer)
{
    MSE_LOG("CharacterSavePageState OnEnter...");
    if (pass_layer != nullptr)
    {
        if (layer != pass_layer)
        {
            layer = pass_layer;
        }
    } else {
        layer = new CharacterSaveUILayer();
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    }
    MSE_LOG("CharacterSavePageState OnEnter...done");
    return true;
}

bool CharacterSavePageState::OnExit(bool pass_layer)
{
    MSE_LOG("CharacterSavePageState OnExit...");
    if (!pass_layer)
    {
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
        layer = nullptr;
    }
    MSE_LOG("CharacterSavePageState OnExit...done");
    return true;
}

bool CharacterSavePageState::OnUpdate(mse::TimeType t)
{
    return true;
}

CharacterUpdatePageState::CharacterUpdatePageState()
{
    page = DTetris::GamePages::CharacterUpdate;
}

CharacterUpdatePageState::~CharacterUpdatePageState()
{}

bool CharacterUpdatePageState::OnEnter(mse::Layer* pass_layer)
{
    MSE_LOG("CharacterUpdatePageState OnEnter...");
    
    game.battleJustFinished = false; // important for layer management
    
    if (game.playerCharacter.level < 3)
    {
        game.playerCharacter.level++;
        printf("Player lvl: %d, ", (int)game.playerCharacter.level);
    }
    
    if (pass_layer != nullptr)
    {
        if (layer != pass_layer)
        {
            layer = pass_layer;
        }
    } else {
        layer = new CharacterUpdateUILayer();
        ((CharacterUpdateUILayer*)layer)->shopLayer = new ShopUILayer();
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    }
    MSE_LOG("CharacterUpdatePageState OnEnter...done");
    return true;
}

bool CharacterUpdatePageState::OnExit(bool pass_layer)
{
    MSE_LOG("CharacterUpdatePageState OnExit...");
    if (!pass_layer)
    {
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
        layer = nullptr;
    }
    MSE_LOG("CharacterUpdatePageState OnExit...done");
    return true;
}

bool CharacterUpdatePageState::OnUpdate(mse::TimeType t)
{
    
    if (game.playerCharacterUpdated)
    {
        // change weapon
        game.playerCharacter.weapon = game.inputWeapon;
        
        // update traits
        if ((game.playerCharacter.main_class.level + game.playerCharacter.sub_class.level) < gameDB.level_max)
        {
            if ((game.inputClass != DTetris::Classes::None) &&
                (game.inputClass != game.playerCharacter.main_class.type))
            {
                // update subclass level
                if (game.playerCharacter.sub_class.type == DTetris::Classes::None)
                {
                    game.playerCharacter.sub_class = {game.inputClass, 1};
                } else {
                    game.playerCharacter.sub_class.level++;
                }
                
                // update traits
                switch (game.inputClass)
                {
                case DTetris::Classes::Rogue:
                    {
                        printf("subclass: ROGUE, ");
                        if (game.playerCharacter.sub_class.level == 1)
                        {
                            game.playerCharacter.traits.push_back(DTetris::Traits::HiddenStrike);
                            printf("+trait \"Hidden Strike\"\n");
                        }
                        if (game.playerCharacter.sub_class.level == 2)
                        {
                            game.playerCharacter.traits.push_back(DTetris::Traits::Agile);
                            printf("+trait \"Agile\"\n");
                        }
                        break;
                    }
                case DTetris::Classes::Warrior:
                    {
                        printf("subclass: Warrior, ");
                        if (game.playerCharacter.sub_class.level == 1)
                        {
                            game.playerCharacter.traits.push_back(DTetris::Traits::Rush);
                            printf("+trait \"Rush\"\n");
                        }
                        if (game.playerCharacter.sub_class.level == 2)
                        {
                            game.playerCharacter.traits.push_back(DTetris::Traits::Shield);
                            printf("+trait \"Shield\"\n");
                        }
                        break;
                    }
                case DTetris::Classes::Barbarian:
                    {
                        printf("subclass: Barbarian, ");
                        if (game.playerCharacter.sub_class.level == 1)
                        {
                            game.playerCharacter.traits.push_back(DTetris::Traits::Rage);
                            printf("+trait \"Rage\"\n");
                        }
                        if (game.playerCharacter.sub_class.level == 2)
                        {
                            game.playerCharacter.traits.push_back(DTetris::Traits::StoneSkin);
                            printf("+trait \"Stone Skin\"\n");
                        }
                        break;
                    }
                default:
                    {}
                }
            } else {
                // update main class level
                game.playerCharacter.main_class.level++;
                
                // update traits
                switch (game.playerCharacter.main_class.type)
                {
                case DTetris::Classes::Rogue:
                    {
                        if (game.playerCharacter.main_class.level == 2)
                        {
                            game.playerCharacter.traits.push_back(DTetris::Traits::Agile);
                            printf("+trait \"Agile\"\n");
                        }
                        if (game.playerCharacter.main_class.level == 3)
                        {
                            game.playerCharacter.traits.push_back(DTetris::Traits::Poison);
                            printf("+trait \"Poison\"\n");
                        }
                        break;
                    }
                case DTetris::Classes::Warrior:
                    {
                        if (game.playerCharacter.main_class.level == 2)
                        {
                            game.playerCharacter.traits.push_back(DTetris::Traits::Shield);
                            printf("+trait \"Shield\"\n");
                        }
                        if (game.playerCharacter.main_class.level == 3)
                        {
                            game.playerCharacter.traits.push_back(DTetris::Traits::Strong);
                            printf("+trait \"Strong\"\n");
                        }
                        break;
                    }
                case DTetris::Classes::Barbarian:
                    {
                        if (game.playerCharacter.main_class.level == 2)
                        {
                            game.playerCharacter.traits.push_back(DTetris::Traits::StoneSkin);
                            printf("+trait \"Stone Skin\"\n");
                        }
                        if (game.playerCharacter.main_class.level == 3)
                        {
                            game.playerCharacter.traits.push_back(DTetris::Traits::Survivor);
                            printf("+trait \"Surviror\"\n");
                        }
                        break;
                    }
                default:
                    {}
                }
            }
        }
        
        // confirm overall player level
        game.playerCharacter.level = game.playerCharacter.main_class.level + game.playerCharacter.sub_class.level;
        
        game.playerCharacterUpdated = false;
        
        // proceed to the next battle
        gsm.ChangeStateTo(DTetris::GamePages::ArenaSetup);
    }
    return true;
}

ArenaSetupPageState::ArenaSetupPageState()
{
    page = DTetris::GamePages::ArenaSetup;
}

ArenaSetupPageState::~ArenaSetupPageState()
{}

bool ArenaSetupPageState::OnEnter(mse::Layer* pass_layer)
{
    MSE_LOG("ArenaSetupPageState OnEnter...");
    
    // pick npc adversary
    int npcCount = gameDB.characters.size();
    int pickedCharacter = std::rand() % npcCount;
    game.npcCharacter = gameDB.characters[pickedCharacter];
    
    printf("Next opponent (of %d) is %s! (%.2f, %.2f, %.2f, %.2f)\n",
           npcCount,
           (char*)(game.npcCharacter.name.c_str()),
           game.npcCharacter.stats.health,
           game.npcCharacter.stats.strength,
           game.npcCharacter.stats.agility,
           game.npcCharacter.stats.endurance);
    
    // configure tetrisMap
    tetrisMap.Resize(12, 20);
    tetrisMap.Clear();
    tetrimino.blocks.resize(16);
    tetrimino.x = 4;
    tetrimino.y = 0;

    // pick random first tetrimino
    DTetris::TetrisEngine_NewBlock();
    
    // configure GUI layer
    if ((pass_layer != nullptr) && (layer != pass_layer))
    {
        layer = pass_layer;
    } else {
        layer = new ArenaUILayer();
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    }
    MSE_LOG("ArenaSetupPageState OnEnter...done");
    return true;
}

bool ArenaSetupPageState::OnExit(bool pass_layer)
{
    MSE_LOG("ArenaSetupPageState OnExit...");
    if (!pass_layer)
    {
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
        layer = nullptr;
    }
    MSE_LOG("ArenaSetupPageState OnExit...done");
    return true;
}

bool ArenaSetupPageState::OnUpdate(mse::TimeType t)
{
    printf("Preparing battle...\n");
    game.battleCounter++;
    
    // calculate actual player stats
    game.playerCharacter.stats = game.playerCharacter.stats_max;
    game.playerCharacter.stats = 
    {
        game.playerCharacter.level * (game.playerCharacter.stats_max.health + game.playerCharacter.stats_max.endurance),
        game.playerCharacter.stats_max.strength,
        game.playerCharacter.stats_max.agility,
        game.playerCharacter.stats_max.endurance,
    };
    
    // apply stats passive traits
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
    
    // start of the battle
    if (game.playerCharacter.stats.agility > game.npcCharacter.stats.agility)
    {
        game.attacker = &(game.playerCharacter);
        game.defender = &(game.npcCharacter);
    } else {
        game.attacker = &(game.npcCharacter);
        game.defender = &(game.playerCharacter);
    }

    game.attacker = &(game.playerCharacter);
    game.defender = &(game.npcCharacter);

    printf("Opponents greet each other and start the combat.\n");
    
    game.turn = 0;
    game.battleJustStarted = true;
    game.battleFinished = false;
    game.UILogger.Clear();

    game.clearedBlocks[DTetris::BlockType::Block] = 0;
    game.clearedBlocks[DTetris::BlockType::Attack] = 0;
    game.clearedBlocks[DTetris::BlockType::Defense] = 0;
    game.clearedBlocks[DTetris::BlockType::Healing] = 0;
    game.clearedBlocks[DTetris::BlockType::Treasure] = 0;
    game.playerAttacks = false;
    game.turnNext = false;

    gsm.ChangeStateTo(DTetris::GamePages::ArenaBattle, layer);
    
    return true;
}

ArenaBattlePageState::ArenaBattlePageState()
{
    page = DTetris::GamePages::ArenaBattle;
}

ArenaBattlePageState::~ArenaBattlePageState()
{}

bool ArenaBattlePageState::OnEnter(mse::Layer* pass_layer)
{
    MSE_LOG("ArenaBattlePageState OnEnter...");
    if (pass_layer != nullptr)
    {
        if (layer != pass_layer)
        {
            layer = pass_layer;
        }
    } else {
        layer = new ArenaUILayer();
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    }

    tetrisMap.Clear();
    DTetris::tetrisEngineData.animateLinesRemove = false;
    DTetris::tetrisEngineData.removeLines = false;
    DTetris::tetrisEngineData.justPlacedTetrimino = false;
    DTetris::tetrisEngineData.state = DTetris::TetrisStates::Processing;
    MSE_LOG("Tetris state: ", (int)(DTetris::tetrisEngineData.state));
    MSE_LOG("ArenaBattlePageState OnEnter...done");
    return true;
}

bool ArenaBattlePageState::OnExit(bool pass_layer)
{
    MSE_LOG("ArenaBattlePageState OnExit...");
    if (!pass_layer)
    {
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
        layer = nullptr;
    }
    MSE_LOG("ArenaBattlePageState OnExit...done");
    return true;
}

bool ArenaBattlePageState::OnUpdate(mse::TimeType t)
{
    game.battleJustStarted = false;
    std::stringstream strForLogger;

    if (!game.battleFinished)
    {
        if (!game.paused)
        {
            if ((localTime - lastLocalTime) >= DTetris::tetrisEngineData.gameSpeedMax) //game.config.turnLength)
            {
                if (DTetris::tetrisEngineData.forceDownMovement || (localTime - lastLocalTime) >= 200)
                {
                    lastLocalTime = localTime;

                    // ###################################################################################
                    //                         BASIC TETRIS logic
                    // ###################################################################################
                    // 1. Pick tetrimino.
                    switch (DTetris::tetrisEngineData.state)
                    {
                        case DTetris::TetrisStates::Processing:
                        {
                            if (DTetris::tetrisEngineData.justPlacedTetrimino)
                            {
                                DTetris::TetrisEngine_NewBlock();
                            }

                            if (DTetris::tetrisEngineData.rotateTetrimino)
                            {
                                if (DTetris::TetrisEngine_rotateAllowed(&tetrimino))
                                {
                                    DTetris::TetrisEngine_Rotate(&tetrimino, DTetris::TetrisRotateDirection::Clockwise);
                                }
                                DTetris::tetrisEngineData.rotateTetrimino = false;
                            }

                            // 2. Check if can move tetrimino (and keep track of its movement counts).
                            bool canMoveDown, canMoveSide = false;
                            if (DTetris::tetrisEngineData.moveDirection != DTetris::TetrisMoveDirection::Down)
                            {
                                canMoveSide = DTetris::TetrisEngine_moveAllowed(&tetrimino, DTetris::tetrisEngineData.moveDirection);
                            }
                            canMoveDown = DTetris::TetrisEngine_moveAllowed(&tetrimino, DTetris::TetrisMoveDirection::Down);


                            if ((!canMoveDown && !canMoveSide))
                            {
                                if (DTetris::tetrisEngineData.justPlacedTetrimino)
                                {
                                    DTetris::tetrisEngineData.state = DTetris::TetrisStates::GameOver;
                                } else {
                                    // 3.2.1. Place the tetrimino.
                                    DTetris::TetrisEngine_AddBlock(&tetrimino);
                                    DTetris::tetrisEngineData.justPlacedTetrimino = true;
                                    DTetris::GameNextTurn(game);
                                    DTetris::tetrisEngineData.state = DTetris::TetrisStates::Autobattle;
                                }
                            } else {
                                if (DTetris::tetrisEngineData.justPlacedTetrimino)
                                {
                                    DTetris::tetrisEngineData.justPlacedTetrimino = false;
                                }

                                if (canMoveSide)
                                {
                                    DTetris::TetrisEngine_Move(&tetrimino, DTetris::tetrisEngineData.moveDirection);
                                    DTetris::tetrisEngineData.moveDirection = DTetris::TetrisMoveDirection::Down;
                                } else {
                                    DTetris::tetrisEngineData.moveDirection = DTetris::TetrisMoveDirection::Down;
                                    DTetris::TetrisEngine_Move(&tetrimino, DTetris::tetrisEngineData.moveDirection);
                                }
                            }

                            // 4. Check for full lines.
                            if (DTetris::tetrisEngineData.justPlacedTetrimino)
                            {
                                int actualWidth = tetrisMap.width - 2;
                                int actualHeight = tetrisMap.height - 1;
                                for (int y = 1; y < actualHeight; ++y)
                                {
                                    int yFactor = y * tetrisMap.width;
                                    int filled = 0;
                                    for (int x = 1; x < tetrisMap.width - 1; ++x)
                                    {
                                        if (tetrisMap.map[x + yFactor].type != DTetris::BlockType::None)
                                        {
                                            filled++;
                                        }
                                    }

                                    // 4.1. If a line is full, clear it.
                                    if (filled == actualWidth)
                                    {
                                        MSE_LOG("Here's a line filled: ", y);
                                        for (int x = 1; x < tetrisMap.width - 1; ++x)
                                        {
                                            tetrisMap.map[x + yFactor].remove = true;
                                            game.clearedBlocks[tetrisMap.map[x + yFactor].type]++;
                                        }

                                        if (DTetris::tetrisEngineData.linesFilled.find(y) == DTetris::tetrisEngineData.linesFilled.end())
                                        {
                                            // register the newly empty line
                                            MSE_LOG("Registered line ", y, " to remove");
                                            DTetris::tetrisEngineData.linesFilled[y] = true;
                                            DTetris::tetrisEngineData.removeLines = true;
                                        }
                                    }
                                }

                                if (DTetris::tetrisEngineData.removeLines)
                                {
                                    // start the lines removal animation
                                    DTetris::tetrisEngineData.animateLinesRemove = true;

                                    DTetris::tetrisEngineData.state = DTetris::TetrisStates::ClearLines;
                                    game.playerAttacks = true;

                                    DTetris::tetrisEngineData.removeLines = false;
                                }
                            }

                            break;
                        }
                        case DTetris::TetrisStates::Paused:
                        {
                            break;
                        }
                        case DTetris::TetrisStates::GameOver:
                        {
                            game.battleFinished = true;
                            break;
                        }
                        case DTetris::TetrisStates::ClearLines:
                        {
                            // 5. Check for registered empty lines.
                            // 5.1. If a line is empty, move the line from the top of it to its place.
                            if (!DTetris::tetrisEngineData.animateLinesRemove)
                            {
                                tetrisMap.ClearLines(DTetris::tetrisEngineData.linesFilled);

                                // DTetris::tetrisEngineData.linesFilled.clear();
                                DTetris::tetrisEngineData.linesFilled.erase(
                                    DTetris::tetrisEngineData.linesFilled.begin(),
                                    DTetris::tetrisEngineData.linesFilled.end()
                                );

                                // heal player character
                                game.AccumulateHeal(game.playerCharacter);
                                game.Heal(game.playerCharacter);

                                DTetris::tetrisEngineData.state = DTetris::TetrisStates::Autobattle;
                            }

                            break;
                        }
                        case DTetris::TetrisStates::Autobattle:
                        {
                            // ###################################################################################
                            //                         BASIC AUTOBATLER logic
                            // ###################################################################################
                            if (game.playerAttacks)
                            {
                                game.AccumulateAttack(game.playerCharacter);
                                game.AccumulateDefense(game.playerCharacter);
                                game.AccumulateTreasure(game.playerCharacter);

                                game.Attack(game.playerCharacter, game.npcCharacter, strForLogger);
                                game.playerAttacks = false;
                            }

                            if (game.turn % (7 - (int)roundf(game.npcCharacter.stats.agility)) == 0)
                            {
                                game.Attack(game.npcCharacter, game.playerCharacter, strForLogger);
                            }

                            DTetris::tetrisEngineData.state = DTetris::TetrisStates::Processing;

                            break;
                        }
                        case DTetris::TetrisStates::Win:
                        {
                            game.battleFinished = true;
                            break;
                        }
                        case DTetris::TetrisStates::None:
                        default:
                        {
                            break;
                        }
                    }
                }
            }
        }
    } else {
        // end of the battle
        if (game.playerCharacter.stats.health > 0.0 && DTetris::tetrisEngineData.state != DTetris::TetrisStates::GameOver)
        {
            DTetris::tetrisEngineData.state = DTetris::TetrisStates::Win;
        } else {
            DTetris::tetrisEngineData.state = DTetris::TetrisStates::GameOver;
        }

        gsm.ChangeStateTo(DTetris::GamePages::ArenaAftermath, layer);
        localTime = 0;
        printf("Battle is finished.\n");
    }
    
    localTime += t;
    
    return true;
}

ArenaAftermathPageState::ArenaAftermathPageState()
{
    page = DTetris::GamePages::ArenaAftermath;
}

ArenaAftermathPageState::~ArenaAftermathPageState()
{}

bool ArenaAftermathPageState::OnEnter(mse::Layer* pass_layer)
{
    MSE_LOG("ArenaAftermathPageState OnEnter...");
    if (pass_layer != nullptr)
    {
        if (layer != pass_layer)
        {
            layer = pass_layer;
        }
    } else {
        layer = new ArenaUILayer();
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    }
    
    if (game.playerCharacter.stats.health > 0.0)
    {
        game.UILogger.Push(U"Победа!");
    } else {
        game.UILogger.Push(U"Поражение.");
    }
    
    MSE_LOG("ArenaAftermathPageState OnEnter...done");
    return true;
}

bool ArenaAftermathPageState::OnExit(bool pass_layer)
{    
    MSE_LOG("ArenaAftermathPageState OnExit...");
    if (!pass_layer)
    {
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
        layer = nullptr;
    }
    
    MSE_LOG("ArenaAftermathPageState OnExit...done");
    return true;
}

bool ArenaAftermathPageState::OnUpdate(mse::TimeType t)
{
    localTime += t;
    if ((localTime > 1000) && game.gamePageHasToChange)
    {
        if (game.playerCharacter.stats.health > 0.0 && DTetris::tetrisEngineData.state != DTetris::TetrisStates::GameOver)
        {
            gsm.ChangeStateTo(DTetris::GamePages::Winner);
        } else {
            gsm.ChangeStateTo(DTetris::GamePages::GameOver);
        }
        
        game.gamePageHasToChange = false;
        game.battleFinished = false;
        
        localTime = 0;
    }
    
    return true;
}

WinnerPageState::WinnerPageState()
{
    page = DTetris::GamePages::Winner;
}

WinnerPageState::~WinnerPageState()
{}

bool WinnerPageState::OnEnter(mse::Layer* pass_layer)
{
    MSE_LOG("WinnerPageState OnEnter...");
    if (pass_layer != nullptr)
    {
        if (layer != pass_layer)
        {
            layer = pass_layer;
        }
    } else {
        layer = new WinnerUILayer();
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    }
    MSE_LOG("WinnerPageState OnEnter...done");
    return true;
}

bool WinnerPageState::OnExit(bool pass_layer)
{
    MSE_LOG("WinnerPageState OnExit...");
    current_time = 0;
    if (!pass_layer)
    {
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
        layer = nullptr;
    }
    MSE_LOG("WinnerPageState OnExit...done");
    return true;
}

bool WinnerPageState::OnUpdate(mse::TimeType t)
{
//    if (current_time < 2000)
//    {
//        current_time += t;
//    } else {
//        current_time = 0;
//        if (game.battleCounter < 5)
//        {
//            game.battleJustFinished = true; // important for layer management
//            gsm.ChangeStateTo(DTetris::GamePages::CharacterUpdate);
//        } else {
//            gsm.ChangeStateTo(DTetris::GamePages::MainMenu);
//            game.battleCounter = 0;
//        }
//    }
    
    return true;
}

GameOverPageState::GameOverPageState()
{
    page = DTetris::GamePages::GameOver;
}

GameOverPageState::~GameOverPageState()
{}

bool GameOverPageState::OnEnter(mse::Layer* pass_layer)
{
    MSE_LOG("GameOverPageState OnEnter...");
    if (pass_layer != nullptr)
    {
        if (layer != pass_layer)
        {
            layer = pass_layer;
        }
    } else {
        layer = new GameOverUILayer();
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    }
    MSE_LOG("GameOverPageState OnEnter...done");
    return true;
}

bool GameOverPageState::OnExit(bool pass_layer)
{
    MSE_LOG("GameOverPageState OnExit...");
    if (!pass_layer)
    {
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
        layer = nullptr;
    }
    MSE_LOG("GameOverPageState OnExit...done");
    return true;
}

bool GameOverPageState::OnUpdate(mse::TimeType t)
{
    return true;
}

HighscoresPageState::HighscoresPageState()
{
    page = DTetris::GamePages::Highscores;
}

HighscoresPageState::~HighscoresPageState()
{}

bool HighscoresPageState::OnEnter(mse::Layer* pass_layer)
{
    MSE_LOG("HighscoresPageState OnEnter...");
    if (pass_layer != nullptr)
    {
        if (layer != pass_layer)
        {
            layer = pass_layer;
        }
    } else {
        layer = new HighscoresUILayer();
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    }
    MSE_LOG("HighscoresPageState OnEnter...done");
    return true;
}

bool HighscoresPageState::OnExit(bool pass_layer)
{
    MSE_LOG("HighscoresPageState OnExit...");
    if (!pass_layer)
    {
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
        layer = nullptr;
    }
    MSE_LOG("HighscoresPageState OnExit...done");
    return true;
}

bool HighscoresPageState::OnUpdate(mse::TimeType t)
{
    return true;
}

SettingsPageState::SettingsPageState()
{
    page = DTetris::GamePages::Settings;
}

SettingsPageState::~SettingsPageState()
{}

bool SettingsPageState::OnEnter(mse::Layer* pass_layer)
{
    MSE_LOG("SettingsPageState OnEnter...");
    if (pass_layer != nullptr)
    {
        if (layer != pass_layer)
        {
            layer = pass_layer;
        }
    } else {
        layer = new SettingsUILayer();
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    }
    MSE_LOG("SettingsPageState OnEnter...done");
    return true;
}

bool SettingsPageState::OnExit(bool pass_layer)
{
    MSE_LOG("SettingsPageState OnExit...");
    if (!pass_layer)
    {
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
        layer = nullptr;
    }
    MSE_LOG("SettingsPageState OnExit...done");
    return true;
}

bool SettingsPageState::OnUpdate(mse::TimeType t)
{
    return true;
}

CreditsPageState::CreditsPageState()
{
    page = DTetris::GamePages::Credits;
}

CreditsPageState::~CreditsPageState()
{}

bool CreditsPageState::OnEnter(mse::Layer* pass_layer)
{
    MSE_LOG("CreditsPageState OnEnter...");
    if (pass_layer != nullptr)
    {
        if (layer != pass_layer)
        {
            layer = pass_layer;
        }
    } else {
        layer = new CreditsUILayer();
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    }
    
    mse::SoundMan::PlayTrack("data/audio/tracks/02_our_tanya_cries_loud.mp3");
    MSE_LOG("CreditsPageState OnEnter...done");
    return true;
}

bool CreditsPageState::OnExit(bool pass_layer)
{
    MSE_LOG("CreditsPageState OnExit...");
    if (!pass_layer)
    {
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
        layer = nullptr;
    }
    mse::SoundMan::StopTrack();
    MSE_LOG("CreditsPageState OnExit...done");
    return true;
}

bool CreditsPageState::OnUpdate(mse::TimeType t)
{
    return true;
}

ExitPageState::ExitPageState()
{
    page = DTetris::GamePages::Exit;
}

ExitPageState::~ExitPageState()
{}

bool ExitPageState::OnEnter(mse::Layer* pass_layer)
{
    MSE_LOG("ExitPageState OnEnter...");
    if (pass_layer != nullptr)
    {
        if (layer != pass_layer)
        {
            layer = pass_layer;
        }
    } else {
        layer = new ExitUILayer();
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    }
    MSE_LOG("ExitPageState OnEnter...done");
    return true;
}

bool ExitPageState::OnExit(bool pass_layer)
{
    MSE_LOG("ExitPageState OnExit...");
    if (!pass_layer)
    {
        mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
        layer = nullptr;
    }
    MSE_LOG("ExitPageState OnExit...done");
    return true;
}

bool ExitPageState::OnUpdate(mse::TimeType t)
{
    mse::Application::GetApplication()->Stop();
    return true;
}
