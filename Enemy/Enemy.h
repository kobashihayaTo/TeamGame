#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Model.h"
#include "DebugText.h"
#include "Function/Function.h"


class Enemy
{
public:		//�����o�֐�

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="texturehandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	float GetRadius() { return radius; }

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

	//�؂�ւ��t���O
	bool LightFlag = false;
	bool LeftFlag = false;

	//�����t���O
	bool invisibleFlag = false;
};

