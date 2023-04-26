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
	loadGaugeHandle_= TextureManager::Load("EnemyHp.png");
	moveKeyHandle_ = TextureManager::Load("MoveKey.png");
	moveKey_unusableHandle_ = TextureManager::Load("MoveKey-unusable.png");
	//スプライトの生成
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
	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void UI::KeyDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	//能力キーの描画
	MoveKeySprite_->Draw();

	//sprite_->Draw();
	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void UI::KeyDraw_unusable()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	//能力キーの描画

	MoveKey_unusableSprite_->Draw();

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
	LoadGaugeCounter -= 1;
	debugText_->SetPos(50, 630);
	debugText_->Printf("LoadGaugeCounter:%f", LoadGaugeCounter);
}

void UI::Reset()
{
	crisisTimer = 0;

	LoadGaugeCounter = 200;
}
