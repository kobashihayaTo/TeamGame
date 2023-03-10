#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Model.h"
#include "DebugText.h"
#include "Function/Function.h"



class Enemy
{
public:		//メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="texturehandle">テクスチャハンドル</param>
	void Initialize(Model* model);

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
	/// フラグリセット
	/// </summary>
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

	//ファンクション
	MyFunc myFunc_;

	//切り替えフラグ
	bool LightFlag = false;
	bool LeftFlag = false;

	//透明フラグ
	bool stopFlag = false;
	bool stopIntervalFlag = false;

	int stopTimer = 100;
	int stopIntervalTimer = 100;

	bool OKFlag = false;
};

