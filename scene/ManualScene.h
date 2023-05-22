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

class ManualScene
{
public:
	/// <summary>
	/// コンストクラタ
	/// </summary>
	ManualScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ManualScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 初期化
	/// </summary>
	void Reset();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// タイトルでセレクト
	/// </summary>
	void ChangeSelect();

	Scene GetNextScene() { return nextScene_; }

	int GetManualFlag() { return manualFlag_; }
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

	Scene nextScene_ = Scene::END;

	int manualFlag_ = false;

	//テクスチャバンドル
	uint32_t menuHandle_1 = 0;
	uint32_t menuHandle_2 = 0;
	uint32_t menuHandle_3 = 0;
	uint32_t menuHandle_4 = 0;
	//スプライト
	Sprite* sprite_1 = nullptr;
	Sprite* sprite_2 = nullptr;
	Sprite* sprite_3 = nullptr;
	Sprite* sprite_4 = nullptr;
	int selectFlag = 0;

};

