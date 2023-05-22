#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

#include "ManualScene.h"

ManualScene::ManualScene() {}

ManualScene::~ManualScene() {
	delete sprite_1;
	delete sprite_2;
	delete sprite_3;
	delete sprite_4;
}

void ManualScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	nextScene_ = Scene::MANUAL;

	//�t�@�C�����w�肵�ăe�N�X�`����ǂݍ���
	menuHandle_1 = TextureManager::Load("Manual/manual_01.png");
	menuHandle_2 = TextureManager::Load("Manual/manual_02.png");
	menuHandle_3 = TextureManager::Load("Manual/manual_03.png");
	menuHandle_4 = TextureManager::Load("Manual/manual_04.png");

	//�X�v���C�g�̐���
	sprite_1 = Sprite::Create(menuHandle_1, { 0,0 });
	sprite_2 = Sprite::Create(menuHandle_2, { 0,0 });
	sprite_3= Sprite::Create(menuHandle_3, { 0,0 });
	sprite_4 = Sprite::Create(menuHandle_4, { 0,0 });
}

void ManualScene::Update()
{


	if (input_->TriggerKey(DIK_SPACE)) {
		selectFlag += 1;
	}

	if (selectFlag == 4) {
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
		sprite_1->Draw();
	}
	if (selectFlag == 1)
	{
		sprite_2->Draw();
	}
	if (selectFlag == 2)
	{
		sprite_3->Draw();
	}
	if (selectFlag == 3)
	{
		sprite_4->Draw();
	}

#ifdef _DEBUG

	// �f�o�b�O�e�L�X�g�̕`��
	debugText_->DrawAll(commandList);
#endif  

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

#pragma endregion

}

void ManualScene::ChangeSelect()
{

}