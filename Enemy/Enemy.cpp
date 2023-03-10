#include "Enemy.h"
#include <cassert>

//������
void Enemy::Initialize(Model* model) {
	//NULL�|�C���g�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL������
	model_ = model;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	//�G�̑傫��(�{��)
	worldTransform_.scale_ = { 0.5f,0.5f,0.5f };
	//�G�̏����ʒu�̐ݒ�
	worldTransform_.translation_ = { 6.0f, 0.9f, -2.7f };
}

//�X�V
void Enemy::Update() {
	//�v���C���[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };

	if (input_->TriggerKey(DIK_S))
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
		if (stopIntervalTimer < 0) {
			stopFlag = false;
			stopIntervalFlag = false;
			stopTimer = 100;
			stopIntervalTimer = 100;
		}
	}



	if (stopFlag == false|| stopIntervalFlag==true) {
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

		//�G�̈ړ�����
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
		if (LeftFlag == true)
		{
			worldTransform_.translation_.x += 0.1f;
		}
		if (LightFlag == true)
		{
			worldTransform_.translation_.x -= 0.1f;
		}
	}

	//�s��̍X�V
	myFunc_.UpdateWorldTransform(worldTransform_);
	worldTransform_.TransferColorMatrix();

	//�f�o�b�O�p�\��
	debugText_->SetPos(50, 120);
	debugText_->Printf("stopFlag:%d", stopFlag);

	debugText_->SetPos(50, 210);
	debugText_->Printf("stopIntervalFlag:%d", stopIntervalFlag);

	debugText_->SetPos(50, 390);
	debugText_->Printf("stopTimer:%d", stopTimer);

	debugText_->SetPos(50, 410);
	debugText_->Printf("stopIntervalTimer:%d", stopIntervalTimer);
}

//�`��
void Enemy::Draw(ViewProjection& viewprojection) {
	//3D���f����`��
	model_->Draw(worldTransform_, viewprojection);

}

void Enemy::Reset()
{
	worldTransform_.translation_ = { 6.0f, 0.9f, -2.7f };
	stopFlag = false;
}

Vector3 Enemy::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;

}

void Enemy::OnCollision() {
	//�������Ȃ�
}