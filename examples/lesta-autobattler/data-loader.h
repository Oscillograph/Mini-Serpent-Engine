#ifndef LESTA_AUTOBATTLER_DATA_LOADER_H
#define LESTA_AUTOBATTLER_DATA_LOADER_H

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
    struct GameConfig
    {
        bool music = false;
        int musicVolume = 0;
        bool sounds = false;
        int soundsVolume = 0;
        bool fullscreen = true;
    };
    
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
    
    /* ========================================================================================== */
    /*                                 YAML ENCODE/DECODE                                         */
    /* ========================================================================================== */
    YAML::Emitter& operator<<(YAML::Emitter& out, const GameConfig& config)
    {
        out << YAML::Flow;
        out << YAML::Key << "Music" << YAML::Value << config.music;
        out << YAML::Key << "MusicVolume" << YAML::Value << config.musicVolume;
        out << YAML::Key << "Sounds" << YAML::Value << config.sounds;
        out << YAML::Key << "SoundsVolume" << YAML::Value << config.soundsVolume;
        out << YAML::Key << "Fullscreen" << YAML::Value << config.fullscreen;
        return out;
    }
    
    
    
    bool LoadConfig(GameConfig& config, const std::string& filename = "data/config.yaml");
    void SaveConfig(const GameConfig& config, const std::string& filename = "data/config.yaml");
    bool LoadHighScores(HighScoresDB& highscores, const std::string filename = "data/highscores.yaml");
    void SaveHighScores(const HighScoresDB& highscores, const std::string& filename = "data/highscores.yaml");
}

#endif
