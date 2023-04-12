#pragma once

#include "WorldTransform.h"
#include "Input.h"
#include "Model.h"
#include "DebugText.h"
#include "Function/Function.h"
#include "MyMath.h"
#include "Material.h"

#include "PrimitiveDrawer.h"


struct CornerPos {
	float cornerZ;	//�v���C���[�̊p��Z���W
	float cornerX;	//�v���C���[�̊p��X���W

};

class Player
{
public:	//�\����

public:	//�����o�֐�

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="texturehandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(bool PlayerKeyFlag);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	// ���[���h���W���擾
	Vector3 GetWorldPosition();

	// ����̃}�b�v�ԍ����擾
	CornerPos LeftTopNum();
	
	// �E��̃}�b�v�ԍ����擾
	CornerPos RightTopNum();
	
	// �����̃}�b�v�ԍ����擾
	CornerPos LeftBottomNum();

	// �E���̃}�b�v�ԍ����擾
	CornerPos RightBottomNum();

	// �Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	Vector3 Getmove() { return move; }

	Vector3 GetTransform() { return Trans; }

	bool GetDeathFlag() { return playerDeathFlag; }

	bool GetSecretFlag() { return playerSecretFlag; }

	bool GetSecretIntervalFlag() { return SecretIntervalFlag; }

	bool GetOKFlag() { return OKFlag; }

	void Reset();

	void FlagReset();

public:
	// speedZ�̃Z�b�^�[
	void SetPlayerSpeedZ(float playerSpeedZ) { playerSpeedZ_ = playerSpeedZ; }

	// speedX�̃Z�b�^�[
	void SetPlayerSpeedX(float playerSpeedX) { playerSpeedX_ = playerSpeedX; }

	void SetWorldPosition(Vector3 worldPosition) { worldTransform_.translation_ = worldPosition; }

	Vector3 GetprePosition() { return prePosition_; }

	float GetRadius() { return radius; }

	//�J�����̃g�����X�̃Z�b�^�[


private:
	//���a
	const float radius = 0.8f;

private:	//�����o�ϐ�

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	// �ړ�����O�̍��W
	Vector3 prePosition_;

	//���f��
	Model* model_ = nullptr;

	//���͏������邽��
	Input* input_ = nullptr;

	//�f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;

	//�t�@���N�V����
	MyFunc myFunc_;

	Material* material = nullptr;

	PrimitiveDrawer* primitiveDrawer = nullptr;

	//�E��A�E���̍��W�̕ϐ�
	int rightTopZ = 0;
	int rightTopX = 0;
	int rightBottomX = 0;
	int rightBottomZ = 0;
	 
	//����A�����̍��W�̕ϐ�
	int leftTopZ = 0;
	int leftTopX = 0;
	int leftBottomX = 0;
	int leftBottomZ = 0;

	//�v���C���[�̑��x
	float playerSpeedX_ = 0.3f;
	float playerSpeedZ_ = 0.3f;
	Vector3 move = { 0,0,0 };

	Vector3 Trans = { 0,0,0 };

	//�t���O�֌W
	bool playerDeathFlag = false;
	bool playerSecretFlag = false;

	bool SecretIntervalFlag = false;

	XMFLOAT4 DefaultColor = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 SecretColor = { 1.0f,1.0f,1.0f,0.5f };

	int SecretTimer = 100;
	float SecretIntervalTimer = 100.0f;

	bool OKFlag = false;


};

