#include "Enemy.h"
#include <cassert>

//初期化
void Enemy::Initialize(Model* model, RailCamera* camera) {
	//NULLポイントチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記憶する
	model_ = model;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	primitive_ = PrimitiveDrawer::GetInstance();

	primitive_->SetViewProjection(&camera->GetViewProjection());

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//敵の大きさ(倍率)
	worldTransform_.scale_ = { 0.5f,0.5f,0.5f };
	//敵の初期位置の設定
	worldTransform_.translation_ = { -10.0f, 0.9f, 12.7f };

	sensorX = worldTransform_.translation_.x;
	sensorZ = worldTransform_.translation_.z;

	sensorVisionX[0] = 5;
	sensorVisionZ[0] = 10;

	sensorVisionX[1] = -5;
	sensorVisionZ[1] = 10;

	speed = 0.2f;

	//視界移動の記録用
	for (int i = 0; i < 2; i++) {
		visionMemoryX[i] = 0;
		visionMemoryZ[i] = 0;
	}
	//視界の移動を制御するフラグ
	visionFlag = 0;
	//視界の当たり判定用フラグ
	visionHitFlag[2] = {};
	//視界が再び動くまでのタイマー
	visionTimer = 15.0f;

	
}

//更新
void Enemy::Update(bool keyFlag, Player* player) {

	//センサーを敵に追従
	sensorX = worldTransform_.translation_.x;
	sensorZ = worldTransform_.translation_.z;

	SensorVision();

	SensorVector(player->GetWorldPosition().z, player->GetWorldPosition().x, player->GetRadius());

	//プレイヤーの移動ベクトル
	Vector3 move = { 0,0,0 };

	if (keyFlag == true)
	{
		if (stopFlag == false) {
			stopFlag = true;
		}
	}

	if (stopFlag == true) {
		stopTimer--;
		if (stopTimer < 0) {
			stopIntervalFlag = true;
		}
	}
	if (stopIntervalFlag == true) {
		stopIntervalTimer--;
		if (stopIntervalTimer <= 0.01f) {
			OKFlag = true;
		}
		if (stopIntervalTimer < 0) {
			OKFlag = false;
			stopFlag = false;
			stopIntervalFlag = false;
			stopTimer = 100;
			stopIntervalTimer = 100;
		}
	}

	prePosition_ = worldTransform_.translation_;

	if (stopFlag == false || stopIntervalFlag == true) {
		//if (worldTransform_.translation_.x <= -11)
		//{
		//	LightFlag = false;
		//	LeftFlag = true;
		//}
		//if (worldTransform_.translation_.x >= 6)
		//{
		//	LeftFlag = false;
		//	LightFlag = true;
		//}

		////敵の移動処理
		//if (worldTransform_.translation_.x <= -11)
		//{
		//	LightFlag = false;
		//	LeftFlag = true;
		//}
		//if (worldTransform_.translation_.x >= 6)
		//{
		//	LeftFlag = false;
		//	LightFlag = true;
		//}

		//if (LeftFlag == true)
		//{
		//	worldTransform_.translation_.x += 0.1;
		//}
		//if (LightFlag == true)
		//{
		//	worldTransform_.translation_.x -= 0.1;
		//}

		Move();

		EnemyMoveCheck(player->GetWorldPosition().x, player->GetWorldPosition().z, player->GetRadius());
	}



	//行列の更新
	myFunc_.UpdateWorldTransform(worldTransform_);
	worldTransform_.TransferColorMatrix();

	//デバッグ用表示
	/*debugText_->SetPos(50, 120);
	debugText_->Printf("stopFlag:%d", stopFlag);

	debugText_->SetPos(50, 210);
	debugText_->Printf("stopIntervalFlag:%d", stopIntervalFlag);

	debugText_->SetPos(50, 390);
	debugText_->Printf("stopTimer:%d", stopTimer);

	debugText_->SetPos(50, 410);
	debugText_->Printf("stopIntervalTimer:%d", stopIntervalTimer);*/

	debugText_->SetPos(50, 90);
	debugText_->Printf("visionHitFlag [0]:%d [1]:%d [2]:%d", visionHitFlag[0], visionHitFlag[1], visionHitFlag[2]);
	debugText_->SetPos(50, 250);
	debugText_->Printf("isMove UP:0 DOWN:1 RIGHT:2 LEFT:3 %d", isMove);
	debugText_->SetPos(50, 270);
	debugText_->Printf("enemyPos X:%f Y:%f Z:%f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

//描画
void Enemy::Draw(ViewProjection& viewprojection) {
	//3Dモデルを描画
	model_->Draw(worldTransform_, viewprojection);
}

//センサーの描画
void Enemy::SensorDraw() {

	Vector3 start = { 0,0,0 };
	Vector3 end = { 0,0,0 };
	Vector4 color = { 1,0,0,1 };
	Vector3 start1 = { sensorVisionX[0] + sensorX,0, sensorVisionZ[0] + sensorZ };
	Vector3 end1 = { sensorVisionX[0] + sensorX,0,sensorVisionZ[0] + sensorZ };
	Vector4 color1 = { 1,1,1,1 };

	if (visionHitFlag[0] == 1 && visionHitFlag[1] == 1 && visionHitFlag[2] == 1)
	{
		for (int i = 0; i < 2; i++)
		{
			start = { sensorX , 0, sensorZ };
			end = { sensorVisionX[i] + sensorX, 0, sensorVisionZ[i] + sensorZ };
			color = { 1,0,0,1 };

			primitive_->DrawLine3d(start, end, color);
		}
		primitive_->DrawLine3d(end, end1, color);

	}
	else
	{
		for (int i = 0; i < 2; i++)
		{
			start = { sensorX , 0, sensorZ };
			end = { sensorVisionX[i] + sensorX, 0, sensorVisionZ[i] + sensorZ };
			color1 = { 1,1,1,1 };

			primitive_->DrawLine3d(start, end, color1);
		}
		primitive_->DrawLine3d(end, end1, color1);
	}
}

void Enemy::FlagReset()
{
	LightFlag = false;
	LeftFlag = false;

	//透明フラグ
	invisibleFlag = false;

	stopFlag = false;
	stopIntervalFlag = false;

	stopTimer = 100;
	stopIntervalTimer = 100;
}

void Enemy::Reset()
{
	//worldTransform_.translation_ = { 6.0f, 0.9f, -2.7f };
	stopFlag = false;
	/*stopIntervalFlag = false;
	stopTimer = 100;
	stopIntervalTimer = 100;*/
}

//void Enemy::FlagReset()
//{
//	stopFlag = false;
//	stopIntervalFlag = false;
//	stopTimer = 100;
//	stopIntervalTimer = 100;
//}

Vector3 Enemy::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;

}

void Enemy::OnCollision() {
	//何もしない
}

void Enemy::SensorVision() {
#pragma region 回転処理
	//if (visionFlag == 0)
	//{
	//	//座標を回転させる
	//	for (int i = 0; i < 2; i++)
	//	{
	//		visionMemoryX[i] = sensorVisionX[i] * cos(PI / 180 * 1) - sensorVisionZ[i] * sin(PI / 90 * 1);
	//		visionMemoryZ[i] = sensorVisionX[i] * sin(PI / 90 * 1) + sensorVisionZ[i] * cos(PI / 180 * 1);
	//	}
	//}
	//else if (visionFlag == 1)
	//{
	//	//座標を回転させる
	//	for (int i = 0; i < 2; i++)
	//	{
	//		visionMemoryX[i] = sensorVisionX[i] * cos(PI / 180 * 1) - sensorVisionZ[i] * -sin(PI / 90 * 1);
	//		visionMemoryZ[i] = sensorVisionX[i] * -sin(PI / 90 * 1) + sensorVisionZ[i] * cos(PI / 180 * 1);
	//	}
	//}

	if (visionFlag == 0)
	{
		//座標を回転させる
		for (int i = 0; i < 2; i++)
		{
			visionMemoryX[i] = sensorVisionX[i] * cos(PI / 180 * 1);
			visionMemoryZ[i] = sensorVisionZ[i] * cos(PI / 180 * 1);
		}
	}
	else if (visionFlag == 1)
	{
		//座標を回転させる
		for (int i = 0; i < 2; i++)
		{
			visionMemoryX[i] = sensorVisionX[i] * cos(PI / 180 * 1);
			visionMemoryZ[i] = sensorVisionZ[i] * cos(PI / 180 * 1);
		}
	}

#pragma endregion
	//角度決め
	if (visionFlag == 0 && sensorVisionX[0] <= 7.3 && sensorVisionZ[0] <= 7.2)
	{
		visionFlag = 3;
	}

	if (visionFlag == 1 && sensorVisionX[1] >= -7.2 && sensorVisionZ[1] <= 7.3)
	{
		visionFlag = 4;
	}
	//----------------------------------
#pragma region 視界が動くまでのタイマーの処理
	if (visionFlag == 3)
	{
		visionTimer--;
		if (visionTimer < 0)
		{
			visionFlag = 1;
			visionTimer = 15.0f;
		}
	}
	if (visionFlag == 4)
	{
		visionTimer--;
		if (visionTimer < 0)
		{
			visionFlag = 0;
			visionTimer = 15.0f;
		}
	}
	if (visionFlag == 5)
	{
		visionTimer--;
		if (visionTimer < 0)
		{
			sensorVisionX[0] = 50;
			sensorVisionZ[0] = 160;

			sensorVisionX[1] = -50;
			sensorVisionZ[1] = 160;
			visionFlag = rand() % 2;
			visionTimer = 15.0f;
		}
	}
#pragma endregion
	//----------------------------------
	//移動させる
	for (int i = 0; i < 2; i++)
	{
		sensorVisionX[i] = visionMemoryX[i];
		sensorVisionZ[i] = visionMemoryZ[i];
	}
}

void Enemy::SensorVector(float playerZ, float playerX, float playerRadius) {
	//それぞれのベクトル
	Vector3 vec[3];	//線
	Vector3 vecPlayer[4];	//Playerまでのベクトル

#pragma region 視界のベクトル
	//左右ラインベクトル
	for (int i = 0; i < 2; i++)
	{
		vec[i].x = (sensorVisionX[i] + sensorX) - sensorX;
		vec[i].y = 0;
		vec[i].z = (sensorVisionZ[i] + sensorZ) - sensorZ;
		vec[i] = Normalize(vec[i]);
	}
	//最終点から最終点までのベクトル
	vec[2].x = (sensorVisionX[1] + sensorX) - (sensorVisionX[0] + sensorX);
	vec[2].y = 0;
	vec[2].z = (sensorVisionZ[1] + sensorZ) - (sensorVisionZ[0] + sensorZ);
	vec[2] = Normalize(vec[2]);
#pragma endregion
#pragma region 各視界の始まりからPlayerまでのベクトル
	//右ラインからPlayerのベクトル
	vecPlayer[0].x = (playerX - playerRadius) - sensorX;
	vecPlayer[0].y = 0;
	vecPlayer[0].z = playerZ - sensorZ;
	vecPlayer[0] = Normalize(vecPlayer[0]);
	//左ラインからPlayerのベクトル
	vecPlayer[1].x = (playerX + playerRadius) - sensorX;
	vecPlayer[1].y = 0;
	vecPlayer[1].z = playerZ - sensorZ;
	vecPlayer[1] = Normalize(vecPlayer[1]);
	//限界点[0]からPlayerのベクトル
	vecPlayer[2].x = playerX - (sensorVisionX[0] + sensorX);
	vecPlayer[2].y = 0;
	vecPlayer[2].z = (playerZ - playerRadius) - (sensorVisionZ[0] + sensorZ);
	vecPlayer[2] = Normalize(vecPlayer[2]);
	//限界点[1]からPlayerのベクトル
	vecPlayer[3].x = playerX - (sensorVisionX[1] + sensorX);
	vecPlayer[3].y = 0;
	vecPlayer[3].z = (playerZ - playerRadius) - (sensorVisionZ[1] + sensorZ);
	vecPlayer[3] = Normalize(vecPlayer[3]);
#pragma endregion
	//当たり判定
	//右ラインベクトルの判定
	if (vec[0].x * vecPlayer[0].z - vec[0].z * vecPlayer[0].x > 0)
	{
		visionHitFlag[0] = 1;
	}
	else
	{
		visionHitFlag[0] = 0;
	}
	//左ラインベクトルの判定
	if (vec[1].x * vecPlayer[1].z - vec[1].z * vecPlayer[1].x < 0)
	{
		visionHitFlag[1] = 1;
	}
	else
	{
		visionHitFlag[1] = 0;
	}
	//視界限界点の判定
	if (vec[2].x * vecPlayer[2].z - vec[2].z * vecPlayer[2].x > 0)
	{
		visionHitFlag[2] = 1;
	}
	else
	{
		visionHitFlag[2] = 0;

	}
#pragma region 視界に入った時の処理
	if (visionHitFlag[0] == 1 && visionHitFlag[1] == 1 && visionHitFlag[2] == 1)
	{
		visionFlag = 5;
		if (visionFlag == 5 && vecPlayer[2].x != vecPlayer[3].x)
		{
			if (vecPlayer[3].x - 0.001 >= -vecPlayer[2].x)
			{
				//座標を回転させる
				for (int i = 0; i < 2; i++)
				{
					visionMemoryX[i] = sensorVisionX[i] * cos(PI / 180 * 1) - sensorVisionZ[i] * -sin(PI / 180 * 1);
					visionMemoryZ[i] = sensorVisionX[i] * -sin(PI / 180 * 1) + sensorVisionZ[i] * cos(PI / 180 * 1);
				}
			}
			else if (-vecPlayer[2].x - 0.001 >= vecPlayer[3].x)
			{
				//座標を回転させる
				for (int i = 0; i < 2; i++)
				{
					visionMemoryX[i] = sensorVisionX[i] * cos(PI / 180 * 1) - sensorVisionZ[i] * sin(PI / 180 * 1);
					visionMemoryZ[i] = sensorVisionX[i] * sin(PI / 180 * 1) + sensorVisionZ[i] * cos(PI / 180 * 1);
				}
			}
		}

	}
	debugText_->SetPos(50, 120);
	debugText_->Printf("vec [x]:%f [y]:%f [z]:%f", vecPlayer[0].x, vecPlayer[0].y, vecPlayer[0].z);
#pragma endregion

}

void Enemy::Move() {
	// 上移動
	
	if (visionHitFlag[0] == 1 && visionHitFlag[1] == 1 && visionHitFlag[2] == 1) {
		if (isMove == UP)
		{
			worldTransform_.translation_.z -= speed;
		}
		// 下移動
		if (isMove == DOWN)
		{
			worldTransform_.translation_.z += speed;
		}
		// 右移動
		if (isMove == RIGHT)
		{
			worldTransform_.translation_.x += speed;
		}
		// 左移動
		if (isMove == LEFT)
		{
			worldTransform_.translation_.x -= speed;
		}
	}
	else {
		if (worldTransform_.translation_.x <= -11)
		{
			LightFlag = false;
			LeftFlag = true;
		}
		if (worldTransform_.translation_.x >= 6)
		{
			LeftFlag = false;
			LightFlag = true;
		}

		//敵の移動処理
		if (worldTransform_.translation_.x <= -11)
		{
			LightFlag = false;
			LeftFlag = true;
		}
		if (worldTransform_.translation_.x >= 6)
		{
			LeftFlag = false;
			LightFlag = true;
		}

		if (LeftFlag == true)
		{
			worldTransform_.translation_.x += 0.1;
		}
		if (LightFlag == true)
		{
			worldTransform_.translation_.x -= 0.1;
		}
	}

	

}

void Enemy::EnemyMoveCheck(float playerX, float playerZ, float playerR) {
	count--;
	if (count > 0) {
		isSearch = 0;
	}
	if (count < 0) {
		isSearch = TRUE;
		// searchFlagが呼ばれたときにmoveFlagを変更するようにする
		if (isSearch == TRUE) {
			EnemyMoveSearch(playerX, playerZ, playerR);
			// カウントを戻す
			count = 20;
		}
	}
}

void Enemy::EnemyMoveSearch(float playerX, float playerZ, float playerR) {
	
	// プレイヤーが敵の右に居る時
	if (playerX > worldTransform_.translation_.x + radius) {
		isMove = RIGHT;
	}
	// プレイヤーが敵の左に居る時
	if (playerX + playerR < worldTransform_.translation_.x) {
		isMove = LEFT;
	}
	// プレイヤーが敵の上に居る時
	if (playerZ + playerR < worldTransform_.translation_.z) {
		isMove = UP;
	}
	// プレイヤーが敵の下にいる時
	if (playerZ > worldTransform_.translation_.z + radius) {
		isMove = DOWN;
	}
}

