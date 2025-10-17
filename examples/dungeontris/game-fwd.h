#ifndef DUNGEONTRIS_AUTOBATTLER_GAME_FWD_H
#define DUNGEONTRIS_AUTOBATTLER_GAME_FWD_H

//#include <mse/mse.h>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <glm/glm.hpp>
#include <mse/systems/platform/renderer/texture.h>
#include <mse/systems/windows/layers/gui/guiitem.h>

// globals
namespace DTetris
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
        int size_max = 31;
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
        Settings,
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
    
    enum class WeaponSprite
    {
        None,
        Fist,
        Dagger,
        Sword,
        Axe,
        Club,
        Spear,
        LegendarySword,
        MaximGun,
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
        int damage = 0;
        DamageType type = DamageType::None;
        WeaponSprite sprite = WeaponSprite::None;
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
        std::vector<Weapon> drop_list;
        Weapon weapon;
    };
    
    struct Trait
    {
        Traits type;
        std::u32string name = U"";
        std::u32string description = U"";
    };
    
    struct CharacterClass
    {
        Classes type;
        std::u32string name = U"";
    };
    
    struct DamageTypeName
    {
        DamageType type;
        std::u32string name = U"";
    };

    struct GameDB
    {
        std::unordered_map<size_t, Weapon> weapons;
        std::unordered_map<DamageType, DamageTypeName> damageTypes;
        std::unordered_map<Traits, Trait> traits;
        std::unordered_map<Classes, CharacterClass> classes;
        std::unordered_map<int, Character> characters;
        int level_max;
    };

    // game configuration
    struct GameConfig
    {
        bool music = false;
        float musicVolume = 0;
        bool sounds = false;
        float soundsVolume = 0;
        bool playInBackground = false;
        bool fullscreen = true;
        bool fastBattle = false;
        int turnLength = 500; // in milliseconds
    };
    
    enum class BlockType
    {
        None,
        Block,
        Healing,
        Treasure,
    };
    
    struct Block
    {
        glm::uvec2 coordinates = {0, 0};
        BlockType type = BlockType::None;
        glm::uvec3 color = {255, 255, 0};
    };
    
    struct BlockSprite
    {
        mse::Texture* texture = nullptr;
        glm::uvec4 place = {0, 0, 0, 0};
    };
    
    struct TetriminoTemplate
    {
//        TetriminoTemplate();
        TetriminoTemplate(const std::vector<int>& data);
        std::vector<int> block = {
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        };
    };
    
    struct TetriminoTemplatesDB
    {
        std::vector<TetriminoTemplate> data = {};
    };
    
    struct Tetrimino
    {
        std::vector<Block> blocks = {}; // 4x4 or 3x3 areas
    };
    
    // Tetris
    struct TetrisMap
    {
        // model
        int width = 15;
        int height = 20;
        std::vector<Block> map;
        
        // view
        std::vector<BlockSprite> blockSprites;
        
        // controller
        void Resize(int w, int h);
//        Update();
    };
    
    // various game data for everything
    struct Game
    {
        GameConfig config;
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
        
//        void GameLogic(GameDB& gameDB);
    };
    
    GameDB& InitAutobattlerData();
    TetriminoTemplatesDB& InitTetriminoDB();
}

namespace mse
{
    namespace gui
    {
        class TetrisMapGUI : public GUIItem
        {
        public:
            // general initialization
            TetrisMapGUI();
            TetrisMapGUI(Layer* layer, 
                         const glm::uvec4& area, 
                         const std::string& spritelist,
                         DTetris::TetrisMap* tetrisMap,
                         int width,
                         int height);
            virtual ~TetrisMapGUI();
            
            // general GUIItem interface
            virtual void Display();
            
        protected:
            // sprite-based
            Resource* m_spriteList = nullptr;
            DTetris::TetrisMap* m_tetrisMap = nullptr;
        };
    }
}

#endif
