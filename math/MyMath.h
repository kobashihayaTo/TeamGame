#pragma once
#include "Vector3.h"
#include "WorldTransform.h"

//�s��ƃx�N�g���̊|���Z
Vector3 Transform(Vector3& velocity, WorldTransform& worldTransform);

//�x�N�g���̒��������߂�
float Length(Vector3& vec);

//���K������
Vector3 Normalize(Vector3& vec);

//���W�Ԃ̋��������߂�
double CalculateDistance(Vector3& vec1, Vector3& vec2);

//�����m�̓����蔻��
bool SphereHit(Vector3& vec1, Vector3& vec2, double radius1, double radius2);

//�x���@���烉�W�A���ɕϊ�
float RadianConversion(float angle);