// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Grid.h"
#include "Tile.h"
#include "Tower.h"
#include "Enemy.h"
#include "AIState.h"
#include <algorithm>

Grid::Grid(class Game* game)
:Actor(game)
,mSelectedTile(nullptr)
{
	// mTilesを7個の要素数に変更
	mTiles.resize(NumRows);

	// 要素それぞれを16個の要素数に変更
	// 7行 16列
	for (size_t i = 0; i < mTiles.size(); i++)
	{
		mTiles[i].resize(NumCols);
	}
	
	// 7 x 16のタイルを作成
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			// i行j列のタイルを初期化
			mTiles[i][j] = new Tile(GetGame());
			// i行j列のタイルの座標を初期化
			mTiles[i][j]->SetPosition(Vector2(TileSize/2.0f + j * TileSize, StartY + i * TileSize));
		}
	}
	
	// スタートとエンドのタイルを設定
	GetStartTile()->SetTileState(Tile::EStart);
	GetEndTile()->SetTileState(Tile::EBase);
	
	// 隣接リストを設定
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			// 一つ前の隣接ノードを設定(一番最初のノードにはない)
			if (i > 0)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i-1][j]);
			}
			// 一つ後の隣接ノードを設定(一番最後のノードにはない)
			if (i < NumRows - 1)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i+1][j]);
			}
			if (j > 0)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i][j-1]);
			}
			if (j < NumCols - 1)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i][j+1]);
			}
		}
	}
	
	// Find path (in reverse)
	FindPath(GetEndTile(), GetStartTile());
	UpdatePathTiles(GetStartTile());
	
	mNextEnemy = EnemyTime;
}

// Tileを選択する
void Grid::SelectTile(size_t row, size_t col)
{
	// 選択したタイルのステートを確認
	Tile::TileState tstate = mTiles[row][col]->GetTileState();

	// 選択したタイルのステートがスタート、ゴールでないなら
	if (tstate != Tile::EStart && tstate != Tile::EBase)
	{
		// 一つ前に選択したタイルの選択状態を解除
		if (mSelectedTile)
		{
			mSelectedTile->ToggleSelect();
		}

		// 選択したタイルを選択状態に
		mSelectedTile = mTiles[row][col];
		mSelectedTile->ToggleSelect();
	}
}

// クリック操作
void Grid::ProcessClick(int x, int y)
{	
	y -= static_cast<int>(StartY - TileSize / 2);
	if (y >= 0)
	{
		x /= static_cast<int>(TileSize);
		y /= static_cast<int>(TileSize);
		if (x >= 0 && x < static_cast<int>(NumCols) && y >= 0 && y < static_cast<int>(NumRows))
		{
			SelectTile(y, x);
		}
	}
}

// A*経路探索を実装
bool Grid::FindPath(Tile* start, Tile* goal)
{
	// 全てのタイルの初期化
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			// ヒューリスティック値
			mTiles[i][j]->g = 0.0f;
			// オープンセットに帰属
			mTiles[i][j]->mInOpenSet = false;
			// クローズドセットに帰属
			mTiles[i][j]->mInClosedSet = false;
		}
	}
	
	// オープンセットの動的配列を定義
	std::vector<Tile*> openSet;
	
	// カレントノードを記録するための変数
	// スタートのノードで初期化
	Tile* current = start;
	// カレントノードをクローズドセットに追加
	current->mInClosedSet = true;
	
	// ゴールするまでループ
	do
	{
		// 隣接ノードをオープンセットに追加
		for (Tile* neighbor : current->mAdjacent)
		{
			// 隣接ノードが壁ならcontinue
			if (neighbor->mBlocked)
			{
				continue;
			}
			
			// 隣接ノードがクローズドセットでない限り追加
			if (!neighbor->mInClosedSet)
			{
				// オープンセットでないなら
				if (!neighbor->mInOpenSet)
				{
					// 隣接ノードの親ノードを設定する
					neighbor->mParent = current;
					// 隣接ノードのヒューリスティック値を設定する
					neighbor->h = (neighbor->GetPosition() - goal->GetPosition()).Length();
					
					// g(x) 経路コスト
					// 親ノードの経路コスト + エッジを辿るコスト 
					neighbor->g = current->g + TileSize;
					// 
					neighbor->f = neighbor->g + neighbor->h;
					// 隣接ノードをオープンセットに追加
					openSet.emplace_back(neighbor);
					// オープンセットのスクラッチデータを設定
					neighbor->mInOpenSet = true;
				}
				else
				{
					// オープンセットの場合、もともと設定していたg(x)と
					// 現在のカレントノードから計算したg(x)を比較して、
					// 元々設定していたg(x)の方が大きい場合は、現在のカレントノードから計算したg(x)を設定

					// 現在のカレントノードからg(x)を計算
					float newG = current->g + TileSize;
					if (newG < neighbor->g)
					{
						// カレントノードを採用
						neighbor->mParent = current;
						neighbor->g = newG;
						// g(x)を変更したのでf(x)も変更
						neighbor->f = neighbor->g + neighbor->h;
					}
				}
			}
		}
		
		// オープンセットが空なら、全ての可能性のある経路は探しつくした
		if (openSet.empty())
		{
			break;
		}
		
		// 最もコストの低いノードをオープンセットから探す
		auto iter = std::min_element(openSet.begin(), openSet.end(),
									 [](Tile* a, Tile* b) {
										 return a->f < b->f;
									 });
		// 最もコストの低いノードをカレントノードに設定
		current = *iter;
		// オープンセットから最もコストの低いノードを消去
		openSet.erase(iter);
		// クローズドセットに設
		current->mInOpenSet = false;
		current->mInClosedSet = true;
	}
	while (current != goal);
	
	// ゴールを見つけれたか否か
	return (current == goal) ? true : false;
}

// 経路タイルを更新
void Grid::UpdatePathTiles(class Tile* start)
{
	// スタートとエンド以外のタイルの状態をリセット
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			if (!(i == 3 && j == 0) && !(i == 3 && j == 15))
			{
				mTiles[i][j]->SetTileState(Tile::EDefault);
			}
		}
	}
	
	// startの親ノードに初期化(逆順なので親ノードはスタートの横のノード)
	Tile* t = start->mParent;

	// tがエンドタイルになるまでノードをさかのぼる
	while (t != GetEndTile())
	{	
		// さかのぼったタイルは経路タイルに設定
		t->SetTileState(Tile::EPath);
		t = t->mParent;
	}
}

// 塔を建てる
void Grid::BuildTower()
{
	// 選択したタイルが塔を建てられるなら
	if (mSelectedTile && !mSelectedTile->mBlocked)
	{	
		// 選択したタイルの状態をblockedに変更
		mSelectedTile->mBlocked = true;

		// 経路探索でスタートからゴールまで到達できたなら
		if (FindPath(GetEndTile(), GetStartTile()))
		{
			// 塔を建てる
			Tower* t = new Tower(GetGame());

			// 塔のPositionを設定
			t->SetPosition(mSelectedTile->GetPosition());
		}
		else
		{
			// 選択したタイルに塔を建てるとゴールができないので、塔を建てないようにする
			mSelectedTile->mBlocked = false;
			FindPath(GetEndTile(), GetStartTile());
		}
		UpdatePathTiles(GetStartTile());
	}
}

Tile* Grid::GetStartTile()
{
	return mTiles[3][0];
}

Tile* Grid::GetEndTile()
{
	return mTiles[3][15];
}

void Grid::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	
	// Is it time to spawn a new enemy?
	mNextEnemy -= deltaTime;
	if (mNextEnemy <= 0.0f)
	{
		new Enemy(GetGame());
		mNextEnemy += EnemyTime;
	}
}
