#ifndef LESTA_AUTOBATTLER_GAME_DATA_H
#define LESTA_AUTOBATTLER_GAME_DATA_H

#include <lesta-autobattler/game-fwd.h>

// globals
namespace LAutobattler
{    
    // ****************************************************************************************** //
    //                             General Game Types and Constants
    // ****************************************************************************************** //
    
    GameDB InitGameData()
    {
        GameDB gameDB;
        gameDB.level_max = 3;
        
        gameDB.weapons =
        {
            // player collectible
            { 0, {U"Кулак",             0,    DamageType::None}},
            { 1, {U"Кастет",            1,    DamageType::None}},
            { 2, {U"Кинжал",            2,    DamageType::Piercing}},
            { 3, {U"Меч",               3,    DamageType::Cutting}},
            { 4, {U"Дубина",            3,    DamageType::Crushing}},
            { 5, {U"Копьё",             3,    DamageType::Piercing}},
            { 6, {U"Топор",             4,    DamageType::Cutting}},
            { 7, {U"Легендарный Меч",  10,    DamageType::Cutting}},
            
            // npc weapons
            { 9, {U"Шип",               1,    DamageType::None}},
            {10, {U"Сгнивший меч",      2,    DamageType::None}},
            {11, {U"Кадило",            3,    DamageType::None}},
            {12, {U"Клык и коготь",     4,    DamageType::None}},
            
            // easter eggs
            {13, {U"Пулемёт Максим",  100,    DamageType::Divine}},
        };
        
        gameDB.characters =
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
                    {gameDB.weapons[2]},  // drop
                    gameDB.weapons[10],   // weapon
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
                    {gameDB.weapons[4]},  // drop
                    gameDB.weapons[10],   // weapon
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
                    {gameDB.weapons[5]},  // drop
                    gameDB.weapons[9],   // weapon
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
                    {gameDB.weapons[3]},  // drop
                    gameDB.weapons[11],   // weapon
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
                    {gameDB.weapons[6]},  // drop
                    gameDB.weapons[9],   // weapon
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
                    {gameDB.weapons[7]},  // drop
                    gameDB.weapons[12],   // weapon
                }
            },
        };
        
        return gameDB;
    }
}

#endif
