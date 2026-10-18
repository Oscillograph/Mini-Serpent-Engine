#include <mse/core.h>
#include <dungeontris/game-fwd.h>
#include <dungeontris/data-loader.h>

#include <yaml-cpp/yaml.h>

/* ============================================================================================== */
/*                                 YAML ENCODE/DECODE                                             */
/* ============================================================================================== */
YAML::Emitter& operator<<(YAML::Emitter& out, const DTetris::GameConfig& config)
{
    out << YAML::Flow;
    out << YAML::Key << "Music" << YAML::Value << config.music;
    out << YAML::Key << "MusicVolume" << YAML::Value << config.musicVolume;
    out << YAML::Key << "Sounds" << YAML::Value << config.sounds;
    out << YAML::Key << "SoundsVolume" << YAML::Value << config.soundsVolume;
    out << YAML::Key << "PlayInBackground" << YAML::Value << config.playInBackground;
    out << YAML::Key << "Fullscreen" << YAML::Value << config.fullscreen;
    out << YAML::Key << "TurnLength" << YAML::Value << config.turnLength;
    out << YAML::Key << "FastBattle" << YAML::Value << config.fastBattle;
    return out;
}


/* ============================================================================================== */
/*                                 DATA LOADER/SAVER CODE                                         */
/* ============================================================================================== */
namespace DTetris
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
            !data["Fullscreen"] ||
            !data["TurnLength"] ||
            !data["FastBattle"])
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
        config.turnLength = data["TurnLength"].as<int>();
        config.fastBattle = data["FastBattle"].as<bool>();
        
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

    bool LoadCharacters(GameDB& gamedb, const std::string& filename)
    {
        std::string yamlRawText = mse::FileIO::GetRawText(filename);
        YAML::Node data = YAML::Load(yamlRawText);

        if (data.size() > 0)
        {
            // fill gamedb.characters
            for (size_t i = 0; i < data.size(); ++i)
            {
                if (!data[i]["name"] ||
                    !data[i]["race"] ||
                    !data[i]["level"] ||
                    !data[i]["stats_max"] ||
                    !data[i]["traits"] ||
                    !data[i]["drops"] ||
                    !data[i]["weapon"])
                {
                    return false;
                }

                gamedb.characters[i] =
                {
                    data[i]["level"].as<size_t>(),
                    utf8::utf8to32(data[i]["name"].as<std::string>()),
                    data[i]["race"].as<Races>(),
                    {
                        data[i]["stats_max"][0].as<float>(),  // health
                        data[i]["stats_max"][1].as<float>(),  // strength
                        data[i]["stats_max"][2].as<float>(),  // agility
                        data[i]["stats_max"][3].as<float>()   // endurance
                    },
                    {
                        data[i]["stats_max"][0].as<float>(),  // health
                        data[i]["stats_max"][1].as<float>(),  // strength
                        data[i]["stats_max"][2].as<float>(),  // agility
                        data[i]["stats_max"][3].as<float>()   // endurance
                    },
                    {Classes::None, 0},
                    {Classes::None, 0},
                    {},
                    {},
                    gamedb.weapons[data[i]["weapon"].as<int>()],
                    {
                        {BlockType::Attack,     0},
                        {BlockType::Defense,    0},
                        {BlockType::Healing,    0},
                        {BlockType::Treasure,   0},
                        {BlockType::Block,      0}
                    }
                };

                for (size_t i = 0; i < data[i]["traits"].size(); ++i)
                {
                    gamedb.characters[i].traits.push_back(data[i]["traits"].as<Traits>());
                }

                for (size_t i = 0; i < data[i]["drops"].size(); ++i)
                {
                    gamedb.characters[i].drop_list.push_back(gamedb.weapons[data[i]["weapon"].as<int>()]);
                }
            }
        }

        return false;
    }

    bool LoadWeapons(GameDB& gamedb, const std::string& filename)
    {
        std::string yamlRawText = mse::FileIO::GetRawText(filename);
        YAML::Node data = YAML::Load(yamlRawText);

        if (data.size() > 0)
        {
            // fill gamedb.weapons
            for (size_t i = 0; i < data.size(); ++i)
            {
                if (!data[i]["name"] ||
                    !data[i]["damage"] ||
                    !data[i]["damage_type"] ||
                    !data[i]["sprite"])
                {
                    return false;
                }

                gamedb.weapons[i] =
                {
                    {utf8::utf8to32(data[i]["name"].as<std::string>())},
                    data[i]["damage"].as<int>(),
                    data[i]["damage_type"].as<DamageType>(),
                    data[i]["sprite"].as<WeaponSprite>()
                };
            }
        }

        return false;
    }

    bool LoadArmors(GameDB& gamedb, const std::string& filename)
    {
        std::string yamlRawText = mse::FileIO::GetRawText(filename);
        YAML::Node data = YAML::Load(yamlRawText);

        // fill gamedb.armors
        if (data.size() > 0)
        {
            for (size_t i = 0; i < data.size(); ++i)
            {
                if (!data[i]["name"] ||
                    !data[i]["defense_cutting"] ||
                    !data[i]["defense_crushing"] ||
                    !data[i]["defense_piercing"] ||
                    !data[i]["defense_skill"] ||
                    !data[i]["defense_stats"])
                {
                    return false;
                }

                gamedb.armors[i] =
                {
                    {utf8::utf8to32(data[i]["name"].as<std::string>())},
                    data[i]["damage_cutting"].as<float>(),
                    data[i]["damage_crushing"].as<float>(),
                    data[i]["damage_piercing"].as<float>(),
                    data[i]["damage_skill"].as<float>(),
                    data[i]["damage_stats"].as<float>(),
                };
            }
        }

        return false;
    }

    bool LoadTraits(GameDB& gamedb, const std::string& filename)
    {
        std::string yamlRawText = mse::FileIO::GetRawText(filename);
        YAML::Node data = YAML::Load(yamlRawText);

        // fill gamedb.traits
        if (data.size() > 0)
        {
            for (size_t i = 0; i < data.size(); ++i)
            {
                if (!data[i]["name"] ||
                    !data[i]["type_hardcoded"] ||
                    !data[i]["description"])
                {
                    return false;
                }

                gamedb.traits[data[i]["type_hardcoded"].as<Traits>()] = {
                    data[i]["type_hardcoded"].as<Traits>(),
                    utf8::utf8to32(data[i]["name"].as<std::string>()),
                    utf8::utf8to32(data[i]["description"].as<std::string>())
                };
            }
        }

        return false;
    }

    bool LoadClasses(GameDB& gamedb, const std::string& filename)
    {
        std::string yamlRawText = mse::FileIO::GetRawText(filename);
        YAML::Node data = YAML::Load(yamlRawText);

        // fill gamedb.classes
        if (data.size() > 0)
        {
            for (size_t i = 0; i < data.size(); ++i)
            {
                if (!data[i]["name"] ||
                    !data[i]["class_hardcoded"])
                {
                    return false;
                }

                gamedb.classes[data[i]["class_hardcoded"].as<Classes>()] = {
                    data[i]["class_hardcoded"].as<Classes>(),
                    utf8::utf8to32(data[i]["name"].as<std::string>())
                };
            }
        }

        return false;
    }
}
