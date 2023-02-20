#pragma once
#pragma once
#include"Useful.h"
#include<cmath>
#include"WorldTransform.h"

class MyFunc {
public:

	void UpdateWorldTransform(WorldTransform& worldTransform);



};

Vector3 Vector3s(Vector3& v1, Vector3& v2);

float length(Vector3& vec);

/// <summary>
/// �X�P�[�����O�s��̍쐬
/// </summary>
/// <param name="scale">WorldTransform��scale_</param>
/// <returns>�쐬�����X�P�[�����O�s��</returns>
Matrix4 CreateMatScale(Vector3 scale);

Vector3 CreateVector(Vector3 velocity, WorldTransform& worldTransform);

/// <summary>
/// �P�ʍs��쐬
/// </summary>
/// <returns>�P�ʍs��</returns>
Matrix4 CreateMatIdentity();


/// <summary>
/// Z���̉�]�s��쐬
/// </summary>
/// <param name="rotation">WorldTransform��rotation_</param>
/// <returns>Z������̉�]�s��</returns>
Matrix4 CreateMatRotationZ(Vector3 rotation);

/// <summary>
/// X���̉�]�s��쐬
/// </summary>
/// <param name="rotation">WorldTransform��rotation_</param>
/// <returns>X���̉�]�s��</returns>
Matrix4 CreateMatRotationX(Vector3 rotation);


/// <summary>
/// Y���̉�]�s��
/// </summary>
/// <param name="rotation">WorldTransform��rotation_</param>
/// <returns>Y���̉�]�s��</returns>
Matrix4 CreateMatRotationY(Vector3 rotation);


/// <summary>
/// ���s�ړ�
/// </summary>
/// <param name="translation">WorldTransform��Translation_</param>
/// <returns>���s�ړ�</returns>
Matrix4 CreateMatTranslation(Vector3 translation);

Vector3 bvector(Vector3& velocity, Matrix4& mat);



void CreateMatrixUpdate(WorldTransform& worldTransform_);



Vector3 Normalize(Vector3& vec);

