#include "Game.h"
#include "SDL_image.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Ship.h"
#include "Character.h"
#include "BGSpriteComponent.h"
#include "TileMapComponent.h"

Game::Game() 
	:mWindow(nullptr),
	mRenderer(nullptr),
	mIsRunning(true),
	mUpdatingActors(false) {

}

bool Game::initialize() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter2)", 100, 100, 1024, 768, 0);
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
	}

	LoadData();

	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput() {

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	// �v���C���[�ł���F���D�̑���
	mShip->ProcessKeyboard(state);
	// �v���C���[�ł���L�����N�^�[�̑���
	mChar->ProcessKeyboard(state);
}


void Game::UpdateGame() {
	// �O�̃t���[������16ms�o�܂ő҂�
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// deltatime�͑O�̃t���[���Ƃ̎����̍���b�ɕϊ������l
	float deltatime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	// deltatime���ő�l�Ő�������
	if (deltatime > 0.05f) {
		deltatime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	// �S�ẴA�N�^�[���X�V
	mUpdatingActors = true;
	for (auto actor : mActors) {
		actor->Update(deltatime);
	}
	mUpdatingActors = false;

	// �҂��ɂȂ��Ă����A�N�^�[��mActors�Ɉړ�
	for (auto pending : mPendingActors) {
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// ���񂾃A�N�^�[���ꎞ�z��ɒǉ�
	std::vector<Actor*> deadActors;

	for (auto actor : mActors) {
		if (actor->GetState() == Actor::EDead) {
			deadActors.emplace_back(actor);
		}
	}

	for (auto actor : deadActors) {
		delete actor;
	}
}

void Game::GenerateOutput() {
	// �o�b�N�o�b�t�@(�Q�[���̃J�����g�o�b�t�@)��P�F�ŃN���A����
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	// �o�b�N�o�b�t�@�����݂̕`��F�ŃN���A����
	SDL_RenderClear(mRenderer);

	// �S�ẴX�v���C�g�R���|�[�l���g��`��
	for (auto sprite : mSprites) {
		sprite->Draw(mRenderer);
	}

	// �t�����g�o�b�t�@�ƃo�b�N�o�b�t�@����������
	SDL_RenderPresent(mRenderer);
}

void Game::LoadData()
{
	// Create player's ship
	mShip = new Ship(this);
	mShip->SetPosition(Vector2(100.0f, 384.0f));
	mShip->SetScale(1.5f);

	// �v���C���[�ł���L�����N�^�[���쐬
	mChar = new Character(this);
	mChar->SetPosition(Vector2(100.0f, 384.0f));
	mChar->SetScale(1.5f);

	// Create actor for the background (this doesn't need a subclass)
	Actor* temp_bg = new Actor(this);
	temp_bg->SetPosition(Vector2(512.0f, 384.0f));

	/*---------------------------------------------------------
	// Create the "far back" background
	BGSpriteComponent* bg = new BGSpriteComponent(temp_bg);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	std::vector<SDL_Texture*> bgtexs = {
		GetTexture("Assets/Farback01.png"),
		GetTexture("Assets/Farback02.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-100.0f);

	// Create the closer background
	bg = new BGSpriteComponent(temp_bg, 50);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	bgtexs = {
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-200.0f);
	---------------------------------------------------------*/

	// �^�C���}�b�v����
	Actor* temp_tm = new Actor(this);
	temp_tm->SetPosition(Vector2(16.0f, 16.0f));
	
	TileMapComponent* tm = new TileMapComponent(temp_tm);
	SDL_Texture* tiletex = GetTexture("Assets/Tiles.png");
	tm->SetTileMap(tiletex);
}

void Game::UnloadData() {
	// actors������
	// ~Actor�ł�RemoveActor���Ăяo�����̂ŁA�ʂ̎�ނ̃��[�v���g��
	while (!mActors.empty()) {
		delete mActors.back();
	}
	// Destroy textures
	for (auto i : mTextures){
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
}

SDL_Texture* Game::GetTexture(const std::string& filename) {
	SDL_Texture* tex = nullptr;
	// �e�N�X�`�������łɃ}�b�v�ɓ����Ă���̂��H
	auto iter = mTextures.find(filename);
	if (iter != mTextures.end()) {
		tex = iter->second;
	}
	else {
		// �t�@�C�����烍�[�h
		SDL_Surface* surf = IMG_Load(filename.c_str());
		if (!surf) {
			SDL_Log("Failed to load texture file %s", filename.c_str());
			return nullptr;
		}

		// �T�[�t�F�C�X����e�N�X�`�����쐬
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		// �T�[�t�F�C�X������
		SDL_FreeSurface(surf);
		if (!tex) {
			SDL_Log("Failed to convert surface to texture for %s", filename.c_str());
			return nullptr;
		}

		// mTextures�ɍ쐬�����e�N�X�`����ǉ�
		mTextures.emplace(filename.c_str(), tex);
	}
	return tex;
}

void Game::Shutdown() {
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddActor(Actor* actor) {
	// �A�N�^�[�̍X�V���Ȃ�҂��ɒǉ�
	if (mUpdatingActors) {
		mPendingActors.emplace_back(actor);
	}
	else {
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor) {
	// mPendingAction��vector����actor���������A�Y������C�e���[�^��Ԃ�
	// actor��������Ȃ��ꍇ�͍Ō�̃C�e���[�^��Ԃ�
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end()) {
		// �Y���̃C�e���[�^��mPendingActors�̈�ԍŌ�Ɉړ����Apop����
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end()) {
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite) {
	// �\�[�g�ς݂̔z��ő}���_��������
	// (����͎������������̍����ŏ��̗v�f�̈ʒu��)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter) {
		if (myDrawOrder < (*iter)->GetDrawOrder()) {
			break;
		}
	}
	// �C�e���[�^�̈ʒu�̑O�ɗv�f��}������
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite) {
	// We can't swap because it ruins ordering
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}