// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <SDL.h>
#include "Math.h"

// 環境光源用の構造体
struct DirectionalLight
{
	// Direction of light
	Vector3 mDirection;
	// Diffuse color
	Vector3 mDiffuseColor;
	// Specular color
	Vector3 mSpecColor;
};

// ===========================================
// 課題6.2

// 点光源用の構造体
struct PointLight
{
	// 光源の位置
	Vector3 mPosition;
	// 拡散反射色
	Vector3  mDiffuseColor;
	// 鏡面反射色
	Vector3 mSpecColor;
	// 鏡面反射指数
	float mSpecPower;
	// 影響半径
	float mRadius;
};
// ===========================================


class Renderer
{
public:
	Renderer(class Game* game);
	~Renderer();

	bool Initialize(float screenWidth, float screenHeight);
	void Shutdown();
	void UnloadData();

	void Draw();

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	//void AddMeshComp(class MeshComponent* mesh);
	void RemoveMeshComp(class MeshComponent* mesh);

	class Texture* GetTexture(const std::string& fileName);
	// ===========================================
	// 課題6.1
	class Mesh* GetMesh(const std::string& fileName, MeshComponent* mc);

	void AddMeshComp(std::string shaderName, class MeshComponent* mesh);
	
	class Shader* GetMeshShaders(const std::string& shaderName, MeshComponent* mc);

	bool Renderer::LoadMeshShaders(std::string& shaderName_cnt, MeshComponent* mc);

	std::string split(std::string str, char split_word, int split_word_num);
	// ===========================================

	void SetViewMatrix(const Matrix4& view) { mView = view; }
	void SetProjectionMatrix(const Matrix4& projection) { mProjection = projection; }

	void SetAmbientLight(const Vector3& ambient) { mAmbientLight = ambient; }
	DirectionalLight& GetDirectionalLight() { return mDirLight; }
	// ===========================================
	// 課題6.2
	void SetPointLightNum(int pointLigtNUm) { mPointLight = new PointLight[pointLigtNUm]; }
	PointLight* GetPointLight() { return mPointLight; }

	std::string format(const char* chars1, const char* chars2, std::string str3);
	// ===========================================

	float GetScreenWidth() const { return mScreenWidth; }
	float GetScreenHeight() const { return mScreenHeight; }
private:
	bool LoadShaders();
	void CreateSpriteVerts();
	void SetLightUniforms(class Shader* shader);

	// Map of textures loaded
	std::unordered_map<std::string, class Texture*> mTextures;
	// Map of meshes loaded
	std::unordered_map<std::string, class Mesh*> mMeshes;

	// All the sprite components drawn
	std::vector<class SpriteComponent*> mSprites;

	// All mesh components drawn
	//std::vector<class MeshComponent*> mMeshComps;

	// Game
	class Game* mGame;

	// Sprite shader
	class Shader* mSpriteShader;
	// Sprite vertex array
	class VertexArray* mSpriteVerts;

	// ===========================================
	// 課題6.1
	
	// All mesh components drawn
	std::map<std::string, class MeshComponent*> mMeshComps;

	// Mesh shader
	//class Shader* mMeshShader;
	std::unordered_map<std::string, class Shader*> mMeshShaders;
	// ===========================================

	// View/projection for 3D shaders
	Matrix4 mView;
	Matrix4 mProjection;
	// Width/height of screen
	float mScreenWidth;
	float mScreenHeight;

	// Lighting data
	Vector3 mAmbientLight;
	DirectionalLight mDirLight;
	// ===========================================
	// 課題6.2
	//const int mPointLightNum;
	PointLight* mPointLight;
	// ===========================================

	// Window
	SDL_Window* mWindow;
	// OpenGL context
	SDL_GLContext mContext;
};