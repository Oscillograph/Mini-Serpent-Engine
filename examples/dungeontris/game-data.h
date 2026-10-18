#ifndef DUNGEONTRIS_AUTOBATTLER_GAME_DATA_H
#define DUNGEONTRIS_AUTOBATTLER_GAME_DATA_H

#include <dungeontris/game-fwd.h>
#include <dungeontris/data-loader.h>

// globals
namespace DTetris
{    
    // ****************************************************************************************** //
    //                             General Game Types and Constants
    // ****************************************************************************************** //
    inline TetriminoTemplate::TetriminoTemplate(const std::vector<int>& data)
    {
        block = data;
    }
        
    
    inline GameDB& InitGameData()
    {
        static GameDB gameDB;
        gameDB.level_max = 3;
        
        LoadWeapons(gameDB);
        LoadArmors(gameDB);
        LoadTraits(gameDB);
        LoadDamageTypes(gameDB);
        LoadClasses(gameDB);
        LoadCharacters(gameDB);

        return gameDB;
    }
    
    inline TetriminoTemplatesDB& InitTetriminoDB()
    {
        static TetriminoTemplatesDB tetriminoDB;
        // empty block
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        }));
        // classic mode
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 1, 0, 0,
            0, 1, 1, 0,
            0, 1, 0, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 1, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 0, 0, 0,
            0, 1, 1, 0,
            0, 1, 1, 0,
            0, 0, 0, 0
        }));            
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 1, 1, 0,
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 1, 1, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 1, 0, 0,
            0, 1, 1, 0,
            0, 0, 1, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 0, 1, 0,
            0, 1, 1, 0,
            0, 1, 0, 0,
            0, 0, 0, 0
        }));
        // brick game mode
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 0, 0, 0,
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 0, 0, 0,
            0, 1, 1, 0,
            0, 1, 0, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            1, 0, 1, 0,
            1, 1, 1, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            1, 1, 1, 0,
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 1, 1, 0,
            0, 1, 0, 0,
            1, 1, 0, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            1, 1, 0, 0,
            0, 1, 0, 0,
            0, 1, 1, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 0, 1, 0,
            0, 0, 1, 0,
            1, 1, 1, 0,
            0, 0, 0, 0
        }));
        tetriminoDB.data.push_back(TetriminoTemplate({
            0, 1, 0, 0,
            1, 1, 1, 0,
            0, 1, 0, 0,
            0, 0, 0, 0
        }));
        
        return tetriminoDB;
    }
}

#endif
