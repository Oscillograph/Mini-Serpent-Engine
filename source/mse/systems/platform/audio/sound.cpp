#include <mse/systems/platform/platform.h>
#include <mse/systems/platform/audio/sound.h>
#include <mse/systems/resources/resource_manager.h>

namespace mse
{
    Sound::Sound()
    {
        MSE_CORE_LOG("Sound object initialized but no data provided");
    }
    
    Sound::Sound(const std::string& path)
    {
        Load(nullptr, path);
    }
    
    Sound::Sound(void* data)
    {
        Load(data, "");
    }
    
    Sound::~Sound()
    {
        Free();
    }
    
    void Sound::Load(void* data, const std::string& path)
    {
        if (m_audio != nullptr)
        {
            Free();
        }
        
        // load from data
        if (data != nullptr)
        {
            return;
        } 
        
        m_path = path;
        m_audio = MIX_LoadAudio(NULL, path.c_str(), false);
        if (!m_audio)
        {
            MSE_CORE_LOG("Sound ", m_path.c_str(), " could not be loaded: ", SDL_GetError());
            return;
        }
        
        MSE_CORE_LOG("Sound ", m_path.c_str(), " loaded");
    }
    
    void Sound::Free()
    {
        MIX_DestroyAudio(m_audio);
        MSE_CORE_LOG("Sound ", m_path.c_str(), " unloaded");
    }
}

