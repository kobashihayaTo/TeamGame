#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Model.h"
#include "DebugText.h"
#include "Function/Function.h"
#include "camera/RailCamera.h"
#include "Player/Player.h"
#include "PrimitiveDrawer.h"

#include <list>

#define PI 3.14


class Enemy
{
public:		//�����o�֐�

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="texturehandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model, RailCamera* camera,Vector3 enemyPos);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(bool keyFlag, Player* player);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();

	/// <summary>
	/// ���E�𓮂����֐�
	/// </summary>
	void SensorVision();

	/// <summary>
	/// ���ꂼ��̃x�N�g���𓾂�֐�
	/// </summary>
	void SensorVector(float playerZ, float playerX, float playerRadius);

	/// <summary>
	/// �Z���T�[�̕`��
	/// </summary>
	void SensorDraw();

	void FlagReset();

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	float GetRadius() { return radius; }

	bool GetStopFlag() { return stopFlag; }

	bool GetOKFlag() { return OKFlag; }

	const std::list<std::unique_ptr<Enemy>>& GetEnemy() { return enemys; }
private:
	//���a
	const float radius = 1.0f;

private:	//�����o�ϐ�

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	//���͏������邽��
	Input* input_ = nullptr;

	//�f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;

	PrimitiveDrawer* primitive_ = nullptr;

	//�t�@���N�V����
	MyFunc myFunc_;

	std::list<std::unique_ptr<Enemy>>enemys;

	//�؂�ւ��t���O
	bool LightFlag = false;
	bool LeftFlag =  false;

	//�����t���O
	bool invisibleFlag = false;

	bool stopFlag = false;
	bool stopIntervalFlag = false;

	int stopTimer = 100;
	int stopIntervalTimer = 100;

	int enemyMax = 5;

	bool OKFlag = false;

	float sensorX;	//�J�����{�̂̍��W
	float sensorZ;
	float sensorR;

	float sensorVisionX[2];	//�J�����̎��E
	float sensorVisionZ[2];

	float visionMemoryX[2];	//���E�ړ��̋L���ϐ�
	float visionMemoryZ[2];

	int visionFlag; //���E�̈ړ��𐧌䂷��t���O
	int visionHitFlag[3]; //���E�̓����蔻��p�t���O

	int speed;

	float visionTimer; //���E���Ăѓ����܂ł̃^�C�}�[
};

