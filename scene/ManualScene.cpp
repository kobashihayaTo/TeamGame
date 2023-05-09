#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

#include "ManualScene.h"

ManualScene::ManualScene() {}

ManualScene::~ManualScene() {
	delete sprite_;
}

void ManualScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	nextScene_ = Scene::MANUAL;

	//ファイル名指定してテクスチャを読み込む
	nameHandle_ = TextureManager::Load("manual_2.png");
	textureHandle_ = TextureManager::Load("manual_04.png");
	manumalHandle_ = TextureManager::Load("manual_05.png");
	manumalHandle_1 = TextureManager::Load("manual_03.png");

	//スプライトの生成
	sprite_ = Sprite::Create(nameHandle_, { 0,0 });
	sprite_2 = Sprite::Create(textureHandle_, { 0,0 });
	sprite_3= Sprite::Create(manumalHandle_, { 0,0 });
	sprite_4 = Sprite::Create(manumalHandle_1, { 0,0 });
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
		sprite_->Draw();
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