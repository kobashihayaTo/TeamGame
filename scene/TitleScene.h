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

#include "Easeing.h"
#include "Enum.h"
#include "GameScene.h"
/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene
{
public://メンバ関数

	/// <summary>
	/// コンストクラタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();


	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	void TitleEase();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// タイトルでセレクト
	/// </summary>
	void ChangeSelect();

	void Reset();

	/// <summary>
	/// サウンド
	/// </summary>
	void SoundStop();

	Scene GetNextScene() { return nextScene_; }

	int GetChangeFlag() { return changeFlag_; }

	int GetSelectFlag() { return selectFlag; }

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

	Scene nextScene_ = Scene::TITLE;

	int changeFlag_ = false;

	//テクスチャバンドル
	uint32_t textureHandle_ = 0;
	uint32_t nameHandle_ = 0;

	//スプライト
	Sprite* sprite_ = nullptr;
	Sprite* sprite_2 = nullptr;
	//サウンド
	uint32_t soundDataHandle_ = 0u;
	//音声再生ハンドル
	uint32_t playHandle = -1;

	int selectFlag = 0;

	int Time = 0;
	float posY = -1080.0f;
	//float posY = 0.0f;
	
	double frame = 0.0f;
	double endFrame = 120.0f;
	double start = -1080.0f;
	double end = 0.0f;
};

