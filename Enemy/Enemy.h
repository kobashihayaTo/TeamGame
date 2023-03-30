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
public:		//メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="texturehandle">テクスチャハンドル</param>
	void Initialize(Model* model, RailCamera* camera,Vector3 enemyPos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(bool keyFlag, Player* player);

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
	/// それぞれのベクトルを得る関数(上)
	/// </summary>
	void UpSensorVector(float playerZ, float playerX, float playerRadius);

	/// <summary>
	/// それぞれのベクトルを得る関数(下)
	/// </summary>
	void DownSensorVector(float playerZ, float playerX, float playerRadius);

	/// <summary>
	/// それぞれのベクトルを得る関数(右)
	/// </summary>
	void RightSensorVector(float playerZ, float playerX, float playerRadius);

	/// <summary>
	/// それぞれのベクトルを得る関数(左)
	/// </summary>
	void LeftSensorVector(float playerZ, float playerX, float playerRadius);

	/// <summary>
	/// センサーの描画
	/// </summary>
	void SensorDraw();

	void FlagReset();

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	Vector3 GetSensorPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	void SensorCollision();

	float GetRadius() { return radius; }

	float GetSensorRadius() { return sensorRadius; }

	bool GetStopFlag() { return stopFlag; }

	bool GetOKFlag() { return OKFlag; }

	const std::list<std::unique_ptr<Enemy>>& GetEnemy() { return enemys; }
private:
	//半径
	const float radius = 1.0f;
	//半径
	const float sensorRadius = 2.0f;

private:	//メンバ変数

	//ワールド変換データ
	WorldTransform worldTransform_;

	WorldTransform sensorTransform_;

	//モデル
	Model* model_ = nullptr;

	//入力処理するため
	Input* input_ = nullptr;

	//デバッグテキスト
	DebugText* debugText_ = nullptr;

	PrimitiveDrawer* primitive_ = nullptr;

	//ファンクション
	MyFunc myFunc_;

	std::list<std::unique_ptr<Enemy>>enemys;

	//切り替えフラグ
	bool LightFlag = false;
	bool LeftFlag =  false;

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

	float sensorVisionX[2];	//カメラの視界
	float sensorVisionZ[2];

	float visionMemoryX[2];	//視界移動の記憶変数
	float visionMemoryZ[2];

	int visionFlag; //視界の移動を制御するフラグ
	int visionHitFlag[3]; //視界の当たり判定用フラグ

	int speed;

	float visionTimer; //視界が再び動くまでのタイマー
};

