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

class ManualScene
{
public:
	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	ManualScene();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ManualScene();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// ������
	/// </summary>
	void Reset();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �^�C�g���ŃZ���N�g
	/// </summary>
	void ChangeSelect();

	Scene GetNextScene() { return nextScene_; }

	int GetManualFlag() { return manualFlag_; }
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

	Scene nextScene_ = Scene::END;

	int manualFlag_ = false;

	//�e�N�X�`���o���h��
	uint32_t menuHandle_1 = 0;
	uint32_t menuHandle_2 = 0;
	uint32_t menuHandle_3 = 0;
	uint32_t menuHandle_4 = 0;
	//�X�v���C�g
	Sprite* sprite_1 = nullptr;
	Sprite* sprite_2 = nullptr;
	Sprite* sprite_3 = nullptr;
	Sprite* sprite_4 = nullptr;
	int selectFlag = 0;

};

