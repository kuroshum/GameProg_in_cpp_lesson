// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <GL/glew.h>
#include <string>
#include "Math.h"

class Shader
{
public:
	Shader();
	~Shader();

	// 指定された名前の頂点 / フラグメントシェーダーを読み込む
	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();

	// アクティブなシェーダープログラムとして設定
	void SetActive();

	// 
	void SetMatrixUniform(const char* name, const Matrix4& matrix);
	
private:
	// シェーダーをコンパイルする
	bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);

	// シェーダーのコンパイルに成功したかの判定
	bool IsCompiled(GLuint shader);
	
	// 頂点のフラグメントプログラムのリンクを確認
	bool IsValidProgram();

private:
	// シェーダーオブジェクトのIDを確認
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};