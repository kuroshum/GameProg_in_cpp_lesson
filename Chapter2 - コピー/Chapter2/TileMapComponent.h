#pragma once
#include "SpriteComponent.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class TileMapComponent : public SpriteComponent {
public:
	TileMapComponent(class Actor* owner, int drawOrder = 10);

	//void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer) override;

	// �^�C���Z�b�g����^�C���}�b�v�𐶐�
	void SetTileMap(SDL_Texture* tile_texture);

	// �^�C���Z�b�g��csv����擾
	bool GetMapLayer(const std::vector<std::string>& filenames, const char delimiter = ',');
private:
	SDL_Texture* mTileTexture;
	std::vector< std::vector<int>> mTileSets;

	int mDrawOrder;

	// �^�C���}�b�v���i�[����2����vector
	std::vector<std::vector<SDL_Rect>> mMapDatas;

	// �^�C���Z�b�g�̉摜�̕��ƍ���
	int mTexWidth;
	int mTexHeight;

	// ��̃^�C���̕��ƍ���
	const int TILE_HEIGHT = 32;
	const int TILE_WIDTH = 32;

	// �`�悷��}�b�v�̕��ƍ���
	const int MAP_HEIGHT = 24;
	const int MAP_WIDTH = 32;

};