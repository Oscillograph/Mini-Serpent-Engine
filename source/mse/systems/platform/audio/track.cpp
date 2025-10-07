#include <mse/systems/platform/platform.h>
#include <mse/systems/platform/audio/track.h>
#include <mse/systems/resources/resource_manager.h>

namespace mse
{
	Track::Track()
    {
        MSE_CORE_LOG("Track object initialized but no data provided");
    }
    
    Track::Track(const std::string& path)
    {
        Load(nullptr, path);
    }
    
    Track::Track(void* data)
    {
        Load(data, "");
    }
    
    Track::~Track()
    {
        Free();
    }
    
    void Track::Load(void* data, const std::string& path)
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
        m_audio = Mix_LoadMUS(path.c_str());
        if (!m_audio)
        {
            MSE_CORE_LOG("Track ", m_path.c_str(), " could not be loaded: ", Mix_GetError());
            return;
        }
        
        MSE_CORE_LOG("Track ", m_path.c_str(), " loaded");
    }
    
    void Track::Free()
    {
        Mix_FreeMusic(m_audio);
        MSE_CORE_LOG("Track ", m_path.c_str(), " unloaded");
    }
}

