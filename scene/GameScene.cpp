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

	//3Dモデルを生成
	mapModel_ = Model::CreateFromOBJ("cube", true);
	playerModel_ = Model::CreateFromOBJ("player", true);
	floorModel_ = Model::CreateFromOBJ("floor", true);
	enemyModel_ = Model::CreateFromOBJ("enemy", true);

#pragma  region 初期化
	//マップの初期化
	newMap->Initialize(mapModel_, floorModel_);
	//カメラの初期位置を設定
	cameraPos = { 10,10,10 };
	cameraRot = { 0,0,0 };
	//レールカメラの初期化
	newCamera->Initialize(cameraPos, cameraRot);
	//プレイヤーの初期化
	newPlayer->Initialize(playerModel_);
	//敵の初期化
	newEnemy->Initialize(enemyModel_);
#pragma endregion
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	model_ = Model::Create();
}

void GameScene::Update() {

	//プレイヤーの更新
	newPlayer->Update();

	//cameraPos = newPlayer->GetTransform();

	//マップの更新
 	newMap->Update(newPlayer.get());

	newEnemy->Update();



	//カメラの更新
	newCamera->Update();
	//レールカメラをゲームシーンのカメラに適応する
	viewProjection_.matView = newCamera->GetViewProjection().matView;
	viewProjection_.matProjection = newCamera->GetViewProjection().matProjection;
	//ビュープロジェクションの転送
	viewProjection_.TransferMatrix();

	SceneChange();

	if (newPlayer->GetSecretFlag() == false) {
 		CheckAllCollisions(newEnemy.get());
	}

	if (input_->TriggerKey(DIK_W)) {
		newPlayer->Reset();
		newEnemy->Reset();
		newMap->Reset();
	}


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

	//敵の描画
	newEnemy->Draw(viewProjection_);

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

void GameScene::CheckAllCollisions(Enemy* enemy) {
	//判定対象AとBの座標
	Vector3 posA, posB;

	//距離を図るための変数
	double distance;

	//半径
	float radius;

	//自キャラとマップチップの当たり判定
#pragma region
	//自キャラの座標
	posA = newPlayer->GetWorldPosition();

	//マップの座標
	posB = enemy->GetWorldPosition();

	//座標AとBの距離を求める
	distance = CalculateDistance(posA, posB);

	radius = newPlayer->GetRadius() + enemy->GetRadius();
	distance = (posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y) + (posA.z - posB.z) * (posA.z - posB.z);
	//自キャラとマップの当たり判定
	if (distance <= radius * radius) {
		//自キャラの衝突時コールバックを呼び出す
		newPlayer->OnCollision();
		//マップの衝突時コールバックを呼び出す
		enemy->OnCollision();
	}

#pragma endregion
}

void GameScene::SceneChange() {
	isEnd_ = false;
	if (input_->TriggerKey(DIK_SPACE) || newMap->GetGoalFlag() == true)
	{
		nextScene_ = Scene::END;
		isEnd_ = true;
	}
	if (newPlayer->GetFlag() == true)
	{
		nextScene_ = Scene::BADEND;
		isEnd_ = true;
	}
}

void GameScene::Reset()
{
	newPlayer->Reset();
	newEnemy->Reset();
	newMap->Reset();
}
