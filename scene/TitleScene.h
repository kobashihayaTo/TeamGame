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

#include "Easeing.h"
#include "Enum.h"
#include "GameScene.h"
/// <summary>
/// �^�C�g���V�[��
/// </summary>
class TitleScene
{
public://�����o�֐�

	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	TitleScene();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~TitleScene();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();


	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	void TitleEase();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �^�C�g���ŃZ���N�g
	/// </summary>
	void ChangeSelect();

	void Reset();

	/// <summary>
	/// �T�E���h
	/// </summary>
	void SoundStop();

	Scene GetNextScene() { return nextScene_; }

	int GetChangeFlag() { return changeFlag_; }

	int GetSelectFlag() { return selectFlag; }

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

	Scene nextScene_ = Scene::TITLE;

	int changeFlag_ = false;

	//�e�N�X�`���o���h��
	uint32_t textureHandle_ = 0;
	uint32_t nameHandle_ = 0;

	//�X�v���C�g
	Sprite* sprite_ = nullptr;
	Sprite* sprite_2 = nullptr;
	//�T�E���h
	uint32_t soundDataHandle_ = 0u;
	//�����Đ��n���h��
	uint32_t playHandle = -1;

	int selectFlag = 0;

	int Time = 0;
	float posY = -1080.0f;
	//float posY = 0.0f;
	
	double frame = 0.0f;
	double endFrame = 120.0f;
	double start = -1080.0f;
	double end = 0.0f;
};

