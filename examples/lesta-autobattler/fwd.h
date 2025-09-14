#ifndef LESTA_AUTOBATTLER_FWD_H
#define LESTA_AUTOBATTLER_FWD_H

#include <mse/mse.h>

// globals
namespace LAutobattler
{
    const size_t level_max = 3;

    enum class GamePages
    {
        None,
        Intro,
        MainMenu,
        CharacterCreation,
        CharacterLoad,
        CharacterSave,
        CharacterUpdate,
        ArenaSetup,
        ArenaBattle,
        ArenaAftermath,
        Highscores,
        Credits,
        Exit
    };

    class GameState
    {
        GamePages id = GamePages::None;

        GameState() {};
        virtual ~GameState() {};

        virtual void Enter() {};
        virtual void Logic();
        virtual void GUI() {};
        virtual bool ExitTo(GamePages state)
        {
            return true;
        };
    };
    
    /*
    Possible usage:
        states_machine.state.logic();
        states_machine.state.GUI();
    
    But seriously. Do I need a state machine here?
    */
    class GameStateMachine
    {
        std::unordered_map<GamePages, GameState> statesMap = {};
        GameState state;
    };

    enum class Races
    {
        None,
        Human,
        Goblin,
        Skeleton,
        Slime,
        Ghost,
        Golem,
        Dragon
    };

    enum class Classes
    {
        None,
        Rogue,
        Warrior,
        Barbarian
    };

    class Class
    {
        Classes type = Classes::None;
        size_t level = 0;
    };

    enum class DamageType
    {
        None,
        Cutting,
        Crushing,
        Piercing,
        Divine,
    };

    struct CharacterStats
    {
        float health = 0.0;
        float strength = 0.0;
        float agility = 0.0;
        float endurance = 0.0;
    };

    struct Item
    {
        std::u32string name;
    };

    struct Weapon : Item
    {
        float damage = 0.0;
        DamageType type = DamageType::None;
    };

    struct Armor : Item
    {

        float weapon_defence_cutting = 0.0;
        float weapon_defence_crushing = 0.0;
        float weapon_defence_pierceing = 0.0;
        float skill_defence = 0.0;
        float stats_defence = 0.0;

    };

    enum class Traits
    {
        // start of the battle
        Rush, // x2 weapon damage at the first turn
        Rage, // +2 to damage dealt during first 3 turns; -1 to damage dealt during the rest of the battle

        // conditional buffs
        HiddenStrike, // +1 to damage dealt if attacker's agility is bigger than defender's
        Shield, // -3 to damage received if strength of the defender is bigger than of the attacker

        // conditional debuffs
        CrushingWeakness, // x2 to damage received if the attacker's weapon is crushing

        // Passive
        StoneSkin, // damage received is reduced by the endurance value
        Agile, // +1 to agility
        Strong, // +1 to strength
        Survivor, // +1 to endurance
        CuttingImmune, // immune to cutting weapon damage

        // turn-dependent
        Poison, // damage dealt is increased on each turn based on arithmetic progression
        FireBreather, // +3 damage by fire every third turn
    };

    // struct both for playable and non-playable character
    struct Character
    {
        size_t level = 0;
        std::string name = "";
        Races race = Races::None;
        CharacterStats stats_max;
        CharacterStats stats; // during battle
        Class main_class = {Classes::None, 0};
        Class sub_class = {Classes::None, 0};
        std::vector<Traits> traits;
        std::vector<Item> drop_list;
        Weapon weapon;
    };

    namespace GameDB
    {
        std::unordered_map<size_t, Weapon> weapons =
        {
            // player collectible
            { 0, {U"Кулак",             0.0,    DamageType::None}},
            { 1, {U"Кастет",            1.0,    DamageType::None}},
            { 2, {U"Кинжал",            2.0,    DamageType::Piercing}},
            { 3, {U"Меч",               3.0,    DamageType::Cutting}},
            { 4, {U"Дубина",            3.0,    DamageType::Crushing}},
            { 5, {U"Копьё",             3.0,    DamageType::Piercing}},
            { 6, {U"Топор",             4.0,    DamageType::Cutting}},
            { 7, {U"Легендарный Меч",  10.0,    DamageType::Cutting}},

            // npc weapons
            { 9, {U"Шип",               1.0,    DamageType::None}},
            {10, {U"Сгнивший меч",      2.0,    DamageType::None}},
            {11, {U"Кадило",            3.0,    DamageType::None}},
            {12, {U"Коготь и клык",     4.0,    DamageType::None}},

            // easter eggs
            {13, {U"Пулемёт Максим",  100.0,    DamageType::Divine}},
        };

        std::unordered_map<size_t, Character> characters =
        {
            // npc
            {
                0,
                {
                    1,                     // level
                    "Гоблин",              // name
                    Races::Goblin,         // race
                    {5, 1, 1, 1},          // stats_max
                    {5, 1, 1, 1},          // stats
                    {Classes::None, 0},    // main class
                    {Classes::None, 0},    // sub class
                    {},                    // traits
                    {GameDB::weapons[2]},  // drop
                    GameDB::weapons[10],   // weapon
                }
            },

            {
                1,
                {
                    1,                     // level
                    "Скелет",              // name
                    Races::Skeleton,         // race
                    {10, 2, 2, 1},          // stats_max
                    {10, 2, 2, 1},          // stats
                    {Classes::None, 0},    // main class
                    {Classes::None, 0},    // sub class
                    {Traits::CrushingWeakness},                    // traits
                    {GameDB::weapons[4]},  // drop
                    GameDB::weapons[10],   // weapon
                }
            },

            {
                2,
                {
                    1,                     // level
                    "Слайм",              // name
                    Races::Slime,         // race
                    {8, 3, 1, 2},          // stats_max
                    {8, 3, 1, 2},          // stats
                    {Classes::None, 0},    // main class
                    {Classes::None, 0},    // sub class
                    {Traits::CuttingImmune},                    // traits
                    {GameDB::weapons[5]},  // drop
                    GameDB::weapons[9],   // weapon
                }
            },

            {
                3,
                {
                    1,                     // level
                    "Призрак",              // name
                    Races::Ghost,         // race
                    {6, 1, 3, 1},          // stats_max
                    {6, 1, 3, 1},          // stats
                    {Classes::None, 0},    // main class
                    {Classes::None, 0},    // sub class
                    {Traits::HiddenStrike},                    // traits
                    {GameDB::weapons[3]},  // drop
                    GameDB::weapons[11],   // weapon
                }
            },

            {
                4,
                {
                    2,                     // level
                    "Голем",              // name
                    Races::Golem,         // race
                    {10, 3, 1, 3},          // stats_max
                    {10, 3, 1, 3},          // stats
                    {Classes::None, 0},    // main class
                    {Classes::None, 0},    // sub class
                    {Traits::StoneSkin},                    // traits
                    {GameDB::weapons[6]},  // drop
                    GameDB::weapons[9],   // weapon
                }
            },

            {
                5,
                {
                    3,                     // level
                    "Дракон",              // name
                    Races::Dragon,         // race
                    {20, 3, 3, 3},          // stats_max
                    {20, 3, 3, 3},          // stats
                    {Classes::None, 0},    // main class
                    {Classes::None, 0},    // sub class
                    {Traits::FireBreather},                    // traits
                    {GameDB::weapons[7]},  // drop
                    GameDB::weapons[12],   // weapon
                }
            },
        };
    }

    namespace Game
    {
        // do I really need to implement a game state here instead of all this? 
        
        GamePages gamePage = GamePages::None;
        GameState gameStateMachine;
        Character playerCharacter;
        Character npcCharacter;
        
        void GameLogic()
        {
            switch (gamePage)
            {
            case GamePages::Intro:
                {
                    break;
                }
            case GamePages::MainMenu:
                {
                    static int mainMenuChoice = 0;
                    switch (mainMenuChoice)
                    {
                    case 1: // create character
                        {
                            gamePage = GamePages::CharacterCreation;
                            break;
                        }
                    case 2: // load character
                        {
                            gamePage = GamePages::CharacterLoad;
                            break;
                        }
                    case 3: // highscores
                        {
                            gamePage = GamePages::Highscores;
                            break;
                        }
                    case 4: // credits
                        {
                            gamePage = GamePages::Credits;
                            break;
                        }
                    case 5: // exit
                        {
                            gamePage = GamePages::Exit;
                            break;
                        }
                    default:
                        break;
                    }
                    mainMenuChoice = 0;
                    
                    break;
                }
            case GamePages::CharacterCreation:
                {
                    Class inputClass = Classes::None;
                    CharacterStats inputStats;
                    Traits inputTrait;
                    Races inputRace = Races::Human;
                    Weapon inputWeapon = GameDB::weapons[0]
                    
                    switch (inputClass)
                    {
                    case Classes::Rogue:
                        {
                            std::srand();
                            int str = std::rand() % 3 + 1;
                            int agi = std::rand() % 3 + 1;
                            int end = std::rand() % 3 + 1;
                            inputStats = {4, str, agi, end};
                            inputTrait = Traits::HiddenStrike;
                            inputWeapon = GameDB::weapons[2];
                            break;
                        }
                    case Classes::Warrior:
                        {
                            std::srand();
                            int str = std::rand() % 3 + 1;
                            int agi = std::rand() % 3 + 1;
                            int end = std::rand() % 3 + 1;
                            inputStats = {5, str, agi, end};
                            inputTrait = Traits::Rush;
                            inputWeapon = GameDB::weapons[3];
                            break;
                        }
                    case Classes::Barbarian:
                        {
                            std::srand();
                            int str = std::rand() % 3 + 1;
                            int agi = std::rand() % 3 + 1;
                            int end = std::rand() % 3 + 1;
                            inputStats = {6, str, agi, end};
                            inputTrait = Traits::Rage;
                            inputWeapon = GameDB::weapons[4];
                            break;
                        }
                    default:
                        {
                            printf("can't create player character: undefined class");
                        }
                    }
                    
                    playerCharacter = 
                    {
                        1,                     // level
                        "Игрок",              // name
                        inputRace,         // race
                        inputStats,          // stats_max
                        inputStats,          // stats
                        {inputClass, 1},    // main class
                        {Classes::None, 0},    // sub class
                        inputTrait,                    // traits
                        {},  // drop
                        inputWeapon,   // weapon
                    };
                    
                    break;
                }
            case GamePages::CharacterLoad:
                {
                    break;
                }
            case GamePages::ArenaSetup:
                {
                    // calculate actual player stats
                    playerCharacter.stats = playerCharacter.stats_max;
                    playerCharacter.stats = 
                    {
                        playerCharacter.level * (playerCharacter.stats_max.health + playerCharacter.stats_max.endurance),
                        playerCharacter.stats_max.strength,
                        playerCharacter.stats_max.agility,
                        playerCharacter.stats_max.endurance,
                    }
                    
                    // apply stats passive traits
                    for (Traits trait : playerCharacter.traits)
                    {
                        if (trait == Traits::Strong)
                        {
                            playerCharacter.stats.strength++;
                        }
                        if (trait == Traits::Agile)
                        {
                            playerCharacter.stats.agility++;
                        }
                        if (trait == Traits::Survivor)
                        {
                            playerCharacter.stats.endurance++;
                        }
                    }
                    
                    // pick npc adversary
                    int npcCount = GameDB::characters.size();
                    std::srand();
                    int pickedCharacter = std::rand() % npcCount;
                    npcCharacter = GameDB::characters[pickedCharacter];
                    break;
                }
            case GamePages::ArenaBattle:
                {
                    bool battleFinished = false;
                    int turn = 0;
                    float weaponDamage = 0.0;
                    float skillDamage = 0.0;
                    float totalDamage = 0.0;
                    
                    Character* attacker = nullptr;
                    Character* defender = nullptr;
                    
                    // start of the battle
                    if (playerCharacter.stats.agility > npcCharacter.stats.agility)
                    {
                        attacker = &playerCharacter;
                        defender = &npcCharacter;
                    } else {
                        attacker = &npcCharacter;
                        defender = &playerCharacter;
                    }
                    
                    while (!battleFinished)
                    {
                        turn++;
                        
                        // swap roles
                        if (turn > 1)
                        {
                            Character* tmp = nullptr;
                            tmp = attacker;
                            attacker = defender;
                            defender = tmp;
                        }
                        
                        // start of the turn
                        // calculate attack chance
                        std::srand();
                        int dice = std::rand() % (attacker->stats.agility + defender->stats.agility);
                        if (dice > defender)
                        {
                            // apply attacker's effects
                            for (Traits trait : attacker->traits)
                            {   
                                if (turn == 1)
                                {
                                    if (trait == Traits::Rush)
                                    {
                                        weaponDamage = attacker->weapon.damage * 2;
                                        skillDamage = attacker->stats.strength * 2;
                                    }
                                }
                                
                                if ((trait == Traits::HiddenStrike) &&
                                    (attacker->stats.agility > defender->stats.agility))
                                {
                                    skillDamage += 1;
                                }
                                
                                if ((trait == Traits::Poison))
                                {
                                    skillDamage += turn;
                                }
                                
                                if (trait == Traits::Rage)
                                {
                                    skillDamage = (turn < 4) ? (skillDamage + 2) : (skillDamage - 1);
                                }
                                
                                if (trait == Traits::FireBreather)
                                {
                                    skillDamage = ((turn % 3) == 0) ? skillDamage + 3 : skillDamage;
                                }
                            }
                            
                            // apply defender's effects
                            for (Traits trait : defender->traits)
                            {
                                if ((trait == Traits::CuttingImmune) &&
                                    (attacker->weapon.type == DamageType::Cutting))
                                {
                                    weaponDamage = 0.0;
                                }
                                
                                if ((trait == Traits::CrushingWeakness) &&
                                    (attacker->weapon.type == DamageType::Crushing))
                                {
                                    weaponDamage = weaponDamage * 2;
                                }
                            }
                            
                            // pre-calculate total damage
                            totalDamage = weaponDamage + skillDamage;
                            
                            // apply defender's effects that affect total damage
                            for (Traits trait : defender->traits)
                            {
                                if (trait == Traits::StoneSkin)
                                {
                                    totalDamage -= defender->stats.endurance;
                                }
                                
                                if ((trait == Traits::Shield) &&
                                    (defender->stats.strength > attacker->stats.strength))
                                {
                                    totalDamage -= 3;
                                }
                            }
                            
                            // clash
                            defender->stats.health -= totalDamage;
                        }
                        
                        if (defender->stats.health <= 0.0)
                        {
                            battleFinished = true;
                        }
                        
                        // end of the turn
                    }
                    
                    // end of the battle
                    gamePage = GamePages::ArenaAftermath;
                    
                    break;
                }
            case GamePages::ArenaAftermath:
                {
                    if (playerCharacter.stats.health > 0.0)
                    {
                        gamePage = GamePages::CharacterUpdate;
                    }
                    
                    break;
                }
            case GamePages::CharacterUpdate:
                {
                    break;
                }
            case GamePages::Highscores:
                {
                    break;
                }
            case GamePages::Exit:
                {
                    break;
                }
            default:
                {
                    break;
                }
            }
        }
    }
}

#endif
