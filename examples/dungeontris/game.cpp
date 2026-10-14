#include "dungeontris/tetris.h"
#include "mse/systems/application/application.h"
#include "mse/systems/ecs/components.h"
#include "mse/systems/platform/events/events.h"
#include <algorithm>
#include <dungeontris/game-fwd.h>
#include <random> // rand
#include <ctime> // clock
#include <sstream> // stringstream
#include <utf8.h> // utf8

#include <mse/systems/platform/input/input.h>
#include <mse/systems/windows/layers/layer.h>
#include <mse/systems/platform/renderer/renderer.h>
#include <mse/systems/resources/resource_manager.h>
#include <mse/systems/platform/audio/soundman.h>
#include <mse/systems/windows/window.h>

#include <dungeontris/game-fwd.h> // DTetris::TetrisEngineData

extern DTetris::Tetrimino tetrimino;
extern DTetris::TetrisEngineData DTetris::tetrisEngineData;

namespace DTetris
{
    void GameNextTurn(Game& game)
    {
        game.turnNext = true;
        game.turn++;
    }

    MessageLog::MessageLog()
    {}
    
    MessageLog::~MessageLog()
    {
        Clear();
    }
    
    void MessageLog::Clear()
    {
        int i = 0;
        while(Pop() && (i < size_max))
        {
            i++;
        }
    }
    
    bool MessageLog::Pop()
    {
        if (stack != nullptr)
        {
            MessageLogItem* current = stack->next;
            delete stack;
            stack = current;
            current = nullptr;
            size--;
            
            return true;
        }
        return false;
    }
    
    void MessageLog::Push(std::u32string text)
    {
        MessageLogItem* current = stack;
        for (int i = 0; i < size_max; ++i)
        {
            if (stack == nullptr)
            {
                stack = new MessageLogItem();
                stack->text = text;
                size++;
                i = size_max; // exit cycle
            } else {
                if (current->next != nullptr)
                {
                    current = current->next;
                } else {
                    current->next = new MessageLogItem();
                    current->next->text = text;
                    size++;
                    i = size_max; // exit cycle
                }
            }
        }
        
        if (size > size_max)
        {
            Pop();
        }
    }

    void Game::Heal(Character& character)
    {
        character.stats.health += character.blocks[DTetris::BlockType::Healing];
        float realMaxHealth = character.level * (character.stats_max.health + character.stats_max.endurance);
        if (character.stats.health > realMaxHealth)
        {
            character.stats.health = realMaxHealth;
        }
        character.blocks[DTetris::BlockType::Healing] = 0;
    }

    void Game::Attack(Character& attacker, Character& defender, std::stringstream& strForLogger)
    {
        float weaponDamage = attacker.weapon.damage;
        float skillDamage = 0.0;
        float totalDamage = 0.0;

        printf("Turn %d: %s attacks.\n",
               turn,
               (char*)(attacker.name.c_str()));

        // start of the turn
        // calculate attack chance
        int agi_sum = attacker.stats.agility + defender.stats.agility;
        int dice = (std::rand() % agi_sum) + 1;
        //                        printf("Rolled %d of %d\n", dice, agi_sum);
        if (dice > defender.stats.agility)
        {
            // apply attacker's effects
            for (DTetris::Traits trait : attacker.traits)
            {
                if (turn == 1)
                {
                    if (trait == DTetris::Traits::Rush)
                    {
                        weaponDamage = attacker.weapon.damage * 2;
                        skillDamage = attacker.stats.strength * 2;
                    }
                }

                if ((trait == DTetris::Traits::HiddenStrike) &&
                    (attacker.stats.agility > defender.stats.agility))
                {
                    skillDamage += 1;
                }

                if (trait == DTetris::Traits::Poison)
                {
                    skillDamage += turn;
                }

                if (trait == DTetris::Traits::Rage)
                {
                    skillDamage = (turn < 4) ? (skillDamage + 2) : (skillDamage - 1);
                }

                if (trait == DTetris::Traits::FireBreather)
                {
                    skillDamage = ((turn % 3) == 0) ? skillDamage + 3 : skillDamage;
                }
            }

            // apply defender's effects
            for (Traits trait : defender.traits)
            {
                if ((trait == Traits::CuttingImmune) &&
                    (attacker.weapon.type == DamageType::Cutting))
                {
                    weaponDamage = 0.0;
                }

                if ((trait == Traits::CrushingWeakness) &&
                    (attacker.weapon.type == DamageType::Crushing))
                {
                    weaponDamage = weaponDamage * 2;
                }
            }

            // apply removed blocks damage
            skillDamage += attacker.blocks[BlockType::Attack];
            attacker.blocks[BlockType::Attack] = 0;

            // pre-calculate total damage
            totalDamage = weaponDamage + skillDamage;

            // apply defender's effects that affect total damage
            for (Traits trait : defender.traits)
            {
                if (trait == Traits::StoneSkin)
                {
                    totalDamage -= defender.stats.endurance;
                }

                if ((trait == Traits::Shield) &&
                    (defender.stats.strength > attacker.stats.strength))
                {
                    totalDamage -= 3;
                }

                // apply removed blocks defence
                totalDamage -= defender.blocks[BlockType::Defense];
                defender.blocks[BlockType::Defense] = 0;
            }

            if (totalDamage < 0.0)
            {
                totalDamage = 0.0;
            }

            // clash
            defender.stats.health -= totalDamage;
            printf("Turn %d: %s deals %.2f damage to %s, leaving %.2f health.\n",
                   turn,
                   (char*)(attacker.name.c_str()),
                   totalDamage,
                   (char*)(defender.name.c_str()),
                   defender.stats.health);

            strForLogger << "Ход " << turn << ": "
            << utf8::utf32to8(attacker.name.c_str())
            //                << "(" << game.attacker->stats.health << ")"
            << " наносит " << totalDamage << " урона "
            //                << utf8::utf32to8(game.defender->name.c_str())
            //                << "(" << game.defender->stats.health << ")"
            << "\n";

            if (!config.fastBattle)
            {
                if (attacker.race == Races::Human)
                {
                    mse::SoundMan::PlaySound("data/audio/sounds/samplefocus.com-sword-cut-fx.wav");
                } else {
                    mse::SoundMan::PlaySound("data/audio/sounds/samplefocus.com-violent-sword-slice-sound-fx_D_minor.wav");
                }
            }
            UILogger.Push(utf8::utf8to32(strForLogger.str()));
        } else {
            printf("Turn %d: %s misses!\n",
                   turn,
                   (char*)(attacker.name.c_str()));

            strForLogger << "Ход " << turn << ": "
            << utf8::utf32to8(attacker.name.c_str())
            //                << "(" << game.attacker->stats.health << ")"
            << " промахивается!\n";
            if (!config.fastBattle)
            {
                mse::SoundMan::PlaySound("data/audio/sounds/samplefocus.com-short-knife-whoosh-fx.wav");
            }
            UILogger.Push(utf8::utf8to32(strForLogger.str()));
        }

        if (defender.stats.health <= 0.0)
        {
            battleFinished = true;
        }
    }

    void Game::AccumulateAttack(Character& character)
    {
        character.blocks[BlockType::Attack] = clearedBlocks[BlockType::Attack];
        clearedBlocks[BlockType::Attack] = 0;
    }

    void Game::AccumulateHeal(Character& character)
    {
        character.blocks[BlockType::Healing] = clearedBlocks[BlockType::Healing];
        clearedBlocks[BlockType::Healing] = 0;
    }

    void Game::AccumulateDefense(Character& character)
    {
        character.blocks[BlockType::Defense] += clearedBlocks[BlockType::Defense];
        clearedBlocks[BlockType::Defense] = 0;
    }

    void Game::AccumulateTreasure(Character& character)
    {
        character.blocks[BlockType::Treasure] += clearedBlocks[BlockType::Treasure];
        clearedBlocks[BlockType::Treasure] = 0;
    }

    void Game::AccumulateScore(Character& character)
    {}

    void TetrisMap::Resize(int w, int h)
    {
        width = w;
        height = h;
        int total = width * height;
        
        map.resize(0);
        map_backend.resize(0);
        blockSprites.resize(0);
        map.resize(total);
        map_backend.resize(total);
        blockSprites.resize(total);
        
        for (int j = 0; j < height; ++j)
        {
            for (int i = 0; i < width; ++i)
            {
                int index = j*width + i;
                map_backend[index].coordinates.x = i;
                map_backend[index].coordinates.y = j;
                map_backend[index].remove = false;
                map_backend[index].type = BlockType::None;
                map_backend[index].color = {0, 0, 0};
                
                blockSprites[index].place = {
                    map_backend[index].coordinates.x * 10,
                    map_backend[index].coordinates.y * 10,
                    10,
                    10
                };
                blockSprites[index].texture = nullptr;
            }
        }

        map.assign(map_backend.begin(), map_backend.end());
    }

    void TetrisMap::Clear()
    {
        for (int j = 0; j < height; ++j)
        {
            for (int i = 0; i < width; ++i)
            {
                int index = j*width + i;
                map_backend[index].coordinates.x = i;
                map_backend[index].coordinates.y = j;
                map_backend[index].remove = false;

                if ((j == height-1) || (i == 0) || (i == width-1))
                {
                    map_backend[index].type = BlockType::Wall;
                } else {
                    map_backend[index].type = BlockType::None;
                    map_backend[index].color = {0, 0, 0};
                }

                blockSprites[index].place = {
                    map_backend[index].coordinates.x * 10,
                    map_backend[index].coordinates.y * 10,
                    10,
                    10
                };
                blockSprites[index].texture = nullptr;
            }
        }

        map.assign(map_backend.begin(), map_backend.end());
    }

    void TetrisMap::ClearLines(std::map<int, bool>& linesFilled)
    {
        for (int j = 0; j < height; ++j)
        {
            for (int i = 0; i < width; ++i)
            {
                int index = j*width + i;
                map_backend[index].coordinates.x = i;
                map_backend[index].coordinates.y = j;

                if ((j == height-1) || (i == 0) || (i == width-1))
                {
                    map_backend[index].type = BlockType::Wall;
                } else {
                    map_backend[index].type = BlockType::None;
                    map_backend[index].color = {0, 0, 0};
                }
                map_backend[index].remove = false;

                blockSprites[index].place = {
                    map_backend[index].coordinates.x * 10,
                    map_backend[index].coordinates.y * 10,
                    10,
                    10
                };
                blockSprites[index].texture = nullptr;
            }
        }

        int moveUpCount = 0;
        int moveUpFactor = 0;
        int yFactor = 0;

        for (int y = height - 1; y > -1; --y)
        {
            yFactor = y * width;
            while (linesFilled.find(y - moveUpCount) != linesFilled.end())
            {
                moveUpCount++;
                MSE_LOG("TetrisMap::ClearLines - Found line to remove: ", y);
                linesFilled.erase(y);
            }

            if ((y - moveUpCount) > 0)
            {
                moveUpFactor = moveUpCount * width;

                MSE_LOG("TetrisMap::ClearLines - y, moveUp: ", y, "; ", moveUpCount);
                for (int x = 1; x < width - 1; ++x)
                {
                    map_backend[x + yFactor] = map[x + yFactor - moveUpFactor];
                    map_backend[x + yFactor].remove = false;
                }
            }
        }

        map.assign(map_backend.begin(), map_backend.end());
    }
}

namespace mse
{
    namespace gui
    {
        TetrisMapGUI::TetrisMapGUI()
        : GUIItem()
        {
            Init(nullptr, {0, 0, 0, 0}, "", nullptr, 0, 0);
        }
        
        TetrisMapGUI::TetrisMapGUI(Layer* layer, 
                                   const glm::uvec4& area, 
                                   const std::string& spritelist, 
                                   DTetris::TetrisMap* tetrisMap, 
                                   int width, 
                                   int height)
        : GUIItem()
        {
            MSE_CORE_LOG("TetrisMapGUI construction");
            Init(layer, area, spritelist, tetrisMap, width, height);
            MSE_CORE_LOG("TetrisMapGUI constructed");
        }
        
        void TetrisMapGUI::Init(Layer* layer, 
                           const glm::uvec4& area, 
                           const std::string& spritelist,
                           DTetris::TetrisMap* tetrisMap,
                           int width,
                           int height)
        {
            MSE_CORE_LOG("TetrisMapGUI: initialization...")
            MSE_CORE_LOG("layer address: ", layer);
            MSE_CORE_LOG("Nullptr is: ", nullptr);
            if (layer != nullptr)
            {
                MSE_CORE_LOG("Layer exists!");
                
                // model
                parentLayer = layer;
                windowUser = layer->GetWindow();
                m_elementName = "TetrisMapGUI";
                layerArea = area;
                m_spriteList = (Texture*)(ResourceManager::UseResource(ResourceType::Texture, spritelist, windowUser)->data);
                m_tetrisMap = tetrisMap;
                m_width = width;
                m_height = height;
                
                layerMask.resize(area.z * area.w);
                for (unsigned int x = 0; x < area.z; ++x)
                {
                    for (unsigned int y = 0; y < area.w; ++y)
                    {
                        layerMask[x + y*area.z] = id;
                    }
                }
                
                // view
                
                // controller
                callbacks[EventTypes::KeyDown] = [&](SDL_Event* event){
                    switch (event->key.key)
                    {
                        case mse::KeyCode::_w:
                        case mse::KeyCode::NumPad_8:
                        case mse::KeyCode::Space:
                        case mse::KeyCode::Up:
                        {
                            MSE_CORE_LOG("TetrisMapGUI: Rotate tetrimino");
                            DTetris::tetrisEngineData.rotateTetrimino = true;
                            break;
                        }
                        case mse::KeyCode::_a:
                        case mse::KeyCode::NumPad_4:
                        case mse::KeyCode::Left:
                        {
                            MSE_CORE_LOG("TetrisMapGUI: Move tetrimino Left");
                            DTetris::tetrisEngineData.moveDirection = DTetris::TetrisMoveDirection::Left;
                            break;
                        }
                        case mse::KeyCode::_d:
                        case mse::KeyCode::NumPad_6:
                        case mse::KeyCode::Right:
                        {
                            MSE_CORE_LOG("TetrisMapGUI: Move tetrimino Right");
                            DTetris::tetrisEngineData.moveDirection = DTetris::TetrisMoveDirection::Right;
                            break;
                        }
                        case mse::KeyCode::_s:
                        case mse::KeyCode::NumPad_2:
                        case mse::KeyCode::Down:
                        {
                            MSE_CORE_LOG("TetrisMapGUI: Move tetrimino Down");
                            DTetris::tetrisEngineData.moveDirection = DTetris::TetrisMoveDirection::Down;
                            DTetris::tetrisEngineData.forceDownMovement = true;
                            break;
                        }

                        case mse::KeyCode::_p:
                        {
                            if (DTetris::tetrisEngineData.state != DTetris::TetrisStates::Paused)
                            {
                                DTetris::tetrisEngineData.state = DTetris::TetrisStates::Paused;
                            } else {
                                DTetris::tetrisEngineData.state = DTetris::TetrisStates::Processing;
                            }
                            break;
                        }
                    }
                };

                callbacks[EventTypes::KeyUp] = [&](SDL_Event* event){
                    switch (event->key.key)
                    {
                        case mse::KeyCode::_s:
                        case mse::KeyCode::NumPad_2:
                        case mse::KeyCode::Down:
                        {
                            if (DTetris::tetrisEngineData.forceDownMovement)
                            {
                                DTetris::tetrisEngineData.forceDownMovement = false;
                            }
                            break;
                        }
                    }
                };

                MSE_CORE_LOG("TetrisMapGUI: initialization complete");
            } else {
                MSE_CORE_LOG("TetrisMap: failed to initialize due to non-existent layer");
            }
        }
        
        TetrisMapGUI::~TetrisMapGUI()
        {}
        
        void TetrisMapGUI::Display()
        {
            static int frameCounter = 0;

            // MSE_CORE_LOG("TetrisMapGUI: Display");
            if (parentLayer != nullptr)
            {
                // MSE_CORE_LOG("TetrisMapGUI: Display");
                SDL_FRect destRect = {
                    0.0f,
                    0.0f,
                    10.0f / windowUser->GetPrefs().width,
                    10.0f / windowUser->GetPrefs().height
                };
                SDL_Rect srcRect = {
                    0,
                    0,
                    10,
                    10
                };

                for (int xIndex = 0; xIndex < m_width; ++xIndex)
                {
                    destRect.x = (float)(layerArea.x) / windowUser->GetPrefs().width + xIndex*destRect.w;
//                    destRect.x = 0 + xIndex*destRect.w;
                    for (int yIndex = 0; yIndex < m_height; ++yIndex)
                    {
                        // skip the iteration if the block is empty
                        if (m_tetrisMap->map[yIndex*m_width + xIndex].type != DTetris::BlockType::None)
                        {
                            destRect.y = (float)(layerArea.y) / windowUser->GetPrefs().height + yIndex*destRect.h;

                            // pick a proper image to draw
                            ChooseSrcRect(m_tetrisMap->map[yIndex*m_width + xIndex].type, srcRect, m_tetrisMap->map[yIndex*m_width + xIndex].remove);

                            // draw the image of a block
                            // MSE_LOG("Drawing at: ", destRect.x, ", ", destRect.y);
                            Renderer::DrawTexture(m_spriteList, &destRect, &srcRect);
                        }
                    }
                }

                int xIndex, yIndex = 0;
                for (int x = 0; x < 4; ++x)
                {
                    xIndex = x + tetrimino.x;
                    destRect.x = (float)(layerArea.x) / windowUser->GetPrefs().width + xIndex*destRect.w;
                    for (int y = 0; y < 4; ++y)
                    {
                        yIndex = y + tetrimino.y;
                        destRect.y = (float)(layerArea.y) / windowUser->GetPrefs().height + yIndex*destRect.h;

                        // pick a proper image to draw
                        ChooseSrcRect(tetrimino.blocks[x + 4*y].type, srcRect, tetrimino.blocks[x + 4*y].remove);

                        // draw the image of a block
                        Renderer::DrawTexture(m_spriteList, &destRect, &srcRect);
                    }
                }

                if (DTetris::tetrisEngineData.animateLinesRemove)
                {
                    DTetris::tetrisEngineData.animateLinesFrame = frameCounter / 20;
                    if (DTetris::tetrisEngineData.animateLinesFrame > 3)
                    {
                        DTetris::tetrisEngineData.animateLinesFrame = 0;
                        DTetris::tetrisEngineData.animateLinesRemove = false;
                    }
                }

            } else {
                MSE_CORE_LOG("TetrisMap: cannot display due to not been initialized");
            }

            if (frameCounter < 80)
            {
                frameCounter++;
            } else {
                frameCounter = 0;
            }
        }

        void TetrisMapGUI::ChooseSrcRect(DTetris::BlockType blockType, SDL_Rect& srcRect, bool remove)
        {
            switch (blockType)
            {
                case DTetris::BlockType::Block:
                {
                    if (remove)
                    {
                        if (DTetris::tetrisEngineData.animateLinesRemove)
                        {
                            srcRect.x = 24 + DTetris::tetrisEngineData.animateLinesFrame * 10;
                        }
                    } else {
                        srcRect.x = 24;
                    }

                    srcRect.y = 11;
                    srcRect.w = 10;
                    srcRect.h = 10;
                    break;
                }
                case DTetris::BlockType::Healing:
                {
                    if (remove)
                    {
                        if (DTetris::tetrisEngineData.animateLinesRemove)
                        {
                            srcRect.x = 24 + DTetris::tetrisEngineData.animateLinesFrame * 10;
                        }
                    } else {
                        srcRect.x = 74;
                    }

                    srcRect.y = 11;
                    srcRect.w = 10;
                    srcRect.h = 10;
                    break;
                }
                case DTetris::BlockType::Treasure:
                {
                    if (remove)
                    {
                        if (DTetris::tetrisEngineData.animateLinesRemove)
                        {
                            srcRect.x = 24 + DTetris::tetrisEngineData.animateLinesFrame * 10;
                        }
                    } else {
                        srcRect.x = 14;
                    }

                    srcRect.y = 11;
                    srcRect.w = 10;
                    srcRect.h = 10;
                    break;
                }
                case DTetris::BlockType::Attack:
                {
                    if (remove)
                    {
                        if (DTetris::tetrisEngineData.animateLinesRemove)
                        {
                            srcRect.x = 24 + DTetris::tetrisEngineData.animateLinesFrame * 10;
                        }
                    } else {
                        srcRect.x = 64;
                    }

                    srcRect.y = 11;
                    srcRect.w = 10;
                    srcRect.h = 10;
                    break;
                }
                case DTetris::BlockType::Defense:
                {
                    if (remove)
                    {
                        if (DTetris::tetrisEngineData.animateLinesRemove)
                        {
                            srcRect.x = 24 + DTetris::tetrisEngineData.animateLinesFrame * 10;
                        }
                    } else {
                        srcRect.x = 84;
                    }

                    srcRect.y = 11;
                    srcRect.w = 10;
                    srcRect.h = 10;
                    break;
                }
                case DTetris::BlockType::Wall:
                {
                    srcRect.x = 153;
                    srcRect.y = 137;
                    srcRect.w = 13;
                    srcRect.h = 12;
                    break;
                }
                default:
                {
                    srcRect.x = 10;
                    srcRect.y = 25;
                    srcRect.w = 10;
                    srcRect.h = 10;
                }
            }
        }
    }
}
