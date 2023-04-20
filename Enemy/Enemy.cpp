#include "Enemy.h"
#include <cassert>

//������
void Enemy::Initialize(Model* model, Model* sensormodel, RailCamera* camera, Vector3 enemyPos, bool WidthHeightFlag) {
	//NULL�|�C���g�`�F�b�N
	assert(model);
	assert(sensormodel);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL������
	model_ = model;
	sensormodel_ = sensormodel;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	primitive_ = PrimitiveDrawer::GetInstance();

	primitive_->SetViewProjection(&camera->GetViewProjection());

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	sensorTransform_.Initialize();

	//�G�̑傫��(�{��)
	worldTransform_.scale_ = { 0.5f,0.5f,0.5f };
	//�G�̏����ʒu�̐ݒ�
	worldTransform_.translation_ = enemyPos;

	//�G�Z���T�[�̑傫��(�{��)
	sensorTransform_.scale_ = { 2.0f,2.0f,2.0f };
	//�Z���T�[�̏����ʒu�̐ݒ�
	sensorTransform_.translation_ = { enemyPos.x, 1000.0f, enemyPos.z };


	sensorX = worldTransform_.translation_.x;
	sensorZ = worldTransform_.translation_.z;

	HeightSensorVisionX[0] = 5;
	HeightSensorVisionZ[0] = 10;
	HeightSensorVisionX[1] = -5;
	HeightSensorVisionZ[1] = 10;

	WidthSensorVisionX[0] = 10;
	WidthSensorVisionZ[0] = 5;
	WidthSensorVisionX[1] = -10;
	WidthSensorVisionZ[1] = 5;

	speed = 0.2f;

	RightMoveFlag = false;
	LeftMoveFlag = false;
	UpMoveFlag = false;
	DownMoveFlag = false;

	//���E�ړ��̋L�^�p
	for (int i = 0; i < 2; i++) {
		visionMemoryX[i] = 0;
		visionMemoryZ[i] = 0;
	}
	//���E�̈ړ��𐧌䂷��t���O
	visionFlag = 0;
	//���E�̓����蔻��p�t���O
	visionHitFlag[2] = {};
	//���E���Ăѓ����܂ł̃^�C�}�[
	visionTimer = 15.0f;
	sensorMovedDis = 0.0f;

	WidthHeightFlag_ = WidthHeightFlag;

	if (WidthHeightFlag_ == false) {
		RightMoveFlag = true;
	}
	else if (WidthHeightFlag_ == true) {
		UpMoveFlag = true;
	}
}

//�X�V
void Enemy::Update(bool keyFlag, Player* player, float moveDis) {


	//�Z���T�[��G�ɒǏ]
	sensorX = worldTransform_.translation_.x;
	sensorZ = worldTransform_.translation_.z;

	sensorTransform_.translation_.x = worldTransform_.translation_.x;
	sensorTransform_.translation_.z = worldTransform_.translation_.z;

	SensorVision();

	//UpSensorVector(player->GetWorldPosition().z, player->GetWorldPosition().x, player->GetRadius());
	//DownSensorVector(player->GetWorldPosition().z, player->GetWorldPosition().x, player->GetRadius());

	sensorTransform_.ColorSetter(sensorColor);

	if (WidthHeightFlag_ == false) {
		if (RightMoveFlag == true) {
			rightFlag = VectorLineCollision(player->GetWorldPosition(), player->GetRadius(), RightStart, RightEnd, RightEnd1);
		}
		if (LeftMoveFlag == true) {
			leftFlag = VectorLineCollision(player->GetWorldPosition(), player->GetRadius(), LeftStart, LeftEnd, LeftEnd1);
		}
	}

	if (WidthHeightFlag_ == true) {
		if (UpMoveFlag == true) {
			upFlag = VectorLineCollision(player->GetWorldPosition(), player->GetRadius(), UpStart, UpEnd, UpEnd1);
		}
		if (DownMoveFlag == true) {
			downFlag = VectorLineCollision(player->GetWorldPosition(), player->GetRadius(), DownStart, DownEnd, DownEnd1);
		}
	}

	//�v���C���[�̈ړ��x�N�g��
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

		EnemyMove(moveDis, WidthHeightFlag_);

		EnemyMoveCheck(player->GetWorldPosition().x, player->GetWorldPosition().z, player->GetRadius());
	}



	//�s��̍X�V
	myFunc_.UpdateWorldTransform(worldTransform_);
	myFunc_.UpdateWorldTransform(sensorTransform_);
	sensorTransform_.TransferColorMatrix();
	worldTransform_.TransferColorMatrix();



	//�f�o�b�O�p�\��
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
	debugText_->Printf("upMove:%d downMove:%d rightMove:%d leftMove:%d", UpMoveFlag, DownMoveFlag, RightMoveFlag, LeftMoveFlag);
	debugText_->SetPos(50, 250);
	debugText_->Printf("isMove UP:0 DOWN:1 RIGHT:2 LEFT:3 %d", isMove);
	debugText_->SetPos(50, 540);
	debugText_->Printf("enemyPos X:%f Y:%f Z:%f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	debugText_->SetPos(50, 570);
	debugText_->Printf("sensorPos X:%f Y:%f Z:%f", sensorTransform_.translation_.x, sensorTransform_.translation_.y, sensorTransform_.translation_.z);
	debugText_->SetPos(600, 300);
	debugText_->Printf("move %f", sensorMovedDis);
}

//�`��
void Enemy::Draw(ViewProjection& viewprojection) {
	//3D���f����`��
	model_->Draw(worldTransform_, viewprojection);
}

//�Z���T�[�̕`��
void Enemy::SensorDraw(ViewProjection& viewProjection) {


	Vector4 color = { 1,0,0,1 };
	Vector4 color1 = { 1,1,1,1 };

	//sensormodel_->Draw(sensorTransform_, viewProjection);

#pragma region ��Z���T�[�̕`��

	if (UpMoveFlag == true) {
		if (upFlag == true) {
			UpStart = { sensorX , 0, sensorZ };
			UpEnd = { HeightSensorVisionX[0] + sensorX, 0, HeightSensorVisionZ[0] + sensorZ };
			UpEnd1 = { HeightSensorVisionX[1] + sensorX,0,HeightSensorVisionZ[1] + sensorZ };

			primitive_->DrawLine3d(UpStart, UpEnd, color);
			primitive_->DrawLine3d(UpEnd, UpEnd1, color);
			primitive_->DrawLine3d(UpEnd1, UpStart, color);

		}
		else {
			UpStart = { sensorX , 0, sensorZ };
			UpEnd = { HeightSensorVisionX[0] + sensorX, 0, HeightSensorVisionZ[0] + sensorZ };
			UpEnd1 = { HeightSensorVisionX[1] + sensorX,0,HeightSensorVisionZ[1] + sensorZ };

			primitive_->DrawLine3d(UpStart, UpEnd, color1);
			primitive_->DrawLine3d(UpEnd, UpEnd1, color1);
			primitive_->DrawLine3d(UpEnd1, UpStart, color1);

		}
	}
#pragma endregion

#pragma region ���Z���T�[�̕`��

	if (DownMoveFlag == true) {
		if (downFlag == true) {
			DownStart = { sensorX , 0, sensorZ };
			DownEnd = { HeightSensorVisionX[1] + sensorX,0,-HeightSensorVisionZ[1] + sensorZ };
			DownEnd1 = { HeightSensorVisionX[0] + sensorX, 0, -HeightSensorVisionZ[0] + sensorZ };

			primitive_->DrawLine3d(DownStart, DownEnd, color);
			primitive_->DrawLine3d(DownEnd, DownEnd1, color);
			primitive_->DrawLine3d(DownEnd1, DownStart, color);

		}
		else {
			DownStart = { sensorX , 0, sensorZ };
			DownEnd = { HeightSensorVisionX[1] + sensorX,0,-HeightSensorVisionZ[1] + sensorZ };
			DownEnd1 = { HeightSensorVisionX[0] + sensorX, 0, -HeightSensorVisionZ[0] + sensorZ };

			primitive_->DrawLine3d(DownStart, DownEnd, color1);
			primitive_->DrawLine3d(DownEnd, DownEnd1, color1);
			primitive_->DrawLine3d(DownEnd1, DownStart, color1);

		}
	}

#pragma endregion

#pragma region �E�Z���T�[�̕`��

	if (RightMoveFlag == true) {
		if (rightFlag == true) {
			RightStart = { sensorX , 0, sensorZ };
			RightEnd = { WidthSensorVisionX[0] + sensorX,0,-WidthSensorVisionZ[0] + sensorZ };
			RightEnd1 = { WidthSensorVisionX[0] + sensorX, 0, WidthSensorVisionZ[0] + sensorZ };

			primitive_->DrawLine3d(RightStart, RightEnd, color);
			primitive_->DrawLine3d(RightEnd, RightEnd1, color);
			primitive_->DrawLine3d(RightEnd1, RightStart, color);

		}
		else {
			RightStart = { sensorX , 0, sensorZ };
			RightEnd = { WidthSensorVisionX[0] + sensorX,0,-WidthSensorVisionZ[0] + sensorZ };
			RightEnd1 = { WidthSensorVisionX[0] + sensorX, 0, WidthSensorVisionZ[0] + sensorZ };

			primitive_->DrawLine3d(RightStart, RightEnd, color1);
			primitive_->DrawLine3d(RightEnd, RightEnd1, color1);
			primitive_->DrawLine3d(RightEnd1, RightStart, color1);
		}
	}

#pragma endregion


#pragma region ���Z���T�[�̕`��

	if (LeftMoveFlag == true) {
		if (leftFlag == true) {
			LeftStart = { sensorX , 0, sensorZ };
			LeftEnd = { WidthSensorVisionX[1] + sensorX,0,WidthSensorVisionZ[1] + sensorZ };
			LeftEnd1 = { WidthSensorVisionX[1] + sensorX, 0, -WidthSensorVisionZ[1] + sensorZ };

			primitive_->DrawLine3d(LeftStart, LeftEnd, color);
			primitive_->DrawLine3d(LeftEnd, LeftEnd1, color);
			primitive_->DrawLine3d(LeftEnd1, LeftStart, color);

		}
		else {
			LeftStart = { sensorX , 0, sensorZ };
			LeftEnd = { WidthSensorVisionX[1] + sensorX,0,WidthSensorVisionZ[1] + sensorZ };
			LeftEnd1 = { WidthSensorVisionX[1] + sensorX, 0, -WidthSensorVisionZ[1] + sensorZ };

			primitive_->DrawLine3d(LeftStart, LeftEnd, color1);
			primitive_->DrawLine3d(LeftEnd, LeftEnd1, color1);
			primitive_->DrawLine3d(LeftEnd1, LeftStart, color1);

		}
	}

#pragma endregion

}

void Enemy::FlagReset(){

	//�����t���O
	invisibleFlag = false;

	stopFlag = false;
	stopIntervalFlag = false;

	stopTimer = 100;
	stopIntervalTimer = 100;

}

void Enemy::Reset() {

	//�����t���O
	invisibleFlag = false;

	stopFlag = false;
	stopIntervalFlag = false;

	stopTimer = 100;
	stopIntervalTimer = 100;
	sensorMovedDis = 0.0f;

}

bool Enemy::VectorLineCollision(Vector3 player, float playerRadius, Vector3 pt1, Vector3 pt2, Vector3 pt3) {
	//���ꂼ��̃x�N�g��
	Vector3 vec[3];	//��
	Vector3 vecPlayer[4];	//Player�܂ł̃x�N�g��


	//�E���C���̃x�N�g��
	vec[0].x = pt2.x - pt1.x;
	vec[0].y = 0;
	vec[0].z = pt2.z - pt1.z;
	vec[0] = Normalize(vec[0]);
	//�����C���̃x�N�g��
	vec[1].x = pt3.x - pt2.x;
	vec[1].y = 0;
	vec[1].z = pt3.z - pt2.z;
	vec[1] = Normalize(vec[1]);
	//�����C���̃x�N�g��
	vec[2].x = pt1.x - pt3.x;
	vec[2].y = 0;
	vec[2].z = pt1.z - pt3.z;
	vec[2] = Normalize(vec[2]);

	//�E���C���ƃv���C���[�̃x�N�g��
	vecPlayer[0].x = player.x - pt1.x;
	vecPlayer[0].y = 0;
	vecPlayer[0].z = player.z - pt1.z;
	vecPlayer[0] = Normalize(vecPlayer[0]);

	//�v���C���[�����C���̍����ɂ�����
	if (vec[0].x * vecPlayer[0].z - vec[0].z * vecPlayer[0].x > 0) {
		visionHitFlag[0] = true;
	}
	else {
		visionHitFlag[0] = false;
	}

	//�����C���ƃv���C���[�̃x�N�g��
	vecPlayer[1].x = player.x - pt2.x;
	vecPlayer[1].y = 0;
	vecPlayer[1].z = player.z - pt2.z;
	vecPlayer[1] = Normalize(vecPlayer[1]);

	//�v���C���[�����C���̍����ɂ�����
	if (vec[1].x * vecPlayer[1].z - vec[1].z * vecPlayer[1].x > 0) {
		visionHitFlag[1] = true;
	}
	else {
		visionHitFlag[1] = false;
	}

	//�����C���ƃv���C���[�̃x�N�g��
	vecPlayer[2].x = player.x - pt3.x;
	vecPlayer[2].y = 0;
	vecPlayer[2].z = player.z - pt3.z;
	vecPlayer[2] = Normalize(vecPlayer[2]);

	//�v���C���[�����C���̍����ɂ�����
	if (vec[2].x * vecPlayer[2].z - vec[2].z * vecPlayer[2].x > 0) {
		visionHitFlag[2] = true;
	}
	else {
		visionHitFlag[2] = false;
	}

	//�s������s��(�s���Ȃ�)
	if (visionHitFlag[0] == true && visionHitFlag[1] == true && visionHitFlag[2] == true) {
		return true;
	}
	else {
		return false;
	}



}

Vector3 Enemy::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return  worldPos;
	/*worldPos_2.x = worldTransform_[1].matWorld_.m[3][0];
	worldPos_2.y = worldTransform_[1].matWorld_.m[3][1];
	worldPos_2.z = worldTransform_[1].matWorld_.m[3][2];
	return  worldPos_2;*/

}

Vector3 Enemy::GetSensorPosition()
{
	//�Z���T�[�̍��W������
	Vector3 sensorPos;
	//
	sensorPos.x = sensorTransform_.matWorld_.m[3][0];
	sensorPos.y = sensorTransform_.matWorld_.m[3][1];
	sensorPos.z = sensorTransform_.matWorld_.m[3][2];

	return sensorPos;
}

void Enemy::OnCollision() {
	//�������Ȃ�
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
#pragma region ��]����

	if (visionFlag == 0)
	{
		//���W����]������
		for (int i = 0; i < 2; i++)
		{
			visionMemoryX[i] = HeightSensorVisionX[i];
			visionMemoryZ[i] = HeightSensorVisionX[i];
		}
	}
	else if (visionFlag == 1)
	{
		//���W����]������
		for (int i = 0; i < 2; i++)
		{
			visionMemoryX[i] = HeightSensorVisionX[i];
			visionMemoryZ[i] = HeightSensorVisionX[i];
		}
	}

#pragma endregion
	//�p�x����
	if (visionFlag == 0 && HeightSensorVisionX[0] <= 7.3 && HeightSensorVisionX[0] <= 7.2)
	{
		visionFlag = 3;
	}

	if (visionFlag == 1 && HeightSensorVisionX[1] >= -7.2 && HeightSensorVisionX[1] <= 7.3)
	{
		visionFlag = 4;
	}
	//----------------------------------
#pragma region ���E�������܂ł̃^�C�}�[�̏���
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
	//�ړ�������
	for (int i = 0; i < 2; i++)
	{
		HeightSensorVisionX[i] = visionMemoryX[i];
		HeightSensorVisionX[i] = visionMemoryZ[i];
	}
}

void Enemy::EnemyMove(float moveDis, bool WidthHeightFlag) {

	bool moveFlag = WidthHeightFlag;
	
	//�Z���T�[�Ō��m�����Ƃ�
	if (visionHitFlag[0] == 1 && visionHitFlag[1] == 1 && visionHitFlag[2] == 1) {
		// ��ړ�
		
		if (isMove == UP) {
			worldTransform_.translation_.z -= speed;
			crisisFlag = true;
		}
		// ���ړ�
		if (isMove == DOWN) {
			worldTransform_.translation_.z += speed;
			crisisFlag = true;
		}
		// �E�ړ�
		if (isMove == RIGHT) {
			worldTransform_.translation_.x += speed;
			crisisFlag = true;
		}
		// ���ړ�
		if (isMove == LEFT) {
			worldTransform_.translation_.x -= speed;
			crisisFlag = true;
		}
	}
	else {
		crisisFlag = false;
		if (WidthHeightFlag == false) {
			//�G�̈ړ�����(��)
			if (sensorMovedDis <= -moveDis) {
				LeftMoveFlag = false;
				RightMoveFlag = true;
			}
			if (sensorMovedDis >= moveDis) {
				RightMoveFlag = false;
				LeftMoveFlag = true;
			}


			if (RightMoveFlag == true) {
				worldTransform_.translation_.x += 0.1;
				sensorMovedDis += 0.1;
				worldTransform_.rotation_.y = RadianConversion(90);
			}
			if (LeftMoveFlag == true) {
				worldTransform_.translation_.x -= 0.1;
				sensorMovedDis -= 0.1;
				worldTransform_.rotation_.y = RadianConversion(270);
			}
		}
		if (WidthHeightFlag == true) {
			//�G�̈ړ�����(�c)
			if (sensorMovedDis >= moveDis) {
				UpMoveFlag = false;
				DownMoveFlag = true;
			}
			if (sensorMovedDis <= -moveDis) {
				DownMoveFlag = false;
				UpMoveFlag = true;
			}

			if (UpMoveFlag == true) {
				worldTransform_.translation_.z += 0.1;
				sensorMovedDis += 0.1;
				worldTransform_.rotation_.y = RadianConversion(0);
			}
			if (DownMoveFlag == true) {
				worldTransform_.translation_.z -= 0.1;
				sensorMovedDis -= 0.1;
				worldTransform_.rotation_.y = RadianConversion(180);
			}
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
		// searchFlag���Ă΂ꂽ�Ƃ���moveFlag��ύX����悤�ɂ���
		if (isSearch == TRUE) {
			EnemyMoveSearch(playerX, playerZ, playerR);
			// �J�E���g��߂�
			count = 1;
		}
	}
}

void Enemy::EnemyMoveSearch(float playerX, float playerZ, float playerR) {
	// �v���C���[���G�̉E�ɋ��鎞
	if (playerX > worldTransform_.translation_.x + radius) {
		isMove = RIGHT;
	}
	// �v���C���[���G�̍��ɋ��鎞
	if (playerX + playerR < worldTransform_.translation_.x) {
		isMove = LEFT;
	}
	// �v���C���[���G�̏�ɋ��鎞
	if (playerZ + playerR < worldTransform_.translation_.z) {
		isMove = UP;
	}
	// �v���C���[���G�̉��ɂ��鎞
	if (playerZ > worldTransform_.translation_.z + radius) {
		isMove = DOWN;
	}
}

