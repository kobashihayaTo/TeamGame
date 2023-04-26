#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

#include "math/MyMath.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete mapModel_;
	delete mapAfterModel_;
	delete playerModel_;
	delete floorModel_;
	delete enemyModel_;
	delete enemySensorModel_;
	delete effectModel;
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//3Dモデルを生成
	mapModel_ = Model::CreateFromOBJ("cube", true);
	mapAfterModel_ = Model::CreateFromOBJ("momomo", true);
	playerModel_ = Model::CreateFromOBJ("charactor", true);
	floorModel_ = Model::CreateFromOBJ("floor", true);
	enemyModel_ = Model::CreateFromOBJ("enemy3D", true);
	enemySensorModel_ = Model::CreateFromOBJ("sensor", true);
	effectModel = Model::CreateFromOBJ("effect", true);

	//UIの初期化
	newUI->Initialize();
#pragma  region 初期化
	//マップの初期化
	newMap->Initialize(mapModel_, floorModel_, effectModel, mapAfterModel_);
	//カメラの初期位置を設定
	cameraPos = { 10,10,10 };
	cameraRot = { 0,0,0 };
	//レールカメラの初期化
	newCamera->Initialize(cameraPos, cameraRot);
	//プレイヤーの初期化
	newPlayer->Initialize(playerModel_);
	//敵の初期化
	newEnemy->Initialize(enemyModel_, enemySensorModel_, newCamera.get(), enemyPos, false);
	newEnemy_1->Initialize(enemyModel_, enemySensorModel_, newCamera.get(), enemyPos_1, true);
	newEnemy_2->Initialize(enemyModel_, enemySensorModel_, newCamera.get(), enemyPos_2, true);
	newEnemy_3->Initialize(enemyModel_, enemySensorModel_, newCamera.get(), enemyPos_3, false);

#pragma endregion
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	model_ = Model::Create();

	//音声
	playBGMHandle = audio_->LoadWave("Sound/stageBGM1.wav");

}

void GameScene::Update() {

	if (newMap->GetFlag() == false) {
		//プレイヤーの更新
		newPlayer->Update(playerkeyFlag);
	}

	/*if (audio_->IsPlaying(playBGMHandle) == false || playBGMHandle == -1)
	{
		playBGMHandle = audio_->PlayWave(playBGMHandle, false, 1);
	}*/

	//マップの更新
	newMap->Update(newPlayer.get(), MapkeyFlag);

	//敵の更新
	newEnemy->Update(keyFlag, newPlayer.get(), 2.0f);
	newMap->EnemyBlockCheck(newEnemy.get());

	newEnemy_1->Update(keyFlag, newPlayer.get(), 2.0f);
	newMap->EnemyBlockCheck(newEnemy_1.get());

	newEnemy_2->Update(keyFlag, newPlayer.get(), 2.0f);
	newMap->EnemyBlockCheck(newEnemy_2.get());

	newEnemy_3->Update(keyFlag, newPlayer.get(), 2.0f);
	newMap->EnemyBlockCheck(newEnemy_3.get());

	//プレイヤー
	if (input_->TriggerKey(DIK_A)) {
		if (MapkeyFlag == false && keyFlag == false) {
			playerkeyFlag = true;
			MapkeyFlag = false;
			keyFlag = false;
		}
	}
	if (newPlayer->GetOKFlag()) {
		playerkeyFlag = false;
		MapkeyFlag = false;
		keyFlag = false;
	}
	//マップ
	if (input_->TriggerKey(DIK_D)) {
		if (playerkeyFlag == false && keyFlag == false) {
			playerkeyFlag = false;
			MapkeyFlag = true;
			keyFlag = false;
		}
	}
	if (newMap->GetOKFlag()) {
		playerkeyFlag = false;
		MapkeyFlag = false;
		keyFlag = false;
	}
	//敵
	if (input_->TriggerKey(DIK_S)) {
		if (playerkeyFlag == false && MapkeyFlag == false) {
			playerkeyFlag = false;
			MapkeyFlag = false;
			keyFlag = true;
		}
	}
	if (newEnemy->GetOKFlag()) {
		playerkeyFlag = false;
		MapkeyFlag = false;
		keyFlag = false;
	}

	//カメラの更新
	newCamera->Update();
	//レールカメラをゲームシーンのカメラに適応する
	viewProjection_.matView = newCamera->GetViewProjection().matView;
	viewProjection_.matProjection = newCamera->GetViewProjection().matProjection;
	//ビュープロジェクションの転送
	viewProjection_.TransferMatrix();

	//シーンを切り替える
	SceneChange();

	if (newPlayer->GetSecretFlag() == false || newPlayer->GetSecretIntervalFlag() == true) {
		CheckAllCollisions(newEnemy.get());
		CheckAllCollisions(newEnemy_1.get());
		CheckAllCollisions(newEnemy_2.get());
		CheckAllCollisions(newEnemy_3.get());
	}

	//リセット処理
	if (input_->TriggerKey(DIK_W)) {
		Reset();
	}

	debugText_->SetPos(50, 450);
	debugText_->Printf("playerkeyFlag:%d", playerkeyFlag);

	debugText_->SetPos(50, 470);
	debugText_->Printf("keyFlag:%d", keyFlag);

	debugText_->SetPos(50, 490);
	debugText_->Printf("MapkeyFlag:%d", MapkeyFlag);


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
	if (newMap->GeteffectOffFlag() == false)
	{
		newPlayer->Draw(viewProjection_);
	}


	//敵の描画
	newEnemy->Draw(viewProjection_);
	newEnemy_1->Draw(viewProjection_);
	newEnemy_2->Draw(viewProjection_);
	newEnemy_3->Draw(viewProjection_);

	newEnemy->SensorDraw(viewProjection_);
	newEnemy_1->SensorDraw(viewProjection_);
	newEnemy_2->SensorDraw(viewProjection_);
	newEnemy_3->SensorDraw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	newUI->Draw(newEnemy.get(),newMap.get());
	newUI->Draw(newEnemy_1.get(),newMap.get());
	newUI->Draw(newEnemy_2.get(), newMap.get());
	newUI->Draw(newEnemy_3.get(), newMap.get());

	if (keyFlag == true||MapkeyFlag == true||playerkeyFlag == true) {
		newUI->KeyDraw_unusable();
	}
	else
	{

		newUI->KeyDraw();
	}
	// デバッグテキストの描画

#ifdef _DEBUG

// デバッグテキストの描画
	debugText_->DrawAll(commandList);
#endif  
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions(Enemy* enemy_) {
	//判定対象AとBの座標
	Vector3 posA, posB, posC;

	//距離を図るための変数
	double distance;

	//半径
	float radius;

	//自キャラとマップチップの当たり判定
#pragma region
	//自キャラの座標
	posA = newPlayer->GetWorldPosition();

	//敵キャラの座標
	posB = enemy_->GetWorldPosition();

	//座標AとBの距離を求める
	distance = CalculateDistance(posA, posB);

	radius = newPlayer->GetRadius() + enemy_->GetRadius();
	distance = (posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y) + (posA.z - posB.z) * (posA.z - posB.z);
	//自キャラと敵の当たり判定
	if (distance <= radius * radius) {
		//自キャラの衝突時コールバックを呼び出す
		newPlayer->OnCollision();
		//敵の衝突時コールバックを呼び出す
		enemy_->OnCollision();
	}

#pragma endregion

#pragma region
	//距離を図るための変数
	double Dis;

	//半径
	float Rad;

	//自キャラの座標
	posA = newPlayer->GetWorldPosition();

	//敵キャラの座標
	posC = enemy_->GetSensorPosition();

	//座標AとBの距離を求める
	Dis = CalculateDistance(posA, posC);

	Rad = newPlayer->GetRadius() + enemy_->GetSensorRadius();
	Dis = (posA.x - posC.x) * (posA.x - posC.x) + (posA.y - posC.y) * (posA.y - posC.y) + (posA.z - posC.z) * (posA.z - posC.z);
	//センサーとプレイヤーの当たり判定
	if (Dis <= Rad * Rad) {
		//マップの衝突時コールバックを呼び出す
		enemy_->SensorCollision();
	}
	else {
		enemy_->OffCollision();
	}

#pragma endregion
}

void GameScene::SceneChange() {
	isEnd_ = false;
	if (newMap->GetGoalFlag() == true)
	{
		nextScene_ = Scene::END;
		isEnd_ = true;
	}


	if (newPlayer->GetDeathFlag() == true)
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
	newUI->Reset();
	PosReset();
}

void GameScene::PosReset()
{
	//敵の初期化
	//falseが縦　trueが横
	newEnemy->Initialize(enemyModel_, enemySensorModel_, newCamera.get(), enemyPos, false);

	newEnemy_1->Initialize(enemyModel_, enemySensorModel_, newCamera.get(), enemyPos_1, true);

	newEnemy_2->Initialize(enemyModel_, enemySensorModel_, newCamera.get(), enemyPos_2, true);

	newEnemy_3->Initialize(enemyModel_, enemySensorModel_, newCamera.get(), enemyPos_3, false);
}

void GameScene::SoundStop()
{
	if (audio_->IsPlaying(playBGMHandle) == true)
	{
		audio_->StopWave(playBGMHandle);
	}
}