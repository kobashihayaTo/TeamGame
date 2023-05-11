#include "scene/TitleScene.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
	delete sprite_;
	delete sprite_2;
}

void TitleScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	audio_ = Audio::GetInstance();
	nextScene_ = Scene::TITLE;

	//�t�@�C�����w�肵�ăe�N�X�`����ǂݍ���
	textureHandle_ = TextureManager::Load("titleBackground.png");
	nameHandle_ = TextureManager::Load("titlename.png");
	spaceHandle_ = TextureManager::Load("SPACE.png");
	space_Selection_= TextureManager::Load("SPACE_selection.png");
	exitHandle_= TextureManager::Load("EXIT.png");
	exit_Selection_ = TextureManager::Load("EXIT_selection.png");
	//�X�v���C�g�̐���
	sprite_ = Sprite::Create(textureHandle_, { 0,0 });
	sprite_2 = Sprite::Create(nameHandle_, { 0,posY });
	space_sprite_ = Sprite::Create(spaceHandle_, { 1100,700 });
	space_Selection_sprite = Sprite::Create(space_Selection_, { 1100,700 });
	exit_sprite_ = Sprite::Create(exitHandle_, { 300,700 });
	exit_Selection_sprite = Sprite::Create(exit_Selection_, { 300,700 });

	selectFlag = 0;

	posY = -1080.0f;

	//�T�E���h
	soundDataHandle_ = audio_->LoadWave("NeonVerseBgm/NeonVerseOP.wav");

}

void TitleScene::Update()
{
	sprite_2->SetPosition({ 0,posY });

	if (audio_->IsPlaying(playHandle) == false || playHandle == -1)
	{
		playHandle = audio_->PlayWave(soundDataHandle_, false, 1);
	}

	changeFlag_ = false;
	if (input_->TriggerKey(DIK_SPACE)) {
		changeFlag_ = true;
		nextScene_ = Scene::MANUAL;
	}
	ChangeSelect();
	TitleEase();

	debugText_->SetPos(50, 330);
	debugText_->Printf("selectFlag:%d", selectFlag);
}

void TitleScene::TitleEase()
{

	if (frame != endFrame)
	{
		frame++;
	}


	posY = start + (end - start) * easeOutBounce(frame / endFrame);


	debugText_->SetPos(50, 350);
	debugText_->Printf("posY:%f", posY);
	debugText_->SetPos(50, 370);
	debugText_->Printf("posY:%lf", frame);

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
	sprite_2->Draw();

	if (selectFlag == 0) {
	space_Selection_sprite->Draw();
	}
	else {
		space_sprite_->Draw();
	}

	if (selectFlag == 1) {
		exit_Selection_sprite->Draw();		
	}
	else{
		exit_sprite_->Draw();
	}
#ifdef _DEBUG

	// �f�o�b�O�e�L�X�g�̕`��
	debugText_->DrawAll(commandList);
#endif  
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

void TitleScene::Reset()
{
	Time = 0;
	posY = -1080.0f;

	frame = 0.0f;
	endFrame = 120.0f;
	start = -1080.0f;
	end = 0.0f;
}

void TitleScene::SoundStop()
{
	if (audio_->IsPlaying(playHandle) == true)
	{
		audio_->StopWave(playHandle);
	}
}
