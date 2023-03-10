#include "Player.h"
#include <cassert>

//������
void Player::Initialize(Model* model) {
	//NULL�|�C���g�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL������
	model_ = model;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	//�v���C���[�̑傫��(�{��)
	worldTransform_.scale_ = { 2.0f,2.0f,2.0f };
	//�v���C���[�̏����ʒu�̐ݒ�
	Trans = { -11.0f, 0.0f, -18.0f };
	worldTransform_.translation_ = Trans;

}

//�X�V
void Player::Update() {

	//�v���C���[�̈ړ��x�N�g��
	if (playerSecretFlag == false ) {
		//3D���f����`��
		worldTransform_.ColorSetter(DefaultColor);
	}
	if (playerSecretFlag == true) {
		//3D���f����`��
		worldTransform_.ColorSetter(SecretColor);
	}
	if (SecretIntervalFlag == true) {
		//3D���f����`��
		worldTransform_.ColorSetter(DefaultColor);
	}
	worldTransform_.TransferColorMatrix();

	prePosition_ = worldTransform_.translation_;

	//�v���C���[�ړ�����
	//�㉺
	if (input_->PushKey(DIK_UP)) {
		worldTransform_.translation_.z += playerSpeedZ_;
		worldTransform_.rotation_.y = RadianConversion(0);
	}
	if (input_->PushKey(DIK_DOWN)) {
		worldTransform_.translation_.z -= playerSpeedZ_;
		worldTransform_.rotation_.y = RadianConversion(180);
	}
	//���E
	if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.translation_.x += playerSpeedX_;
		worldTransform_.rotation_.y = RadianConversion(90);
	}
	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.translation_.x -= playerSpeedX_;
		worldTransform_.rotation_.y = RadianConversion(270);
	}

	if (input_->TriggerKey(DIK_A)) {
		if (playerSecretFlag == false) {
			playerSecretFlag = true;
		}
	}

	if (playerSecretFlag == true){
		SecretTimer--;
		if (SecretTimer < 0){
			SecretIntervalFlag = true;
		}
	}

	if (SecretIntervalFlag == true) {
		SecretIntervalTimer--;
		if (SecretIntervalTimer < 0) {
			playerSecretFlag = false;
			SecretIntervalFlag = false;
			SecretTimer = 100;
			SecretIntervalTimer = 100;
		}

	}
	move = worldTransform_.translation_;



	//�s��̍X�V
	myFunc_.UpdateWorldTransform(worldTransform_);
	//�f�o�b�O�p�\��
	debugText_->SetPos(50, 150);
	debugText_->Printf("Player pos:(%f, %f, %f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

	debugText_->SetPos(50, 300);
	debugText_->Printf("playerSecretFlag:%d", playerSecretFlag);

	debugText_->SetPos(50, 330);
	debugText_->Printf("SecretTimer:%d", SecretTimer);

	debugText_->SetPos(50, 350);
	debugText_->Printf("SecretIntervalTimer:%d", SecretIntervalTimer);

	debugText_->SetPos(50, 370);
	debugText_->Printf("SecretIntervalFlag:%d", SecretIntervalFlag);
}

//�`��
void Player::Draw(ViewProjection& viewprojection) {
	//3D���f����`��
	model_->Draw(worldTransform_, viewprojection);
}

Vector3 Player::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;

}

void Player::OnCollision() {
	//�f�X�t���O�𗧂Ă�
	playerDeathFlag = true;
}

void Player::Reset()
{
	playerDeathFlag = false;
	playerSecretFlag = false;
	//�v���C���[�̏����ʒu�̐ݒ�
	worldTransform_.translation_ = { -11.0f,0.0f ,-18.0f };

	playerSecretFlag = false;
	SecretIntervalFlag = false;
	SecretTimer = 100;
	SecretIntervalTimer = 100;
}
	//�������Ȃ�

/*
	�v���C���[�̃}�b�v��̂ǂ��ɂ��邩���~�������߃}�b�v�ԍ����擾�ł���悤�ɂ�����
	�}�b�v�ԍ��擾�o�����牽�s���񂩂�������
	map[pNumZ][pNumX] == BLOXK;�Ō��ݒn���u���b�N���ǂ������f�ł���
	����map�������Ă���̂�Map�N���X�ɂȂ�̂�map�N���X�Ƀv���C���[�̃}�b�v�ԍ���n���ău���b�N���ǂ������`�F�b�N���Ă��炤

	�}�b�v�ԍ��͉��s����ƂȂ邽�ߓ�̐����̒l�ł���
	�������܂Ƃ߂�return������
	���Ⴀ�ǂ�����H
	�\���̂��g���čs����܂Ƃ߂�

*/

//����̍��W���擾
CornerPos Player::LeftTopNum() {
	CornerPos cornerPos;

	cornerPos.cornerZ = worldTransform_.translation_.z - radius;
	cornerPos.cornerX = worldTransform_.translation_.x - radius;

	return cornerPos;
}

//�E��̍��W���擾
CornerPos Player::RightTopNum() {
	CornerPos cornerPos;

	cornerPos.cornerZ = worldTransform_.translation_.z - radius;
	cornerPos.cornerX = worldTransform_.translation_.x + radius;

	return cornerPos;
}

//�����̍��W���擾
CornerPos Player::LeftBottomNum() {
	CornerPos cornerPos;

	cornerPos.cornerZ = worldTransform_.translation_.z + radius;
	cornerPos.cornerX = worldTransform_.translation_.x - radius;

	return cornerPos;
}

//�E���̍��W���擾
CornerPos Player::RightBottomNum() {
	CornerPos cornerPos;

	cornerPos.cornerZ = worldTransform_.translation_.z + radius;
	cornerPos.cornerX = worldTransform_.translation_.x + radius;

	return cornerPos;
}