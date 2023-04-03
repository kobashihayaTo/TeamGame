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
public:
	enum Move {
		UP,		// 0
		DOWN,	// 1
		RIGHT,	// 2
		LEFT	// 3
	};

public:		//�����o�֐�

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="texturehandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model, Model* sensormodel, RailCamera* camera,Vector3 enemyPos);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(bool keyFlag, Player* player,float moveDis, bool WidthHeightFlag);

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
	/// ���ꂼ��̃x�N�g���𓾂�֐�(��)
	/// </summary>
	void UpSensorVector(float playerZ, float playerX, float playerRadius);

	/// <summary>
	/// ���ꂼ��̃x�N�g���𓾂�֐�(��)
	/// </summary>
	void DownSensorVector(float playerZ, float playerX, float playerRadius);

	/// <summary>
	/// ���ꂼ��̃x�N�g���𓾂�֐�(�E)
	/// </summary>
	void RightSensorVector(float playerZ, float playerX, float playerRadius);

	/// <summary>
	/// ���ꂼ��̃x�N�g���𓾂�֐�(��)
	/// </summary>
	void LeftSensorVector(float playerZ, float playerX, float playerRadius);

	/// <summary>
	/// �Z���T�[�̕`��
	/// </summary>
	void SensorDraw(ViewProjection& viewProjection);

	/// <summary>
	/// �ړ�����
	/// </summary>
	void EnemyMove(float moveDis, bool WidthHeightFlag);

	/// <summary>
	/// �v���C���[���G���猩�Ăǂ��ɂ��邩������
	/// </summary>
	void EnemyMoveSearch(float playerX, float playerZ, float playerR);

	/// <summary>
	/// 
	/// </summary>
	void EnemyMoveCheck(float playerX, float playerZ, float playerR);

	/// <summary>
	/// �ϐ��̏�����
	/// </summary>
	void FlagReset();

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	Vector3 GetSensorPosition();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	void SetWorldPosition(Vector3 worldPosition) { worldTransform_.translation_ = worldPosition; }

	void SensorCollision();

	float GetRadius() { return radius; }

	float GetSensorRadius() { return sensorRadius; }

	bool GetStopFlag() { return stopFlag; }

	Vector3 GetprePosition() { return prePosition_; }

	bool GetOKFlag() { return OKFlag; }

	const std::list<std::unique_ptr<Enemy>>& GetEnemy() { return enemys; }
private:
	//���a
	const float radius = 1.0f;
	//���a
	const float sensorRadius = 2.0f;

private:	//�����o�ϐ�

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	WorldTransform sensorTransform_;

	//���f��
	Model* model_ = nullptr;
	Model* sensormodel_ = nullptr;

	//���͏������邽��
	Input* input_ = nullptr;

	//�f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;

	// �ړ�����O�̍��W
	Vector3 prePosition_;

	PrimitiveDrawer* primitive_ = nullptr;

	//�t�@���N�V����
	MyFunc myFunc_;

	std::list<std::unique_ptr<Enemy>>enemys;

	//�؂�ւ��t���O
	bool RightFlag = true;
	bool LeftFlag =  false;
	bool UpFlag = true;
	bool DownFlag = false;

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

	float speed;

	float visionTimer; //���E���Ăѓ����܂ł̃^�C�}�[

	int count = 20;
	int isSearch = FALSE;
	int isMove = UP;
	int isMove_1 = RIGHT;
	float sensorMovedDis = 0.0f;

	XMFLOAT4 sensorColor = { 1.0f,1.0f,1.0f,0.2f };
};

