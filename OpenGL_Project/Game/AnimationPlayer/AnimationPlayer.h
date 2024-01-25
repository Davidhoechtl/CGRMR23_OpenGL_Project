#include "AnimationTrack.h"

#include <vector>
#include <string>

#pragma once
class AnimationPlayer
{
	public:
		void SetCurrentTrackByName(string trackName);
		void AddTrack(AnimationTrack* track);
		int GetNextTextureId();

		~AnimationPlayer();

	private:
		vector<AnimationTrack*> tracks;
		AnimationTrack* currentTrack;
};