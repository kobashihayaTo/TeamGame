#include "Enemy.h"
#include <cassert>

//初期化
void Enemy::Initialize(Model* model) {
	//NULLポイントチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記憶する
	model_ = model;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//敵の大きさ(倍率)
	worldTransform_.scale_ = { 0.5f,0.5f,0.5f };
	//敵の初期位置の設定
	worldTransform_.translation_ = { 6.0f, 0.9f, -2.7f };
}

//更新
void Enemy::Update() {
	//プレイヤーの移動ベクトル
	Vector3 move = { 0,0,0 };

	if (worldTransform_.translation_.x <= -11)
	{
		LightFlag = false;
		LeftFlag = true;
	}
	if (worldTransform_.translation_.x >= 6)
	{
		LeftFlag = false;
		LightFlag = true;
	}

	if (LeftFlag == true)
	{
		worldTransform_.translation_.x+=0.1;
	}
	if (LightFlag == true)
	{
		worldTransform_.translation_.x-=0.1;
	}
	//行列の更新
	myFunc_.UpdateWorldTransform(worldTransform_);

	//デバッグ用表示
	debugText_->SetPos(50, 180);
	debugText_->Printf("Enemy pos:(%f, %f, %f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

//描画
void Enemy::Draw(ViewProjection& viewprojection) {
	//3Dモデルを描画
	model_->Draw(worldTransform_, viewprojection);
}

Vector3 Enemy::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;

}

void Enemy::OnCollision() {
	//何もしない
}