#pragma once
#include "SpriteComponent.h"
#include <vector>

class AnimSpriteComponent : public SpriteComponent {
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	// �t���[�����ƂɃA�j���[�V�������X�V����(component����I�[�o�[���C�h)
	void Update(float deltatime) override;
	// �A�j���[�V�����Ɏg���e�N�X�`����ݒ肷��
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures, bool loop_flag);
	// �A�j���[�V������FPS��ݒ� / �擾
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
private:
	// �A�j���[�V�����ł̂��ׂẴe�N�X�`��
	std::vector<std::vector<SDL_Texture*>> mAnimTextures;
	//���ݕ\�����Ă���t���[��
	std::vector<float> mCurrentFrame;
	// �A�j���[�V�����̃t���[�����[�g
	float mAnimFPS;

	// �A�j���[�V���������[�v�����邩
	std::vector<bool> mLoopFlag;
};