#include "App.h"
#include "Audio.h"

#include "Defs.h"
#include "Log.h"

// NOTE: Recommended using: Additional Include Directories,
// instead of 'hardcoding' library location path in code logic
#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

// NOTE: Library linkage is configured in Linker Options
//#pragma comment(lib, "../Game/Source/External/SDL_mixer/libx86/SDL2_mixer.lib")

AudioManager::AudioManager() : Module()
{
	music = NULL;
	name.Create("audio");
}

AudioManager::~AudioManager()
{}

bool AudioManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// Load support for the OGG formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	// Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	pugi::xml_node music = config.child("music");
	CreatePlaylists(music);

	pugi::xml_node sFx = config.child("fx");
	LoadAllFx(sFx);

	Mix_AllocateChannels(360);
	SetChannelAngles();
	
	return ret;
}

bool AudioManager::CleanUp()
{
	if(!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	ListItem<Mix_Chunk*>* item;
	for(item = fx.start; item != NULL; item = item->next)
		Mix_FreeChunk(item->data);

	fx.Clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

void AudioManager::PauseMusic()
{
	if (active)
	{
		if (Mix_PlayingMusic())
		{
			if (!Mix_PausedMusic())
				Mix_PauseMusic();
			else
				Mix_ResumeMusic();
		}
	}
	else
		return;
}

void AudioManager::StopMusic()
{
	if (active)
		Mix_HaltMusic();
	else
		return;
}

bool AudioManager::PlayMusic(const char* path, float fadeTime)
{
	bool ret = true;

	if(!active)
		return false;

	if(music != NULL)
	{
		if(fadeTime > 0.0f)
		{
			Mix_FadeOutMusic(int(fadeTime * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fadeTime > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fadeTime * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}

void AudioManager::CreatePlaylists(pugi::xml_node& ms_node)
{

}

void AudioManager::LoadAllFx(pugi::xml_node& fx_node)
{
	for (pugi::xml_node sound = fx_node.child("sound"); 
		 sound != nullptr; sound = sound.next_sibling("sound"))
	{
		const char* path = sound.child_value();
		LoadFx(path);
	}
}

unsigned int AudioManager::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.Add(chunk);
		ret = fx.Count();
	}

	return ret;
}

bool AudioManager::UnLoadFx(uint id)
{
	bool ret = true;
	if (active)
	{
		if (fx[id - 1] != nullptr)
		{
			ListItem<Mix_Chunk*>* itemfx = fx.At(id - 1);
			Mix_FreeChunk(itemfx->data);
			ret = fx.Del(itemfx);
		}
	}
	return ret;
}

bool AudioManager::PlayFx(uint id, int repeat)
{
	bool ret = false;

	if(!active)
		return false;

	if(id > 0 && id <= fx.Count())
	{
		Mix_PlayChannel(-1, fx[id - 1], repeat);
	}

	return ret;
}

bool AudioManager::PlayFxOnChannel(uint id, uint channel, uint distance, int repeat)
{
	bool ret = true;

	if (!active)
		return ret;

	if (fx.At(id - 1) != nullptr)
	{
		while (Mix_Playing(channel))
		{
			channel++;
			if (channel >= MAX_CHANNELS)
				channel = 0;
		}

		Mix_SetPosition(channel, channel, distance);
		Mix_PlayChannel(channel, fx[id - 1], repeat);
	}
	else
		ret = false;
	return ret;
}

uint AudioManager::GetAngle(iPoint player, iPoint enemy)
{
	//iPoint vec = enemy - player;
	iPoint vec(enemy.x - player.x, enemy.y - player.y);

	float dot = (yAxis.x * vec.x) + (yAxis.y * vec.y);
	float det = (yAxis.x * vec.y) - (yAxis.y * vec.x);

	double angle = atan2(det, dot) * RADS_TO_DEG;
	angle += 180.0f;

	if (angle < 0)
		angle += 180.0f;
	LOG("angle: %f", angle);

	uint a_ret = static_cast<uint>(angle);
	LOG("final angle: %d", a_ret);

	return a_ret;
}

uint AudioManager::GetVolumeFromDistance(iPoint player, iPoint enemy)
{
	iPoint vec = enemy - player;
	float screen_dist = sqrt(pow(vec.x, 2) + pow(vec.y , 2));
	LOG("screen distance: %f", screen_dist);

	if (screen_dist >= MUTE_DISTANCE)
		return uint(MUTE_DISTANCE_VOL);

	float scaled_dist = screen_dist * (MUTE_DISTANCE_VOL / MAX_DISTANCE);
	LOG("scaled distance: %f", scaled_dist);

	uint volume = static_cast<uint>(scaled_dist);

	LOG("volume: %d", volume);
	if (volume > MAX_DISTANCE_VOL)
		volume = MAX_DISTANCE_VOL;

	return volume;
}

void AudioManager::SetChannelAngles()
{
	for (int i = 0; i <= 360; i++) {
		Mix_SetPosition(i, i, 1);
	}
}
