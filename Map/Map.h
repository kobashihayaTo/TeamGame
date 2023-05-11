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
#include <Enemy/Enemy.h>
#include "Enum.h"

class Map
{
public:		//Enum
	enum Mapinfo {
		NONE, //0
		WALL, //1
		BLOCK,//2
		GOAL,  //3	
		RELAY, //4
		ECHIGO //5
	};

public:		//メンバ関数

	Map();

	~Map();

	//初期化
	void Initialize(Model* model, Model* floorModel, Model* effectmodel_, Model* afterModel, Model* goalModel);

	//更新
	void Update(Player* player, bool MapkeyFlag);

	//描画
	void Draw(ViewProjection& viewProjection);

	//床の描画
	void FloorDraw(ViewProjection& viewProjection);

	//ブロックサイズを取得
	int GetBlockSize() { return BlockSize; }

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision(Vector3 playerPos, float radius);


	/// <summary>
	/// マップ上のブロックに当たっているかチェック
	/// </summary>
	/// <param name="line"></param>
	/// <param name="culumn"></param>
	/// <returns></returns>
	void EnemyBlockCheck(Enemy* enemy);

	void PlayerBlockCheck(Player* player);

	bool CheckCollision(Vector3 pos1, Vector3 pos2, float radius1, float radius2);

	int GetMapFlag() { return MapFlag; }

	bool GetGoalFlag() { return goalFlag; }

	bool GetAnswerFlag() { return AnswerFlag; }

	bool GetOKFlag() { return OKFlag; }

	void Reset();

	bool GeteffectOffFlag() { return effectOffFlag; }

	bool GetFlag() { return goalReadyFlag; }

	int GetGoalCount() { return GoalCount; }

	int GetGoalRELAYCount() { return GoalRELAYCount; }
	int GetGoalECHIGOCount() { return GoalECHIGOCount; }


	bool GetUIFlag() { return UIFlag; }
	bool GetTestFlag() { return testFlag; }

	int Getgoal_() { return goal_; }

	bool GetResetFlag() { return ResetFlag; }

public:
	//半径
	const float radius = 0.9f;

public:		//マップ

	//第一ステージ
	int FirstMap[40][25]{
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,2,2,0,0,2,2,0,0,2,2,2,2,2,2,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,0,0,2,2,0,0,0,0,2,2,2,2,2,2,0,0,1},
		{1,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,1},
		{1,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,1},
		{1,0,0,0,0,0,0,5,2,2,0,0,2,2,0,0,0,0,2,2,2,2,0,0,1},
		{1,0,0,0,0,0,0,0,2,2,0,0,2,2,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,2,2,2,2,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,2,2,2,2,0,0,0,0,2,2,2,2,2,2,0,0,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,0,0,2,2,2,2,2,2,2,2,0,0,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,0,0,2,2,2,2,2,2,0,0,0,0,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,2,2,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,1},
		{1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,1},
		{1,0,0,2,0,0,0,0,0,0,2,2,2,4,0,0,0,0,2,2,2,2,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,2,2,0,0,1},
		{1,0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,2,2,0,0,1},
		{1,0,0,0,0,2,2,2,2,2,2,2,2,0,0,2,2,0,0,0,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,2,2,2,2,2,0,0,2,2,2,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,1},
		{1,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,1},
		{1,0,0,2,2,0,0,0,0,2,2,2,2,0,0,0,2,2,0,0,2,2,0,0,1},
		{1,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,2,2,0,0,1},
		{1,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,1},
		{1,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,2,2,2,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,2,2,2,2,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,2,2,2,2,0,0,0,0,2,2,0,0,0,2,2,0,0,1},
		{1,0,0,2,2,2,2,2,2,2,0,0,0,0,2,2,2,2,0,0,2,2,0,0,1},
		{1,0,0,2,2,2,2,2,2,2,0,0,0,0,2,2,2,2,0,0,2,2,0,0,1},
		{1,0,0,2,2,2,2,0,0,0,0,0,0,0,0,2,2,0,0,0,2,2,0,0,1},
		{1,0,0,2,2,2,2,3,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};




private:	//メンバ変数
	//ワールド変換データ
	WorldTransform worldTransform_[40][25] = {};
	int BlockSize;
	WorldTransform floorWorldTransform_;
	WorldTransform effectworldTrans;

	//操作
	Input* input_ = nullptr;
	//モデル
	Model* model_ = nullptr;
	Model* effectmodel = nullptr;

	//床のモデル
	Model* floorModel_ = nullptr;

	Model* afterModel_ = nullptr;

	//ゴールのモデル
	Model* goalModel_ = nullptr;

	//デバッグテキスト
	DebugText* debugText_ = nullptr;

	//ファンクション
	MyFunc myFunc_;

	Player* player_ = nullptr;

	//マップフラグ
	int MapFlag = 0;

	//透明フラグ
	bool AnswerFlag = false;
	bool AnswerIntervalFlag = false;
	int  AnswerTimer = 100;
	int  AnswerIntervalTimer = 100;

	bool goalFlag = false;
	bool goalReadyFlag = false;
	//
	int GoalCount = 50;
	int GoalRELAYCount = 50;
	int GoalECHIGOCount = 50;

	int goalcount = 0;

	int Map_Z = 40;
	int Map_X = 25;

	bool OKFlag = false;
	bool effectOffFlag = false;

	int playerTimer = 0;

	bool UIFlag = false;

	bool testFlag = false;

	int goal_ = 0;

	bool ResetFlag = false;
};