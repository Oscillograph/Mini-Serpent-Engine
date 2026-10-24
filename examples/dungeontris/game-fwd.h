#ifndef DUNGEONTRIS_AUTOBATTLER_GAME_FWD_H
#define DUNGEONTRIS_AUTOBATTLER_GAME_FWD_H

//#include <mse/mse.h>
// #include <cmath>
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
    //                               Tetris Types and Constants
    // ****************************************************************************************** //
    enum class BlockType
    {
        None,
        Block,
        Healing,
        Treasure,
        Attack,
        Defense,
        Wall,
    };

    enum class TetrisStates
    {
        None		= 0,
        Processing	= 1,
        Paused		= 2,
        GameOver	= 3,
        ClearLines  = 4,
        Autobattle  = 5,
        Win		    = 6,
    };

    enum class TetrisMoveDirection
    {
        Down		= 0,
        Left		= 1,
        Right		= 2,
        Up			= 3,
    };

    enum class TetrisRotateDirection
    {
        None                = 0,
        Clockwise           = 1,
        CounterClockwise    = 2,
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
        Shop,
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

    enum class Races : int
    {
        None,
        Human,
        Goblin,
        Skeleton,
        Slime,
        Ghost,
        Golem,
        Dragon,
        OldGod
    };

    enum class Classes : int
    {
        None,
        Rogue,
        Warrior,
        Barbarian,
        Monk,
        Cultist,
        Witcher
    };

    struct Class
    {
        Classes type = Classes::None;
        int level = 0;
    };

    enum class DamageType : int
    {
        None,
        Cutting,
        Crushing,
        Piercing,
        Divine,
    };
    
    enum class WeaponSprite : int
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
        int cost = 0;
    };

    struct Armor : Item
    {

        float weapon_defence_cutting = 0.0;
        float weapon_defence_crushing = 0.0;
        float weapon_defence_pierceing = 0.0;
        float skill_defence = 0.0;
        float stats_defence = 0.0;
        int cost = 0;

    };

    enum class Traits : int
    {
        None                = 0,

        // start of the battle
        Rush                = 1, // x2 weapon damage at the first turn
        Rage                = 2, // +2 to damage dealt during first 3 turns; -1 to damage dealt during the rest of the battle

        // conditional buffs
        HiddenStrike        = 3, // +1 to damage dealt if attacker's agility is bigger than defender's
        Shield              = 4, // -3 to damage received if strength of the defender is bigger than of the attacker

        // conditional debuffs
        CrushingWeakness    = 5, // x2 to damage received if the attacker's weapon is crushing

        // Passive
        StoneSkin           = 6, // damage received is reduced by the endurance value
        Agile               = 7, // +1 to agility
        Strong              = 8, // +1 to strength
        Survivor            = 9, // +1 to endurance
        CuttingImmune       = 10, // immune to cutting weapon damage
        MarkOfChaos         = 14, // +1 to every stat for each battle won (up to 6/6/6)
        SchoolOfBeaver      = 15, // multiply treasures in bank by 2 after each battle

        // turn-dependent
        Poison              = 11, // damage dealt is increased on each turn based on arithmetic progression
        FireBreather        = 12, // +3 damage by fire every third turn
        Meditation          = 13, // heals by 1 every tenth turn
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
        Class sub_class = {Classes::None, 0}; // TODO: remove subclasses mechanic
        std::vector<Traits> traits;
        std::vector<Weapon> drop_list;
        Weapon weapon;

        std::unordered_map<BlockType, int> blocks = {
            {BlockType::Attack,     0},
            {BlockType::Defense,    0},
            {BlockType::Healing,    0},
            {BlockType::Treasure,   0},
            {BlockType::Block,      0}
        };
    };
    
    struct Trait
    {
        Traits type;
        std::u32string name = U"";
        std::u32string description = U"";
        int cost = 0;
    };
    
    struct CharacterClass
    {
        Classes type;
        std::u32string name = U"";
        float health = 0.0f;
        float strength = 0.0f;
        float agility = 0.0f;
        float endurance = 0.0f;
        std::u32string tips = U"";
        Traits startingTrait;
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
        std::unordered_map<size_t, Armor> armors;
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
        unsigned int turnLength = 500; // in milliseconds
    };
    
    struct Block
    {
        glm::uvec2 coordinates = {0, 0};
        BlockType type = BlockType::None;
        glm::uvec3 color = {255, 255, 0};
        bool remove = false; // flag for removal
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
        int x, y = 0; // left-upper corner coordinates
        std::vector<Block> blocks = {}; // 4x4 or 3x3 areas
        bool fell = false;
        int id = 0;
    };
    
    // Tetris
    struct TetrisMap
    {
        // model
        int width = 12;
        int height = 20;
        std::vector<Block> map_backend; // this is only blocks that landed
        std::vector<Block> map;         // this is all blocks PLUS player's tetrimino
        
        // view
        std::vector<BlockSprite> blockSprites;
        
        // controller
        void Resize(int w, int h);
        void Clear();
        void ClearLines(std::map<int, bool>& linesFilled);
    };

    struct TetrisEngineData
    {
        // Game speed formulae: 10ms*(gameSpeedMax - gameSpeed)
        unsigned int gameSpeed = 1;
        unsigned int gameSpeedMax = 25;
        uint32_t gameTimer = 0;
        int gameDifficulty = 1; // 0 means A, 1 means C-, 2 means C
        int playerScore = 0;
        bool rotateTetrimino = false;
        bool justPlacedTetrimino = false;
        bool animateLinesRemove = false;
        int animateLinesFrame = 0;
        bool forceDownMovement = false;
        bool removeLines = false;
        std::map<int, bool> linesFilled = {};
        TetrisStates state = TetrisStates::None;
        TetrisMoveDirection moveDirection = TetrisMoveDirection::Down;
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

        std::unordered_map<Traits, bool> shop_traits = {};
        std::unordered_map<WeaponSprite, bool> shop_weapons = {};
        
        bool keyPressed = false;
        bool hardcoreTetrisMod = false;
        
        int battleCounter = 0;
        int turn = 0;
        bool turnNext = false;
        bool playerAttacks = false;
        bool paused = false;
        std::unordered_map<BlockType, int> clearedBlocks = {
            {BlockType::Attack,     0},
            {BlockType::Defense,    0},
            {BlockType::Healing,    0},
            {BlockType::Treasure,   0},
            {BlockType::Block,      0}
        };
        
        Character playerCharacter;
        Character npcCharacter;
        
        Character* attacker = nullptr;
        Character* defender = nullptr;
        
//        void GameLogic(GameDB& gameDB);
        void Heal(Character& character);
        void Attack(Character& attacker, Character& defender, std::stringstream& strForLogger);
        void AccumulateAttack(Character& character);
        void AccumulateHeal(Character& character);
        void AccumulateDefense(Character& character);
        void AccumulateTreasure(Character& character);
        void AccumulateScore(Character& character);

    };
    
    GameDB& InitGameData();
    TetriminoTemplatesDB& InitTetriminoDB();
    void GameNextTurn(Game& game);
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
            void Init(Layer* layer, 
                 const glm::uvec4& area, 
                 const std::string& spritelist,
                 DTetris::TetrisMap* tetrisMap,
                 int width,
                 int height);
            virtual ~TetrisMapGUI();
            
            // general GUIItem interface
            virtual void Display() override;
            
        protected:
            void ChooseSrcRect(DTetris::BlockType blockType, SDL_Rect& srcRect, bool remove = false);

            // sprite-based
            Texture* m_spriteList = nullptr;
            DTetris::TetrisMap* m_tetrisMap = nullptr;
            int m_width = 0;
            int m_height = 0;
        };
    }
}

#endif
