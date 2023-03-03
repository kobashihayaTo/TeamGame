#include "RailCamera.h"
#include "math/MyMath.h"
#include "Function/Function.h"

void RailCamera::Initialize(Vector3& position, Vector3& rotation, Player* player) {
	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールドトランスフォームの初期設定
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;

	//ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.eye = { 0,40.0f,-15.0f };
	viewProjection_.Initialize();

	newPlayer = player;
}

void RailCamera::Update() {

	ZoomIn();
#ifdef _DEBUG
	//デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf("RailCamera pos:(%f, %f, %f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	debugText_->SetPos(50, 270);
	debugText_->Printf("viewtarget:(%f, %f, %f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);

#endif
}

void RailCamera::ZoomIn() {
	//ワールドトランスフォームの座標の数値を加減算する(移動)
	Vector3 cameraMove = { 0.0f,0.0f,0.0f };


	
	//カメラの位置の移動処理
	if (input_->PushKey(DIK_W)) {
		cameraMove.y += 0.1;
	}

	if (input_->PushKey(DIK_S)) {
		cameraMove.y -= 0.1;
	}

	if (input_->PushKey(DIK_D)) {
		cameraMove.z += 0.1;
	}

	if (input_->PushKey(DIK_A)) {
		cameraMove.z -= 0.1;
	}

	if (input_->PushKey(DIK_E)) {
		cameraMove.x += 0.1;
	}

	if (input_->PushKey(DIK_Q)) {
		cameraMove.x -= 0.1;
	}


	worldTransform_.translation_ += cameraMove;
	////行列の更新
	myFunc_.UpdateWorldTransform(worldTransform_);

	//カメラ視点座標を設定
	viewProjection_.eye += cameraMove;

	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	//レールカメラの回転を判定
	//forward = newPlayer->GetTransform();
	//視点から前方に適当な距離進んだ位置が注視点
	//forward += viewProjection_.eye;
	viewProjection_.target = { newPlayer->Getmove().x,newPlayer->Getmove().y,newPlayer->Getmove().z + 1 };

	////ワールド上方ベクトル
	//Vector3 up(0, 1, 0);
	////レールカメラの回転を反映(レールカメラの上方ベクトル)
	//viewProjection_.up = Transform(up, worldTransform_);

	//上方向の回転の速さ
	const float kUpRotSpeed = 0.05f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_SPACE)) {
		viewAngle += kUpRotSpeed;
		//2πを超えたら0に戻す
		viewAngle = fmodf(viewAngle, 3.14 * 2.0f);
	}

	//上方向ベクトルを計算(半径１の円周上の座標)
	viewProjection_.up = { 0.0f, 1.0f, 0.0f };

	//ビュープロジェクションを更新
	viewProjection_.UpdateMatrix();
}

void RailCamera::PlayerCamera()
{
	////ワールド前方ベクトル
	//Vector3 forward(0, 0, 1);
	////レールカメラの回転を判定
	////forward = newPlayer->GetTransform();
	////視点から前方に適当な距離進んだ位置が注視点
	////forward += viewProjection_.eye;
	//viewProjection_.target.y = newPlayer->GetTransform().y;
	//viewProjection_.target.y=


	////ビュープロジェクションを更新
	//viewProjection_.UpdateMatrix();
}
