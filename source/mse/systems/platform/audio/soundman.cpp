#include <mse/systems/platform/platform.h>
#include <mse/systems/platform/audio/soundman.h>
#include <mse/systems/platform/audio/sound.h>
#include <mse/systems/platform/audio/track.h>
#include <mse/systems/resources/resource_manager.h>
#include <mse/systems/application/application.h>

namespace mse
{
    std::unordered_map<std::string, Sound*> SoundMan::sounds_bank = {};
    std::unordered_map<std::string, Track*> SoundMan::tracks_bank = {};
    std::vector<std::string> SoundMan::tracks_playlist = {};
    int SoundMan::tracks_playlist_current = -1;
    
    bool SoundMan::paused = false;
    bool SoundMan::music_enabled = true;
    bool SoundMan::sounds_enabled = true;
    bool SoundMan::play_in_background = false;
    int SoundMan::channels = MIX_CHANNELS;
    // MIX_CHANNELS is 8 by default
    std::unordered_map<int, int> SoundMan::sounds_volume {
        {0, MIX_MAX_VOLUME},
        {1, MIX_MAX_VOLUME},
        {2, MIX_MAX_VOLUME},
        {3, MIX_MAX_VOLUME},
        {4, MIX_MAX_VOLUME},
        {5, MIX_MAX_VOLUME},
        {6, MIX_MAX_VOLUME},
        {7, MIX_MAX_VOLUME},
    };
    int SoundMan::tracks_volume = MIX_MAX_VOLUME;
    
    void SoundMan::SetUpChannels(int number)
    {
        channels = Mix_AllocateChannels(number);
        for (int i = 0; i < number; ++i)
        {
            sounds_volume[i] = Application::config.soundsVolume;
            Mix_Volume(i, sounds_volume[i]);
        }
        
        Mix_VolumeMusic(tracks_volume);
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
            Mix_PlayChannel(-1, sounds_bank[path]->GetAudio(), 0);
            MSE_CORE_LOG("SoundMan: Playing ", path.c_str());
            return true;
        }
        
        MSE_CORE_LOG("SoundMan: could not find ", path.c_str());
        return false;
    }
    
    bool SoundMan::IsPlayingTrack()
    {
        if (Mix_PlayingMusic())
        {
//            MSE_CORE_LOG("SoundMan: is playing music");
            return true;
        }
//        MSE_CORE_LOG("SoundMan: is not playing music");
        return false;
    }
    
    bool SoundMan::PlayTrack(const std::string& path)
    {
        if (tracks_bank.find(path) != tracks_bank.end())
        {
            Mix_PlayMusic(tracks_bank[path]->GetAudio(), 0);
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
        if (Mix_PlayingMusic())
        {
            Mix_HaltMusic();
        }
    }
    
    void SoundMan::PauseTrack()
    {
        if (!Mix_PausedMusic())
        {
            Mix_PauseMusic();
        }
    }
    
    void SoundMan::UnPauseTrack()
    {
        if (Mix_PausedMusic())
        {
            Mix_ResumeMusic();
        }
    }
    
    // for the lazyiest dev in the world
    bool SoundMan::Play(const std::string& path)
    {
        if (PlaySound(path))
        {
            return true;
        }
        if (!PlayTrack(path))
        {
            return true;
        }
        return false;
    }
    
    void SoundMan::PauseAll()
    {
        for (int i = 0; i < channels; ++i)
        {
            Mix_Pause(i);
        }
        Mix_PauseMusic();
        paused = true;
    }
    
    void SoundMan::UnPauseAll()
    {
        for (int i = 0; i < channels; ++i)
        {
            Mix_Resume(i);
        }
        Mix_ResumeMusic();
        paused = false;
    }
    
    void SoundMan::StopAll()
    {
        for (int i = 0; i < channels; ++i)
        {
            Mix_HaltChannel(i);
        }
        Mix_HaltMusic();
    }
    
    void SoundMan::AdjustSoundsVolume(int level)
    {
        for (int i = 0; i < channels; ++i)
        {
            sounds_volume[i] = Mix_Volume(i, level);
        }
        MSE_CORE_LOG("SoundMan: sounds volume changed to ", level);
    }
    
    void SoundMan::AdjustTrackVolume(int level)
    {
        PauseTrack();
        tracks_volume = Mix_VolumeMusic(level);
        MSE_CORE_LOG("SoundMan: music volume changed to ", tracks_volume);
        UnPauseTrack();
    }
}

