#include <mse/core.h>
#include <lesta-autobattler/data-loader.h>

#include <yaml-cpp/yaml.h>
#include <mse/utils/fileio.h>

/* ============================================================================================== */
/*                                 YAML ENCODE/DECODE                                             */
/* ============================================================================================== */
YAML::Emitter& operator<<(YAML::Emitter& out, const mse::GameConfig& config)
{
    out << YAML::Flow;
    out << YAML::Key << "Music" << YAML::Value << config.music;
    out << YAML::Key << "MusicVolume" << YAML::Value << config.musicVolume;
    out << YAML::Key << "Sounds" << YAML::Value << config.sounds;
    out << YAML::Key << "SoundsVolume" << YAML::Value << config.soundsVolume;
    out << YAML::Key << "PlayInBackground" << YAML::Value << config.playInBackground;
    out << YAML::Key << "Fullscreen" << YAML::Value << config.fullscreen;
    return out;
}


/* ============================================================================================== */
/*                                 DATA LOADER/SAVER CODE                                         */
/* ============================================================================================== */
namespace LAutobattler
{
    bool LoadConfig(mse::GameConfig& config, const std::string& filename)
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
        config.musicVolume = data["MusicVolume"].as<float>();
        config.sounds = data["Sounds"].as<bool>();
        config.soundsVolume = data["SoundsVolume"].as<float>();
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
    
    void SaveConfig(const mse::GameConfig& config, const std::string& filename)
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
