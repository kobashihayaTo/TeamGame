#include "Player.h"
#include <cassert>

//初期化
void Player::Initialize(Model* model) {
	//NULLポイントチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記憶する
	model_ = model;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//プレイヤーの大きさ(倍率)
	worldTransform_.scale_ = { 2.0f,2.0f,2.0f };
	//プレイヤーの初期位置の設定
	worldTransform_.translation_ = { -11.0f, 0.0f, -18.0f };


}

//更新
void Player::Update() {
	//プレイヤーの移動ベクトル


	prePosition_ = worldTransform_.translation_;

	//プレイヤー移動処理
	//上下
	if (input_->PushKey(DIK_UP)) {
		worldTransform_.translation_.z += playerSpeedZ_;
	}
	if (input_->PushKey(DIK_DOWN)) {
		worldTransform_.translation_.z -= playerSpeedZ_;
	}
	//左右
	if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.translation_.x += playerSpeedX_;
	}
	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.translation_.x -= playerSpeedX_;
	}

	move = worldTransform_.translation_;

	//行列の更新
	myFunc_.UpdateWorldTransform(worldTransform_);

	//デバッグ用表示
	debugText_->SetPos(50, 150);
	debugText_->Printf("Player pos:(%f, %f, %f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

//描画
void Player::Draw(ViewProjection& viewprojection) {
	//3Dモデルを描画
	model_->Draw(worldTransform_, viewprojection);
}

Vector3 Player::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;

}

void Player::OnCollision() {
	//デスフラグを立てる
	playerDeathFlag = true;
}

void Player::Reset()
{
	playerDeathFlag = false;
	//プレイヤーの初期位置の設定
	worldTransform_.translation_ = { -11.0f,0.0f ,-18.0f };
}
	//何もしない

/*
	プレイヤーのマップ上のどこにいるかが欲しいためマップ番号を取得できるようにしたい
	マップ番号取得出来たら何行何列かが分かる
	map[pNumZ][pNumX] == BLOXK;で現在地がブロックかどうか判断できる
	ただmapを持っているのがMapクラスになるのでmapクラスにプレイヤーのマップ番号を渡してブロックかどうかをチェックしてもらう

	マップ番号は何行何列となるため二つの整数の値である
	これらをまとめてreturnしたい
	じゃあどうする？
	構造体を使って行列をまとめる

*/

//左上の座標を取得
CornerPos Player::LeftTopNum() {
	CornerPos cornerPos;

	cornerPos.cornerZ = worldTransform_.translation_.z - radius;
	cornerPos.cornerX = worldTransform_.translation_.x - radius;

	return cornerPos;
}

//右上の座標を取得
CornerPos Player::RightTopNum() {
	CornerPos cornerPos;

	cornerPos.cornerZ = worldTransform_.translation_.z - radius;
	cornerPos.cornerX = worldTransform_.translation_.x + radius;

	return cornerPos;
}

//左下の座標を取得
CornerPos Player::LeftBottomNum() {
	CornerPos cornerPos;

	cornerPos.cornerZ = worldTransform_.translation_.z + radius;
	cornerPos.cornerX = worldTransform_.translation_.x - radius;

	return cornerPos;
}

//右下の座標を取得
CornerPos Player::RightBottomNum() {
	CornerPos cornerPos;

	cornerPos.cornerZ = worldTransform_.translation_.z + radius;
	cornerPos.cornerX = worldTransform_.translation_.x + radius;

	return cornerPos;
}