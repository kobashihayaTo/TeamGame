#include "scene/TitleScene.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	audio_ = Audio::GetInstance();
	nextScene_ = Scene::TITLE;

	//ファイル名指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("titlescreen.png");

	//スプライトの生成
	sprite_ = Sprite::Create(textureHandle_, { 0,0 });

	selectFlag = 0;
}

void TitleScene::Update()
{
	changeFlag_ = false;
	if (input_->TriggerKey(DIK_SPACE)) {
		changeFlag_ = true;
		nextScene_ = Scene::MANUAL;
	}
	ChangeSelect();

	debugText_->SetPos(50, 330);
	debugText_->Printf("selectFlag:%d", selectFlag);
}

void TitleScene::Draw()
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
	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
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
