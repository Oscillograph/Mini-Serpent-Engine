#include <mse/systems/platform/renderer/cursor.h>
#include <mse/systems/resources/resource_manager.h>

namespace mse
{
    Cursor::Cursor(Resource* texture, Window* user, int hotX, int hotY)
    : m_texture(texture), m_window(user)
    {
        Texture* cursorTexture = (Texture*)(m_texture->data);
        m_Cursor = SDL_CreateColorCursor(cursorTexture->GetSurface(), hotX, hotY);
        MSE_CORE_LOG("Cursor constructed");
    }
    
    Cursor::~Cursor()
    {
        SDL_FreeCursor(m_Cursor);
        ResourceManager::DropResource(m_texture, m_window);
        MSE_CORE_LOG("Cursor destroyed");
    }
}

