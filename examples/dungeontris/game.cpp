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
#include <mse/systems/windows/window.h>

#include <dungeontris/game-fwd.h> // DTetris::TetrisEngineData

extern DTetris::Tetrimino tetrimino;
extern DTetris::TetrisEngineData DTetris::tetrisEngineData;

namespace DTetris
{
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

        int actualHeight = height - 1;
        int moveUpCount = 0;
        int moveUpFactor = 0;
        int yFactor = 0;

        for (int y = actualHeight; y > -1; --y)
        {
            yFactor = y * width;
            if (linesFilled.find(y) != linesFilled.end())
            {
                moveUpCount++;

                if ((y - moveUpCount) < 0)
                {
                    break;
                }
            }

            moveUpFactor = (y - moveUpCount) * width;

            for (int x = 1; x < width - 1; ++x)
            {
                map_backend[x + yFactor] = map[x + moveUpFactor];
                map_backend[x + yFactor].remove = false;
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
                    srcRect.x = 36;
                    srcRect.y = 194;
                    srcRect.w = 10;
                    srcRect.h = 10;
                    break;
                }
                case DTetris::BlockType::Treasure:
                {
                    srcRect.x = 47;
                    srcRect.y = 194;
                    srcRect.w = 10;
                    srcRect.h = 10;
                    break;
                }
                case DTetris::BlockType::Attack:
                {
                    srcRect.x = 25;
                    srcRect.y = 216;
                    srcRect.w = 10;
                    srcRect.h = 10;
                    break;
                }
                case DTetris::BlockType::Defence:
                {
                    srcRect.x = 36;
                    srcRect.y = 216;
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
                    srcRect.y = 10;
                    srcRect.w = 10;
                    srcRect.h = 10;
                }
            }
        }
    }
}
