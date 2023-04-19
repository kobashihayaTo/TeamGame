#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

#include "BadScene.h"

BadScene::BadScene(){}

BadScene::~BadScene(){}

void BadScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	audio_ = Audio::GetInstance();
	nextScene_ = Scene::BADEND;

	//ファイル名指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("gameover.png");
	nameHandle_ = TextureManager::Load("overName.png");
	//スプライトの生成
	sprite_ = Sprite::Create(textureHandle_, { 0,0 });
	sprite_2 = Sprite::Create(nameHandle_, { 0,posY });

	//サウンド
	soundDataHandle_ = audio_->LoadWave("Sound/gameover.wav");
}

void BadScene::Update()
{
	sprite_2->SetPosition({ 0,posY });
	
	if (audio_->IsPlaying(playHandle) == false || playHandle == -1)
	{
		playHandle = audio_->PlayWave(soundDataHandle_, false, 1);
	}
	TitleEase();
	endFlag_ = false;
	if (input_->TriggerKey(DIK_SPACE)) {
 		endFlag_ = true;
		nextScene_ = Scene::TITLE;
	}
}

void BadScene::TitleEase()
{

	if (frame != endFrame)
	{
		frame++;
	}

	posY = start + (end - start) * easeInOutBounce(frame / endFrame);


	debugText_->SetPos(50, 350);
	debugText_->Printf("posY:%f", posY);
	debugText_->SetPos(50, 370);
	debugText_->Printf("frame:%lf", frame);

}

void BadScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	//Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// 3Dモデル描画 

	// 3Dオブジェクト描画後処理
	//Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	sprite_->Draw();
	sprite_2->Draw();
	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void BadScene::SoundStop()
{
	if (audio_->IsPlaying(playHandle) == true)
	{
		audio_->StopWave(playHandle);
	}
}
