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
	loadGaugeHandle_= TextureManager::Load("EnemyHp.png");
	moveKeyHandle_ = TextureManager::Load("MoveKey.png");
	moveKey_unusableHandle_ = TextureManager::Load("MoveKey-unusable.png");
	//�X�v���C�g�̐���
	sprite_ = Sprite::Create(textureHandle_, { 0,0 });
	operationSprite_ = Sprite::Create(operationHandle_, { 1680,840 });
	LoadGaugeSprite_= Sprite::Create(loadGaugeHandle_, { 0,0 });
	LoadGaugeSprite_->SetPosition({ 1000,0 });

	MoveKeySprite_ = Sprite::Create(moveKeyHandle_, { 0,840 });
	MoveKey_unusableSprite_ = Sprite::Create(moveKey_unusableHandle_, { 0,840 });
}

void UI::Update()
{
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
		LoadGaugeCount();
	}

	if (map_->GetTestFlag() == false) {
		Reset();
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

void UI::KeyDraw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(commandList);

	/// <summary>
	/// �����ɑO�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>

	//�\�̓L�[�̕`��
	MoveKeySprite_->Draw();

	//sprite_->Draw();
	// �f�o�b�O�e�L�X�g�̕`��
	debugText_->DrawAll(commandList);
	//
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

#pragma endregion

}

void UI::KeyDraw_unusable()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(commandList);

	/// <summary>
	/// �����ɑO�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>

	//�\�̓L�[�̕`��

	MoveKey_unusableSprite_->Draw();

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
	LoadGaugeCounter -= 1;
	debugText_->SetPos(50, 630);
	debugText_->Printf("LoadGaugeCounter:%f", LoadGaugeCounter);
}

void UI::Reset()
{
	crisisTimer = 0;

	LoadGaugeCounter = 200;
}
