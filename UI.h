#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Function/Function.h"

#include "Enemy/Enemy.h"
#include "Map/Map.h"



class UI
{

public: // �����o�֐�
	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	UI();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~UI();

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
	void Draw(Enemy* enemy_);

private:

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	Map* map_ = nullptr;

	uint32_t textureHandle_ = 0;
	uint32_t operationHandle_ = 0;
	uint32_t hpHandle_ = 0;
	//�X�v���C�g
	Sprite* sprite_ = nullptr;
	Sprite* operationSprite_ = nullptr;
	Sprite* hpSprite_ = nullptr;

	int crisisTimer = 0;
};

