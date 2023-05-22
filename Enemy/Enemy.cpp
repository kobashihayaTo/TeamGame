#include "Enemy.h"
#include <cassert>

//初期化
void Enemy::Initialize(Model* model, Model* sensormodel, RailCamera* camera, Vector3 enemyPos, bool WidthHeightFlag) {
	//NULLポイントチェック
	assert(model);
	assert(sensormodel);

	//引数として受け取ったデータをメンバ変数に記憶する
	model_ = model;
	sensorModel_ = sensormodel;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	primitive_ = PrimitiveDrawer::GetInstance();

	primitive_->SetViewProjection(&camera->GetViewProjection());

	//ワールド変換の初期化
	worldTransform_.Initialize();
	sensorTransform_.Initialize();

	//敵の大きさ(倍率)
	worldTransform_.scale_ = { 0.5f,0.5f,0.5f };
	//敵の初期位置の設定
	worldTransform_.translation_ = enemyPos;

	//敵センサーの大きさ(倍率)
	sensorTransform_.scale_ = { 2.0f,2.0f,2.0f };
	//センサーの初期位置の設定
	sensorTransform_.translation_ = { enemyPos.x, 1000.0f, enemyPos.z };


	sensorX = worldTransform_.translation_.x;
	sensorZ = worldTransform_.translation_.z;

	HeightSensorVisionX[0] = 3;
	HeightSensorVisionZ[0] = 10;
	HeightSensorVisionX[1] = -3;
	HeightSensorVisionZ[1] = 10;

	WidthSensorVisionX[0] = 10;
	WidthSensorVisionZ[0] = 3;
	WidthSensorVisionX[1] = -10;
	WidthSensorVisionZ[1] = 3;

	speed = 0.2f;

	rightMoveFlag = false;
	leftMoveFlag = false;
	upMoveFlag = false;
	downMoveFlag = false;

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
	sensorMovedDis = 0.0f;

	WidthHeightFlag_ = WidthHeightFlag;

	if (WidthHeightFlag_ == false) {
		rightMoveFlag = true;
	}
	else if (WidthHeightFlag_ == true) {
		upMoveFlag = true;
	}
}

//更新
void Enemy::Update(bool keyFlag, Player* player, float moveDis) {


	//センサーを敵に追従
	sensorX = worldTransform_.translation_.x;
	sensorZ = worldTransform_.translation_.z;

	sensorTransform_.translation_.x = worldTransform_.translation_.x;
	sensorTransform_.translation_.z = worldTransform_.translation_.z;

	SensorVision();

	//UpSensorVector(player->GetWorldPosition().z, player->GetWorldPosition().x, player->GetRadius());
	//DownSensorVector(player->GetWorldPosition().z, player->GetWorldPosition().x, player->GetRadius());

	sensorTransform_.ColorSetter(SensorColor);

	if (player->GetSecretFlag() == false) {
		if (WidthHeightFlag_ == false) {
			if (rightMoveFlag == true) {
				rightFlag = VectorLineCollision(player->GetWorldPosition(), player->GetRadius(), RightStart_Pt1, RightEnd_Pt2, RightEnd_Pt3, player);
			}
			if (leftMoveFlag == true) {
				leftFlag = VectorLineCollision(player->GetWorldPosition(), player->GetRadius(), LeftStart_Pt1, LeftEnd_Pt2, LeftEnd_Pt3, player);
			}
		}

		if (WidthHeightFlag_ == true) {
			if (upMoveFlag == true) {
				upFlag = VectorLineCollision(player->GetWorldPosition(), player->GetRadius(), UpStart_Pt1, UpEnd_Pt2, UpEnd_Pt3, player);
			}
			if (downMoveFlag == true) {
				downFlag = VectorLineCollision(player->GetWorldPosition(), player->GetRadius(), DownStart_Pt1, DownEnd_Pt2, DownEnd_Pt3, player);
			}
		}
	}

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

	PrePosition_ = worldTransform_.translation_;

	if (stopFlag == false || stopIntervalFlag == true) {

		EnemyMove(moveDis, WidthHeightFlag_);

		EnemyMoveCheck(player->GetWorldPosition().x, player->GetWorldPosition().z, player->GetRadius(), WidthHeightFlag_);
	}

	//行列の更新
	myFunc_.UpdateWorldTransform(worldTransform_);
	myFunc_.UpdateWorldTransform(sensorTransform_);
	sensorTransform_.TransferColorMatrix();
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
	debugText_->Printf("up:%d down:%d right:%d left:%d", upFlag, downFlag, rightFlag, leftFlag);
	debugText_->SetPos(50, 120);
	debugText_->Printf("upMove:%d downMove:%d rightMove:%d leftMove:%d", upMoveFlag, downMoveFlag, rightMoveFlag, leftMoveFlag);
	debugText_->SetPos(50, 250);
	debugText_->Printf("isMove UP:0 DOWN:1 RIGHT:2 LEFT:3 %d", isMove_Height);
	debugText_->SetPos(50, 540);
	debugText_->Printf("enemyPos X:%f Y:%f Z:%f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	debugText_->SetPos(50, 570);
	debugText_->Printf("sensorPos X:%f Y:%f Z:%f", sensorTransform_.translation_.x, sensorTransform_.translation_.y, sensorTransform_.translation_.z);
	debugText_->SetPos(600, 300);
	debugText_->Printf("move %f", sensorMovedDis);
}

//描画
void Enemy::Draw(ViewProjection& viewprojection) {
	//3Dモデルを描画
	model_->Draw(worldTransform_, viewprojection);
}

//センサーの描画
void Enemy::SensorDraw(ViewProjection& viewProjection) {


	Vector4 color = { 1,0,0,1 };
	Vector4 color1 = { 1,1,1,1 };

	//sensormodel_->Draw(sensorTransform_, viewProjection);

#pragma region 上センサーの描画

	if (upMoveFlag == true) {
		if (upFlag == true) {
			UpStart_Pt1 = { sensorX , 0, sensorZ };
			UpEnd_Pt2 = { HeightSensorVisionX[0] + sensorX, 0, HeightSensorVisionZ[0] + sensorZ };
			UpEnd_Pt3 = { HeightSensorVisionX[1] + sensorX,0,HeightSensorVisionZ[1] + sensorZ };

			primitive_->DrawLine3d(UpStart_Pt1, UpEnd_Pt2, color);
			primitive_->DrawLine3d(UpEnd_Pt2, UpEnd_Pt3, color);
			primitive_->DrawLine3d(UpEnd_Pt3, UpStart_Pt1, color);

		}
		else {
			UpStart_Pt1 = { sensorX , 0, sensorZ };
			UpEnd_Pt2 = { HeightSensorVisionX[0] + sensorX, 0, HeightSensorVisionZ[0] + sensorZ };
			UpEnd_Pt3 = { HeightSensorVisionX[1] + sensorX,0,HeightSensorVisionZ[1] + sensorZ };

			primitive_->DrawLine3d(UpStart_Pt1, UpEnd_Pt2, color1);
			primitive_->DrawLine3d(UpEnd_Pt2, UpEnd_Pt3, color1);
			primitive_->DrawLine3d(UpEnd_Pt3, UpStart_Pt1, color1);

		}
	}
#pragma endregion

#pragma region 下センサーの描画

	if (downMoveFlag == true) {
		if (downFlag == true) {
			DownStart_Pt1 = { sensorX , 0, sensorZ };
			DownEnd_Pt2 = { HeightSensorVisionX[1] + sensorX,0,-HeightSensorVisionZ[1] + sensorZ };
			DownEnd_Pt3 = { HeightSensorVisionX[0] + sensorX, 0, -HeightSensorVisionZ[0] + sensorZ };

			primitive_->DrawLine3d(DownStart_Pt1, DownEnd_Pt2, color);
			primitive_->DrawLine3d(DownEnd_Pt2, DownEnd_Pt3, color);
			primitive_->DrawLine3d(DownEnd_Pt3, DownStart_Pt1, color);

		}
		else {
			DownStart_Pt1 = { sensorX , 0, sensorZ };
			DownEnd_Pt2 = { HeightSensorVisionX[1] + sensorX,0,-HeightSensorVisionZ[1] + sensorZ };
			DownEnd_Pt3 = { HeightSensorVisionX[0] + sensorX, 0, -HeightSensorVisionZ[0] + sensorZ };

			primitive_->DrawLine3d(DownStart_Pt1, DownEnd_Pt2, color1);
			primitive_->DrawLine3d(DownEnd_Pt2, DownEnd_Pt3, color1);
			primitive_->DrawLine3d(DownEnd_Pt3, DownStart_Pt1, color1);

		}
	}

#pragma endregion

#pragma region 右センサーの描画

	if (rightMoveFlag == true) {
		if (rightFlag == true) {
			RightStart_Pt1 = { sensorX , 0, sensorZ };
			RightEnd_Pt2 = { WidthSensorVisionX[0] + sensorX,0,-WidthSensorVisionZ[0] + sensorZ };
			RightEnd_Pt3 = { WidthSensorVisionX[0] + sensorX, 0, WidthSensorVisionZ[0] + sensorZ };

			primitive_->DrawLine3d(RightStart_Pt1, RightEnd_Pt2, color);
			primitive_->DrawLine3d(RightEnd_Pt2, RightEnd_Pt3, color);
			primitive_->DrawLine3d(RightEnd_Pt3, RightStart_Pt1, color);

		}
		else {
			RightStart_Pt1 = { sensorX , 0, sensorZ };
			RightEnd_Pt2 = { WidthSensorVisionX[0] + sensorX,0,-WidthSensorVisionZ[0] + sensorZ };
			RightEnd_Pt3 = { WidthSensorVisionX[0] + sensorX, 0, WidthSensorVisionZ[0] + sensorZ };

			primitive_->DrawLine3d(RightStart_Pt1, RightEnd_Pt2, color1);
			primitive_->DrawLine3d(RightEnd_Pt2, RightEnd_Pt3, color1);
			primitive_->DrawLine3d(RightEnd_Pt3, RightStart_Pt1, color1);
		}
	}

#pragma endregion


#pragma region 左センサーの描画

	if (leftMoveFlag == true) {
		if (leftFlag == true) {
			LeftStart_Pt1 = { sensorX , 0, sensorZ };
			LeftEnd_Pt2 = { WidthSensorVisionX[1] + sensorX,0,WidthSensorVisionZ[1] + sensorZ };
			LeftEnd_Pt3 = { WidthSensorVisionX[1] + sensorX, 0, -WidthSensorVisionZ[1] + sensorZ };

			primitive_->DrawLine3d(LeftStart_Pt1, LeftEnd_Pt2, color);
			primitive_->DrawLine3d(LeftEnd_Pt2, LeftEnd_Pt3, color);
			primitive_->DrawLine3d(LeftEnd_Pt3, LeftStart_Pt1, color);

		}
		else {
			LeftStart_Pt1 = { sensorX , 0, sensorZ };
			LeftEnd_Pt2 = { WidthSensorVisionX[1] + sensorX,0,WidthSensorVisionZ[1] + sensorZ };
			LeftEnd_Pt3 = { WidthSensorVisionX[1] + sensorX, 0, -WidthSensorVisionZ[1] + sensorZ };

			primitive_->DrawLine3d(LeftStart_Pt1, LeftEnd_Pt2, color1);
			primitive_->DrawLine3d(LeftEnd_Pt2, LeftEnd_Pt3, color1);
			primitive_->DrawLine3d(LeftEnd_Pt3, LeftStart_Pt1, color1);

		}
	}

#pragma endregion

}

void Enemy::Reset() {

	//透明フラグ
	invisibleFlag = false;

	stopFlag = false;
	stopIntervalFlag = false;

	stopTimer = 100;
	stopIntervalTimer = 100;
	sensorMovedDis = 0.0f;


	OKFlag = false;
	crisisFlag = false;
}

bool Enemy::VectorLineCollision(Vector3 player, float playerRadius, Vector3 pt1, Vector3 pt2, Vector3 pt3, Player* player_) {
	//それぞれのベクトル
	Vector3 vec[3];	//線
	Vector3 vecPlayer[4];	//Playerまでのベクトル


	//右ラインのベクトル
	vec[0].x = pt2.x - pt1.x;
	vec[0].y = 0;
	vec[0].z = pt2.z - pt1.z;
	vec[0] = Normalize(vec[0]);
	//横ラインのベクトル
	vec[1].x = pt3.x - pt2.x;
	vec[1].y = 0;
	vec[1].z = pt3.z - pt2.z;
	vec[1] = Normalize(vec[1]);
	//左ラインのベクトル
	vec[2].x = pt1.x - pt3.x;
	vec[2].y = 0;
	vec[2].z = pt1.z - pt3.z;
	vec[2] = Normalize(vec[2]);

	//右ラインとプレイヤーのベクトル
	vecPlayer[0].x = player.x - pt1.x;
	vecPlayer[0].y = 0;
	vecPlayer[0].z = player.z - pt1.z;
	vecPlayer[0] = Normalize(vecPlayer[0]);

	//プレイヤーがラインの左側にいたら

	if (vec[0].x * vecPlayer[0].z - vec[0].z * vecPlayer[0].x > 0) {
		if (player_->GetSecretFlag() == true) {
			visionHitFlag[0] = false;
		}
		visionHitFlag[0] = true;
	}
	else {
		visionHitFlag[0] = false;
	}

	//横ラインとプレイヤーのベクトル
	vecPlayer[1].x = player.x - pt2.x;
	vecPlayer[1].y = 0;
	vecPlayer[1].z = player.z - pt2.z;
	vecPlayer[1] = Normalize(vecPlayer[1]);

	//プレイヤーがラインの左側にいたら
	if (vec[1].x * vecPlayer[1].z - vec[1].z * vecPlayer[1].x > 0) {
		if (player_->GetSecretFlag() == true) {
			visionHitFlag[1] = false;
		}
		visionHitFlag[1] = true;
	}
	else {
		visionHitFlag[1] = false;
	}

	//左ラインとプレイヤーのベクトル
	vecPlayer[2].x = player.x - pt3.x;
	vecPlayer[2].y = 0;
	vecPlayer[2].z = player.z - pt3.z;
	vecPlayer[2] = Normalize(vecPlayer[2]);

	//プレイヤーがラインの左側にいたら
	if (vec[2].x * vecPlayer[2].z - vec[2].z * vecPlayer[2].x > 0) {
		if (player_->GetSecretFlag() == true) {
			visionHitFlag[2] = false;
		}
		visionHitFlag[2] = true;
	}
	else {
		visionHitFlag[2] = false;
	}

	//行けたら行く(行かない)

	if (visionHitFlag[0] == true && visionHitFlag[1] == true && visionHitFlag[2] == true) {
		if (player_->GetSecretFlag() == true || player_->GetSecretIntervalFlag() == true) {
			return false;
		}
		else
		{
			return true;
		}
	}
	else {
		return false;
	}



}

Vector3 Enemy::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return  worldPos;
}

Vector3 Enemy::GetSensorPosition()
{
	//センサーの座標を入れる
	Vector3 sensorPos;
	//
	sensorPos.x = sensorTransform_.matWorld_.m[3][0];
	sensorPos.y = sensorTransform_.matWorld_.m[3][1];
	sensorPos.z = sensorTransform_.matWorld_.m[3][2];

	return sensorPos;
}

void Enemy::OnCollision() {
	//何もしない
}

void Enemy::OffCollision()
{
	visionHitFlag[0] = 0;
	visionHitFlag[1] = 0;
	visionHitFlag[2] = 0;
}

void Enemy::SensorCollision()
{
	visionHitFlag[0] = 1;
	visionHitFlag[1] = 1;
	visionHitFlag[2] = 1;
	debugText_->SetPos(50, 150);
	debugText_->Printf("fuck");
}

void Enemy::SensorVision() {
#pragma region 回転処理

	if (visionFlag == 0)
	{
		//座標を回転させる
		for (int i = 0; i < 2; i++)
		{
			visionMemoryX[i] = HeightSensorVisionX[i];
			visionMemoryZ[i] = HeightSensorVisionX[i];
		}
	}
	else if (visionFlag == 1)
	{
		//座標を回転させる
		for (int i = 0; i < 2; i++)
		{
			visionMemoryX[i] = HeightSensorVisionX[i];
			visionMemoryZ[i] = HeightSensorVisionX[i];
		}
	}

#pragma endregion
	//角度決め
	if (visionFlag == 0 && HeightSensorVisionX[0] <= 7.3 && HeightSensorVisionX[0] <= 7.2)
	{
		visionFlag = 3;
	}

	if (visionFlag == 1 && HeightSensorVisionX[1] >= -7.2 && HeightSensorVisionX[1] <= 7.3)
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
			HeightSensorVisionX[0] = 50;
			HeightSensorVisionX[0] = 160;

			HeightSensorVisionX[1] = -50;
			HeightSensorVisionX[1] = 160;
			visionFlag = rand() % 2;
			visionTimer = 15.0f;
		}
	}
#pragma endregion
	//----------------------------------
	//移動させる
	for (int i = 0; i < 2; i++)
	{
		HeightSensorVisionX[i] = visionMemoryX[i];
		HeightSensorVisionX[i] = visionMemoryZ[i];
	}
}

void Enemy::EnemyMove(float moveDis, bool WidthHeightFlag) {

	bool moveFlag = WidthHeightFlag;

	//センサーで検知したとき
	if (visionHitFlag[0] == 1 && visionHitFlag[1] == 1 && visionHitFlag[2] == 1) {
		// 上移動
		if (isMove_Height == UP) {
			worldTransform_.translation_.z -= speed;
			crisisFlag = true;
		}
		// 下移動
		if (isMove_Height == DOWN) {
			worldTransform_.translation_.z += speed;
			crisisFlag = true;
		}
		// 右移動
		if (isMove_Height == RIGHT) {
			worldTransform_.translation_.x += speed;
			crisisFlag = true;
		}
		// 左移動
		if (isMove_Height == LEFT) {
			worldTransform_.translation_.x -= speed;
			crisisFlag = true;
		}
	}
	else {
		crisisFlag = false;
		if (WidthHeightFlag == false) {
			//敵の移動処理(横)
			if (sensorMovedDis <= -moveDis) {
				leftMoveFlag = false;
				rightMoveFlag = true;
			}
			if (sensorMovedDis >= moveDis) {
				rightMoveFlag = false;
				leftMoveFlag = true;
			}


			if (rightMoveFlag == true) {
				worldTransform_.translation_.x += 0.1;
				sensorMovedDis += 0.1;
				worldTransform_.rotation_.y = RadianConversion(90);
			}
			if (leftMoveFlag == true) {
				worldTransform_.translation_.x -= 0.1;
				sensorMovedDis -= 0.1;
				worldTransform_.rotation_.y = RadianConversion(270);
			}
		}
		if (WidthHeightFlag == true) {
			//敵の移動処理(縦)
			if (sensorMovedDis >= moveDis) {
				upMoveFlag = false;
				downMoveFlag = true;
			}
			if (sensorMovedDis <= -moveDis) {
				downMoveFlag = false;
				upMoveFlag = true;
			}

			if (upMoveFlag == true) {
				worldTransform_.translation_.z += 0.1;
				sensorMovedDis += 0.1;
				worldTransform_.rotation_.y = RadianConversion(0);
			}
			if (downMoveFlag == true) {
				worldTransform_.translation_.z -= 0.1;
				sensorMovedDis -= 0.1;
				worldTransform_.rotation_.y = RadianConversion(180);
			}
		}
	}



}

void Enemy::EnemyMoveCheck(float playerX, float playerZ, float playerR, bool WidthHeightFlag) {
	count--;
	if (count > 0) {
		isSearch = 0;
	}
	if (count < 0) {
		isSearch = TRUE;
		// searchFlagが呼ばれたときにmoveFlagを変更するようにする
		if (isSearch == TRUE) {
			EnemyMoveSearch(playerX, playerZ, playerR, WidthHeightFlag);
			// カウントを戻す
			count = 1;
		}
	}
}

void Enemy::EnemyMoveSearch(float playerX, float playerZ, float playerR, bool WidthHeightFlag) {
	//敵が横移動の時、縦の追従強め
	if (WidthHeightFlag == false) {
		// プレイヤーが敵の右に居る時
		if (playerX > worldTransform_.translation_.x + radius) {
			isMove_Height = RIGHT;
		}
		// プレイヤーが敵の左に居る時
		if (playerX + playerR < worldTransform_.translation_.x) {
			isMove_Height = LEFT;
		}
		// プレイヤーが敵の上に居る時
		if (playerZ + playerR < worldTransform_.translation_.z) {
			isMove_Height = UP;
		}
		// プレイヤーが敵の下にいる時
		if (playerZ > worldTransform_.translation_.z + radius) {
			isMove_Height = DOWN;
		}
	}
	//敵が縦移動の時、横の追従強め
	if (WidthHeightFlag == true) {
		// プレイヤーが敵の上に居る時
		if (playerZ + playerR < worldTransform_.translation_.z) {
			isMove_Height = UP;
		}
		// プレイヤーが敵の下にいる時
		if (playerZ > worldTransform_.translation_.z + radius) {
			isMove_Height = DOWN;
		}
		// プレイヤーが敵の右に居る時
		if (playerX > worldTransform_.translation_.x + radius) {
			isMove_Height = RIGHT;
		}
		// プレイヤーが敵の左に居る時
		if (playerX + playerR < worldTransform_.translation_.x) {
			isMove_Height = LEFT;
		}
	}
}

