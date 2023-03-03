#include "scene/TitleScene.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	audio_ = Audio::GetInstance();
	nextScene_ = Scene::TITLE;

	//�t�@�C�����w�肵�ăe�N�X�`����ǂݍ���
	textureHandle_ = TextureManager::Load("titlescreen.png");

	//�X�v���C�g�̐���
	sprite_ = Sprite::Create(textureHandle_, { 0,0 });

	selectFlag = 0;
}

void TitleScene::Update()
{
	changeFlag_ = false;
	if (input_->TriggerKey(DIK_SPACE)) {
		changeFlag_ = true;
		nextScene_ = Scene::MANUAL;
	}
	ChangeSelect();

	debugText_->SetPos(50, 330);
	debugText_->Printf("selectFlag:%d", selectFlag);
}

void TitleScene::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(commandList);

	/// <summary>
	/// �����ɔw�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>


	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3D�I�u�W�F�N�g�`��
	// 3D�I�u�W�F�N�g�`��O����
	//Model::PreDraw(commandList);

	/// <summary>
	/// ������3D�I�u�W�F�N�g�̕`�揈����ǉ��ł���
	/// </summary>
	// 3D���f���`�� 

	// 3D�I�u�W�F�N�g�`��㏈��
	//Model::PostDraw();
#pragma endregion

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(commandList);

	/// <summary>
	/// �����ɑO�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>
	sprite_->Draw();
	// �f�o�b�O�e�L�X�g�̕`��
	debugText_->DrawAll(commandList);
	//
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

#pragma endregion
}

void TitleScene::ChangeSelect()
{
	if (input_->TriggerKey(DIK_LEFT)) {
		selectFlag -= 1;
	}
	if (input_->TriggerKey(DIK_RIGHT)) {
		selectFlag += 1;
	}

	if (selectFlag < 0) {
		selectFlag = 1;
	}
	if (selectFlag > 1) {
		selectFlag = 0;
	}
}
