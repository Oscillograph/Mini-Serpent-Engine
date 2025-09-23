#ifndef LESTA_AUTOBATTLER_GAME_FWD_H
#define LESTA_AUTOBATTLER_GAME_FWD_H

//#include <mse/mse.h>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

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

    struct GameDB
    {
        std::unordered_map<size_t, Weapon> weapons;
        std::unordered_map<int, Character> characters;
        int level_max;
    };

    // various game data for everything
    struct Game
    {
        GamePages gamePage = GamePages::None;
        bool gamePageHasToChange = false;
        GamePages gamePageFrom = GamePages::None;
        GamePages gamePageTo = GamePages::None;
        
        MessageLog UILogger;
        
        Classes inputClass = Classes::None;
        CharacterStats inputStats;
        Traits inputTrait;
        Races inputRace = Races::Human;
        Weapon inputWeapon;
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
        
        void GameLogic(GameDB& gameDB);
    };
    
    GameDB InitGameData();
}

#endif
