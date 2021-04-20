#pragma once
#include "SDL.h"
#include <unordered_map>
#include <string>
#include <vector>

class Game {
public:
	Game();
	bool initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	SDL_Texture* GetTexture(const std::string& fileName);
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// Map of textures loaded
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	// �Q�[����̑S�ẴA�N�^�[
	std::vector<class Actor*> mActors;
	// �҂���Ԃ̃A�N�^�[
	std::vector<class Actor*> mPendingActors;

	// all the sprite component drawn
	std::vector<class SpriteComponent*> mSprites;

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;

	// �A�N�^�[���X�V������ǐ�
	bool mUpdatingActors;

	class Ship* mShip; // Player's ship

	class Character* mChar;

};