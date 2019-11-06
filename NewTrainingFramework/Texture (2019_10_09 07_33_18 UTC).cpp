#include "stdafx.h"
#include "Texture.h"
#include "TextureResource.h"

void getFacePixelArray(char* source, char* destination, int height, int width, int offset, int startx, int nr_bytes)
{
	for (int i = startx; i < startx + height / 3; i++)
		memcpy(destination + (i - startx) * nr_bytes * width / 4, source + i * width * nr_bytes + offset * nr_bytes, width * nr_bytes / 4);
}

Texture::Texture(TextureResource* t)
{
	tr = t;
	Load();
}

void Texture::Bind(int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	if (tr->type == "2d")
		glBindTexture(GL_TEXTURE_2D, texture_id);
}

void Texture::Load()
{
	glGenTextures(1, &texture_id);
	GLuint textureType;
	if (tr->type == "2d")
		textureType = GL_TEXTURE_2D;
	else
		textureType = GL_TEXTURE_CUBE_MAP;

	
	glBindTexture(textureType, texture_id);
	if (tr->min_filter == "LINEAR")
		glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	if (tr->mag_filter == "LINEAR")
		glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (tr->wrap_s == "CLAMP_TO_EDGE")
	{
		glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	}
	else
	{
		if (tr->wrap_s == "GL_REPEAT")
			glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	}

	if (tr->wrap_t == "CLAMP_TO_EDGE")
	{
		glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else
	{
		if (tr->wrap_t == "GL_REPEAT")
			glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}


	int width, height, bpp;
	char* pixels_array = LoadTGA(tr->path.c_str(), &width, &height, &bpp);

	if (tr->type == "2d")
	{
		switch (bpp)
		{
		case 32: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_array); break;
		case 24: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels_array); break;
		default: std::cout << "Unknown texture... too bad:(" << std::endl; assert(false);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		GLuint rgb, nr_bytes;
		if (bpp == 32)
		{
			nr_bytes = 4;
			rgb = GL_RGBA;
		}
		else
			if (bpp == 24)
			{
				nr_bytes = 3;
				rgb = GL_RGB;
			}
			else
			{
				std::cout << "Unknown texture... too bad:(" << std::endl; assert(false);
			}

		char* subbuffer = new char[width / 4 * height / 3 * nr_bytes];

		getFacePixelArray(pixels_array, subbuffer, height, width, width / 2, height / 3, nr_bytes);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, rgb, width / 4, height / 3, 0, rgb, GL_UNSIGNED_BYTE, subbuffer);

		getFacePixelArray(pixels_array, subbuffer, height, width, 0, height / 3, nr_bytes);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, rgb, width / 4, height / 3, 0, rgb, GL_UNSIGNED_BYTE, subbuffer);

		getFacePixelArray(pixels_array, subbuffer, height, width, width / 4, 0, nr_bytes);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, rgb, width / 4, height / 3, 0, rgb, GL_UNSIGNED_BYTE, subbuffer);

		getFacePixelArray(pixels_array, subbuffer, height, width, width / 4, 2 * height / 3, nr_bytes);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, rgb, width / 4, height / 3, 0, rgb, GL_UNSIGNED_BYTE, subbuffer);

		getFacePixelArray(pixels_array, subbuffer, height, width, width / 4, height / 3, nr_bytes);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, rgb, width / 4, height / 3, 0, rgb, GL_UNSIGNED_BYTE, subbuffer);

		getFacePixelArray(pixels_array, subbuffer, height, width, 3 * width / 4, height / 3, nr_bytes);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, rgb, width / 4, height / 3, 0, rgb, GL_UNSIGNED_BYTE, subbuffer);

		delete[] subbuffer;
	}

	delete[] pixels_array;
}

Texture::~Texture()
{
}