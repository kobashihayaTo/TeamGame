#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

#include "ManualScene.h"

ManualScene::ManualScene() {}

ManualScene::~ManualScene() {
	delete sprite_;
}

void ManualScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	nextScene_ = Scene::MANUAL;

	//�t�@�C�����w�肵�ăe�N�X�`����ǂݍ���
	textureHandle_ = TextureManager::Load("manual.png");
	nameHandle_ = TextureManager::Load("manual_2.png");

	//�X�v���C�g�̐���
	sprite_ = Sprite::Create(textureHandle_, { 0,0 });
	sprite_2 = Sprite::Create(nameHandle_, { 0,0 });

}

void ManualScene::Update()
{


	if (input_->TriggerKey(DIK_SPACE)) {
		selectFlag += 1;
	}

	if (selectFlag == 2) {
		manualFlag_ = true;
		nextScene_ = Scene::GAME;
	}
	ChangeSelect();
	debugText_->SetPos(50, 100);
	debugText_->Printf("selectFlag:%d", selectFlag);

}

void ManualScene::Reset()
{

	selectFlag = 0;
	manualFlag_ = false;
}

void ManualScene::Draw()
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
	if (selectFlag == 0)
	{
		sprite_->Draw();
	}
	if (selectFlag == 1)
	{
		sprite_2->Draw();
	}


	// �f�o�b�O�e�L�X�g�̕`��
	debugText_->DrawAll(commandList);

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

#pragma endregion

}

void ManualScene::ChangeSelect()
{

}