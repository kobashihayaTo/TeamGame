#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

#include "ManualScene.h"

ManualScene::ManualScene() {}

ManualScene::~ManualScene() {
	delete sprite_1;
	delete sprite_2;
	delete sprite_3;
	delete sprite_4;
}

void ManualScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	nextScene_ = Scene::MANUAL;

	//ファイル名指定してテクスチャを読み込む
	menuHandle_1 = TextureManager::Load("Manual/manual_01.png");
	menuHandle_2 = TextureManager::Load("Manual/manual_02.png");
	menuHandle_3 = TextureManager::Load("Manual/manual_03.png");
	menuHandle_4 = TextureManager::Load("Manual/manual_04.png");

	//スプライトの生成
	sprite_1 = Sprite::Create(menuHandle_1, { 0,0 });
	sprite_2 = Sprite::Create(menuHandle_2, { 0,0 });
	sprite_3= Sprite::Create(menuHandle_3, { 0,0 });
	sprite_4 = Sprite::Create(menuHandle_4, { 0,0 });
}

void ManualScene::Update()
{


	if (input_->TriggerKey(DIK_SPACE)) {
		selectFlag += 1;
	}

	if (selectFlag == 4) {
		manualFlag_ = true;
		nextScene_ = Scene::GAME;
	}
	ChangeSelect();
	debugText_->SetPos(50, 100);
	debugText_->Printf("selectFlag:%d", selectFlag);

}

void ManualScene::Reset()
{

	selectFlag = 0;
	manualFlag_ = false;
}

void ManualScene::Draw()
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
	if (selectFlag == 0)
	{
		sprite_1->Draw();
	}
	if (selectFlag == 1)
	{
		sprite_2->Draw();
	}
	if (selectFlag == 2)
	{
		sprite_3->Draw();
	}
	if (selectFlag == 3)
	{
		sprite_4->Draw();
	}

#ifdef _DEBUG

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
#endif  

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void ManualScene::ChangeSelect()
{

}