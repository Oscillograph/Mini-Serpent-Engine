#include <dungeontris/game-fwd.h>
#include <random> // rand
#include <ctime> // clock
#include <sstream> // stringstream
#include <utf8.h> // utf8
#include <SDL2/SDL.h> // SDL_Delay

#include <mse/systems/windows/layers/layer.h>
#include <mse/systems/platform/renderer/renderer.h>
#include <mse/systems/resources/resource_manager.h>
#include <mse/systems/windows/window.h>

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
        blockSprites.resize(0);
        map.resize(total);
        blockSprites.resize(total);
        
        for (int j = 0; j < height; ++j)
        {
            for (int i = 0; i < width; ++i)
            {
                int index = j*width + i;
                map[index].coordinates.x = i;
                map[index].coordinates.y = j;
                map[index].type = BlockType::None;
                map[index].color = {0, 0, 0};
                
                blockSprites[index].place = {
                    map[index].coordinates.x * 10,
                    map[index].coordinates.y * 10,
                    10,
                    10
                };
                blockSprites[index].texture = nullptr;
            }
        }
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
                for (int x = 0; x < area.z; ++x)
                {
                    for (int y = 0; y < area.w; ++y)
                    {
                        layerMask[x + y*area.z] = id;
                    }
                }
                
                // view
                
                // controller
                callbacks[EventTypes::GUIItemMouseButtonDown] = [&](SDL_Event* event){
                };
                
                callbacks[EventTypes::GUIItemMouseButtonUp] = [&](SDL_Event* event){
                };
                
                callbacks[EventTypes::GUIItemMouseOver] = [&](SDL_Event* event){
                };
                
                callbacks[EventTypes::GUIItemMouseOut] = [&](SDL_Event* event){
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
//            MSE_CORE_LOG("TetrisMapGUI: Display");
            if (parentLayer != nullptr)
            {
                SDL_FRect destRect = {0, 0, 10.0 / windowUser->GetPrefs().width, 10.0 / windowUser->GetPrefs().height};
                SDL_Rect srcRect = {0, 0, 10, 10};
                for (int xIndex = 0; xIndex < m_width; ++xIndex)
                {
                    destRect.x = (float)(layerArea.x) / windowUser->GetPrefs().width + xIndex*destRect.w;
//                    destRect.x = 0 + xIndex*destRect.w;
                    for (int yIndex = 0; yIndex < m_height; ++yIndex)
                    {
                        // skip the iteration if the block is empty
                        if (m_tetrisMap->map[yIndex*m_width + xIndex].type == DTetris::BlockType::None)
                        {
                            continue;
                        }
                        
                        destRect.y = (float)(layerArea.y) / windowUser->GetPrefs().height + yIndex*destRect.h;
//                        destRect.y = 0 + xIndex*destRect.h;
                        
                        // pick a proper image to draw
                        switch (m_tetrisMap->map[yIndex*m_width + xIndex].type)
                        {
                        case DTetris::BlockType::Block:
                            {
                                srcRect.x = 25;
                                srcRect.y = 194;
                                break;
                            }
                        case DTetris::BlockType::Healing:
                            {
                                srcRect.x = 36;
                                srcRect.y = 194;
                                break;
                            }
                        case DTetris::BlockType::Treasure:
                            {
                                srcRect.x = 47;
                                srcRect.y = 194;
                                break;
                            }
                        case DTetris::BlockType::Attack:
                            {
                                srcRect.x = 25;
                                srcRect.y = 216;
                                break;
                            }
                        case DTetris::BlockType::Defence:
                            {
                                srcRect.x = 36;
                                srcRect.y = 216;
                                break;
                            }
                        }
                        
                        // draw the image of a block
//                        MSE_LOG("Drawing at: ", destRect.x, ", ", destRect.y);
                        Renderer::DrawTexture(m_spriteList, &destRect, &srcRect);
                    }
                }
            } else {
                MSE_CORE_LOG("TetrisMap: cannot display due to not been initialized");
            }
        }
    }
}
