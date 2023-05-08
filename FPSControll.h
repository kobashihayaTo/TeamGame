#pragma once

#include <Windows.h>
#include <tchar.h>
#include <sstream>
#include <mmsystem.h>
#include "DebugText.h"

class FPSControll
{
public: //�����o�֐�

	//������
	void Initialize();

	//�X�V
	void Update(LARGE_INTEGER timeStart, LARGE_INTEGER timeEnd, LARGE_INTEGER timeFreq);

public: //�����o�ϐ�

	//�f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;

	const float MIN_FREAM_TIME = 1.0f / 60;
	float frameTime = 0;

	float fps = 0;

};

