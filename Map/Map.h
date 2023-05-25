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
		MIDDLEGOAL1, //4
		MIDDLEGOAL2 //5
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

	void WallDraw(ViewProjection& viewProjection);

	//���̕`��
	void FloorDraw(ViewProjection& viewProjection);

	//�u���b�N�T�C�Y���擾
	int GetBlockSize() { return blockSize; }

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

	int GetMapFlag() { return mapFlag; }

	bool GetGoalFlag() { return goalFlag; }

	bool GetAnswerFlag() { return answerFlag; }

	bool GetOKFlag() { return OKFlag; }

	void Reset();

	bool GetEffectOffFlag() { return effectOffFlag; }

	bool GetFlag() { return goalReadyFlag; }

	int GetGoalCount() { return goalCount; }

	int GetGoalMidCount1() { return goalMidCount1; }
	int GetGoalMidCount2() { return goalMidCount2; }


	bool GetUIFlag() { return UIFlag; }
	bool GetTestFlag() { return testFlag; }

	int GetGoal_() { return goal_; }

	bool GetResetFlag() { return resetFlag; }

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
		{1,0,0,0,0,0,0,3,2,2,0,0,2,2,0,0,0,0,2,2,2,2,0,0,1},
		{1,0,0,0,0,0,0,0,2,2,0,0,2,2,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,2,2,2,2,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,2,2,2,2,0,0,0,0,2,2,2,2,2,2,0,0,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,0,0,2,2,2,2,2,2,2,2,0,0,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,0,0,2,2,2,2,2,2,0,0,0,0,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,2,2,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,1},
		{1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,1},
		{1,0,0,2,0,0,0,0,0,0,0,0,0,5,0,0,0,0,2,2,2,2,0,0,1},
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
		{1,0,0,0,0,0,0,2,2,2,2,0,0,0,2,2,0,0,0,0,2,2,0,0,1},
		{1,0,0,2,2,2,2,2,2,2,0,0,0,2,2,2,2,0,0,0,2,2,0,0,1},
		{1,0,0,2,2,2,2,2,2,2,0,0,0,2,2,2,2,0,0,0,2,2,0,0,1},
		{1,0,0,2,2,2,2,0,0,0,0,0,0,0,2,2,0,0,0,0,2,2,0,0,1},
		{1,0,0,2,2,2,2,4,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,1},
		{1,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};




private:	//�����o�ϐ�
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_[40][25] = {};
	int blockSize;
	WorldTransform floorWorldTransform_;
	WorldTransform effectWorldTrans;

	//����
	Input* input_ = nullptr;
	//���f��
	Model* model_ = nullptr;
	Model* effectModel = nullptr;

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
	int mapFlag = 0;

	//�����t���O
	bool answerFlag = false;
	bool answerIntervalFlag = false;
	int  answerTimer = 100;
	int  answerIntervalTimer = 100;

	bool goalFlag = false;
	bool goalReadyFlag = false;
	//
	float goalCount = 50;
	float goalMidCount1 = 50;
	float goalMidCount2 = 50;

	int goalCounter = 0;

	int map_Z = 40;
	int map_X = 25;

	bool OKFlag = false;
	bool effectOffFlag = false;

	int playerTimer = 0;

	bool UIFlag = false;

	bool testFlag = false;

	int goal_ = 0;

	bool resetFlag = false;
};