#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Function/Function.h"
#include <cassert>
#include "DebugText.h"
#include"Input.h"
#include "MathUtility.h"
#include "ViewProjection.h"
#include <Player/Player.h>

class Map
{
public:		//メンバ関数

	Map();

	~Map();

	//初期化
	void Initialize(Model* model, Model* floorModel);

	//更新
	void Update(Player* player);

	//描画
	void Draw(ViewProjection& viewProjection);

	//床の描画
	void FloorDraw(ViewProjection& viewProjection);

	//ブロックサイズを取得
	int GetBlockSize() {return BlockSize; }

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision(Vector3 playerPos, float radius);

	enum Mapinfo {
		NONE, //0
		WALL, //1
		BLOCK//2
	};


	/// <summary>
	/// マップ上のブロックに当たっているかチェック
	/// </summary>
	/// <param name="line"></param>
	/// <param name="culumn"></param>
	/// <returns></returns>
	int BlockCheck(int line, int culumn);
	
	void BlockCheck(Player* player);

	bool CheckCollision(Vector3 pos1, Vector3 pos2, float radius1, float radius2);

	int GetMapFlag() { return MapFlag; }

	bool GetInvisible() { return invisibleFlag; }

	void Reset();

public:
	//半径
	const float radius = 0.9f;

public:		//マップ

	//第一ステージ
	int FirstMap[20][25]{
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,1},
		{1,0,0,0,0,2,2,2,2,2,0,0,0,0,2,2,0,2,2,0,0,2,2,0,1},
		{1,0,0,0,0,2,2,2,2,2,0,2,2,0,2,2,0,2,2,0,0,2,2,0,1},
		{1,0,0,0,0,2,2,0,0,0,0,2,2,0,0,0,0,2,2,0,0,2,2,0,1},
		{1,0,0,0,0,2,2,0,0,0,0,2,2,0,2,2,2,2,2,0,0,2,2,0,1},
		{1,0,0,0,0,2,2,0,0,0,0,2,2,0,2,2,2,2,2,0,0,2,2,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,2,2,2,2,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,2,2,2,2,0,0,0,2,2,2,2,2,2,2,2,2,2,2,0,0,0,1},
		{1,0,0,2,2,2,2,0,0,0,2,2,2,2,2,2,2,2,2,2,2,0,0,0,1},
		{1,0,0,2,2,2,2,0,0,0,2,2,2,2,2,2,2,2,2,2,2,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},

	};
	
	//第二ステージ
	/*int SecondMap[20][25]{
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},

	};*/


private:	//メンバ変数
	//ワールド変換データ
	WorldTransform worldTransform_[20][25] = {};
	int BlockSize;
	WorldTransform floorWorldTransform_;

	//操作
	Input* input_ = nullptr;
	//モデル
	Model* model_ = nullptr;

	//床のモデル
	Model* floorModel_ = nullptr;

	//デバッグテキスト
	DebugText* debugText_ = nullptr;

	//ファンクション
	MyFunc myFunc_;

	//マップフラグ
	int MapFlag = 0;

	//透明フラグ
	bool invisibleFlag = false;
};