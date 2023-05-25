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

	//ファイル名指定してテクスチャを読み込む
	textureHandle1_ = TextureManager::Load("GAMECLEAR/clear1_table.png");
	textureHandle2_ = TextureManager::Load("GAMECLEAR/clear2.png");
	textureHandle3_ = TextureManager::Load("GAMECLEAR/clear3.png");

	//スプライトの生成
	sprite1_ = Sprite::Create(textureHandle1_, { 0,0 });
	sprite2_ = Sprite::Create(textureHandle2_, { 0,0 });
	sprite3_ = Sprite::Create(textureHandle3_, { 0,0 });

	//サウンド
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
	sprite1_->Draw();

	//クリア(色が淡いやつ)
	if (lightingTimer <= 250 && lightingTimer >= 220) {
		sprite2_->Draw();
	}
	if (lightingTimer <= 190 && lightingTimer >= 150) {
		sprite2_->Draw();
	}
	if (lightingTimer <= 90 && lightingTimer >= 60) {
		sprite2_->Draw();
	}

	//クリア(白のハイライト)
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

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
#endif  
	//
	// スプライト描画後処理
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
