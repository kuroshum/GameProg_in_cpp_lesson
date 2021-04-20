// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "VertexArray.h"
#include <GL/glew.h>

VertexArray::VertexArray(const float* verts, unsigned int numVerts,
	const unsigned int* indices, unsigned int numIndices)
	:mNumVerts(numVerts)
	, mNumIndices(numIndices)
{
	// 頂点配列オブジェクトを作成し、そのIDをmVertexArrayに保存
	glGenVertexArrays(1, &mVertexArray);
	// 作成した頂点配列オブジェクトを頂点配列に使う
	glBindVertexArray(mVertexArray);

	// 頂点バッファを作成
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVerts * 9 * sizeof(float), verts, GL_STATIC_DRAW);

	// インデックスバッファを作成
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// カラーバッファを作成
	//glGenBuffers(1, &mColorBuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	//glBufferData(GL_ARRAY_BUFFER, numVerts * 4 * sizeof(float), colors, GL_STATIC_DRAW);
	

	// Specify the vertex attributes
	// (For now, assume one vertex format)
	// Position is 3 floats starting at offset 0

	/*--------------------------------------------------------------------------
	 * 頂点バッファが以下のように、頂点座標(3つ)・テクスチャ座標(2つ)・頂点カラー(4つ)で構成
	 
	float vertices[] = {
		-0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, // top left
		 0.5f,  0.5f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, // top right
		 0.5f, -0.5f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, // bottom right
		-0.5f, -0.5f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f  // bottom left
	};
	--------------------------------------------------------------------------*/

	// 頂点座標
	// ストライドは9、頂点座標は0～2番目の要素だからオフセットは0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	
	// テクスチャ座標
	// 頂点座標は3～4番目の要素だからオフセットは3
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 9,
		reinterpret_cast<void*>(sizeof(float) * 3));

	// 頂点カラー
	// 頂点座標は5～8番目の要素だからオフセットは5
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 9,
		reinterpret_cast<void*>(sizeof(float) * 5));

	
}

VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteBuffers(1, &mColorBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive()
{
	glBindVertexArray(mVertexArray);
}
