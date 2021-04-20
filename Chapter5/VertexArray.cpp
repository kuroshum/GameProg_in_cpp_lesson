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
	// ���_�z��I�u�W�F�N�g���쐬���A����ID��mVertexArray�ɕۑ�
	glGenVertexArrays(1, &mVertexArray);
	// �쐬�������_�z��I�u�W�F�N�g�𒸓_�z��Ɏg��
	glBindVertexArray(mVertexArray);

	// ���_�o�b�t�@���쐬
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVerts * 9 * sizeof(float), verts, GL_STATIC_DRAW);

	// �C���f�b�N�X�o�b�t�@���쐬
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// �J���[�o�b�t�@���쐬
	//glGenBuffers(1, &mColorBuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	//glBufferData(GL_ARRAY_BUFFER, numVerts * 4 * sizeof(float), colors, GL_STATIC_DRAW);
	

	// Specify the vertex attributes
	// (For now, assume one vertex format)
	// Position is 3 floats starting at offset 0

	/*--------------------------------------------------------------------------
	 * ���_�o�b�t�@���ȉ��̂悤�ɁA���_���W(3��)�E�e�N�X�`�����W(2��)�E���_�J���[(4��)�ō\��
	 
	float vertices[] = {
		-0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, // top left
		 0.5f,  0.5f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, // top right
		 0.5f, -0.5f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, // bottom right
		-0.5f, -0.5f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f  // bottom left
	};
	--------------------------------------------------------------------------*/

	// ���_���W
	// �X�g���C�h��9�A���_���W��0�`2�Ԗڂ̗v�f������I�t�Z�b�g��0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	
	// �e�N�X�`�����W
	// ���_���W��3�`4�Ԗڂ̗v�f������I�t�Z�b�g��3
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 9,
		reinterpret_cast<void*>(sizeof(float) * 3));

	// ���_�J���[
	// ���_���W��5�`8�Ԗڂ̗v�f������I�t�Z�b�g��5
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
