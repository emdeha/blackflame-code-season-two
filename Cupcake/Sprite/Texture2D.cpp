#include "stdafx.h"
#include "Texture2D.h"


Texture2D::Texture2D()
{
#ifdef FREEIMAGE_LIB
	FreeImage_Initialize();
#endif
}

bool Texture2D::Load(const std::string &fileName)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP *dib(0);
	BYTE *bits(0);

	unsigned int width(0), height(0);

	fif = FreeImage_GetFileType(fileName.c_str(), 0);

	if(fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(fileName.c_str());
	if(fif == FIF_UNKNOWN)
		return false;

	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, fileName.c_str());

	if(!dib)
		return false;

	bits = FreeImage_GetBits(dib);
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);

	if((bits == 0) || (width == 0) || (height == 0))
		return false;


	glGenTextures(1, &textureObject);
	glBindTexture(GL_TEXTURE_2D, textureObject);


	glTexImage2D(GL_TEXTURE_2D,
				 0, 
				 GL_RGBA, 
				 width, height,
				 0,
				 GL_BGRA, 
				 GL_UNSIGNED_BYTE,
				 bits);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Texture2D::Bind(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureObject);
}