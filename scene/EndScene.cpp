#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

#include "EndScene.h"

EndScene::EndScene() {}

EndScene::~EndScene() {
	delete sprite1_;
	delete sprite2_;
	delete sprite3_;
}

void EndScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	audio_ = Audio::GetInstance();
	nextScene_ = Scene::END;

	//�t�@�C�����w�肵�ăe�N�X�`����ǂݍ���
	textureHandle1_ = TextureManager::Load("GAMECLEAR/clear1_table.png");
	textureHandle2_ = TextureManager::Load("GAMECLEAR/clear2.png");
	textureHandle3_ = TextureManager::Load("GAMECLEAR/clear3.png");

	//�X�v���C�g�̐���
	sprite1_ = Sprite::Create(textureHandle1_, { 0,0 });
	sprite2_ = Sprite::Create(textureHandle2_, { 0,0 });
	sprite3_ = Sprite::Create(textureHandle3_, { 0,0 });

	//�T�E���h
	soundDataHandle_ = audio_->LoadWave("Sound/gameclear.wav");

	lightingTimer = 300;
}

void EndScene::Update()
{
	
	lightingTimer--;
	

	if (audio_->IsPlaying(playHandle) == false || playHandle == -1){
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
	sprite1_->Draw();

	//�N���A(�F���W�����)
	if (lightingTimer <= 250 && lightingTimer >= 220) {
		sprite2_->Draw();
	}
	if (lightingTimer <= 190 && lightingTimer >= 150) {
		sprite2_->Draw();
	}
	if (lightingTimer <= 90 && lightingTimer >= 60) {
		sprite2_->Draw();
	}

	//�N���A(���̃n�C���C�g)
	if (lightingTimer <= 170 && lightingTimer >= 160) {
		sprite3_->Draw();
	}
	if (lightingTimer <= 80 && lightingTimer >= 70) {
		sprite3_->Draw();
	}
	

	if (lightingTimer <= 0) {
		sprite2_->Draw();
		sprite3_->Draw();
	}

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
