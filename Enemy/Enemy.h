#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Model.h"
#include "DebugText.h"
#include "Function/Function.h"
#include "camera/RailCamera.h"
#include "Player/Player.h"
#include "PrimitiveDrawer.h"

#include <list>

#define PI 3.14


class Enemy
{
public:

	//敵の向き
	enum Direction {
		UP,		// 0
		DOWN,	// 1
		RIGHT,	// 2
		LEFT	// 3
	};

public:		//メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="texturehandle">テクスチャハンドル</param>
	void Initialize(Model* model, Model* sensorModel, RailCamera* camera, Vector3 enemyPos, bool WidthHeightFlag);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(bool keyFlag, Player* player, float moveDis);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 視界を動かす関数
	/// </summary>
	void SensorVision();

	/// <summary>
	/// センサーの描画
	/// </summary>
	void SensorDraw(ViewProjection& viewProjection);

	/// <summary>
	/// 移動処理
	/// </summary>
	void EnemyMove(float moveDis, bool WidthHeightFlag);

	/// <summary>
	/// プレイヤーが敵から見てどこにいるかを見る
	/// </summary>
	void EnemyMoveSearch(float playerX, float playerZ, float playerR, bool WidthHeightFlag);

	/// <summary>
	/// プレイヤーが敵から見てどこにいるかをチェックする
	/// </summary>
	void EnemyMoveCheck(float playerX, float playerZ, float playerR, bool WidthHeightFlag);

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	/// <summary>
	/// ベクトルと線の当たり判定
	/// </summary>
	/// <returns></returns>
	bool VectorLineCollision(Vector3 player, float playerRadius, Vector3 start, Vector3 end, Vector3 pt3, Player* player_);

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	Vector3 GetSensorPosition();


	void OffCollision();

	void SetWorldPosition(Vector3 worldPosition) { worldTransform_.translation_ = worldPosition; }

	void SensorCollision();

	float GetRadius() { return radius; }

	float GetSensorRadius() { return sensorRadius; }

	bool GetStopFlag() { return stopFlag; }

	Vector3 GetprePosition() { return PrePosition_; }

	bool GetOKFlag() { return OKFlag; }

	bool GetCrisisFlag() { return crisisFlag; }

	const std::list<std::unique_ptr<Enemy>>& GetEnemy() { return enemys; }
private:
	//半径
	const float radius = 0.8f;
	//半径
	const float sensorRadius = 2.0f;

private:	//メンバ変数

	//ワールド変換データ
	WorldTransform worldTransform_;

	WorldTransform sensorTransform_;

	//モデル
	Model* model_ = nullptr;
	Model* sensorModel_ = nullptr;



	//入力処理するため
	Input* input_ = nullptr;

	//デバッグテキスト
	DebugText* debugText_ = nullptr;

	// 移動する前の座標
	Vector3 PrePosition_;

	PrimitiveDrawer* primitive_ = nullptr;

	//ファンクション
	MyFunc myFunc_;

	std::list<std::unique_ptr<Enemy>>enemys;

	//切り替えフラグ
	bool rightMoveFlag = false;
	bool leftMoveFlag = false;
	bool upMoveFlag = false;
	bool downMoveFlag = false;

	//透明フラグ
	bool invisibleFlag = false;

	bool stopFlag = false;
	bool stopIntervalFlag = false;

	int stopTimer = 100;
	int stopIntervalTimer = 100;

	int enemyMax = 5;

	bool OKFlag = false;

	float sensorX;	//カメラ本体の座標
	float sensorZ;
	float sensorR;

	float HeightSensorVisionX[2];	//カメラの視界
	float HeightSensorVisionZ[2];

	float WidthSensorVisionX[2];	//カメラの視界
	float WidthSensorVisionZ[2];

	float visionMemoryX[2];	//視界移動の記憶変数
	float visionMemoryZ[2];

	int visionFlag; //視界の移動を制御するフラグ
	int visionHitFlag[3]; //視界の当たり判定用フラグ

	float speed;

	float visionTimer; //視界が再び動くまでのタイマー

	int count = 20;
	int isSearch = FALSE;
	int isMove_Height = UP;
	int isMove_Width = RIGHT;
	float sensorMovedDis = 0.0f;

	bool crisisFlag = false;

	XMFLOAT4 SensorColor = { 1.0f,1.0f,1.0f,0.2f };

	int direction = UP;

	bool WidthHeightFlag_ = false;

	bool upFlag = false;
	bool downFlag = false;
	bool rightFlag = false;
	bool leftFlag = false;

	//センサーの頂点
	Vector3 UpStart_Pt1 = { 0,0,0 };
	Vector3 UpEnd_Pt2 = { 0,0,0 };
	Vector3 UpEnd_Pt3 = { 0,0,0 };
	Vector3 DownStart_Pt1 = { 0,0,0 };
	Vector3 DownEnd_Pt2 = { 0,0,0 };
	Vector3 DownEnd_Pt3 = { 0,0,0 };
	Vector3 RightStart_Pt1 = { 0,0,0 };
	Vector3 RightEnd_Pt2 = { 0,0,0 };
	Vector3 RightEnd_Pt3 = { 0,0,0 };
	Vector3 LeftStart_Pt1 = { 0,0,0 };
	Vector3 LeftEnd_Pt2 = { 0,0,0 };
	Vector3 LeftEnd_Pt3 = { 0,0,0 };
};

