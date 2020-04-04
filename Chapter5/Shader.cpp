// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Shader.h"
#include "Texture.h"
#include <SDL.h>
#include <fstream>
#include <sstream>

Shader::Shader()
	:mShaderProgram(0)
	, mVertexShader(0)
	, mFragShader(0)
{

}

Shader::~Shader()
{

}

bool Shader::Load(const std::string& vertName, const std::string& fragName)
{
	// 頂点シェーダとフラグメントシェーダーをコンパイルする
	if (!CompileShader(vertName, GL_VERTEX_SHADER, mVertexShader) || !CompileShader(fragName, GL_FRAGMENT_SHADER, mFragShader))
	{
		return false;
	}

	// 頂点 / フラグメントシェーダーをリンクして
	// シェーダープログラムを作る
	mShaderProgram = glCreateProgram();
	glAttachShader(mShaderProgram, mVertexShader);
	glAttachShader(mShaderProgram, mFragShader);
	
	glBindAttribLocation(mShaderProgram, 0, "inPosition");
	glBindFragDataLocation(mShaderProgram, 0, "outColor");
	
	glLinkProgram(mShaderProgram);

	

	// プログラムが正しくリンクされたことを確認
	if (!IsValidProgram())
	{
		return false;
	}

	return true;
}

void Shader::Unload()
{
	// Delete the program/shaders
	glDeleteProgram(mShaderProgram);
	glDeleteShader(mVertexShader);
	glDeleteShader(mFragShader);
}

void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix)
{
	// Find the uniform by this name
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// Send the matrix data to the uniform
	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.GetAsFloatPtr());
}

void Shader::SetActive()
{
	// Set this program as the active one
	glUseProgram(mShaderProgram);
}

bool Shader::CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader)
{
	// ファイルを開く
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open())
	{
		// 全てのテキストを一つの文字列に読み込む
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		// ファイル全体の内容をcontentsにロード
		std::string contents = sstream.str();
		// 文字列ポインタに変換
		const char* contentsChar = contents.c_str();

		// 指定されたタイプのシェーダーを作成
		outShader = glCreateShader(shaderType);
		// 読み込んだ文字列のコンパイルを試みる
		glShaderSource(outShader, 1, &(contentsChar), nullptr);
		glCompileShader(outShader);

		if (!IsCompiled(outShader))
		{
			SDL_Log("Failed to compile shader %s", fileName.c_str());
			return false;
		}
	}
	else
	{
		SDL_Log("Shader file not found: %s", fileName.c_str());
		return false;
	}

	return true;
}

bool Shader::IsCompiled(GLuint shader)
{
	GLint status;

	// コンパイル状態を問い合わせる
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, sizeof(buffer));
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		SDL_Log("GLSL Compile Failed:\n%s", buffer);
		return false;
	}

	return true;
}

bool Shader::IsValidProgram()
{

	GLint status;
	// Query the link status
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(mShaderProgram, 511, nullptr, buffer);
		SDL_Log("GLSL Link Status:\n%s", buffer);
		return false;
	}

	return true;
}
