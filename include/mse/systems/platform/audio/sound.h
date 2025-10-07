#ifndef MSE_SYSTEMS_PLATFORM_AUDIO_SOUND_H
#define MSE_SYSTEMS_PLATFORM_AUDIO_SOUND_H

#include <mse/core.h>
#include <mse/systems/platform/platform.h>

namespace mse
{
    class Sound
	{
	public:
        Sound();
        Sound(const std::string& path);
        Sound(void* data);
        virtual ~Sound();
        
        void Load(void* data = nullptr, const std::string& path = "");
        
        void Free();
        
        inline Mix_Chunk* GetAudio() { return m_audio; }
		
	protected:
        std::string m_path = "";
        Mix_Chunk* m_audio = nullptr;
	};
}

#endif

