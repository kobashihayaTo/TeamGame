#pragma once
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "Audio.h"

#include "Enum.h"

/// <summary>
/// �^�C�g���V�[��
/// </summary>
class BadScene
{
public://�����o�֐�

	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	BadScene();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~BadScene();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();


	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �T�E���h
	/// </summary>
	void SoundStop();

	Scene GetNextScene() { return nextScene_; }

	int GetEndFlag() { return endFlag_; }
private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	Audio* audio_ = nullptr;

	//�f�o�b�O�J����
	DebugCamera* debugCamera_ = nullptr;

	//�f�o�b�O�J�����L��
	bool isDebugCameraActive_ = false;

	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	Scene nextScene_ = Scene::BADEND;

	int endFlag_ = false;

	//�e�N�X�`���o���h��
	uint32_t textureHandle_ = 0;
	//�X�v���C�g
	Sprite* sprite_ = nullptr;

	//�T�E���h
	uint32_t soundDataHandle_ = 0;
	//�����Đ��n���h��
	uint32_t playHandle = -1;
};

