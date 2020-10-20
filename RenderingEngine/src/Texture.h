#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Texture
{
private:
	unsigned int rendererID;
	std::string filepath;
	unsigned char* buffer;
	int width;
	int height;
	int bitsPerPixel;
public:
	Texture(const std::string& filepath);
	~Texture();

	void Bind(unsigned int unit = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
};

