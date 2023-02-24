#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "DebugText.h"
#include "Input.h"
#include "Function/Function.h"



class RailCamera
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">初期座標</param>
	/// <param name="rotation">初期角度</param>
	void Initialize(Vector3& position, Vector3& rotation);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void ZoomOut();

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns></returns>
	ViewProjection& GetViewProjection() { return viewProjection_; }

	//ワールド行列を取得
	WorldTransform* GetWorldMatrix() { return &worldTransform_; }

	//セッター
	void SetWorldPosition(Vector3 worldPosition) { worldTransform_.translation_ = worldPosition; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグテキスト
	DebugText* debugText_ = nullptr;

	//入力処理するため
	Input* input_ = nullptr;

	//ファンクション
	MyFunc myFunc_;


	//カメラ上方向の角度
	float viewAngle = 0.0f;

};
