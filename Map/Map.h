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
		MIDDLEGOAL1, //4
		MIDDLEGOAL2 //5
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

	void WallDraw(ViewProjection& viewProjection);

	//床の描画
	void FloorDraw(ViewProjection& viewProjection);

	//ブロックサイズを取得
	int GetBlockSize() { return blockSize; }

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

	int GetMapFlag() { return mapFlag; }

	bool GetGoalFlag() { return goalFlag; }

	bool GetAnswerFlag() { return answerFlag; }

	bool GetOKFlag() { return OKFlag; }

	void Reset();

	bool GetEffectOffFlag() { return effectOffFlag; }

	bool GetFlag() { return goalReadyFlag; }

	int GetGoalCount() { return goalCount; }

	int GetGoalMidCount1() { return goalMidCount1; }
	int GetGoalMidCount2() { return goalMidCount2; }


	bool GetUIFlag() { return UIFlag; }
	bool GetTestFlag() { return testFlag; }

	int GetGoal_() { return goal_; }

	bool GetResetFlag() { return resetFlag; }

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
		{1,0,0,0,0,0,0,3,2,2,0,0,2,2,0,0,0,0,2,2,2,2,0,0,1},
		{1,0,0,0,0,0,0,0,2,2,0,0,2,2,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,2,2,2,2,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,2,2,2,2,0,0,0,0,2,2,2,2,2,2,0,0,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,0,0,2,2,2,2,2,2,2,2,0,0,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,0,0,2,2,2,2,2,2,0,0,0,0,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,2,2,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,1},
		{1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,1},
		{1,0,0,2,0,0,0,0,0,0,0,0,0,5,0,0,0,0,2,2,2,2,0,0,1},
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
		{1,0,0,0,0,0,0,2,2,2,2,0,0,0,2,2,0,0,0,0,2,2,0,0,1},
		{1,0,0,2,2,2,2,2,2,2,0,0,0,2,2,2,2,0,0,0,2,2,0,0,1},
		{1,0,0,2,2,2,2,2,2,2,0,0,0,2,2,2,2,0,0,0,2,2,0,0,1},
		{1,0,0,2,2,2,2,0,0,0,0,0,0,0,2,2,0,0,0,0,2,2,0,0,1},
		{1,0,0,2,2,2,2,4,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};




private:	//メンバ変数
	//ワールド変換データ
	WorldTransform worldTransform_[40][25] = {};
	int blockSize;
	WorldTransform floorWorldTransform_;
	WorldTransform effectWorldTrans;

	//操作
	Input* input_ = nullptr;
	//モデル
	Model* model_ = nullptr;
	Model* effectModel = nullptr;

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
	int mapFlag = 0;

	//透明フラグ
	bool answerFlag = false;
	bool answerIntervalFlag = false;
	int  answerTimer = 100;
	int  answerIntervalTimer = 100;

	bool goalFlag = false;
	bool goalReadyFlag = false;
	//
	float goalCount = 50;
	float goalMidCount1 = 50;
	float goalMidCount2 = 50;

	int goalCounter = 0;

	int map_Z = 40;
	int map_X = 25;

	bool OKFlag = false;
	bool effectOffFlag = false;

	int playerTimer = 0;

	bool UIFlag = false;

	bool testFlag = false;

	int goal_ = 0;

	bool resetFlag = false;
};