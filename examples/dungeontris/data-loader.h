#ifndef DUNGEONTRIS_AUTOBATTLER_DATA_LOADER_H
#define DUNGEONTRIS_AUTOBATTLER_DATA_LOADER_H

#include <mse/core.h>
#include <dungeontris/game-fwd.h>
#include <yaml-cpp/yaml.h>

namespace YAML 
{
    // custom types
    template<>
    struct convert<DTetris::Races>
    {
        static Node encode(const DTetris::Races& rhs)
        {
            Node node;
            node.push_back((int)rhs);
            return node;
        }

        static bool decode(const Node& node, DTetris::Races& rhs)
        {
            rhs = static_cast<DTetris::Races>(node.as<int>());
            return true;
        }
    };

    template<>
    struct convert<DTetris::Traits>
    {
        static Node encode(const DTetris::Traits& rhs)
        {
            Node node;
            node.push_back((int)rhs);
            return node;
        }

        static bool decode(const Node& node, DTetris::Traits& rhs)
        {
            rhs = static_cast<DTetris::Traits>(node.as<int>());
            return true;
        }
    };

    template<>
    struct convert<DTetris::WeaponSprite>
    {
        static Node encode(const DTetris::WeaponSprite& rhs)
        {
            Node node;
            node.push_back((int)rhs);
            return node;
        }

        static bool decode(const Node& node, DTetris::WeaponSprite& rhs)
        {
            rhs = static_cast<DTetris::WeaponSprite>(node.as<int>());
            return true;
        }
    };

    template<>
    struct convert<DTetris::DamageType>
    {
        static Node encode(const DTetris::DamageType& rhs)
        {
            Node node;
            node.push_back((int)rhs);
            return node;
        }

        static bool decode(const Node& node, DTetris::DamageType& rhs)
        {
            rhs = static_cast<DTetris::DamageType>(node.as<int>());
            return true;
        }
    };

    template<>
    struct convert<DTetris::Classes>
    {
        static Node encode(const DTetris::Classes& rhs)
        {
            Node node;
            node.push_back((int)rhs);
            return node;
        }

        static bool decode(const Node& node, DTetris::Classes& rhs)
        {
            rhs = static_cast<DTetris::Classes>(node.as<int>());
            return true;
        }
    };
//    template<>
//    struct convert<glm::vec3> 
//    {
//        static Node encode(const glm::vec3& rhs) 
//        {
//            Node node;
//            node.push_back(rhs.x);
//            node.push_back(rhs.y);
//            node.push_back(rhs.z);
//            return node;
//        }
//        
//        static bool decode(const Node& node, glm::vec3& rhs) 
//        {
//            if (!node.IsSequence() || (node.size() != 3))
//            {
//                return false;
//            }
//            rhs.x = node[0].as<float>();
//            rhs.y = node[1].as<float>();
//            rhs.z = node[2].as<float>();
//            return true;
//        }
//    };
//    
//    template<>
//    struct convert<glm::vec4> 
//    {
//        static Node encode(const glm::vec4& rhs) 
//        {
//            Node node;
//            node.push_back(rhs.x);
//            node.push_back(rhs.y);
//            node.push_back(rhs.z);
//            node.push_back(rhs.w);
//            return node;
//        }
//        
//        static bool decode(const Node& node, glm::vec4& rhs) 
//        {
//            if (!node.IsSequence() || (node.size() != 4))
//            {
//                return false;
//            }
//            rhs.x = node[0].as<float>();
//            rhs.y = node[1].as<float>();
//            rhs.z = node[2].as<float>();
//            rhs.w = node[3].as<float>();
//            return true;
//        }
//    };
}


namespace DTetris
{
    /* ========================================================================================== */
    /*                                        TYPES                                               */
    /* ========================================================================================== */
    struct HighScore
    {
        std::u32string nick = U"";
        Class characterClass = {Classes::None, 1};
        Weapon weapon;
        int score = 0;
    };
    
    struct HighScoresDB
    {
        std::vector<HighScore> data;
    };
    
    bool LoadConfig(GameConfig& config, const std::string& filename = "data/config.yaml");
    void SaveConfig(const GameConfig& config, const std::string& filename = "data/config.yaml");
    bool LoadHighScores(HighScoresDB& highscores, const std::string filename = "data/highscores.yaml");
    void SaveHighScores(const HighScoresDB& highscores, const std::string& filename = "data/highscores.yaml");

    bool LoadWeapons(GameDB& gamedb, const std::string& filename = "data/weapons.yaml");
    bool LoadArmors(GameDB& gamedb, const std::string& filename = "data/armors.yaml");
    bool LoadTraits(GameDB& gamedb, const std::string& filename = "data/traits.yaml");
    bool LoadDamageTypes(GameDB& gamedb, const std::string& filename = "data/damagetypes.yaml");
    bool LoadClasses(GameDB& gamedb, const std::string& filename = "data/classes.yaml");
    bool LoadCharacters(GameDB& gamedb, const std::string& filename = "data/characters.yaml");
}

#endif
