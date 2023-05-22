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

	//�G�̌���
	enum Direction {
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
	void Initialize(Model* model, Model* sensorModel, RailCamera* camera, Vector3 enemyPos, bool WidthHeightFlag);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(bool keyFlag, Player* player, float moveDis);

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
	void EnemyMoveSearch(float playerX, float playerZ, float playerR, bool WidthHeightFlag);

	/// <summary>
	/// �v���C���[���G���猩�Ăǂ��ɂ��邩���`�F�b�N����
	/// </summary>
	void EnemyMoveCheck(float playerX, float playerZ, float playerR, bool WidthHeightFlag);

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	/// <summary>
	/// �x�N�g���Ɛ��̓����蔻��
	/// </summary>
	/// <returns></returns>
	bool VectorLineCollision(Vector3 player, float playerRadius, Vector3 start, Vector3 end, Vector3 pt3, Player* player_);

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	Vector3 GetSensorPosition();


	void OffCollision();

	void SetWorldPosition(Vector3 worldPosition) { worldTransform_.translation_ = worldPosition; }

	void SensorCollision();

	float GetRadius() { return radius; }

	float GetSensorRadius() { return sensorRadius; }

	bool GetStopFlag() { return stopFlag; }

	Vector3 GetprePosition() { return PrePosition_; }

	bool GetOKFlag() { return OKFlag; }

	bool GetCrisisFlag() { return crisisFlag; }

	const std::list<std::unique_ptr<Enemy>>& GetEnemy() { return enemys; }
private:
	//���a
	const float radius = 0.8f;
	//���a
	const float sensorRadius = 2.0f;

private:	//�����o�ϐ�

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	WorldTransform sensorTransform_;

	//���f��
	Model* model_ = nullptr;
	Model* sensorModel_ = nullptr;



	//���͏������邽��
	Input* input_ = nullptr;

	//�f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;

	// �ړ�����O�̍��W
	Vector3 PrePosition_;

	PrimitiveDrawer* primitive_ = nullptr;

	//�t�@���N�V����
	MyFunc myFunc_;

	std::list<std::unique_ptr<Enemy>>enemys;

	//�؂�ւ��t���O
	bool rightMoveFlag = false;
	bool leftMoveFlag = false;
	bool upMoveFlag = false;
	bool downMoveFlag = false;

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

	float HeightSensorVisionX[2];	//�J�����̎��E
	float HeightSensorVisionZ[2];

	float WidthSensorVisionX[2];	//�J�����̎��E
	float WidthSensorVisionZ[2];

	float visionMemoryX[2];	//���E�ړ��̋L���ϐ�
	float visionMemoryZ[2];

	int visionFlag; //���E�̈ړ��𐧌䂷��t���O
	int visionHitFlag[3]; //���E�̓����蔻��p�t���O

	float speed;

	float visionTimer; //���E���Ăѓ����܂ł̃^�C�}�[

	int count = 20;
	int isSearch = FALSE;
	int isMove_Height = UP;
	int isMove_Width = RIGHT;
	float sensorMovedDis = 0.0f;

	bool crisisFlag = false;

	XMFLOAT4 SensorColor = { 1.0f,1.0f,1.0f,0.2f };

	int direction = UP;

	bool WidthHeightFlag_ = false;

	bool upFlag = false;
	bool downFlag = false;
	bool rightFlag = false;
	bool leftFlag = false;

	//�Z���T�[�̒��_
	Vector3 UpStart_Pt1 = { 0,0,0 };
	Vector3 UpEnd_Pt2 = { 0,0,0 };
	Vector3 UpEnd_Pt3 = { 0,0,0 };
	Vector3 DownStart_Pt1 = { 0,0,0 };
	Vector3 DownEnd_Pt2 = { 0,0,0 };
	Vector3 DownEnd_Pt3 = { 0,0,0 };
	Vector3 RightStart_Pt1 = { 0,0,0 };
	Vector3 RightEnd_Pt2 = { 0,0,0 };
	Vector3 RightEnd_Pt3 = { 0,0,0 };
	Vector3 LeftStart_Pt1 = { 0,0,0 };
	Vector3 LeftEnd_Pt2 = { 0,0,0 };
	Vector3 LeftEnd_Pt3 = { 0,0,0 };
};

