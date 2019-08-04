#include "AnimSpriteComponent.h"
#include "Math.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder) 
	:SpriteComponent(owner, drawOrder), 
	mCurrentFrame(0.0f),
	mAnimFPS(24.0f){

}

void AnimSpriteComponent::Update(float deltaTime) {
	SpriteComponent::Update(deltaTime);
	
	// 各テクスチャ(ship, Character)のアニメーションを指定
	int tex_num = 0;
	for (auto AnimTexture : mAnimTextures) {

		if (AnimTexture.size() > 0) {
			// フレームレートとデルタタイムに基づいて
			// カレントフレームを更新する
			mCurrentFrame[tex_num] += mAnimFPS * deltaTime;

			// ループさせないアニメーションは止める
			if (mLoopFlag[tex_num]==false && mCurrentFrame[tex_num] >= AnimTexture.size()) {
				break;
			}

			// 必要に応じてカレントフレームを巻き戻す
			while (mCurrentFrame[tex_num] >= AnimTexture.size()) {
				mCurrentFrame[tex_num] -= AnimTexture.size();
			}

			// 現時点でのテクスチャを設定する
			SetTexture(AnimTexture[static_cast<int>(mCurrentFrame[tex_num])]);
		}
		tex_num++;
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures, bool loop_flag) {
	// アニメーションのテクスチャを格納するvectorを初期化
	mAnimTextures.emplace_back();
	
	// ループするか否かの判定を追加
	mLoopFlag.emplace_back(loop_flag);

	// アニメーションのテクスチャを格納
	for (auto texture : textures) {
		mAnimTextures.back().emplace_back(texture);
	}

	// アクティブなテクスチャを最初のフレームに設定する
	if (mAnimTextures.size() > 0) {
		mCurrentFrame.emplace_back(0.0f);
		SetTexture(mAnimTextures.back()[0]);
	}
}