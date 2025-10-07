#ifndef MSE_SYSTEMS_PLATFORM_AUDIO_TRACK_H
#define MSE_SYSTEMS_PLATFORM_AUDIO_TRACK_H

#include <mse/core.h>
#include <mse/systems/platform/platform.h>

namespace mse
{
	class Track
	{
	public:
		Track();
        Track(const std::string& path);
        Track(void* data);
        virtual ~Track();
        
        void Load(void* data = nullptr, const std::string& path = "");
        
        void Free();
        
        inline Mix_Music* GetAudio() { return m_audio; }
	private:
		std::string m_path = "";
        Mix_Music* m_audio = nullptr;
	};
}

#endif

