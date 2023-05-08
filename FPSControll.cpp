#include "FPSControll.h"

void FPSControll::Initialize() {
	const float MIN_FREAM_TIME = 1.0f / 60;
	float frameTime = 0;

	float fps = 0;
}

void FPSControll::Update(LARGE_INTEGER timeStart, LARGE_INTEGER timeEnd, LARGE_INTEGER timeFreq) {
	//���̎��Ԃ��擾
	QueryPerformanceCounter(&timeEnd);
	//(���̎��� - �O�t���[���̎���) / ���g�� = �o�ߎ���(�b�P��)
	frameTime = static_cast<float>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<float>(timeFreq.QuadPart);

	if (frameTime < MIN_FREAM_TIME) {
		//�~���b�ɕϊ�
		DWORD sleepTime = static_cast<DWORD>((MIN_FREAM_TIME - frameTime) * 1000);

		//����\���グ��
		timeBeginPeriod(1);
		//�Q��
		Sleep(sleepTime);
		//�߂�
		timeEndPeriod(1);

		return;
	}

	if (frameTime > 0.0) {
		//����fps���v�Z
		fps = (fps * 0.99f) + (0.01f / frameTime);

		//�f�o�b�O�p
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

	//����ւ�
	timeStart = timeEnd;

}