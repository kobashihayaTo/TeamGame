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

	//ファイル名指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("Alert.png");
	operationHandle_ = TextureManager::Load("operation.png");
	LoadGaugeHandle_= TextureManager::Load("EnemyHp.png");
	//スプライトの生成
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
	
	//操作矢印
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
	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void UI::LoadGaugeCount()
{
	LoadGaugeSprite_->SetSize({ LoadGaugeCounter,32 });
}
