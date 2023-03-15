#pragma once

#include "Vector3.h"
#include <DirectXMath.h>
#include "Matrix4.h"
#include <d3d12.h>
#include <wrl.h>

// Microsoft::WRL::を省略
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
// DirectX::を省略
using XMFLOAT2 = DirectX::XMFLOAT2;
using XMFLOAT3 = DirectX::XMFLOAT3;
using XMFLOAT4 = DirectX::XMFLOAT4;
using XMMATRIX = DirectX::XMMATRIX;

// 定数バッファ用データ構造体
struct ConstBufferDataWorldTransform {
	Matrix4 matWorld;           // ローカル → ワールド変換行列
	XMFLOAT4 color = { 1,1,1,1 };
};

/// <summary>
/// ワールド変換データ
/// </summary>
struct WorldTransform {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataWorldTransform* constMap = nullptr;
	// ローカルスケール
	Vector3 scale_ = {1, 1, 1};
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = {0, 0, 0};
	// ローカル座標
	Vector3 translation_ = {0, 0, 0};
	// ローカル → ワールド変換行列
	Matrix4 matWorld_;
	// 親となるワールド変換へのポインタ
	const WorldTransform* parent_ = nullptr;



	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();

	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();
	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransferMatrix();

	XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
	void ColorSetter(XMFLOAT4& color) { this->color = color; }


	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransferColorMatrix() 
	{
		TransferMatrix();
		constMap->color = color;
	}

	// 色変更するための関数セッター
	// 色をシェーダーに転送する処理関数
};
