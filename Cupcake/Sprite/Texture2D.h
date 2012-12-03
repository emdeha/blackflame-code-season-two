#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H


#include "../glfiles/gl-headers/glload/gl_3_3.h"
#include "../glfiles/gl-headers/glload/gll.hpp"

#include <string>

#include "../glfiles/gl-headers/FreeImage.h"


class Texture2D
{
private:
	GLuint textureObject;

public:
	Texture2D();

	bool Load(const std::string &fileName);

	void Bind(GLenum textureUnit);
};


#endif