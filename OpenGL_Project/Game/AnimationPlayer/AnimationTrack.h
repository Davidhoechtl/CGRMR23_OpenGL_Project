#include <vector>
#include <string>
#include "../../ImageLoader/ImageLoader.h"

using namespace std;

#pragma once
class AnimationTrack
{
	public:
		string name;

		AnimationTrack(ImageLoader imageLoader, string ressourceFolder, string name, float interval);
		int GetNextTextureId();
		~AnimationTrack();

	private:
		vector<int> textureIds; // order of elements matter!
		int intervalInMilliseconds;
		int timeOfOneCyrcleInMilliseconds;
};

