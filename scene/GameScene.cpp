#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

#include "math/MyMath.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete mapModel_;
	delete playerModel_;
	delete floorModel_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

 	mapTH_ = TextureManager::Load("cube/cube.jpg");
	floorTH_ = TextureManager::Load("ground/floor.png");

	//3Dモデルを生成
	mapModel_ = Model::CreateFromOBJ("cube", true);
	playerModel_ = Model::CreateFromOBJ("player", true);
	floorModel_ = Model::CreateFromOBJ("floor", true);

#pragma  region 初期化
	//マップの初期化
	newMap->Initialize(mapModel_, floorModel_);
	//カメラの初期位置を設定
	Vector3 cameraPos(0, 0, 0);
	Vector3 cameraRot(0, 0, 0);
	//レールカメラの初期化
	newCamera->Initialize(cameraPos, cameraRot);
	//プレイヤーの初期化
	newPlayer->Initialize(playerModel_);
#pragma endregion
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	model_ = Model::Create();
}

void GameScene::Update() {

	if (newMap->GetInvisible() == false)
	{
		
	}

	//プレイヤーの更新
	newPlayer->Update();

	//マップの更新
	newMap->Update(newPlayer.get());

	//カメラの更新
	newCamera->Update();
	//レールカメラをゲームシーンのカメラに適応する
	viewProjection_.matView = newCamera->GetViewProjection().matView;
	viewProjection_.matProjection = newCamera->GetViewProjection().matProjection;
	//ビュープロジェクションの転送
	viewProjection_.TransferMatrix();

	SceneChange();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//マップの描画
	newMap->Draw(viewProjection_);

	//床の描画
	newMap->FloorDraw(viewProjection_);

	//プレイヤーの描画
	newPlayer->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::SceneChange() {
	isEnd_ = false;
	if (input_->TriggerKey(DIK_SPACE))
	{
		nextScene_ = Scene::END;
		isEnd_ = true;
	}
}
