#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"

#include "Point.h"
#include "List.h"

#define MAX_CHANNELS			360.0f
#define MAX_FX					100.0f
								   
#define MAX_DISTANCE			500.0f
#define MAX_DISTANCE_VOL		250.0f
								   
#define MUTE_DISTANCE			510.0f
#define MUTE_DISTANCE_VOL		255.0f

#define RADS_TO_DEG 180 / 3.14

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

//enum Playlist
//{
//	NONE = -1,
//
//	WORLD,
//	BATTLE
//};

class AudioManager : public Module
{
public:

	AudioManager();

	virtual ~AudioManager();

	bool Awake(pugi::xml_node&);

	bool CleanUp();

	//Music Fucntions
	void PauseMusic();
	void StopMusic();
	bool PlayMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	//Music Loaders
	void CreatePlaylists(pugi::xml_node& ms_node);

	//FX Loaders
	void LoadAllFx(pugi::xml_node& fx_node);
	unsigned int LoadFx(const char* path);
	bool UnLoadFx(uint id);

	//FX Players
	bool PlayFx(uint fx, int repeat = 0);
	bool PlayFxOnChannel(uint id, uint channel, uint distance = 1, int repeat = 0);

	//Tools
	uint GetAngle(iPoint player, iPoint enemy);
	uint GetVolumeFromDistance(iPoint player, iPoint enemy);
	void SetChannelAngles();
public:

private:

	const iPoint yAxis = { 0, 1 };

	_Mix_Music* music;
	List<Mix_Chunk *> fx;
};

#endif // __AUDIO_H__