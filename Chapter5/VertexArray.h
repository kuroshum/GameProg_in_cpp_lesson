// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once

class VertexArray
{
public:
	VertexArray(const float* verts, unsigned int numVerts, const unsigned int* indices, unsigned int numIndices);
	~VertexArray();

	// この頂点配列をアクティブする（描画できるようにする）
	void SetActive();

	unsigned int GeNtumIndices() const { return mNumIndices; }
	unsigned int GetNumVerts() const { return mNumVerts; }

private:
	// 頂点バッファにある頂点の数
	unsigned int mNumVerts;
	// インデックスバッファにあるインデックスの数
	unsigned int mNumIndices;

	unsigned int mNumColors;

	// 頂点バッファの OpenGL ID
	unsigned int mVertexBuffer;
	// インデックスバッファの OpenGL ID
	unsigned int mIndexBuffer;
	// 頂点配列オブジェクトの OpenGL ID
	unsigned int mVertexArray;
	// カラーバッファの OpenGL ID
	unsigned int mColorBuffer;
};