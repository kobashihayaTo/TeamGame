#pragma once

#include "WorldTransform.h"
#include "Input.h"
#include "Model.h"
#include "DebugText.h"
#include "Function/Function.h"
#include "MyMath.h"
#include "Material.h"

#include "PrimitiveDrawer.h"


struct CornerPos {
	float cornerZ;	//プレイヤーの角のZ座標
	float cornerX;	//プレイヤーの角のX座標

};

class Player
{
public:	//構造体

public:	//メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="texturehandle">テクスチャハンドル</param>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(bool playerKeyFlag);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// 左上のマップ番号を取得
	CornerPos LeftTopNum();
	
	// 右上のマップ番号を取得
	CornerPos RightTopNum();
	
	// 左下のマップ番号を取得
	CornerPos LeftBottomNum();

	// 右下のマップ番号を取得
	CornerPos RightBottomNum();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	Vector3 GetMove() { return Move; }

	Vector3 GetTransform() { return Trans; }

	bool GetDeathFlag() { return playerDeathFlag; }

	bool GetSecretFlag() { return playerSecretFlag; }

	bool GetSecretIntervalFlag() { return secretIntervalFlag; }

	bool GetOKFlag() { return OKFlag; }

	void Reset();

public:
	// speedZのセッター
	void SetPlayerSpeedZ(float playerSpeedZ) { playerSpeedZ_ = playerSpeedZ; }

	// speedXのセッター
	void SetPlayerSpeedX(float playerSpeedX) { playerSpeedX_ = playerSpeedX; }

	void SetWorldPosition(Vector3 worldPosition) { worldTransform_.translation_ = worldPosition; }

	Vector3 GetprePosition() { return PrePosition_; }

	float GetRadius() { return radius; }



	//カメラのトランスのセッター


private:
	//半径
	const float radius = 0.8f;

private:	//メンバ変数

	//ワールド変換データ
	WorldTransform worldTransform_;

	WorldTransform effectTransform;

	// 移動する前の座標
	Vector3 PrePosition_;

	//モデル
	Model* model_ = nullptr;
	Model* effectModel = nullptr;

	//入力処理するため
	Input* input_ = nullptr;

	//デバッグテキスト
	DebugText* debugText_ = nullptr;

	//ファンクション
	MyFunc myFunc_;

	Material* material = nullptr;

	PrimitiveDrawer* primitiveDrawer = nullptr;

	//右上、右下の座標の変数
	int rightTopZ = 0;
	int rightTopX = 0;
	int rightBottomX = 0;
	int rightBottomZ = 0;
	 
	//左上、左下の座標の変数
	int leftTopZ = 0;
	int leftTopX = 0;
	int leftBottomX = 0;
	int leftBottomZ = 0;

	//プレイヤーの速度
	float playerSpeedX_ = 0.3f;
	float playerSpeedZ_ = 0.3f;
	Vector3 Move = { 0,0,0 };

	Vector3 Trans = { 0,0,0 };

	//フラグ関係
	bool playerDeathFlag = false;
	bool playerSecretFlag = false;

	bool secretIntervalFlag = false;

	XMFLOAT4 DefaultColor = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 SecretColor = { 1.0f,1.0f,1.0f,0.5f };

	int secretTimer = 100;
	float secretIntervalTimer = 100.0f;

	bool OKFlag = false;


};

