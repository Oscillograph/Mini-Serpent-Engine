#ifndef LESTA_AUTOBATTLER_FWD_H
#define LESTA_AUTOBATTLER_FWD_H

#include <mse/mse.h>

// globals
namespace LAutobattler
{
    struct MessageLogItem
    {
        std::u32string text = U"";
        MessageLogItem* next = nullptr;
    };
    
    struct MessageLog
    {
        int size_max = 10;
        int size = 0;
        MessageLogItem* stack = nullptr;
        
        ~MessageLog()
        {
            Clear();
        }
        
        inline void Clear()
        {
            int i = 0;
            while(Pop() && (i < size_max))
            {
                i++;
            }
        }
        
        inline bool Pop()
        {
            if (stack != nullptr)
            {
                MessageLogItem* current = stack->next;
                delete stack;
                stack = current;
                current = nullptr;
                size--;
                
                return true;
            }
            return false;
        }
        
        inline void Push(std::u32string text)
        {
            MessageLogItem* current = stack;
            for (int i = 0; i < size_max; ++i)
            {
                if (stack == nullptr)
                {
                    stack = new MessageLogItem();
                    stack->text = text;
                    size++;
                    i = size_max; // exit cycle
                } else {
                    if (current->next != nullptr)
                    {
                        current = current->next;
                    } else {
                        current->next = new MessageLogItem();
                        current->next->text = text;
                        size++;
                        i = size_max; // exit cycle
                    }
                }
            }
            
            if (size > size_max)
            {
                Pop();
            }
        }
    };
    
    
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
        GameOver,
        Winner,
        Highscores,
        Credits,
        Exit
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

    struct Class
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
        std::u32string name = U"";
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

        std::unordered_map<int, Character> characters =
        {
            // npc
            {
                0,
                {
                    1,                     // level
                    U"Гоблин",              // name
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
                    U"Скелет",              // name
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
                    U"Слайм",              // name
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
                    U"Призрак",              // name
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
                    U"Голем",              // name
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
                    U"Дракон",              // name
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
        bool gamePageHasToChange = false;
        GamePages gamePageFrom = GamePages::None;
        GamePages gamePageTo = GamePages::None;
        
        MessageLog UILogger;
        
        Classes inputClass = Classes::None;
        CharacterStats inputStats;
        Traits inputTrait;
        Races inputRace = Races::Human;
        Weapon inputWeapon = GameDB::weapons[0];
        bool playerCharacterUpdated = false;
        bool battleJustStarted = false;
        bool battleJustFinished = false;
        bool battleFinished = false;
        
        bool keyPressed = false;
        
        int battleCounter = 0;
        int turn = 0;
        
        Character playerCharacter;
        Character npcCharacter;
        
        Character* attacker = nullptr;
        Character* defender = nullptr;
        
        void GameLogic()
        {
            std::srand(std::clock());
            
            switch (gamePage)
            {
            case GamePages::Intro:
                {
                    printf("~~~ Intro ~~~\n");
                    gamePageHasToChange = true;
                    gamePageTo = GamePages::MainMenu;
                    break;
                }
            case GamePages::MainMenu:
                {
                    static int mainMenuChoice = 0;
                    switch (mainMenuChoice)
                    {
                    case 1: // create character
                        {
                            gamePageHasToChange = true;
                            gamePageTo = GamePages::CharacterCreation;
                            break;
                        }
                    case 2: // load character
                        {
                            gamePageHasToChange = true;
                            gamePageTo = GamePages::CharacterLoad;
                            break;
                        }
                    case 3: // highscores
                        {
                            gamePageHasToChange = true;
                            gamePageTo = GamePages::Highscores;
                            break;
                        }
                    case 4: // credits
                        {
                            gamePageHasToChange = true;
                            gamePageTo = GamePages::Credits;
                            break;
                        }
                    case 5: // exit
                        {
                            gamePageHasToChange = true;
                            gamePageTo = GamePages::Exit;
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
                    if (inputClass != Classes::None)
                    {
                        switch (inputClass)
                        {
                        case Classes::Rogue:
                            {
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
                                printf("can't create player character: undefined class\n");
                            }
                        }
                        
                        playerCharacter = 
                        {
                            1,                     // level
                            U"Игрок",              // name
                            inputRace,         // race
                            inputStats,          // stats_max
                            inputStats,          // stats
                            {inputClass, 1},    // main class
                            {Classes::None, 0},    // sub class
                            {inputTrait},                    // traits
                            {},  // drop
                            inputWeapon   // weapon
                        };
                        printf("Character %s created! (%.2f, %.2f, %.2f, %.2f)\n", 
                               playerCharacter.name.c_str(),
                               playerCharacter.stats.health,
                               playerCharacter.stats.strength,
                               playerCharacter.stats.agility,
                               playerCharacter.stats.endurance);
                        
                        battleCounter = 0;
                        
                        inputClass = Classes::None;
                        
                        gamePageHasToChange = true;
                        gamePageTo = GamePages::ArenaSetup;
                    }
                    
                    break;
                }
            case GamePages::CharacterLoad:
                {
                    break;
                }
            case GamePages::ArenaSetup:
                {
                    printf("Preparing battle...\n");
                    battleCounter++;
                    
                    // calculate actual player stats
                    playerCharacter.stats = playerCharacter.stats_max;
                    playerCharacter.stats = 
                    {
                        playerCharacter.level * (playerCharacter.stats_max.health + playerCharacter.stats_max.endurance),
                        playerCharacter.stats_max.strength,
                        playerCharacter.stats_max.agility,
                        playerCharacter.stats_max.endurance,
                    };
                    
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
                            playerCharacter.stats.health = playerCharacter.level * (playerCharacter.stats_max.health + playerCharacter.stats.endurance);
                        }
                    }
                    
                    // pick npc adversary
                    int npcCount = GameDB::characters.size();
                    int pickedCharacter = std::rand() % npcCount;
                    npcCharacter = GameDB::characters[pickedCharacter];
                    
                    printf("Next opponent (of %d) is %s! (%.2f, %.2f, %.2f, %.2f)\n",
                           npcCount,
                           npcCharacter.name.c_str(),
                           npcCharacter.stats.health,
                           npcCharacter.stats.strength,
                           npcCharacter.stats.agility,
                           npcCharacter.stats.endurance);
                    
                    // start of the battle
                    if (playerCharacter.stats.agility > npcCharacter.stats.agility)
                    {
                        attacker = &playerCharacter;
                        defender = &npcCharacter;
                    } else {
                        attacker = &npcCharacter;
                        defender = &playerCharacter;
                    }
                    printf("Opponents greet each other and start the combat.\n");
                    
                    turn = 0;
                    battleJustStarted = true;
                    battleFinished = false;
                    UILogger.Clear();
                    
                    gamePageHasToChange = true;
                    gamePageTo = GamePages::ArenaBattle;
                    break;
                }
            case GamePages::ArenaBattle:
                {
                    float weaponDamage = 0.0;
                    float skillDamage = 0.0;
                    float totalDamage = 0.0;
                    battleJustStarted = false;
                    
                    std::stringstream strForLogger;
                    
                    if ((!battleFinished) && (turn < 30))
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
                        
                        weaponDamage = attacker->weapon.damage;
                        skillDamage = 0.0;
                        totalDamage = 0.0;
                        
                        printf("Turn %d: %s attacks.\n",
                               turn,
                               attacker->name.c_str());
                        
                        // start of the turn
                        // calculate attack chance
//                        std::srand(0);
                        int agi_sum = attacker->stats.agility + defender->stats.agility;
                        int dice = (std::rand() % agi_sum) + 1;
//                        printf("Rolled %d of %d\n", dice, agi_sum);
                        if (dice > defender->stats.agility)
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
                            
                            if (totalDamage < 0.0)
                            {
                                totalDamage = 0.0;
                            }
                            
                            // clash
                            defender->stats.health -= totalDamage;
                            printf("Turn %d: %s deals %.2f damage to %s, leaving %.2f health.\n", 
                                   turn, 
                                   attacker->name.c_str(), 
                                   totalDamage, 
                                   defender->name.c_str(), 
                                   defender->stats.health);
                            
                            strForLogger << "Ход " << turn << ": " 
                                << utf8::utf32to8(attacker->name.c_str()) << "(" << attacker->stats.health 
                                << ") наносит " << totalDamage << " урона " 
                                << utf8::utf32to8(defender->name.c_str()) << "(" << defender->stats.health << ")\n";
                            UILogger.Push(utf8::utf8to32(strForLogger.str()));
                        } else {
                            printf("Turn %d: %s misses!\n", 
                                   turn, 
                                   attacker->name.c_str());
                            
                            strForLogger << "Ход " << turn << ": " 
                                << utf8::utf32to8(attacker->name.c_str()) << "(" << attacker->stats.health 
                                << ") промахивается!\n";
                            UILogger.Push(utf8::utf8to32(strForLogger.str()));
                        }
                        
                        if (defender->stats.health <= 0.0)
                        {
                            battleFinished = true;
                        }
                 
                        SDL_Delay(300);
                        // end of the turn
                    } else {
                        // end of the battle
                        gamePageHasToChange = true;
                        gamePageTo = GamePages::ArenaAftermath;
                        printf("Battle is finished.\n");
                    }
                    
                    break;
                }
            case GamePages::ArenaAftermath:
                {
                    if (playerCharacter.stats.health > 0.0)
                    {
                        if (battleCounter < 5)
                        {
                            battleJustFinished = true; // important for layer management
                            gamePage = GamePages::CharacterUpdate;
//                            inputWeapon = *(Weapon*)(&(npcCharacter.drop_list[0]));
                            inputWeapon = playerCharacter.weapon;
                        } else {
                            gamePageHasToChange = true;
                            gamePageTo = GamePages::Winner;
                        }
                    } else {
                        gamePageHasToChange = true;
                        gamePageTo = GamePages::GameOver;
                    }
                    
                    break;
                }
            case GamePages::CharacterUpdate:
                {
                    battleJustFinished = false; // important for layer management
                    
                    if (playerCharacterUpdated)
                    {
                        // change weapon
                        playerCharacter.weapon = inputWeapon;
                        
                        if (playerCharacter.level < 3)
                        {
                            playerCharacter.level++;
                            printf("Player lvl: %d, ", playerCharacter.level);
                            
                            if ((inputClass != Classes::None))
                            {
                                switch (inputClass)
                                {
                                case Classes::Rogue:
                                {
                                    playerCharacter.sub_class = {Classes::Rogue, playerCharacter.sub_class.level + 1};
                                    printf("subclass: ROGUE, ");
                                    if (playerCharacter.sub_class.level > 1)
                                    {
                                        playerCharacter.traits.push_back(Traits::Agile);
                                        printf("+trait \"Agile\"\n");
                                    } else {
                                        playerCharacter.traits.push_back(Traits::HiddenStrike);
                                        printf("+trait \"Hidden Strike\"\n");
                                    }
                                    break;
                                }
                                case Classes::Warrior:
                                    {
                                        playerCharacter.sub_class = {Classes::Warrior, playerCharacter.sub_class.level + 1};
                                        printf("subclass: Warrior, ");
                                        if (playerCharacter.sub_class.level > 1)
                                        {
                                            playerCharacter.traits.push_back(Traits::Shield);
                                            printf("+trait \"Shield\"\n");
                                        } else {
                                            playerCharacter.traits.push_back(Traits::Rush);
                                            printf("+trait \"Rush\"\n");
                                        }
                                        break;
                                    }
                                case Classes::Barbarian:
                                    {
                                        playerCharacter.sub_class = {Classes::Barbarian, playerCharacter.sub_class.level + 1};
                                        printf("subclass: Barbarian, ");
                                        if (playerCharacter.sub_class.level > 1)
                                        {
                                            playerCharacter.traits.push_back(Traits::StoneSkin);
                                            printf("+trait \"Stone Skin\"\n");
                                        } else {
                                            playerCharacter.traits.push_back(Traits::Rage);
                                            printf("+trait \"Rage\"\n");
                                        }
                                        break;
                                    }
                                }
                            } else {
                                switch (playerCharacter.main_class.type)
                                {
                                case Classes::Rogue:
                                {
                                    playerCharacter.main_class = {Classes::Rogue, playerCharacter.main_class.level + 1};
                                    if (playerCharacter.main_class.level == 2)
                                    {
                                        playerCharacter.traits.push_back(Traits::Agile);
                                        printf("+trait \"Agile\"\n");
                                    }
                                    if (playerCharacter.main_class.level == 3)
                                    {
                                        playerCharacter.traits.push_back(Traits::Poison);
                                        printf("+trait \"Poison\"\n");
                                    }
                                    break;
                                }
                                case Classes::Warrior:
                                    {
                                        playerCharacter.main_class = {Classes::Warrior, playerCharacter.main_class.level + 1};
                                        if (playerCharacter.main_class.level == 2)
                                        {
                                            playerCharacter.traits.push_back(Traits::Shield);
                                            printf("+trait \"Shield\"\n");
                                        }
                                        if (playerCharacter.main_class.level == 3)
                                        {
                                            playerCharacter.traits.push_back(Traits::Strong);
                                            printf("+trait \"Strong\"\n");
                                        }
                                        break;
                                    }
                                case Classes::Barbarian:
                                    {
                                        playerCharacter.main_class = {Classes::Barbarian, playerCharacter.main_class.level + 1};
                                        if (playerCharacter.main_class.level == 2)
                                        {
                                            playerCharacter.traits.push_back(Traits::StoneSkin);
                                            printf("+trait \"Stone Skin\"\n");
                                        }
                                        if (playerCharacter.main_class.level == 3)
                                        {
                                            playerCharacter.traits.push_back(Traits::Survivor);
                                            printf("+trait \"Surviror\"\n");
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                        
                        playerCharacterUpdated = false;
                        
                        gamePageHasToChange = true;
                        gamePageTo = GamePages::ArenaSetup;
                    }
                    
                    break;
                }
            case GamePages::GameOver:
                {
                    printf("Game over.\n");
                    gamePageHasToChange = true;
                    gamePageTo = GamePages::Exit;
                    break;
                }
            case GamePages::Winner:
                {
                    printf("Congratulations! You are a winner.\n");
                    gamePageHasToChange = true;
                    gamePageTo = GamePages::Exit;
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
