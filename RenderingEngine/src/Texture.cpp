#include "Texture.h"

#include <iostream>

#include <stb_image.h>

Texture::Texture(const std::string& filepath, unsigned int unit)
	: rendererID(0), filepath(filepath), buffer(nullptr), 
	width(0), height(0), bitsPerPixel(0), unit(unit)
{
	//std::cout << "Loading texture: " << filepath << std::endl;
	stbi_set_flip_vertically_on_load(1); //PNG starts images from top, so true. Is dependent on format
	buffer = stbi_load(filepath.c_str(), &width, &height, &bitsPerPixel, 4);	//4 = RGBA
	if (!buffer) {
		std::cout << "Warning: Failed to load texture at " << filepath << std::endl;
		return;
	}
	glGenTextures(1, &rendererID);
	glBindTexture(GL_TEXTURE_2D, rendererID);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(buffer);
}

Texture::~Texture()
{
	glDeleteTextures(1, &rendererID);
}

void Texture::Bind() const
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, rendererID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
