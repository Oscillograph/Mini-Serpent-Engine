#ifndef LESTA_AUTOBATTLER_DATA_LOADER_H
#define LESTA_AUTOBATTLER_DATA_LOADER_H

#include <mse/core.h>
#include <lesta-autobattler/game-fwd.h>
#include <yaml-cpp/yaml.h>

namespace YAML 
{
    // custom types
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


namespace LAutobattler
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
    
    bool LoadConfig(mse::GameConfig& config, const std::string& filename = "data/config.yaml");
    void SaveConfig(const mse::GameConfig& config, const std::string& filename = "data/config.yaml");
    bool LoadHighScores(HighScoresDB& highscores, const std::string filename = "data/highscores.yaml");
    void SaveHighScores(const HighScoresDB& highscores, const std::string& filename = "data/highscores.yaml");
}

#endif
