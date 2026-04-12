#ifndef MSE_SYSTEMS_PLATFORM_AUDIO_SOUNDMAN_H
#define MSE_SYSTEMS_PLATFORM_AUDIO_SOUNDMAN_H

#include "SDL3/SDL_mixer.h"
#include <mse/core.h>
#include <mse/systems/platform/platform.h>

// bricks of the audio system
//#include <mse/systems/platform/audio/sound.h>
//#include <mse/systems/platform/audio/track.h>

namespace mse
{
    enum class AudioType
    {
        Sound,
        Track,
    };
    
    class SoundMan
	{
	public:
		// system setup and utilities
        static void Init(int channels_count = 8);
		static void LoadSounds(const std::vector<std::string>& paths);
        static void LoadTracks(const std::vector<std::string>& paths);
        static void LoadPlaylist(const std::vector<std::string>& paths);
        static void DropPlaylist();
        static void DropSoundsBank();
        static void DropTracksBank();
        static void DropBanks();
        static void Shutdown();
        
		// low-level methods - sounds, tracks
        static bool PlaySound(const std::string& path);
        
        static bool IsPlayingTrack();
        static bool PlayTrack(const std::string& path);
        static bool PlayNext(); // if playlist is set
        static void PauseTrack();
        static void UnPauseTrack();
        static void StopTrack();
        
        static bool Play(const std::string& path); // for the laziest dev in the world
        static void PauseAll();
        static void UnPauseAll();
        static void StopAll();
        
        static void AdjustSoundsVolume(float level);
        static void AdjustTrackVolume(float level);
        
		// mid-level methods - sound sources and environments, listener modelling
		
        // high-level methods - mixing, complex fx
		
	public:
		static std::unordered_map<std::string, Sound*> sounds_bank;
        static std::unordered_map<std::string, Track*> tracks_bank;
        static std::vector<std::string> tracks_playlist;
        static int tracks_playlist_current;
        static bool music_enabled;
        static bool sounds_enabled;
        static bool play_in_background;
        static std::unordered_map<int, float> sounds_volume;
        static std::unordered_map<int, MIX_Track*> channels;
        static float tracks_volume;
        
        static bool paused;
        static int channels_max;
        static MIX_Mixer* mixer;
	};
}

#endif

