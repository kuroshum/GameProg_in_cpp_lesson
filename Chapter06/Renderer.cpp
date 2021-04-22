// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Renderer.h"
#include "Texture.h"
#include "Mesh.h"
#include <algorithm>
#include <sstream>
#include <charconv>
#include "Shader.h"
#include "VertexArray.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include <GL/glew.h>

Renderer::Renderer(Game* game)
	:mGame(game)
	,mSpriteShader(nullptr)
{
}

Renderer::~Renderer()
{
}

bool Renderer::Initialize(float screenWidth, float screenHeight)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	// Set OpenGL attributes
	// Use the core OpenGL profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Specify version 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Request a color buffer with 8-bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 6)", 100, 100,
		static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), SDL_WINDOW_OPENGL);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// Create an OpenGL context
	mContext = SDL_GL_CreateContext(mWindow);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}

	// On some platforms, GLEW will emit a benign error code,
	// so clear it
	glGetError();

	// Make sure we can create/compile shaders
	if (!LoadShaders())
	{
		SDL_Log("Failed to load shaders.");
		return false;
	}

	// Create quad for drawing sprites
	CreateSpriteVerts();

	return true;
}

void Renderer::Shutdown()
{
	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	// ===========================================
	// 課題6.1
	for (auto mc : mMeshShaders)
	{
		mc.second->Unload();
	}
	mMeshShaders.clear();
	//mMeshShader->Unload();
	//delete mMeshShader;
	// ===========================================
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}

void Renderer::UnloadData()
{
	// Destroy textures
	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();

	// Destroy meshes
	for (auto i : mMeshes)
	{
		i.second->Unload();
		delete i.second;
	}
	mMeshes.clear();
}

void Renderer::Draw()
{
	// Set the clear color to light grey
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw mesh components
	// Enable depth buffering/disable alpha blend
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	
	// ===========================================
	// 課題6.1
	Shader* s;
	for (auto mc : mMeshComps)
	{
		s = mMeshShaders.at(mc.first);
		// Set the mesh shader active
		s->SetActive();
		// Update view-projection matrix
		s->SetMatrixUniform("uViewProj", mView * mProjection);
		// Update lighting uniforms
		SetLightUniforms(s);

		mc.second->Draw(s);
	}

	// ===========================================

	// Draw all sprite components
	// Disable depth buffering
	glDisable(GL_DEPTH_TEST);
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// Set shader/vao as active
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();
	for (auto sprite : mSprites)
	{
		sprite->Draw(mSpriteShader);
	}

	// Swap the buffers
	SDL_GL_SwapWindow(mWindow);
}

void Renderer::AddSprite(SpriteComponent* sprite)
{
	// Find the insertion point in the sorted vector
	// (The first element with a higher draw order than me)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	mSprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

void Renderer::AddMeshComp(std::string shaderName, MeshComponent* mesh)
{
	mMeshComps.emplace(shaderName, mesh);
}

// ===========================================
// 課題6.1
void Renderer::RemoveMeshComp(MeshComponent* mesh)
{
	for (std::pair<std::string, MeshComponent*> meshComp : mMeshComps)
	{
		if (meshComp.second == mesh)
		{
			mMeshComps.erase(meshComp.first);
			break;
		}
	}
}
// ===========================================

Texture* Renderer::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		tex = new Texture();
		if (tex->Load(fileName))
		{
			mTextures.emplace(fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}

Mesh* Renderer::GetMesh(const std::string & fileName, MeshComponent* mc)
{
	Mesh* m = nullptr;
	
	// ===========================================
	// 課題6.1

	// ファイル名に番号を付け加える
	// 　mapでシェーダやメッシュコンポーネントを管理するためには、
	//   ファイル名を異なるものにする必要があるから
	std::string under_bar("_");
	std::string mMeshes_size(std::to_string(mMeshes.size()));
	std::string fileName_cnt = fileName.c_str() + under_bar + mMeshes_size;
	//std::string fileName_cnt = fileName + mMeshes_size;
	// ===========================================
	
	auto iter = mMeshes.find(fileName_cnt);
	if (iter != mMeshes.end())
	{
		m = iter->second;
	}
	else
	{
		m = new Mesh();
		if (m->Load(fileName_cnt, this, mc))
		{
			mMeshes.emplace(fileName_cnt, m);
		}
		else
		{
			delete m;
			m = nullptr;
		}
	}
	return m;
}
// ===========================================
// 課題6.1

// 数字の文字列(区切り文字つき)をsplitする関数
std::string Renderer::split(std::string str, char split_word, int split_word_num)
{

	// 数字を格納する配列
	std::vector<std::string> str_vec;

	// 数字を一時保存する変数
	std::string buf = "";

	int split_word_cnt = 0;

	// 区切り文字を検索し、数字を抽出
	for (int ind = 0; ind < str.length(); ind++)
	{
		// 現在確認している文字が区切り文字でない場合
		if (str[ind] != split_word)
		{
			// bufに文字を追加
			buf.push_back(str[ind]);

			// 最後の文字ならばnum_vecに格納
			if (ind == str.length() - 1)
			{
				str_vec.emplace_back(buf.c_str());
			}
		}
		// 現在確認している文字が区切り文字である場合
		else
		{
			// 最初の文字の場合は保存する数字がないのでループを飛ばす
			if (buf == "") continue;

			if (split_word_cnt >= split_word_num-1)
			{
				// bufに保存している数字をnum_vecに格納
				str_vec.emplace_back(buf.c_str());
				// bufを初期化
				buf = "";
			}
			else
			{
				// bufに文字を追加
				buf.push_back(str[ind]);
			}
			split_word_cnt++;

		}
	}

	// 区切り文字を抜いた数字の配列を返す
    std::string name = str_vec[0];

	return name;
}

Shader* Renderer::GetMeshShaders(const std::string& shader_and_fileName_cnt, MeshComponent* mc)
{
	Shader* s = nullptr;

 	std::string shaderName = split(shader_and_fileName_cnt, '_', 1);

	std::string shader_path("Shaders/");
	std::string vert_extension(".vert");
	std::string frag_extension(".frag");

	std::string vertName = shader_path + shaderName + vert_extension;
	std::string fragName = shader_path + shaderName + frag_extension;

	auto iter = mMeshShaders.find(shader_and_fileName_cnt);
	if (iter != mMeshShaders.end())
	{
		s = iter->second;
	}
	else
	{
		s = new Shader();
		if (s->Load(vertName, fragName))
		{
			s->SetActive();
			mMeshShaders.emplace(shader_and_fileName_cnt, s);
		}
		else
		{
			delete s;
			s = nullptr;
			
		}
	}

	return s;
}
// =========================================

bool Renderer::LoadShaders()
{
	// Create sprite shader
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}

	mSpriteShader->SetActive();
	// Set the view-projection matrix
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);
	
	return true;
}

// ===========================================
// 課題6.1
bool Renderer::LoadMeshShaders(std::string& shader_and_fileName_cnt, MeshComponent* mc)
{
	if (!GetMeshShaders(shader_and_fileName_cnt, mc))
	{
		return false;
	}

	// Set the view-projection matrix
	mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
		mScreenWidth, mScreenHeight, 25.0f, 10000.0f);
	GetMeshShaders(shader_and_fileName_cnt, mc)->SetMatrixUniform("uViewProj", mView * mProjection);
	AddMeshComp(shader_and_fileName_cnt, mc);

	return true;
}
// ===========================================


void Renderer::CreateSpriteVerts()
{
	float vertices[] = {
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
		0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // top right
		0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
		-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // bottom left
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

// ===========================================
// 課題6.2
std::string Renderer::format(const char* chars1, const char* chars2, std::string str3)
{
	std::string str1(chars1);
	std::string str2(chars2);

	std::string str11 = str1 + str3 + str2;

	return str11;

	/* char配列の結合を試したけどエラー出て諦めた
	const char* str33 = str3.c_str();

	//int len = strlen(str1) + strlen(str2) + strlen(str33);
	int len = std::strlen(str1) + std::strlen(str2) + std::strlen(str33);
	char* buf = new char[len];
	
	strcpy_s(buf, std::strlen(buf)+1, str1);

	
	strcat_s(buf, sizeof(buf), str33);
	strcat_s(buf, sizeof(buf), str2);
	
	return buf;
	*/
}
// ===========================================

void Renderer::SetLightUniforms(Shader* shader)
{
	// Camera position is from inverted view
	Matrix4 invView = mView;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
	// Ambient light
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);
	// Directional light
	shader->SetVectorUniform("uDirLight.mDirection",
		mDirLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor",
		mDirLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor",
		mDirLight.mSpecColor);

	// ===========================================
	// 課題6.2

	std::string tmp_uniform_name;
	for (int i = 0; i < 4; i++)
	{
		tmp_uniform_name = format("uPointLight[", "].mPosition", std::to_string(i));
		shader->SetVectorUniform(tmp_uniform_name.c_str(), mPointLight[i].mPosition);
		
		tmp_uniform_name = format("uPointLight[", "].mDiffuseColor", std::to_string(i));
		shader->SetVectorUniform(tmp_uniform_name.c_str(), mPointLight[i].mDiffuseColor);
		
		tmp_uniform_name = format("uPointLight[", "].mSpecColor", std::to_string(i));
		shader->SetVectorUniform(tmp_uniform_name.c_str(), mPointLight[i].mSpecColor);
		
		tmp_uniform_name = format("uPointLight[", "].mSpecPower", std::to_string(i));
		shader->SetFloatUniform(tmp_uniform_name.c_str(), mPointLight[i].mSpecPower);

		tmp_uniform_name = format("uPointLight[", "].mRadius", std::to_string(i));
		shader->SetFloatUniform(tmp_uniform_name.c_str(), mPointLight[i].mRadius);
	}
	// ===========================================

}
