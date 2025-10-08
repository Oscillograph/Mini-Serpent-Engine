#include <lesta-autobattler/data-loader.h>

#include <yaml-cpp/yaml.h>

namespace LAutobattler
{
    GameConfig& LoadConfig(const std::string& filename)
    {
        // prepare data structure
        static GameConfig gameConfig;
        
        // open file and read its data
        
        // parse data
        
        // edit prepared data structure
        
        // return the results
        return gameConfig;
    }
    
    HighScoresDB& LoadHighScores(const std::string filename)
    {
        // prepare data structure
        static HighScoresDB highscores;
        
        // open file and read its data
        
        // parse data
        
        // edit prepared data structure
        
        // return the results
        return highscores;
    }
}
