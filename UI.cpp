#include "UI.h"

UI::UI()
{
}

UI::~UI()
{
	delete sprite_;
	delete operationSprite_;
}

void UI::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//�t�@�C�����w�肵�ăe�N�X�`����ǂݍ���
	textureHandle_ = TextureManager::Load("Alert.png");
	operationHandle_ = TextureManager::Load("operation.png");
	LoadGaugeHandle_= TextureManager::Load("EnemyHp.png");
	//�X�v���C�g�̐���
	sprite_ = Sprite::Create(textureHandle_, { 0,0 });
	operationSprite_ = Sprite::Create(operationHandle_, { 1680,840 });
	LoadGaugeSprite_= Sprite::Create(LoadGaugeHandle_, { 0,0 });
	LoadGaugeSprite_->SetPosition({ 0,0 });
}

void UI::Update()
{
	LoadGaugeCount();
}

void UI::Draw(Enemy* enemy_, Map* map_)
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
	
	//������
	operationSprite_->Draw();

	if (map_->GetUIFlag() == true) {
		LoadGaugeSprite_->Draw();
		LoadGaugeCounter--;
		LoadGaugeCount();
	}

	if (enemy_->GetCrisisFlag() == true) {
		crisisTimer++;
		if (crisisTimer < 15) {
			sprite_->Draw();
		}
		if (crisisTimer > 25)
		{
			crisisTimer = 0;
		}
	}
	//sprite_->Draw();
	// �f�o�b�O�e�L�X�g�̕`��
	debugText_->DrawAll(commandList);
	//
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

#pragma endregion

}

void UI::LoadGaugeCount()
{
	LoadGaugeSprite_->SetSize({ LoadGaugeCounter,32 });
}
