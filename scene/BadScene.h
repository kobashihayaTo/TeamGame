#pragma once
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "Audio.h"

#include "Enum.h"

/// <summary>
/// タイトルシーン
/// </summary>
class BadScene
{
public://メンバ関数

	/// <summary>
	/// コンストクラタ
	/// </summary>
	BadScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BadScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();


	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// サウンド
	/// </summary>
	void SoundStop();

	Scene GetNextScene() { return nextScene_; }

	int GetEndFlag() { return endFlag_; }
private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	Audio* audio_ = nullptr;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	Scene nextScene_ = Scene::BADEND;

	int endFlag_ = false;

	//テクスチャバンドル
	uint32_t textureHandle_ = 0;
	//スプライト
	Sprite* sprite_ = nullptr;

	//サウンド
	uint32_t soundDataHandle_ = 0;
	//音声再生ハンドル
	uint32_t playHandle = -1;
};

