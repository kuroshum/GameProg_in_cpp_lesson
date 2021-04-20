#pragma once
#include "Component.h"
#include "SDL.h"

class SpriteComponent : public Component {
public:
	// •`‰æ€”õ(drawOrder)‚ª’á‚¢‚Ù‚Ç‰“‚­‚É’u‚©‚ê‚é
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(SDL_Renderer* renderer);
	virtual void SetTexture(SDL_Texture* texture);
	
	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexHeight() const { return mTexHeight; }
	int GetTexWidth() const { return mTexWidth; }
protected:
	SDL_Texture* mTexture;
	int mDrawOrder;
	int mTexWidth;
	int mTexHeight;
};