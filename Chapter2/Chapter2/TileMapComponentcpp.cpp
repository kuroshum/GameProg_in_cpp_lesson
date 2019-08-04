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

	// タイルマップからマップを描画
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

		//ファイルを開く
		std::fstream filestream(filename);

		// ファイルが開かなかった場合は終了する
		if (!filestream.is_open()) {
			return false;
		}


		//ファイルを読み込む
		while (!filestream.eof()) {
			// 1行読み込む
			std::string buffer;
			filestream >> buffer;

			// ファイルから読み込んだ1行の文字列を区切り文字で分けてリストに追加する
			std::istringstream streambuffer(buffer);
			std::string token;
			int token_num;

			while (std::getline(streambuffer, token, delimiter)) {
				token_num = atoi(token.c_str());
				// 1セルの文字列をリストに追加する
				mTileSets.back().emplace_back(token_num);
			}
		}
	}

	return true;
}

void TileMapComponent::SetTileMap(SDL_Texture* tile_texture) {
	mTileTexture = tile_texture;
	// テクスチャの幅と高さを求める
	SDL_QueryTexture(tile_texture, nullptr, nullptr, &mTexWidth, &mTexHeight);

	bool status = false;
	std::vector<std::string> filenames = {
		"Assets/MapLayer1.csv",
		"Assets/MapLayer2.csv",
		"Assets/MapLayer3.csv",
	};

	// CSVファイルの内容を取得する
	status = GetMapLayer(filenames, ',');

	
	SDL_Rect temp;

	// タイルセットからタイルマップを生成
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