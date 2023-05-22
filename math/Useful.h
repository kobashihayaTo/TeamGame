#pragma once
#pragma once
#include "Matrix4.h"
#include "WorldTransform.h"
#include "math.h"
#include "MathUtility.h"

namespace useful {

	enum AXIS_NO {
		AXIS_NONE,	//0
		AXIS_X,		//1
		AXIS_Y,		//2
		AXIS_XY,	//3
		AXIS_Z,		//4
		AXIS_XZ,	//5
		AXIS_YZ,	//6
		AXIS_XYZ	//7
	};

	typedef struct UsefulMat {
		Matrix4 scale = MathUtility::Matrix4Identity();
		Matrix4 rotateX = MathUtility::Matrix4Identity();
		Matrix4 rotateY = MathUtility::Matrix4Identity();
		Matrix4 rotateZ = MathUtility::Matrix4Identity();
		Matrix4 translate = MathUtility::Matrix4Identity();
	};

	//(1)

	//(1)-1 設定する関数
	void SetScaleMat(Matrix4& mat, WorldTransform& worldTransform);
	void SetRotateMat(UsefulMat& mat, WorldTransform& worldTransform);
	void SetTranslateMat(Matrix4& mat, WorldTransform& worldTransform);

	//(2)-2 行列を使って変換した値を返す関数
	Matrix4 GenerateScaleMat(WorldTransform& worldTransform);

	Matrix4 GenerateRotateXMat(WorldTransform& worldTransform);
	Matrix4 GenerateRotateYMat(WorldTransform& worldTransform);
	Matrix4 GenerateRotateZMat(WorldTransform& worldTransform);

	Matrix4 GenerateTransMat(WorldTransform& worldTransform);

	//(2) ワールド行列を生成する関数
	void SetTransformationWolrdMat(UsefulMat& usefulMat, WorldTransform& worldTransform);

	Vector3 MatVector(Vector3 v, Matrix4 mat);
}