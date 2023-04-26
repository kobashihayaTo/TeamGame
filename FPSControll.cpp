#include "FPSControll.h"

void FPSControll::Initialize() {
	const float MIN_FREAM_TIME = 1.0f / 60;
	float frameTime = 0;

	float fps = 0;
}

void FPSControll::Update(LARGE_INTEGER timeStart, LARGE_INTEGER timeEnd, LARGE_INTEGER timeFreq) {
	//今の時間を取得
	QueryPerformanceCounter(&timeEnd);
	//(今の時間 - 前フレームの時間) / 周波数 = 経過時間(秒単位)
	frameTime = static_cast<float>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<float>(timeFreq.QuadPart);

	if (frameTime < MIN_FREAM_TIME) {
		//ミリ秒に変換
		DWORD sleepTime = static_cast<DWORD>((MIN_FREAM_TIME - frameTime) * 1000);

		//分解能を上げる
		timeBeginPeriod(1);
		//寝る
		Sleep(sleepTime);
		//戻す
		timeEndPeriod(1);

		return;
	}

	if (frameTime > 0.0) {
		//平均fpsを計算
		fps = (fps * 0.99f) + (0.01f / frameTime);

		//デバッグ用
#ifdef _DEBUG
#ifdef UNICODE
		std::wstringstream stream;
#else
		std::stringstream stream;
#endif // UNICODE
		stream << fps << "FPS" << std::endl;

		OutputDebugString(stream.str().c_str());
#endif // _DEBUG

	}

	//入れ替え
	timeStart = timeEnd;

}