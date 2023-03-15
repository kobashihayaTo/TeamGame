#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Model.h"
#include "DebugText.h"
#include "Function/Function.h"
#include "camera/RailCamera.h"
#include "PrimitiveDrawer.h"

#define PI 3.14


class Enemy
{
public:		//メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="texturehandle">テクスチャハンドル</param>
	void Initialize(Model* model, RailCamera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(bool keyFlag);

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
	/// それぞれのベクトルを得る関数
	/// </summary>
	void SensorVector(float playerZ, float playerX, float playerRadius);

	/// <summary>
	/// センサーの描画
	/// </summary>
	void SensorDraw();

	void FlagReset();

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	float GetRadius() { return radius; }

	bool GetStopFlag() { return stopFlag; }

	bool GetOKFlag() { return OKFlag; }
private:
	//半径
	const float radius = 1.0f;

private:	//メンバ変数

	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//入力処理するため
	Input* input_ = nullptr;

	//デバッグテキスト
	DebugText* debugText_ = nullptr;

	PrimitiveDrawer* primitive_ = nullptr;

	//ファンクション
	MyFunc myFunc_;

	//切り替えフラグ
	bool LightFlag = false;
	bool LeftFlag = false;

	//透明フラグ
	bool invisibleFlag = false;

	bool stopFlag = false;
	bool stopIntervalFlag = false;

	int stopTimer = 100;
	int stopIntervalTimer = 100;

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

