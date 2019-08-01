#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder) 
	:Component(owner), 
	mTexture(nullptr), 
	mDrawOrder(drawOrder), 
	mTexWidth(0), 
	mTexHeight(0){
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() {
	mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer) {
	if (mTexture) {
		SDL_Rect r;
		// 幅と高さを所有アクターのスケールで拡縮する
		r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
		r.h = static_cast<int>(mTexHeight * mOwner->GetScale());
		// 矩形の中心を所有アクターの位置に合わせる
		r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
		r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);

		// 描画 (角度をラジアンから度に、時計回りを反時計回りに変換する必要がある)
		SDL_RenderCopyEx(renderer, 
			mTexture,									// 描画したいテクスチャ
			nullptr,									// 描画したいテクスチャの範囲
			&r,											// 出力先の矩形
			-Math::ToDegrees(mOwner->GetRotation()),	// 変換された回転角
			nullptr,									// 回転中心
			SDL_FLIP_NONE);								// 反転方法
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture) {
	mTexture = texture;
	// テクスチャの幅と高さを求める
	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}