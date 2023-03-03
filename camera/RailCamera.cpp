#include "RailCamera.h"
#include "math/MyMath.h"
#include "Function/Function.h"

void RailCamera::Initialize(Vector3& position, Vector3& rotation, Player* player) {
	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�g�����X�t�H�[���̏����ݒ�
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;

	//�r���[�v���W�F�N�V�����̏�����
	viewProjection_.farZ = 2000.0f;
	viewProjection_.eye = { 0,40.0f,-15.0f };
	viewProjection_.Initialize();

	newPlayer = player;
}

void RailCamera::Update() {

	ZoomIn();
#ifdef _DEBUG
	//�f�o�b�O�p�\��
	debugText_->SetPos(50, 50);
	debugText_->Printf("RailCamera pos:(%f, %f, %f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	debugText_->SetPos(50, 270);
	debugText_->Printf("viewtarget:(%f, %f, %f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);

#endif
}

void RailCamera::ZoomIn() {
	//���[���h�g�����X�t�H�[���̍��W�̐��l�������Z����(�ړ�)
	Vector3 cameraMove = { 0.0f,0.0f,0.0f };


	
	//�J�����̈ʒu�̈ړ�����
	if (input_->PushKey(DIK_W)) {
		cameraMove.y += 0.1;
	}

	if (input_->PushKey(DIK_S)) {
		cameraMove.y -= 0.1;
	}

	if (input_->PushKey(DIK_D)) {
		cameraMove.z += 0.1;
	}

	if (input_->PushKey(DIK_A)) {
		cameraMove.z -= 0.1;
	}

	if (input_->PushKey(DIK_E)) {
		cameraMove.x += 0.1;
	}

	if (input_->PushKey(DIK_Q)) {
		cameraMove.x -= 0.1;
	}


	worldTransform_.translation_ += cameraMove;
	////�s��̍X�V
	myFunc_.UpdateWorldTransform(worldTransform_);

	//�J�������_���W��ݒ�
	viewProjection_.eye += cameraMove;

	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//���[���J�����̉�]�𔻒�
	//forward = newPlayer->GetTransform();
	//���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	//forward += viewProjection_.eye;
	viewProjection_.target = { newPlayer->Getmove().x,newPlayer->Getmove().y,newPlayer->Getmove().z + 1 };

	////���[���h����x�N�g��
	//Vector3 up(0, 1, 0);
	////���[���J�����̉�]�𔽉f(���[���J�����̏���x�N�g��)
	//viewProjection_.up = Transform(up, worldTransform_);

	//������̉�]�̑���
	const float kUpRotSpeed = 0.05f;

	//�����������ňړ��x�N�g����ύX
	if (input_->PushKey(DIK_SPACE)) {
		viewAngle += kUpRotSpeed;
		//2�΂𒴂�����0�ɖ߂�
		viewAngle = fmodf(viewAngle, 3.14 * 2.0f);
	}

	//������x�N�g�����v�Z(���a�P�̉~����̍��W)
	viewProjection_.up = { 0.0f, 1.0f, 0.0f };

	//�r���[�v���W�F�N�V�������X�V
	viewProjection_.UpdateMatrix();
}

void RailCamera::PlayerCamera()
{
	////���[���h�O���x�N�g��
	//Vector3 forward(0, 0, 1);
	////���[���J�����̉�]�𔻒�
	////forward = newPlayer->GetTransform();
	////���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	////forward += viewProjection_.eye;
	//viewProjection_.target.y = newPlayer->GetTransform().y;
	//viewProjection_.target.y=


	////�r���[�v���W�F�N�V�������X�V
	//viewProjection_.UpdateMatrix();
}
