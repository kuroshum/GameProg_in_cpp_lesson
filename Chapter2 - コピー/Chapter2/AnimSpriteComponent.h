#pragma once
#include "SpriteComponent.h"
#include <vector>

class AnimSpriteComponent : public SpriteComponent {
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	// フレームごとにアニメーションを更新する(componentからオーバーライド)
	void Update(float deltatime) override;
	// アニメーションに使うテクスチャを設定する
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures, bool loop_flag);
	// アニメーションのFPSを設定 / 取得
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
private:
	// アニメーションでのすべてのテクスチャ
	std::vector<std::vector<SDL_Texture*>> mAnimTextures;
	//現在表示しているフレーム
	std::vector<float> mCurrentFrame;
	// アニメーションのフレームレート
	float mAnimFPS;

	// アニメーションをループさせるか
	std::vector<bool> mLoopFlag;
};