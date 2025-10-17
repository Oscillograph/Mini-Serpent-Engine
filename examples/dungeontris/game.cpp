#include <dungeontris/game-fwd.h>
#include <random> // rand
#include <ctime> // clock
#include <sstream> // stringstream
#include <utf8.h> // utf8
#include <SDL2/SDL.h> // SDL_Delay

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
