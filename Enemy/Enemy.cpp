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
		worldTransform_.translation_.x+=0.1f;
	}
	if (LightFlag == true)
	{
		worldTransform_.translation_.x-=0.1f;
	}
	//�s��̍X�V
	myFunc_.UpdateWorldTransform(worldTransform_);

	//�����t���O�̐؂�ւ�
	if (input_->TriggerKey(DIK_K)) {
		if (invisibleFlag == true) {
			invisibleFlag = false;
		}
		else if (invisibleFlag == false) {
			invisibleFlag = true;
		}
	}


	//�f�o�b�O�p�\��
	debugText_->SetPos(50, 120);
	debugText_->Printf("Enemy pos:(%f, %f, %f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

//�`��
void Enemy::Draw(ViewProjection& viewprojection) {
	//3D���f����`��
	if (invisibleFlag == true)
	{
		model_->Draw(worldTransform_, viewprojection);
	}
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