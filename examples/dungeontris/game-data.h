#ifndef DUNGEONTRIS_AUTOBATTLER_GAME_DATA_H
#define DUNGEONTRIS_AUTOBATTLER_GAME_DATA_H

#include <dungeontris/game-fwd.h>

// globals
namespace DTetris
{    
    // ****************************************************************************************** //
    //                             General Game Types and Constants
    // ****************************************************************************************** //
    TetriminoTemplate::TetriminoTemplate(const std::vector<int>& data)
    {
        block = data;
    }
        
    
    GameDB& InitAutobattlerData()
    {
        static GameDB gameDB;
        gameDB.level_max = 3;
        
        gameDB.weapons =
        {
            // player collectible
            { 0, {U"Кулак",             0,    DamageType::None,        WeaponSprite::Fist}},
            { 1, {U"Кастет",            1,    DamageType::None,        WeaponSprite::Fist}},
            { 2, {U"Кинжал",            2,    DamageType::Piercing,    WeaponSprite::Dagger}},
            { 3, {U"Меч",               3,    DamageType::Cutting,     WeaponSprite::Sword}},
            { 4, {U"Дубина",            3,    DamageType::Crushing,    WeaponSprite::Club}},
            { 5, {U"Копьё",             3,    DamageType::Piercing,    WeaponSprite::Spear}},
            { 6, {U"Топор",             4,    DamageType::Cutting,     WeaponSprite::Axe}},
            { 7, {U"Легендарный Меч",  10,    DamageType::Cutting,     WeaponSprite::LegendarySword}},
            
            // npc weapons
            { 9, {U"Шип",               1,    DamageType::None,        WeaponSprite::None}},
            {10, {U"Сгнивший меч",      2,    DamageType::None,        WeaponSprite::None}},
            {11, {U"Кадило",            3,    DamageType::None,        WeaponSprite::None}},
            {12, {U"Клык и коготь",     4,    DamageType::None,        WeaponSprite::None}},
            
            // easter eggs
            {13, {U"Пулемёт Максим",  100,    DamageType::Divine,      WeaponSprite::MaximGun}},
        };
        
        gameDB.traits = 
        {
            {
                Traits::HiddenStrike,
                {
                    Traits::HiddenStrike,
                    U"Скрытый удар",
                    U"+1 к урону, если ловчее цели"
                }
            },
            {
                Traits::Rush,
                {
                    Traits::Rush,
                    U"Рывок",
                    U"удвоенный урон в первый ход"
                }
            },
            {
                Traits::Rage,
                {
                    Traits::Rage,
                    U"Ярость",
                    U"+2 к урону первые три хода, -1 к урону во все остальные"
                }
            },
            {
                Traits::StoneSkin,
                {
                    Traits::StoneSkin,
                    U"Каменная кожа",
                    U"ущерб снижается на значение выносливости"
                }
            },
            {
                Traits::Strong,
                {
                    Traits::Strong,
                    U"Силач",
                    U"+1 к силе"
                }
            },
            {
                Traits::Agile,
                {
                    Traits::Agile,
                    U"Ловкач",
                    U"+1 к ловкости"
                }
            },
            {
                Traits::Survivor,
                {
                    Traits::Survivor,
                    U"Выживальщик",
                    U"+1 к выносливости"
                }
            },
            {
                Traits::Shield,
                {
                    Traits::Shield,
                    U"Щит",
                    U"-3 к ущербу, если сильнее цели"
                }
            },
            {
                Traits::Poison,
                {
                    Traits::Poison,
                    U"Яд",
                    U"+N к урону, где N - номер хода"
                }
            },
            {
                Traits::CuttingImmune,
                {
                    Traits::CuttingImmune,
                    U"Не режется",
                    U"иммунитет к режущему оружию"
                }
            },
            {
                Traits::CrushingWeakness,
                {
                    Traits::CrushingWeakness,
                    U"Страх дубин",
                    U"двойной ущерб от дробящего оружия"
                }
            },
            {
                Traits::FireBreather,
                {
                    Traits::FireBreather,
                    U"Огненное дыхание",
                    U"+3 к урону каждый третий ход"
                }
            },
        };
        
        gameDB.damageTypes = 
        {
            {
                DamageType::None,
                {
                    DamageType::None,
                    U"Нет"
                }
            },
            {
                DamageType::Cutting,
                {
                    DamageType::Cutting,
                    U"режущий"
                }
            },
            {
                DamageType::Crushing,
                {
                    DamageType::Crushing,
                    U"дробящий"
                }
            },
            {
                DamageType::Piercing,
                {
                    DamageType::Piercing,
                    U"колющий"
                }
            },
            {
                DamageType::Divine,
                {
                    DamageType::Divine,
                    U"божественный"
                }
            },
        };
        
        gameDB.classes =
        {
            {
                Classes::None,
                {
                    Classes::None,
                    U"Нет"
                }
            },
            {
                Classes::Warrior,
                {
                    Classes::Warrior,
                    U"Воин"
                }
            },
            {
                Classes::Barbarian,
                {
                    Classes::Barbarian,
                    U"Варвар"
                }
            },
            {
                Classes::Rogue,
                {
                    Classes::Rogue,
                    U"Разбойник"
                }
            },
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
    
    TetriminoTemplatesDB& InitTetriminoDB()
    {
        static TetriminoTemplatesDB tetriminoDB;
        // empty block
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        }));
        // classic mode
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 1, 0, 0,
            0, 1, 1, 0,
            0, 1, 0, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 1, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 0, 0, 0,
            0, 1, 1, 0,
            0, 1, 1, 0,
            0, 0, 0, 0
        }));            
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 1, 1, 0,
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 1, 1, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 1, 0, 0,
            0, 1, 1, 0,
            0, 0, 1, 0,
            0, 0, 0, 0
        }));
        // brick game mode
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 0, 0, 0,
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 0, 0, 0,
            0, 1, 1, 0,
            0, 1, 0, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            1, 0, 1, 0,
            1, 1, 1, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            1, 1, 1, 0,
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 1, 1, 0,
            0, 1, 0, 0,
            1, 1, 0, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            1, 1, 0, 0,
            0, 1, 0, 0,
            0, 1, 1, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 0, 1, 0,
            0, 0, 1, 0,
            1, 1, 1, 0,
            0, 0, 0, 0
        }));
        
        return tetriminoDB;
    }
}

#endif
