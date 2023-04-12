#include "Enemy.h"
#include <cassert>

//������
void Enemy::Initialize(Model* model, Model* sensormodel, RailCamera* camera, Vector3 enemyPos) {
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
	sensorTransform_.translation_ = { enemyPos.x, enemyPos.y, enemyPos.z };


	sensorX = worldTransform_.translation_.x;
	sensorZ = worldTransform_.translation_.z;

	sensorVisionX[0] = 5;
	sensorVisionZ[0] = 10;

	sensorVisionX[1] = -5;
	sensorVisionZ[1] = 10;

	speed = 0.2f;

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

}

//�X�V
void Enemy::Update(bool keyFlag, Player* player, float moveDis, bool WidthHeightFlag) {


	//�Z���T�[��G�ɒǏ]
	sensorX = worldTransform_.translation_.x;
	sensorZ = worldTransform_.translation_.z;

	sensorTransform_.translation_.x = worldTransform_.translation_.x;
	sensorTransform_.translation_.z = worldTransform_.translation_.z;

	SensorVision();

	//UpSensorVector(player->GetWorldPosition().z, player->GetWorldPosition().x, player->GetRadius());

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

		EnemyMove(moveDis, WidthHeightFlag);

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
	debugText_->Printf("visionHitFlag [0]:%d [1]:%d [2]:%d", visionHitFlag[0], visionHitFlag[1], visionHitFlag[2]);
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

	/*Vector3 start = { 0,0,0 };
	Vector3 end = { 0,0,0 };
	Vector4 color = { 1,0,0,1 };
	Vector3 start1 = { sensorVisionX[0] + sensorX,0,sensorVisionZ[0] + sensorZ };
	Vector3 end1 = { sensorVisionX[0] + sensorX,0,sensorVisionZ[0] + sensorZ };
	Vector4 color1 = { 1,1,1,1 };*/

	sensormodel_->Draw(sensorTransform_, viewProjection);

	//#pragma region ��Z���T�[�̕`��
	//	if (visionHitFlag[0] == 1 && visionHitFlag[1] == 1 && visionHitFlag[2] == 1)
	//	{
	//		for (int i = 0; i < 2; i++)
	//		{
	//			start = { sensorX , 0, sensorZ };
	//			end = { sensorVisionX[i] + sensorX, 0, sensorVisionZ[i] + sensorZ };
	//			color = { 1,0,0,1 };
	//
	//			primitive_->DrawLine3d(start, end, color);
	//		}
	//		primitive_->DrawLine3d(end, end1, color);
	//
	//	}
	//	else
	//	{
	//		for (int i = 0; i < 2; i++)
	//		{
	//			start = { sensorX , 0, sensorZ };
	//			end = { sensorVisionX[i] + sensorX, 0, sensorVisionZ[i] + sensorZ };
	//			color1 = { 1,1,1,1 };
	//
	//			primitive_->DrawLine3d(start, end, color1);
	//		}
	//		primitive_->DrawLine3d(end, end1, color1);
	//	}
	//#pragma endregion

	//#pragma region ���Z���T�[�̕`��
	//	if (visionHitFlag[0] == 1 && visionHitFlag[1] == 1 && visionHitFlag[2] == 1)
	//	{
	//		for (int i = 0; i < 2; i++)
	//		{
	//			start = { sensorX , 0, sensorZ };
	//			end = { -sensorVisionX[i] - sensorX, 0, sensorVisionZ[i] + sensorZ };
	//			color = { 1,0,0,1 };
	//
	//			primitive_->DrawLine3d(start, end, color);
	//		}
	//		primitive_->DrawLine3d(end, end1, color);
	//
	//	}
	//	else
	//	{
	//		for (int i = 0; i < 2; i++)
	//		{
	//			start = { sensorX , 0, sensorZ };
	//			end = { sensorVisionX[i] + sensorX, 0, -sensorVisionZ[i] - sensorZ };
	//			color1 = { 1,1,1,1 };
	//
	//			primitive_->DrawLine3d(start, end, color1);
	//		}
	//		end1 = { sensorVisionX[0] + sensorX, 0, -sensorVisionZ[0] - sensorZ };
	//		primitive_->DrawLine3d(end, end1, color1);
	//	}
	//#pragma endregion

	//#pragma region �E�Z���T�[�̕`��
	//	if (visionHitFlag[0] == 1 && visionHitFlag[1] == 1 && visionHitFlag[2] == 1)
	//	{
	//		for (int i = 0; i < 2; i++)
	//		{
	//			start = { sensorX , 0, sensorZ };
	//			end = { sensorVisionX[i] + sensorX, 0, sensorVisionZ[i] + sensorZ };
	//			color = { 1,0,0,1 };
	//
	//			primitive_->DrawLine3d(start, end, color);
	//		}
	//		primitive_->DrawLine3d(end, end1, color);
	//
	//	}
	//	else
	//	{
	//		for (int i = 0; i < 2; i++)
	//		{
	//			start = { sensorX , 0, sensorZ };
	//			end = { sensorVisionX[i] + sensorX, 0, sensorVisionZ[i] + sensorZ };
	//			color1 = { 1,1,1,1 };
	//
	//			primitive_->DrawLine3d(start, end, color1);
	//		}
	//		primitive_->DrawLine3d(end, end1, color1);
	//	}
	//#pragma endregion
	//
	//#pragma region ���Z���T�[�̕`��
	//	if (visionHitFlag[0] == 1 && visionHitFlag[1] == 1 && visionHitFlag[2] == 1)
	//	{
	//		for (int i = 0; i < 2; i++)
	//		{
	//			start = { sensorX , 0, sensorZ };
	//			end = { sensorVisionX[i] + sensorX, 0, sensorVisionZ[i] + sensorZ };
	//			color = { 1,0,0,1 };
	//
	//			primitive_->DrawLine3d(start, end, color);
	//		}
	//		primitive_->DrawLine3d(end, end1, color);
	//
	//	}
	//	else
	//	{
	//		for (int i = 0; i < 2; i++)
	//		{
	//			start = { sensorX , 0, sensorZ };
	//			end = { sensorVisionX[i] + sensorX, 0, sensorVisionZ[i] + sensorZ };
	//			color1 = { 1,1,1,1 };
	//
	//			primitive_->DrawLine3d(start, end, color1);
	//		}
	//		primitive_->DrawLine3d(end, end1, color1);
	//	}
	//#pragma endregion
}

void Enemy::FlagReset()
{
	RightFlag = true;
	LeftFlag = false;


	//�����t���O
	invisibleFlag = false;

	stopFlag = false;
	stopIntervalFlag = false;

	stopTimer = 100;
	stopIntervalTimer = 100;

}

void Enemy::Reset() {

	RightFlag = true;
	LeftFlag = false;


	//�����t���O
	invisibleFlag = false;

	stopFlag = false;
	stopIntervalFlag = false;

	stopTimer = 100;
	stopIntervalTimer = 100;
	sensorMovedDis = 0.0f;

}

//void Enemy::FlagReset()
//{
//	stopFlag = false;
//	stopIntervalFlag = false;
//	stopTimer = 100;
//	stopIntervalTimer = 100;
//}

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
	//if (visionFlag == 0)
	//{
	//	//���W����]������
	//	for (int i = 0; i < 2; i++)
	//	{
	//		visionMemoryX[i] = sensorVisionX[i] * cos(PI / 180 * 1) - sensorVisionZ[i] * sin(PI / 90 * 1);
	//		visionMemoryZ[i] = sensorVisionX[i] * sin(PI / 90 * 1) + sensorVisionZ[i] * cos(PI / 180 * 1);
	//	}
	//}
	//else if (visionFlag == 1)
	//{
	//	//���W����]������
	//	for (int i = 0; i < 2; i++)
	//	{
	//		visionMemoryX[i] = sensorVisionX[i] * cos(PI / 180 * 1) - sensorVisionZ[i] * -sin(PI / 90 * 1);
	//		visionMemoryZ[i] = sensorVisionX[i] * -sin(PI / 90 * 1) + sensorVisionZ[i] * cos(PI / 180 * 1);
	//	}
	//}

	//if (visionFlag == 0)
	//{
	//	//���W����]������
	//	for (int i = 0; i < 2; i++)
	//	{
	//		visionMemoryX[i] = sensorVisionX[i] * cos(PI / 180 * 1);
	//		visionMemoryZ[i] = sensorVisionZ[i] * cos(PI / 180 * 1);
	//	}
	//}
	//else if (visionFlag == 1)
	//{
	//	//���W����]������
	//	for (int i = 0; i < 2; i++)
	//	{
	//		visionMemoryX[i] = sensorVisionX[i] * cos(PI / 180 * 1);
	//		visionMemoryZ[i] = sensorVisionZ[i] * cos(PI / 180 * 1);
	//	}
	//}

	if (visionFlag == 0)
	{
		//���W����]������
		for (int i = 0; i < 2; i++)
		{
			visionMemoryX[i] = sensorVisionX[i];
			visionMemoryZ[i] = sensorVisionZ[i];
		}
	}
	else if (visionFlag == 1)
	{
		//���W����]������
		for (int i = 0; i < 2; i++)
		{
			visionMemoryX[i] = sensorVisionX[i];
			visionMemoryZ[i] = sensorVisionZ[i];
		}
	}

#pragma endregion
	//�p�x����
	if (visionFlag == 0 && sensorVisionX[0] <= 7.3 && sensorVisionZ[0] <= 7.2)
	{
		visionFlag = 3;
	}

	if (visionFlag == 1 && sensorVisionX[1] >= -7.2 && sensorVisionZ[1] <= 7.3)
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
	//�ړ�������
	for (int i = 0; i < 2; i++)
	{
		sensorVisionX[i] = visionMemoryX[i];
		sensorVisionZ[i] = visionMemoryZ[i];
	}
}

void Enemy::UpSensorVector(float playerZ, float playerX, float playerRadius) {
	//���ꂼ��̃x�N�g��
	Vector3 vec[3];	//��
	Vector3 vecPlayer[4];	//Player�܂ł̃x�N�g��

#pragma region ���E�̃x�N�g��
	//���E���C���x�N�g��
	for (int i = 0; i < 2; i++)
	{
		vec[i].x = (sensorVisionX[i] + sensorX) - sensorX;
		vec[i].y = 0;
		vec[i].z = (sensorVisionZ[i] + sensorZ) - sensorZ;
		vec[i] = Normalize(vec[i]);
	}
	//�ŏI�_����ŏI�_�܂ł̃x�N�g��
	vec[2].x = (sensorVisionX[1] + sensorX) - (sensorVisionX[0] + sensorX);
	vec[2].y = 0;
	vec[2].z = (sensorVisionZ[1] + sensorZ) - (sensorVisionZ[0] + sensorZ);
	vec[2] = Normalize(vec[2]);
#pragma endregion
#pragma region �e���E�̎n�܂肩��Player�܂ł̃x�N�g��
	//�E���C������Player�̃x�N�g��
	vecPlayer[0].x = (playerX - playerRadius) - sensorX;
	vecPlayer[0].y = 0;
	vecPlayer[0].z = playerZ - sensorZ;
	vecPlayer[0] = Normalize(vecPlayer[0]);
	//�����C������Player�̃x�N�g��
	vecPlayer[1].x = (playerX + playerRadius) - sensorX;
	vecPlayer[1].y = 0;
	vecPlayer[1].z = playerZ - sensorZ;
	vecPlayer[1] = Normalize(vecPlayer[1]);
	//���E�_[0]����Player�̃x�N�g��
	vecPlayer[2].x = playerX - (sensorVisionX[0] + sensorX);
	vecPlayer[2].y = 0;
	vecPlayer[2].z = (playerZ - playerRadius) - (sensorVisionZ[0] + sensorZ);
	vecPlayer[2] = Normalize(vecPlayer[2]);
	//���E�_[1]����Player�̃x�N�g��
	vecPlayer[3].x = playerX - (sensorVisionX[1] + sensorX);
	vecPlayer[3].y = 0;
	vecPlayer[3].z = (playerZ - playerRadius) - (sensorVisionZ[1] + sensorZ);
	vecPlayer[3] = Normalize(vecPlayer[3]);


#pragma endregion
#pragma region	�����蔻��
	//�E���C���x�N�g���̔���
	if (vec[0].x * vecPlayer[0].z - vec[0].z * vecPlayer[0].x > 0)
	{
		visionHitFlag[0] = 1;
	}
	else
	{
		visionHitFlag[0] = 0;
	}
	//�����C���x�N�g���̔���
	if (vec[1].x * vecPlayer[1].z - vec[1].z * vecPlayer[1].x < 0)
	{
		visionHitFlag[1] = 1;
	}
	else
	{
		visionHitFlag[1] = 0;
	}
	//���E���E�_�̔���
	if (vec[2].x * vecPlayer[2].z - vec[2].z * vecPlayer[2].x > 0)
	{
		visionHitFlag[2] = 1;
	}
	else
	{
		visionHitFlag[2] = 0;

	}
#pragma endregion
#pragma region ���E�ɓ��������̏���
	if (visionHitFlag[0] == 1 && visionHitFlag[1] == 1 && visionHitFlag[2] == 1)
	{
		visionFlag = 5;
		if (visionFlag == 5 && vecPlayer[2].x != vecPlayer[3].x)
		{
			if (vecPlayer[3].x - 0.001 >= -vecPlayer[2].x)
			{
				//���W����]������
				for (int i = 0; i < 2; i++)
				{
					visionMemoryX[i] = sensorVisionX[i] * cos(PI / 180 * 1) - sensorVisionZ[i] * -sin(PI / 180 * 1);
					visionMemoryZ[i] = sensorVisionX[i] * -sin(PI / 180 * 1) + sensorVisionZ[i] * cos(PI / 180 * 1);
				}
			}
			else if (-vecPlayer[2].x - 0.001 >= vecPlayer[3].x)
			{
				//���W����]������
				for (int i = 0; i < 2; i++)
				{
					visionMemoryX[i] = sensorVisionX[i] * cos(PI / 180 * 1) - sensorVisionZ[i] * sin(PI / 180 * 1);
					visionMemoryZ[i] = sensorVisionX[i] * sin(PI / 180 * 1) + sensorVisionZ[i] * cos(PI / 180 * 1);
				}
			}
		}

	}



#pragma endregion

}

void Enemy::DownSensorVector(float playerZ, float playerX, float playerRadius) {
	//���ꂼ��̃x�N�g��
	Vector3 vec[3];	//��
	Vector3 vecPlayer[4];	//Player�܂ł̃x�N�g��

#pragma region ���E�̃x�N�g��
	//���E���C���x�N�g��
	for (int i = 0; i < 2; i++)
	{
		vec[i].x = (sensorVisionX[i] + sensorX) - sensorX;
		vec[i].y = 0;
		vec[i].z = (sensorVisionZ[i] + sensorZ) - sensorZ;
		vec[i] = Normalize(vec[i]);
	}
	//�ŏI�_����ŏI�_�܂ł̃x�N�g��
	vec[2].x = (sensorVisionX[1] + sensorX) - (sensorVisionX[0] + sensorX);
	vec[2].y = 0;
	vec[2].z = (sensorVisionZ[1] + sensorZ) - (sensorVisionZ[0] + sensorZ);
	vec[2] = Normalize(vec[2]);
#pragma endregion
#pragma region �e���E�̎n�܂肩��Player�܂ł̃x�N�g��
	//�E���C������Player�̃x�N�g��
	vecPlayer[0].x = (playerX - playerRadius) - sensorX;
	vecPlayer[0].y = 0;
	vecPlayer[0].z = playerZ - sensorZ;
	vecPlayer[0] = Normalize(vecPlayer[0]);
	//�����C������Player�̃x�N�g��
	vecPlayer[1].x = (playerX + playerRadius) - sensorX;
	vecPlayer[1].y = 0;
	vecPlayer[1].z = playerZ - sensorZ;
	vecPlayer[1] = Normalize(vecPlayer[1]);
	//���E�_[0]����Player�̃x�N�g��
	vecPlayer[2].x = playerX - (sensorVisionX[0] + sensorX);
	vecPlayer[2].y = 0;
	vecPlayer[2].z = (playerZ - playerRadius) - (sensorVisionZ[0] + sensorZ);
	vecPlayer[2] = Normalize(vecPlayer[2]);
	//���E�_[1]����Player�̃x�N�g��
	vecPlayer[3].x = playerX - (sensorVisionX[1] + sensorX);
	vecPlayer[3].y = 0;
	vecPlayer[3].z = (playerZ - playerRadius) - (sensorVisionZ[1] + sensorZ);
	vecPlayer[3] = Normalize(vecPlayer[3]);


#pragma endregion
#pragma region	�����蔻��
	//�E���C���x�N�g���̔���
	if (vec[0].x * vecPlayer[0].z - vec[0].z * vecPlayer[0].x > 0)
	{
		visionHitFlag[0] = 1;
	}
	else
	{
		visionHitFlag[0] = 0;
	}
	//�����C���x�N�g���̔���
	if (vec[1].x * vecPlayer[1].z - vec[1].z * vecPlayer[1].x < 0)
	{
		visionHitFlag[1] = 1;
	}
	else
	{
		visionHitFlag[1] = 0;
	}
	//���E���E�_�̔���
	if (vec[2].x * vecPlayer[2].z - vec[2].z * vecPlayer[2].x > 0)
	{
		visionHitFlag[2] = 1;
	}
	else
	{
		visionHitFlag[2] = 0;

	}
#pragma endregion
#pragma region ���E�ɓ��������̏���
	if (visionHitFlag[0] == 1 && visionHitFlag[1] == 1 && visionHitFlag[2] == 1)
	{
		visionFlag = 5;
		if (visionFlag == 5 && vecPlayer[2].x != vecPlayer[3].x)
		{
			if (vecPlayer[3].x - 0.001 >= -vecPlayer[2].x)
			{
				//���W����]������
				for (int i = 0; i < 2; i++)
				{
					visionMemoryX[i] = sensorVisionX[i] * cos(PI / 180 * 1) - sensorVisionZ[i] * -sin(PI / 180 * 1);
					visionMemoryZ[i] = sensorVisionX[i] * -sin(PI / 180 * 1) + sensorVisionZ[i] * cos(PI / 180 * 1);
				}
			}
			else if (-vecPlayer[2].x - 0.001 >= vecPlayer[3].x)
			{
				//���W����]������
				for (int i = 0; i < 2; i++)
				{
					visionMemoryX[i] = sensorVisionX[i] * cos(PI / 180 * 1) - sensorVisionZ[i] * sin(PI / 180 * 1);
					visionMemoryZ[i] = sensorVisionX[i] * sin(PI / 180 * 1) + sensorVisionZ[i] * cos(PI / 180 * 1);
				}
			}
		}

	}



#pragma endregion
}

void Enemy::RightSensorVector(float playerZ, float playerX, float playerRadius)
{
}

void Enemy::LeftSensorVector(float playerZ, float playerX, float playerRadius)
{
}



void Enemy::EnemyMove(float moveDis, bool WidthHeightFlag) {

	//�Z���T�[�Ō��m�����Ƃ�
	if (visionHitFlag[0] == 1 && visionHitFlag[1] == 1 && visionHitFlag[2] == 1) {
		// ��ړ�
		
		if (isMove == UP) {
			//worldTransform_.translation_.z -= speed;
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
				RightFlag = true;
				LeftFlag = false;
			}
			if (sensorMovedDis >= moveDis) {
				LeftFlag = true;
				RightFlag = false;
			}


			if (RightFlag == true) {
				worldTransform_.translation_.x += 0.1;
				sensorMovedDis += 0.1;
			}
			if (LeftFlag == true) {
				worldTransform_.translation_.x -= 0.1;
				sensorMovedDis -= 0.1;
			}
		}
		if (WidthHeightFlag == true) {
			//�G�̈ړ�����(�c)
			if (sensorMovedDis >= moveDis) {
				UpFlag = false;
				DownFlag = true;
			}
			if (sensorMovedDis <= -moveDis) {
				DownFlag = false;
				UpFlag = true;
			}

			if (UpFlag == true) {
				worldTransform_.translation_.z += 0.1;
				sensorMovedDis += 0.1;
			}
			if (DownFlag == true) {
				worldTransform_.translation_.z -= 0.1;
				sensorMovedDis -= 0.1;
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
			count = 20;
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

