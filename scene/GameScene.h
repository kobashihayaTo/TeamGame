#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Function/Function.h"

#include "camera/RailCamera.h"
#include "Enum.h"
#include "Player/Player.h"
#include "Map/Map.h"
#include "Enemy/Enemy.h"
#include "UI.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

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

	void CheckAllCollisions(Enemy* enemy);

	/// <summary>
	/// シーン切り替え
	/// </summary>
	void SceneChange();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	Scene GetNextScene() { return nextScene_; }

	int GetIsEnd() { return isEnd_; }

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	WorldTransform worldTransform_;

	//レールカメラ
	std::unique_ptr<RailCamera> newCamera = std::make_unique<RailCamera>();
	
	Model* model_ = nullptr;

	//マップ生成
	std::unique_ptr<Map> newMap = std::make_unique<Map>();
	//マップモデル
	Model* mapModel_ = nullptr;
	//マップテクスチャ
	uint32_t mapTH_ = 0u;

	//プレイヤー生成
	std::unique_ptr<Player> newPlayer = std::make_unique<Player>();
	//プレイヤーモデル
	Model* playerModel_ = nullptr;
	
	//テクスチャハンドル
	uint32_t floorTH_ = 0u;
	//床モデル
	Model* floorModel_ = nullptr;
	//床のスプライト
	Sprite* floorSprite = nullptr;	uint32_t ruleTH_ = 0u;

	//敵生成
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	std::unique_ptr<Enemy> newEnemy_1 = std::make_unique<Enemy>();
	//敵モデル
	Model* enemyModel_ = nullptr;

	Vector3 cameraPos = newPlayer->GetTransform();
	Vector3 cameraRot = { 0,0,0 };

	//UI
	std::unique_ptr<UI> newUI = std::make_unique<UI>();

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	Scene nextScene_ = Scene::GAME;
	int isEnd_ = false;

	bool keyFlag = false;
	bool MapkeyFlag = false;
	bool playerkeyFlag = false;


};
