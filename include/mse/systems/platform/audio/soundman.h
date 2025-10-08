#ifndef MSE_SYSTEMS_PLATFORM_AUDIO_SOUNDMAN_H
#define MSE_SYSTEMS_PLATFORM_AUDIO_SOUNDMAN_H

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
        static void SetUpChannels(int number = MIX_CHANNELS);
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
        
        static void AdjustSoundsVolume(int level);
        static void AdjustTrackVolume(int level);
        
		// mid-level methods - sound sources and environments, listener modelling
		
        // high-level methods - mixing, complex fx
		
	public:
		static std::unordered_map<std::string, Sound*> sounds_bank;
        static std::unordered_map<std::string, Track*> tracks_bank;
        static std::vector<std::string> tracks_playlist;
        static int tracks_playlist_current;
        static std::unordered_map<int, int> sounds_volume;
        static int tracks_volume;
        
        static bool paused;
        static int channels;
	};
}

#endif

