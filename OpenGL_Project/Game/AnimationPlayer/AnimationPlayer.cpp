#include "AnimationPlayer.h"

void AnimationPlayer::SetCurrentTrackByName(string trackName)
{
	for (AnimationTrack track : this->tracks)
	{
		if (track.name == trackName)
		{
			this->currentTrack = track;
			return;
		}
	}
}

void AnimationPlayer::AddTrack(AnimationTrack track)
{
	this->tracks.push_back(track);
}

int AnimationPlayer::GetNextTextureId()
{
	return this->currentTrack.GetNextTextureId();
}

AnimationPlayer::~AnimationPlayer()
{
	for (AnimationTrack track : this->tracks)
	{
		track.~AnimationTrack();
	}
}