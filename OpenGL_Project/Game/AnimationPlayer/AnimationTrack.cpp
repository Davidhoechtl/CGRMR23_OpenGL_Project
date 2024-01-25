#include "AnimationTrack.h"

#include <glad/glad.h>
#include <filesystem>
#include <iostream>

#include "../Utils/StaticGameInfo.h"
#include "../ImageLoader/ImageLoader.h"

vector<int> getTextureIds(const string& ressourceFolder, ImageLoader& imageLoader)
{
	vector<int> textureIds;

	try
	{
		if (!std::filesystem::exists(ressourceFolder))
		{
			throw exception("Ressource folder does not exist");
		}

		for (const auto& entry : std::filesystem::directory_iterator(ressourceFolder))
		{
			string filePath = entry.path().string();
			string extension = entry.path().extension().string();

			int textureId = -1;
			if (extension == ".png") {
				textureId = imageLoader.loadTransparentImage(filePath.c_str(), true);
			}
			else if (extension == ".jpg") {
				textureId = imageLoader.loadImage(filePath.c_str(), true);
			}
			else {
				throw exception("File extension not supported");
			}

			textureIds.push_back(textureId);
		}
	}
	catch (exception e)
	{
		cout << "Error reading animation files: " << e.what() << endl;
	}

	return textureIds;
}

AnimationTrack::AnimationTrack(ImageLoader imageLoader, string ressourceFolder, string name, int interval)
{
	this->intervalInMilliseconds = interval;
	this->textureIds = getTextureIds(ressourceFolder, imageLoader);
	this->timeOfOneCyrcleInMilliseconds = this->textureIds.size() * interval;
	this->name = name;
}

int AnimationTrack::GetNextTextureId()
{
	int time = StaticGameInfo::GameTime * 1000;
	int cyrcleTime = time % timeOfOneCyrcleInMilliseconds;
	int index = cyrcleTime / this->intervalInMilliseconds;
	return this->textureIds[index];
}

AnimationTrack::~AnimationTrack()
{
	for (const GLuint textureId : this->textureIds)
	{
		// free loaded textures from gpu memory
		glDeleteTextures(1, &textureId);
	}
}