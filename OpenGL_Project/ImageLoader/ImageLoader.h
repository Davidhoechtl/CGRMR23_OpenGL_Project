#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <glad/glad.h>

#include <stb_image.h>
#include <iostream>

class ImageLoader
{
	public:
		unsigned int loadImage(const char* resPath, bool flipTextureVertically) {
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			// set the texture wrapping/filtering options (on the currently bound texture object)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// load and generate the texture
			int width, height, nrChannels;
			if (flipTextureVertically)
				stbi_set_flip_vertically_on_load(true);

			unsigned char* data = stbi_load(resPath, &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				std::cout << "Failed to load texture" << std::endl;
			}

			stbi_image_free(data);
			stbi_set_flip_vertically_on_load(false);
			//glEnable(GL_TEXTURE_2D);
			return texture;
		}

		unsigned int loadTransparentImage(const char* resPath, bool flipTextureVertically) {
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			// set the texture wrapping/filtering options (on the currently bound texture object)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// load and generate the texture
			int width, height, nrChannels;
			if(flipTextureVertically)
				stbi_set_flip_vertically_on_load(true);

			unsigned char* data = stbi_load(resPath, &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				std::cout << "Failed to load texture" << std::endl;
			}

			stbi_image_free(data);
			//glEnable(GL_TEXTURE_2D);
			stbi_set_flip_vertically_on_load(false);
			return texture;
		}
};

#endif

