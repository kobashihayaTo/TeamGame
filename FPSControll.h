#pragma once

#include <Windows.h>
#include <tchar.h>
#include <sstream>
#include <mmsystem.h>
#include "DebugText.h"

class FPSControll
{
public: //メンバ関数

	//初期化
	void Initialize();

	//更新
	void Update(LARGE_INTEGER timeStart, LARGE_INTEGER timeEnd, LARGE_INTEGER timeFreq);

public: //メンバ変数

	//デバッグテキスト
	DebugText* debugText_ = nullptr;

	const float MIN_FREAM_TIME = 1.0f / 60;
	float frameTime = 0;

	float fps = 0;

};

