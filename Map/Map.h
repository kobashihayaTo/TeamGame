#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Function/Function.h"
#include <cassert>
#include "DebugText.h"
#include"Input.h"
#include "MathUtility.h"
#include "ViewProjection.h"
#include <Player/Player.h>
#include <Enemy/Enemy.h>
#include "Enum.h"

class Map
{
public:		//Enum
	enum Mapinfo {
		NONE, //0
		WALL, //1
		BLOCK,//2
		GOAL,  //3	
		RELAY, //4
		ECHIGO //5
	};

public:		//�����o�֐�

	Map();

	~Map();

	//������
	void Initialize(Model* model, Model* floorModel, Model* effectmodel_, Model* afterModel, Model* goalModel);

	//�X�V
	void Update(Player* player, bool MapkeyFlag);

	//�`��
	void Draw(ViewProjection& viewProjection);

	//���̕`��
	void FloorDraw(ViewProjection& viewProjection);

	//�u���b�N�T�C�Y���擾
	int GetBlockSize() { return BlockSize; }

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision(Vector3 playerPos, float radius);


	/// <summary>
	/// �}�b�v��̃u���b�N�ɓ������Ă��邩�`�F�b�N
	/// </summary>
	/// <param name="line"></param>
	/// <param name="culumn"></param>
	/// <returns></returns>
	void EnemyBlockCheck(Enemy* enemy);

	void PlayerBlockCheck(Player* player);

	bool CheckCollision(Vector3 pos1, Vector3 pos2, float radius1, float radius2);

	int GetMapFlag() { return MapFlag; }

	bool GetGoalFlag() { return goalFlag; }

	bool GetAnswerFlag() { return AnswerFlag; }

	bool GetOKFlag() { return OKFlag; }

	void Reset();

	bool GeteffectOffFlag() { return effectOffFlag; }

	bool GetFlag() { return goalReadyFlag; }

	int GetGoalCount() { return GoalCount; }
	int GetGoalRELAYCount() { return GoalRELAYCount; }
	int GetGoalECHIGOCount() { return GoalECHIGOCount; }


	bool GetUIFlag() { return UIFlag; }
	bool GetTestFlag() { return testFlag; }


public:
	//���a
	const float radius = 0.9f;

public:		//�}�b�v

	//���X�e�[�W
	int FirstMap[40][25]{
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,2,2,0,0,2,2,0,0,2,2,2,2,2,2,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,0,0,2,2,0,0,0,0,2,2,2,2,2,2,0,0,1},
		{1,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,1},
		{1,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,1},
		{1,0,0,0,0,0,0,5,2,2,0,0,2,2,0,0,0,0,2,2,2,2,0,0,1},
		{1,0,0,0,0,0,0,0,2,2,0,0,2,2,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,2,2,2,2,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,2,2,2,2,0,0,0,0,2,2,2,2,2,2,0,0,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,0,0,2,2,2,2,2,2,2,2,0,0,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,0,0,2,2,2,2,2,2,0,0,0,0,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,2,2,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,1},
		{1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,1},
		{1,0,0,2,0,0,0,0,0,0,2,2,2,4,0,0,0,0,2,2,2,2,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,2,2,0,0,1},
		{1,0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,2,2,0,0,1},
		{1,0,0,0,0,2,2,2,2,2,2,2,2,0,0,2,2,0,0,0,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,2,2,2,2,2,0,0,2,2,2,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,1},
		{1,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,1},
		{1,0,0,2,2,0,0,0,0,2,2,2,2,0,0,0,2,2,0,0,2,2,0,0,1},
		{1,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,2,2,0,0,1},
		{1,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,1},
		{1,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,2,2,2,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,2,2,2,2,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,2,2,2,2,0,0,0,0,2,2,0,0,0,2,2,0,0,1},
		{1,0,0,2,2,2,2,2,2,2,0,0,0,0,2,2,2,2,0,0,2,2,0,0,1},
		{1,0,0,2,2,2,2,2,2,2,0,0,0,0,2,2,2,2,0,0,2,2,0,0,1},
		{1,0,0,2,2,2,2,0,0,0,0,0,0,0,0,2,2,0,0,0,2,2,0,0,1},
		{1,0,0,2,2,2,2,3,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};




private:	//�����o�ϐ�
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_[40][25] = {};
	int BlockSize;
	WorldTransform floorWorldTransform_;
	WorldTransform effectworldTrans;

	//����
	Input* input_ = nullptr;
	//���f��
	Model* model_ = nullptr;
	Model* effectmodel = nullptr;

	//���̃��f��
	Model* floorModel_ = nullptr;

	Model* afterModel_ = nullptr;

	//�S�[���̃��f��
	Model* goalModel_ = nullptr;

	//�f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;

	//�t�@���N�V����
	MyFunc myFunc_;

	Player* player_ = nullptr;

	//�}�b�v�t���O
	int MapFlag = 0;

	//�����t���O
	bool AnswerFlag = false;
	bool AnswerIntervalFlag = false;
	int  AnswerTimer = 100;
	int  AnswerIntervalTimer = 100;

	bool goalFlag = false;
	bool goalReadyFlag = false;
	//
	int GoalCount = 50;
	int GoalRELAYCount = 50;
	int GoalECHIGOCount = 50;

	int goalcount = 0;

	int Map_Z = 40;
	int Map_X = 25;

	bool OKFlag = false;
	bool effectOffFlag = false;

	int playerTimer = 0;

	bool UIFlag = false;

	bool testFlag = false;

	int goal_ = 0;
};