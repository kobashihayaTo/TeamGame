#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Model.h"
#include "DebugText.h"
#include "Function/Function.h"
#include "math/MyMath.h"
#include "Player/Player.h"
#include "PrimitiveDrawer.h"
#include "camera/RailCamera.h"

#define PI 3.14



class Enemy
{
public:		//�����o�֐�

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="texturehandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model, RailCamera* camera);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(bool keyFlag);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// �Z���T�[�̕`��
	/// </summary>
	void SensorDraw();

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();

	/// <summary>
	/// �t���O���Z�b�g
	/// </summary>
	void FlagReset();

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//�G�̃Z���T�[�̏���
	void SensorVision();

	void SensorVector(float playerZ, float playeryY, float playerRadius);

	float GetRadius() { return radius; }

	bool GetStopFlag() { return stopFlag; }

	bool GetOKFlag() { return OKFlag; }
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

	//�t�@���N�V����
	MyFunc myFunc_;

	//
	PrimitiveDrawer* primitive_ = nullptr;

	//�؂�ւ��t���O
	bool LightFlag = false;
	bool LeftFlag = false;

	//�����t���O
	bool stopFlag = false;
	bool stopIntervalFlag = false;

	int stopTimer = 100;
	int stopIntervalTimer = 100;

	bool OKFlag = false;
};

