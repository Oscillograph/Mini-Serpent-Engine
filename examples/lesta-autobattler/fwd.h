#ifndef LESTA_AUTOBATTLER_FWD_H
#define LESTA_AUTOBATTLER_FWD_H

#include <mse/mse.h>

// globals
namespace LAutobattler
{
    // ****************************************************************************************** //
    //                             Queue Message Logger
    // ****************************************************************************************** //
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
        
        MessageLog();
        ~MessageLog();
        void Clear();
        bool Pop();
        void Push(std::u32string text);
    };
    
    // ****************************************************************************************** //
    //                             General Game Types and Constants
    // ****************************************************************************************** //
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
        
        void GameLogic();
    }
}

#endif
