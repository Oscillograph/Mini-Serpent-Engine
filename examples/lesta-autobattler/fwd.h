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
    
    enum class DamageType
    {
        None,
        Cutting,
        Crushing,
        Piercing,
    };
    
    struct CharacterStats
    {
        double health = 0.0;
        double strength = 0.0;
        double agility = 0.0;
        double endurance = 0.0;
    };
    
    struct Item
    {
        std::u32string name;
    };
    
    struct Weapon : Item
    {
        double damage = 0.0;
    };
    
    std::unordered_map<size_t, Weapon> weapons = {
        {0, {U"Кулак",             0.0}},
        {1, {U"Кастет",            1.0}},
        {2, {U"Кинжал",            2.0}},
        {3, {U"Меч",               3.0}},
        {4, {U"Дубина",            3.0}},
        {5, {U"Копьё",             3.0}},
        {6, {U"Топор",             4.0}},
        {7, {U"Легендарный Меч",  10.0}},
        {8, {U"Пулемёт Максим",  100.0}},
    };
    
    struct Armor : Item
    {
        
        double weapon_defence_cutting = 0.0;
        double weapon_defence_crushing = 0.0;
        double weapon_defence_pierceing = 0.0;
        double skill_defence = 0.0;
        double stats_defence = 0.0;
        
    };
    
    struct Trait
    {};
    
    // +1 to damage dealt if attacker's agility is bigger than defender's 
    struct hidden_strike : Trait
    {};
    
    // x2 weapon damage at the first turn
    struct rush : Trait
    {};
    
    // +2 to damage dealt during first 3 turns; -1 to damage dealt during the rest of the battle
    struct rage : Trait
    {};
    
    // +1 to agility
    struct agile : Trait
    {};
    
    // -3 to damage received if strength of the defender is bigger than of the attacker
    struct shield : Trait
    {};
    
    // damage received is reduced by the endurance value
    struct stone_skin : Trait
    {};
    
    // damage dealt is increased on each turn based on arithmetic progression
    struct poison : Trait
    {};
    
    // +1 to strength
    struct strong : Trait
    {};
    
    // +1 to endurance
    struct survivor : Trait
    {};
    
    // x2 to damage received if the attacker's weapon is crushing
    struct crushing_weakness : Trait
    {};
    
    // immune to cutting weapon damage
    struct cutting_immune : Trait
    {};
    
    // +3 damage by fire every third turn
    struct fire_breather : Trait
    {};
    
    struct Skill
    {
    };
    
    // struct both for playable and non-playable character
    struct Character
    {
        size_t level = 0;
        Races race = Races::None;
        CharacterStats stats_max;
        CharacterStats stats; // during battle
        Classes main_class = Classes::None;
        Classes sub_class = Classes::None;
        std::vector<Trait> traits;
        std::vector<Skill> skills;
        std::vector<Item> inventory;
        std::vector<Item> drop_list;
        Weapon weapon;
    };
}

#endif
