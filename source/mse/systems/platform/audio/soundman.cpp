#include "SDL3/SDL_audio.h"
#include "SDL3/SDL_mixer.h"
#include "mse/utils/logger.h"
#include <mse/systems/platform/platform.h>
#include <mse/systems/platform/audio/soundman.h>
#include <mse/systems/platform/audio/sound.h>
#include <mse/systems/platform/audio/track.h>
#include <mse/systems/resources/resource_manager.h>
#include <mse/systems/application/application.h>

// TODO: Rewrite Soundman API to avoid confusion between what Track means in SDL and what Track means here. Or leave it as it is.

namespace mse
{
    std::unordered_map<std::string, Sound*> SoundMan::sounds_bank = {};
    std::unordered_map<std::string, Track*> SoundMan::tracks_bank = {};
    std::unordered_map<int, MIX_Track*> SoundMan::channels = {};
    std::vector<std::string> SoundMan::tracks_playlist = {};
    int SoundMan::tracks_playlist_current = -1;
    
    bool SoundMan::paused = false;
    bool SoundMan::music_enabled = true;
    bool SoundMan::sounds_enabled = true;
    bool SoundMan::play_in_background = false;

    MIX_Mixer* SoundMan::mixer = NULL;

    int SoundMan::channels_max = 8;
    // MIX_CHANNELS is 8 by default
    std::unordered_map<int, float> SoundMan::sounds_volume {};
    float SoundMan::tracks_volume = 1.0f;
    
    void SoundMan::Init(int channels_count)
    {
        // General plan if channels_count is more than 1:
        // channel 0 - for background music
        // channel 1..7 - for sounds (including characters speech)

        if (channels_count > channels_max)
        {
            channels_count = channels_max;
        }

        mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
        if (!mixer)
        {
            MSE_CORE_ERROR("SoundMan: couldn't create mixer on default device: ", SDL_GetError());
        }

        for (int i = 0; i < channels_count; ++i)
        {
            channels[i] = MIX_CreateTrack(mixer);

            sounds_volume[i] = Application::config.soundsVolume;
            MIX_SetTrackGain(channels[i], sounds_volume[i]);
        }
    }
    
    void SoundMan::LoadSounds(const std::vector<std::string>& paths)
    {
        Sound* sound = nullptr;
        MSE_CORE_LOG("SoundMan: Load ", paths.size(), " sounds");
        for (int i = 0; i < paths.size(); ++i)
        {
            sound = new Sound(paths[i]);
            if (!sound->GetAudio())
            {
                MSE_CORE_LOG("Could not load ", paths[i].c_str());
            } else {
                sounds_bank[paths[i]] = sound;
            }
        }
    }
    
    void SoundMan::LoadTracks(const std::vector<std::string>& paths)
    {
        Track* track = nullptr;
        MSE_CORE_LOG("SoundMan: Load ", paths.size(), " tracks");
        for (int i = 0; i < paths.size(); ++i)
        {
            MSE_CORE_LOG(paths[i].c_str());
            if (tracks_bank.find(paths[i]) == tracks_bank.end())
            {
                track = new Track(paths[i]);
                if (!track)
                {
                    MSE_CORE_LOG("Could not load ", paths[i].c_str());
                } else {
                    tracks_bank[paths[i]] = track;
                }
            }
        }
    }
    
    void SoundMan::LoadPlaylist(const std::vector<std::string>& paths)
    {
        LoadTracks(paths);
        tracks_playlist.clear();
        tracks_playlist.resize(paths.size());
        for (int i = 0; i < paths.size(); ++i)
        {
            if (tracks_bank.find(paths[i]) != tracks_bank.end())
            {
                tracks_playlist[i] = paths[i];
            }
        }
        tracks_playlist_current = -1;
    }
    
    void SoundMan::DropPlaylist()
    {}
    
    void SoundMan::DropSoundsBank()
    {
        for (std::pair<std::string, Sound*> item : sounds_bank)
        {
            delete item.second;
            item.second = nullptr;
        }
        MSE_CORE_LOG("SoundMan: Sounds bank cleared");
    }
    
    void SoundMan::DropTracksBank()
    {
        for (std::pair<std::string, Track*> item : tracks_bank)
        {
            delete item.second;
            item.second = nullptr;
        }
        MSE_CORE_LOG("SoundMan: Tracks bank cleared");
    }
    
    void SoundMan::DropBanks()
    {
        DropSoundsBank();
        DropTracksBank();
    }
    
    void SoundMan::Shutdown()
    {
        MSE_CORE_LOG("SoundMan: Shutdown...")
        StopAll();
        DropBanks();

        for (int i = 0; i < channels_max; ++i)
        {
            MIX_DestroyTrack(channels[i]);
        }
        MSE_CORE_LOG("SoundMan: Shutdown complete.");
    }
    
    bool SoundMan::PlaySound(const std::string& path)
    {
        if (sounds_bank.find(path) == sounds_bank.end())
        {
            LoadSounds({path});
        }
        
        if (sounds_bank.find(path) != sounds_bank.end())
        {
            for (size_t i = 0; i < channels.size(); ++i)
            {
                if (!MIX_TrackPlaying(channels[i]))
                {
                    MIX_SetTrackAudio(channels[i], sounds_bank[path]->GetAudio());
                    MIX_PlayTrack(channels[i], 0);
                }
            }

            MSE_CORE_LOG("SoundMan: Playing ", path.c_str());
            return true;
        }
        
        MSE_CORE_LOG("SoundMan: could not find ", path.c_str());
        return false;
    }
    
    bool SoundMan::IsPlayingTrack()
    {
        return MIX_TrackPlaying(channels[0]);
    }
    
    bool SoundMan::PlayTrack(const std::string& path)
    {
        if (tracks_bank.find(path) != tracks_bank.end())
        {
            MIX_SetTrackAudio(channels[0], tracks_bank[path]->GetAudio());
            MIX_PlayTrack(channels[0], 0);
            MSE_CORE_LOG("SoundMan: Playing ", path.c_str());
            return true;
        }
        
        MSE_CORE_LOG("SoundMan: could not find ", path.c_str());
        return false;
    }
    
    bool SoundMan::PlayNext()
    {
        tracks_playlist_current++;
        
        if (tracks_playlist.size() > 0)
        {
            if (tracks_playlist_current < 0)
            {
                tracks_playlist_current = 0;
            }
            if (tracks_playlist_current >= tracks_playlist.size())
            {
                tracks_playlist_current = 0;
            }
            
            return PlayTrack(tracks_playlist[tracks_playlist_current]);
        }
        
        return false;
    }
    
    void SoundMan::StopTrack()
    {
        if (MIX_TrackPlaying(channels[0]))
        {
            MIX_StopTrack(channels[0], 0);
        }

        MSE_CORE_LOG("SoundMan: Stopped ");
    }
    
    void SoundMan::PauseTrack()
    {
        if (MIX_TrackPlaying(channels[0]))
        {
            MIX_PauseTrack(channels[0]);
        }

        MSE_CORE_LOG("SoundMan: Paused ");
    }
    
    void SoundMan::UnPauseTrack()
    {
        if (MIX_TrackPaused(channels[0]))
        {
            MIX_ResumeTrack(channels[0]);
        }

        MSE_CORE_LOG("SoundMan: Resumed music");
    }
    
    // for the lazyiest dev in the world
    bool SoundMan::Play(const std::string& path)
    {
        if (PlaySound(path))
        {
            return true;
        }
        if (PlayTrack(path))
        {
            return true;
        }
        return false;
    }
    
    void SoundMan::PauseAll()
    {
        for (size_t i = 0; i < channels.size(); ++i)
        {
            if (MIX_TrackPlaying(channels[i]))
            {
                MIX_PauseTrack(channels[i]);
            }
        }

        paused = true;
        MSE_CORE_LOG("SoundMan: Paused all tracks ");
    }
    
    void SoundMan::UnPauseAll()
    {
        for (size_t i = 0; i < channels.size(); ++i)
        {
            if (MIX_TrackPaused(channels[i]))
            {
                MIX_ResumeTrack(channels[i]);
            }
        }

        paused = false;
        MSE_CORE_LOG("SoundMan: Resumed all tracks ");
    }
    
    void SoundMan::StopAll()
    {
        for (size_t i = 0; i < channels.size(); ++i)
        {
            if (MIX_TrackPlaying(channels[i]))
            {
                MIX_StopTrack(channels[i], 0);
            }
        }

        MSE_CORE_LOG("SoundMan: Stopped all tracks ");
    }
    
    void SoundMan::AdjustSoundsVolume(float level)
    {
        for (size_t i = 0; i < channels.size(); ++i)
        {
            sounds_volume[i] = level;
            MIX_SetTrackGain(channels[i], sounds_volume[i]);
        }

        MSE_CORE_LOG("SoundMan: sounds volume changed to ", level);
    }
    
    void SoundMan::AdjustTrackVolume(float level)
    {
        PauseTrack();
        tracks_volume = level;
        MIX_SetTrackGain(channels[0], tracks_volume);
        MSE_CORE_LOG("SoundMan: music volume changed to ", tracks_volume);
        UnPauseTrack();
    }
}

