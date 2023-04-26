#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

#include "EndScene.h"

EndScene::EndScene() {}

EndScene::~EndScene() {}

void EndScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	audio_ = Audio::GetInstance();
	nextScene_ = Scene::END;

	//�t�@�C�����w�肵�ăe�N�X�`����ǂݍ���
	textureHandle_ = TextureManager::Load("clearscreen.png");

	//�X�v���C�g�̐���
	sprite_ = Sprite::Create(textureHandle_, { 0,0 });

	//�T�E���h
	soundDataHandle_ = audio_->LoadWave("Sound/gameclear.wav");
}

void EndScene::Update()
{
	if (audio_->IsPlaying(playHandle) == false || playHandle == -1)
	{
		playHandle = audio_->PlayWave(soundDataHandle_, false, 1);
	}
	endFlag_ = false;
	if (input_->TriggerKey(DIK_SPACE)) {
		endFlag_ = true;
		nextScene_ = Scene::TITLE;
	}
}

void EndScene::Draw()
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

#ifdef _DEBUG

	// �f�o�b�O�e�L�X�g�̕`��
	debugText_->DrawAll(commandList);
#endif  
	//
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

#pragma endregion

}

void EndScene::SoundStop()
{
	if (audio_->IsPlaying(playHandle) == true)
	{
		audio_->StopWave(playHandle);
	}
}
