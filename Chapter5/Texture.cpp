// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Texture.h"
#include <SOIL.h>
#include <GL/glew.h>
#include <SDL.h>

Texture::Texture()
	:mTextureID(0)
	, mWidth(0)
	, mHeight(0)
{

}

Texture::~Texture()
{

}

bool Texture::Load(const std::string& fileName)
{
	int channels = 0;

	// テクスチャを読み込む
	// 幅、高さ、チャネル数が記録される
	unsigned char* image = SOIL_load_image(fileName.c_str(), &mWidth, &mHeight, &channels, SOIL_LOAD_AUTO);

	// 読み込めなかったらエラーを吐く
	if (image == nullptr)
	{
		SDL_Log("SOIL failed to load image %s: %s", fileName.c_str(), SOIL_last_result());
		return false;
	}

	// 読み込んだテクスチャがRGBかRGBAかを判定
	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}

	// OpenG+テクスチャオブジェクトを作成
	glGenTextures(1, &mTextureID);
	// テクスチャをアクティブに
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	// テクスチャを OpenGL が管理するテクスチャメモリに割り当てる
	glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format,
		GL_UNSIGNED_BYTE, image);

	// テクスチャメモリに割り当てたので、メモリから解放する
	SOIL_free_image_data(image);

	// バイリニアフィルタを有効にする
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Texture::Unload()
{
	glDeleteTextures(1, &mTextureID);
}

void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}
