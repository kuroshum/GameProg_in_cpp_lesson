#include "TileMapComponent.h"
#include "Actor.h"
#include "Math.h"

TileMapComponent::TileMapComponent(class Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder),
	mTileTexture(nullptr),
	mDrawOrder(drawOrder) {

}

void TileMapComponent::Draw(SDL_Renderer* renderer) {

	SDL_Rect temp;

	// �^�C���}�b�v����}�b�v��`��
	for (int j = mMapDatas.size()-1; j >= 0; j--) {
		for (int i = 0; i < MAP_HEIGHT*MAP_WIDTH; i++) {
			temp.h = TILE_HEIGHT;
			temp.w = TILE_WIDTH;

			temp.x = static_cast<int>((i % MAP_WIDTH) * TILE_WIDTH);
			temp.y = static_cast<int>((i / MAP_WIDTH) * TILE_HEIGHT);

			SDL_RenderCopyEx(renderer,
				mTileTexture,
				&mMapDatas[j][i],
				&temp,
				0,
				nullptr,
				SDL_FLIP_NONE);
		}
	}
	

}

bool TileMapComponent::GetMapLayer(const std::vector<std::string>& filenames, const char delimiter) {

	for (std::string filename : filenames) {

		mTileSets.emplace_back();

		//�t�@�C�����J��
		std::fstream filestream(filename);

		// �t�@�C�����J���Ȃ������ꍇ�͏I������
		if (!filestream.is_open()) {
			return false;
		}


		//�t�@�C����ǂݍ���
		while (!filestream.eof()) {
			// 1�s�ǂݍ���
			std::string buffer;
			filestream >> buffer;

			// �t�@�C������ǂݍ���1�s�̕��������؂蕶���ŕ����ă��X�g�ɒǉ�����
			std::istringstream streambuffer(buffer);
			std::string token;
			int token_num;

			while (std::getline(streambuffer, token, delimiter)) {
				token_num = atoi(token.c_str());
				// 1�Z���̕���������X�g�ɒǉ�����
				mTileSets.back().emplace_back(token_num);
			}
		}
	}

	return true;
}

void TileMapComponent::SetTileMap(SDL_Texture* tile_texture) {
	mTileTexture = tile_texture;
	// �e�N�X�`���̕��ƍ��������߂�
	SDL_QueryTexture(tile_texture, nullptr, nullptr, &mTexWidth, &mTexHeight);

	bool status = false;
	std::vector<std::string> filenames = {
		"Assets/MapLayer1.csv",
		"Assets/MapLayer2.csv",
		"Assets/MapLayer3.csv",
	};

	// CSV�t�@�C���̓��e���擾����
	status = GetMapLayer(filenames, ',');

	
	SDL_Rect temp;

	// �^�C���Z�b�g����^�C���}�b�v�𐶐�
	for (auto TileSet : mTileSets) {
		mMapDatas.emplace_back();
		for (int i = 0; i < MAP_HEIGHT*MAP_WIDTH; i++) {
			temp.h = TILE_HEIGHT;
			temp.w = TILE_WIDTH;

			temp.x = static_cast<int>((TileSet[i] % (mTexWidth / TILE_WIDTH)) * TILE_WIDTH);
			temp.y = static_cast<int>((TileSet[i] / (mTexWidth / TILE_WIDTH)) * TILE_HEIGHT);

			mMapDatas.back().emplace_back(temp);
		}
	}


}