#include <lesta-autobattler/gamestates.h>
#include <lesta-autobattler/layers.h>
//#include <mse/mse.h>
#include <mse/systems/platform/renderer/renderer.h>
#include <mse/systems/platform/audio/soundman.h>
#include <mse/systems/application/application.h>
#include <mse/systems/windows/window.h>
#include <mse/systems/windows/layers/layer.h>

extern LAutobattler::GameDB gameDB;
extern LAutobattler::Game game;
extern GameStateMachine gsm;

GameState::GameState()
{}

GameState::~GameState()
{}

void GameStateMachine::ChangeStateTo(LAutobattler::GamePages gamePage, mse::Layer* pass_layer)
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
    if (current->changeTo != LAutobattler::GamePages::None)
    {
        ChangeStateTo(current->changeTo);
    }
}

// ********************************************************************************************** //
//                                    STATES (to control gameplay)
// ********************************************************************************************** //

IntroPageState::IntroPageState()
{
    page = LAutobattler::GamePages::Intro;
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
        gsm.ChangeStateTo(LAutobattler::GamePages::MainMenu);
    }
    
//    MSE_LOG("IntroPageState OnUpdate...done");
    return true;
}

MainPageState::MainPageState()
{
    page = LAutobattler::GamePages::MainMenu;
}

MainPageState::~MainPageState()
{}

bool MainPageState::OnEnter(mse::Layer* pass_layer)
{
    MSE_LOG("MainPageState OnEnter...");
    layer = new MainMenuUILayer();
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    MSE_LOG("MainPageState OnEnter...done");
    return true;
}

bool MainPageState::OnExit(bool pass_layer)
{
    MSE_LOG("MainPageState OnExit...");
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
    layer = nullptr;
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
    page = LAutobattler::GamePages::CharacterCreation;
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
    game.inputClass = LAutobattler::Classes::None;
    
    return true;
}

bool CharacterCreatePageState::OnExit(bool pass_layer)
{
    MSE_LOG("CharacterCreatePageState OnExit..");
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
    layer = nullptr;
    MSE_LOG("CharacterCreatePageState OnExit..done");
    return true;
}

bool CharacterCreatePageState::OnUpdate(mse::TimeType t)
{
    if (game.inputClass != LAutobattler::Classes::None)
    {
        switch (game.inputClass)
        {
        case LAutobattler::Classes::Rogue:
            {
                game.inputTrait = LAutobattler::Traits::HiddenStrike;
                game.inputWeapon = gameDB.weapons[2];
                break;
            }
        case LAutobattler::Classes::Warrior:
            {
                game.inputTrait = LAutobattler::Traits::Rush;
                game.inputWeapon = gameDB.weapons[3];
                break;
            }
        case LAutobattler::Classes::Barbarian:
            {
                game.inputTrait = LAutobattler::Traits::Rage;
                game.inputWeapon = gameDB.weapons[4];
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
            {LAutobattler::Classes::None, 0},    // sub class
            {game.inputTrait},                    // traits
            {},  // drop
            game.inputWeapon   // weapon
        };
        printf("Character %s created! (%.2f, %.2f, %.2f, %.2f)\n", 
               game.playerCharacter.name.c_str(),
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
        
        game.inputClass = LAutobattler::Classes::None;
        
        gsm.ChangeStateTo(LAutobattler::GamePages::ArenaSetup);
    }
    
    return true;
}

CharacterLoadPageState::CharacterLoadPageState()
{
    page = LAutobattler::GamePages::CharacterLoad;
}

CharacterLoadPageState::~CharacterLoadPageState()
{}

bool CharacterLoadPageState::OnEnter(mse::Layer* pass_layer)
{
    MSE_LOG("CharacterLoadPageState OnEnter...");
    layer = new CharacterLoadUILayer();
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    MSE_LOG("CharacterLoadPageState OnEnter...done");
    return true;
}

bool CharacterLoadPageState::OnExit(bool pass_layer)
{
    MSE_LOG("CharacterLoadPageState OnExit...");
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
    layer = nullptr;
    MSE_LOG("CharacterLoadPageState OnExit...done");
    return true;
}

bool CharacterLoadPageState::OnUpdate(mse::TimeType t)
{
    return true;
}

CharacterSavePageState::CharacterSavePageState()
{
    page = LAutobattler::GamePages::CharacterSave;
}

CharacterSavePageState::~CharacterSavePageState()
{}

bool CharacterSavePageState::OnEnter(mse::Layer* pass_layer)
{
    MSE_LOG("CharacterSavePageState OnEnter...");
    layer = new CharacterSaveUILayer();
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    MSE_LOG("CharacterSavePageState OnEnter...done");
    return true;
}

bool CharacterSavePageState::OnExit(bool pass_layer)
{
    MSE_LOG("CharacterSavePageState OnExit...");
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
    layer = nullptr;
    MSE_LOG("CharacterSavePageState OnExit...done");
    return true;
}

bool CharacterSavePageState::OnUpdate(mse::TimeType t)
{
    return true;
}

CharacterUpdatePageState::CharacterUpdatePageState()
{
    page = LAutobattler::GamePages::CharacterUpdate;
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
        printf("Player lvl: %d, ", game.playerCharacter.level);
    }
    
    layer = new CharacterUpdateUILayer();
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Attach(layer);
    MSE_LOG("CharacterUpdatePageState OnEnter...done");
    return true;
}

bool CharacterUpdatePageState::OnExit(bool pass_layer)
{
    MSE_LOG("CharacterUpdatePageState OnExit...");
    mse::Renderer::GetActiveWindow()->GetLayerManager()->Detach(layer);
    layer = nullptr;
    MSE_LOG("CharacterUpdatePageState OnExit...done");
    return true;
}

bool CharacterUpdatePageState::OnUpdate(mse::TimeType t)
{
    
    if (game.playerCharacterUpdated)
    {
        // change weapon
        game.playerCharacter.weapon.name = game.inputWeapon.name;
        game.playerCharacter.weapon.type = game.inputWeapon.type;
        game.playerCharacter.weapon.damage = game.inputWeapon.damage;
        game.playerCharacter.weapon.sprite = game.inputWeapon.sprite;
        
        // update traits
        if ((game.playerCharacter.main_class.level + game.playerCharacter.sub_class.level) < gameDB.level_max)
        {
            if ((game.inputClass != LAutobattler::Classes::None) &&
                (game.inputClass != game.playerCharacter.main_class.type))
            {
                // update subclass level
                if (game.playerCharacter.sub_class.type == LAutobattler::Classes::None)
                {
                    game.playerCharacter.sub_class = {game.inputClass, 1};
                } else {
                    game.playerCharacter.sub_class.level++;
                }
                
                // update traits
                switch (game.inputClass)
                {
                case LAutobattler::Classes::Rogue:
                    {
                        printf("subclass: ROGUE, ");
                        if (game.playerCharacter.sub_class.level == 1)
                        {
                            game.playerCharacter.traits.push_back(LAutobattler::Traits::HiddenStrike);
                            printf("+trait \"Hidden Strike\"\n");
                        }
                        if (game.playerCharacter.sub_class.level == 2)
                        {
                            game.playerCharacter.traits.push_back(LAutobattler::Traits::Agile);
                            printf("+trait \"Agile\"\n");
                        }
                        break;
                    }
                case LAutobattler::Classes::Warrior:
                    {
                        printf("subclass: Warrior, ");
                        if (game.playerCharacter.sub_class.level == 1)
                        {
                            game.playerCharacter.traits.push_back(LAutobattler::Traits::Rush);
                            printf("+trait \"Rush\"\n");
                        }
                        if (game.playerCharacter.sub_class.level == 2)
                        {
                            game.playerCharacter.traits.push_back(LAutobattler::Traits::Shield);
                            printf("+trait \"Shield\"\n");
                        }
                        break;
                    }
                case LAutobattler::Classes::Barbarian:
                    {
                        printf("subclass: Barbarian, ");
                        if (game.playerCharacter.sub_class.level == 1)
                        {
                            game.playerCharacter.traits.push_back(LAutobattler::Traits::Rage);
                            printf("+trait \"Rage\"\n");
                        }
                        if (game.playerCharacter.sub_class.level == 2)
                        {
                            game.playerCharacter.traits.push_back(LAutobattler::Traits::StoneSkin);
                            printf("+trait \"Stone Skin\"\n");
                        }
                        break;
                    }
                }
            } else {
                // update main class level
                game.playerCharacter.main_class.level++;
                
                // update traits
                switch (game.playerCharacter.main_class.type)
                {
                case LAutobattler::Classes::Rogue:
                    {
                        if (game.playerCharacter.main_class.level == 2)
                        {
                            game.playerCharacter.traits.push_back(LAutobattler::Traits::Agile);
                            printf("+trait \"Agile\"\n");
                        }
                        if (game.playerCharacter.main_class.level == 3)
                        {
                            game.playerCharacter.traits.push_back(LAutobattler::Traits::Poison);
                            printf("+trait \"Poison\"\n");
                        }
                        break;
                    }
                case LAutobattler::Classes::Warrior:
                    {
                        if (game.playerCharacter.main_class.level == 2)
                        {
                            game.playerCharacter.traits.push_back(LAutobattler::Traits::Shield);
                            printf("+trait \"Shield\"\n");
                        }
                        if (game.playerCharacter.main_class.level == 3)
                        {
                            game.playerCharacter.traits.push_back(LAutobattler::Traits::Strong);
                            printf("+trait \"Strong\"\n");
                        }
                        break;
                    }
                case LAutobattler::Classes::Barbarian:
                    {
                        if (game.playerCharacter.main_class.level == 2)
                        {
                            game.playerCharacter.traits.push_back(LAutobattler::Traits::StoneSkin);
                            printf("+trait \"Stone Skin\"\n");
                        }
                        if (game.playerCharacter.main_class.level == 3)
                        {
                            game.playerCharacter.traits.push_back(LAutobattler::Traits::Survivor);
                            printf("+trait \"Surviror\"\n");
                        }
                        break;
                    }
                }
            }
        }
        
        // confirm overall player level
        game.playerCharacter.level = game.playerCharacter.main_class.level + game.playerCharacter.sub_class.level;
        
        game.playerCharacterUpdated = false;
        
        // proceed to the next battle
        gsm.ChangeStateTo(LAutobattler::GamePages::ArenaSetup);
    }
    return true;
}

ArenaSetupPageState::ArenaSetupPageState()
{
    page = LAutobattler::GamePages::ArenaSetup;
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
           game.npcCharacter.name.c_str(),
           game.npcCharacter.stats.health,
           game.npcCharacter.stats.strength,
           game.npcCharacter.stats.agility,
           game.npcCharacter.stats.endurance);
    
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
    
    // start of the battle
    if (game.playerCharacter.stats.agility > game.npcCharacter.stats.agility)
    {
        game.attacker = &(game.playerCharacter);
        game.defender = &(game.npcCharacter);
    } else {
        game.attacker = &(game.npcCharacter);
        game.defender = &(game.playerCharacter);
    }
    printf("Opponents greet each other and start the combat.\n");
    
    game.turn = 0;
    game.battleJustStarted = true;
    game.battleFinished = false;
    game.UILogger.Clear();
    
    gsm.ChangeStateTo(LAutobattler::GamePages::ArenaBattle, layer);
    
    return true;
}

ArenaBattlePageState::ArenaBattlePageState()
{
    page = LAutobattler::GamePages::ArenaBattle;
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
    float weaponDamage = 0.0;
    float skillDamage = 0.0;
    float totalDamage = 0.0;
    game.battleJustStarted = false;
    static mse::TimeType localTime = 0;
    std::stringstream strForLogger;
    
    if ((!game.battleFinished) && (game.turn < 30))
    {
        if (localTime > 300)
        {
            game.turn++;
            
            // swap roles
            if (game.turn > 1)
            {
                LAutobattler::Character* tmp = nullptr;
                tmp = game.attacker;
                game.attacker = game.defender;
                game.defender = tmp;
            }
            
            weaponDamage = game.attacker->weapon.damage;
            skillDamage = 0.0;
            totalDamage = 0.0;
            
            printf("Turn %d: %s attacks.\n",
                   game.turn,
                   game.attacker->name.c_str());
            
            // start of the turn
            // calculate attack chance
            int agi_sum = game.attacker->stats.agility + game.defender->stats.agility;
            int dice = (std::rand() % agi_sum) + 1;
//                        printf("Rolled %d of %d\n", dice, agi_sum);
            if (dice > game.defender->stats.agility)
            {
                // apply attacker's effects
                for (LAutobattler::Traits trait : game.attacker->traits)
                {   
                    if (game.turn == 1)
                    {
                        if (trait == LAutobattler::Traits::Rush)
                        {
                            weaponDamage = game.attacker->weapon.damage * 2;
                            skillDamage = game.attacker->stats.strength * 2;
                        }
                    }
                    
                    if ((trait == LAutobattler::Traits::HiddenStrike) &&
                        (game.attacker->stats.agility > game.defender->stats.agility))
                    {
                        skillDamage += 1;
                    }
                    
                    if ((trait == LAutobattler::Traits::Poison))
                    {
                        skillDamage += game.turn;
                    }
                    
                    if (trait == LAutobattler::Traits::Rage)
                    {
                        skillDamage = (game.turn < 4) ? (skillDamage + 2) : (skillDamage - 1);
                    }
                    
                    if (trait == LAutobattler::Traits::FireBreather)
                    {
                        skillDamage = ((game.turn % 3) == 0) ? skillDamage + 3 : skillDamage;
                    }
                }
                
                // apply defender's effects
                for (LAutobattler::Traits trait : game.defender->traits)
                {
                    if ((trait == LAutobattler::Traits::CuttingImmune) &&
                        (game.attacker->weapon.type == LAutobattler::DamageType::Cutting))
                    {
                        weaponDamage = 0.0;
                    }
                    
                    if ((trait == LAutobattler::Traits::CrushingWeakness) &&
                        (game.attacker->weapon.type == LAutobattler::DamageType::Crushing))
                    {
                        weaponDamage = weaponDamage * 2;
                    }
                }
                
                // pre-calculate total damage
                totalDamage = weaponDamage + skillDamage;
                
                // apply defender's effects that affect total damage
                for (LAutobattler::Traits trait : game.defender->traits)
                {
                    if (trait == LAutobattler::Traits::StoneSkin)
                    {
                        totalDamage -= game.defender->stats.endurance;
                    }
                    
                    if ((trait == LAutobattler::Traits::Shield) &&
                        (game.defender->stats.strength > game.attacker->stats.strength))
                    {
                        totalDamage -= 3;
                    }
                }
                
                if (totalDamage < 0.0)
                {
                    totalDamage = 0.0;
                }
                
                // clash
                game.defender->stats.health -= totalDamage;
                printf("Turn %d: %s deals %.2f damage to %s, leaving %.2f health.\n", 
                       game.turn, 
                       game.attacker->name.c_str(), 
                       totalDamage, 
                       game.defender->name.c_str(), 
                       game.defender->stats.health);
                
                strForLogger << "Ход " << game.turn << ": " 
                << utf8::utf32to8(game.attacker->name.c_str()) 
//                << "(" << game.attacker->stats.health << ")" 
                << " наносит " << totalDamage << " урона " 
//                << utf8::utf32to8(game.defender->name.c_str()) 
//                << "(" << game.defender->stats.health << ")"
                << "\n";
                game.UILogger.Push(utf8::utf8to32(strForLogger.str()));
            } else {
                printf("Turn %d: %s misses!\n", 
                       game.turn, 
                       game.attacker->name.c_str());
                
                strForLogger << "Ход " << game.turn << ": " 
                << utf8::utf32to8(game.attacker->name.c_str()) 
//                << "(" << game.attacker->stats.health << ")" 
                << " промахивается!\n";
                game.UILogger.Push(utf8::utf8to32(strForLogger.str()));
            }
            
            if (game.defender->stats.health <= 0.0)
            {
                game.battleFinished = true;
            }
            
            // end of the turn
            localTime = 0;
        }
    } else {
        // end of the battle
        game.battleFinished = true;
        gsm.ChangeStateTo(LAutobattler::GamePages::ArenaAftermath, layer);
        localTime = 0;
        printf("Battle is finished.\n");
    }
    
    localTime += t;
    
    return true;
}

ArenaAftermathPageState::ArenaAftermathPageState()
{
    page = LAutobattler::GamePages::ArenaAftermath;
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
        if (game.playerCharacter.stats.health > 0.0)
        {
            gsm.ChangeStateTo(LAutobattler::GamePages::Winner);
        } else {
            gsm.ChangeStateTo(LAutobattler::GamePages::GameOver);
        }
        
        game.gamePageHasToChange = false;
        game.battleFinished = false;
        
        localTime = 0;
    }
    
    return true;
}

WinnerPageState::WinnerPageState()
{
    page = LAutobattler::GamePages::Winner;
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
//            gsm.ChangeStateTo(LAutobattler::GamePages::CharacterUpdate);
//        } else {
//            gsm.ChangeStateTo(LAutobattler::GamePages::MainMenu);
//            game.battleCounter = 0;
//        }
//    }
    
    return true;
}

GameOverPageState::GameOverPageState()
{
    page = LAutobattler::GamePages::GameOver;
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
    page = LAutobattler::GamePages::Highscores;
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

CreditsPageState::CreditsPageState()
{
    page = LAutobattler::GamePages::Credits;
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
    page = LAutobattler::GamePages::Exit;
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
