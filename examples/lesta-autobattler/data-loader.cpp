#include <lesta-autobattler/data-loader.h>

#include <yaml-cpp/yaml.h>
#include <mse/utils/fileio.h>

namespace LAutobattler
{
    bool LoadConfig(GameConfig& config, const std::string& filename)
    {
        // open file and read its data
        std::string configRawText = mse::FileIO::GetRawText(filename);
        
        // parse data
        YAML::Node data = YAML::Load(configRawText);
        if (!data["Music"] || 
            !data["MusicVolume"] ||
            !data["Sounds"] ||
            !data["SoundsVolume"] ||
            !data["PlayInBackground"] ||
            !data["Fullscreen"])
        {
            // failed structure
            return false;
        }
        
        // edit prepared data structure
        config.music = data["Music"].as<bool>();
        config.musicVolume = data["MusicVolume"].as<int>();
        config.sounds = data["Sounds"].as<bool>();
        config.soundsVolume = data["SoundsVolume"].as<int>();
        config.playInBackground = data["PlayInBackground"].as<bool>();
        config.fullscreen = data["Fullscreen"].as<bool>();
        
        // return the results
        return true;
    }
    
    bool LoadHighScores(HighScoresDB& highscores, const std::string filename)
    {
        // open file and read its data
        std::string highscoresRawText = mse::FileIO::GetRawText(filename);
        
        // parse data
        YAML::Node data = YAML::Load(highscoresRawText);
        
        // edit prepared data structure
        
        // return the results
        return false;
    }
    
    void SaveConfig(const GameConfig& config, const std::string& filename)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << config;
        out << YAML::EndMap;
        
        mse::FileIO::WriteRawText(filename, out.c_str());
    }
    
    void SaveHighScores(const HighScoresDB& highscores, const std::string& filename)
    {}
}
