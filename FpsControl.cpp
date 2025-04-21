#include "FpsControl.h"
#include <math.h>
#include "DxLib.h"
static int mStartTime;      //����J�n����
static int mCount;          //�J�E���^
static float mFps;          //fps
static const int N = 60;  //���ς����T���v����
static const int FPS = 60;  //�ݒ肵��FPS
static int mFrameCount = 0;

static int mPrevTime = 0;
static float mDeltaTime = 1.0f / FPS;  // �����l��1/FPS�b

float FpsControl_GetDeltaTime(){
    return mDeltaTime;
}
//������
void FpsControl_Initialize() {
    mStartTime = GetNowCount();
    mCount = 0;
    mFps = 0;
}
//FPS����
bool FpsControl_Update() {

    int now = GetNowCount();
    //if (mCount == 0) {
    //    mStartTime = now;
    //    mPrevTime = now;
    //}

    // �f���^�^�C���v�Z�i�~���b �� �b�j
    mDeltaTime = (now - mPrevTime) / 1000.0f;
    mPrevTime = now;

    if (mCount == 0) { //1�t���[���ڂȂ玞�����L��
        mStartTime = GetNowCount();
    }
    if (mCount == N) { //60�t���[���ڂȂ畽�ς��v�Z����
        int t = GetNowCount();
        mFps = 1000.f / ((t - mStartTime) / (float)N);
        mCount = 0;
        mStartTime = t;
    }
    mCount++;
    mFrameCount++;  // �� ��ɉ��Z
    return true;
}
//FPS�\��
void FpsControl_Draw() {
     DrawFormatString(0, 20, GetColor(255, 255, 255), "%.1f", mFps);
     DrawFormatString(0, 40, GetColor(200, 200, 200), "DeltaTime: %.4f", FpsControl_GetDeltaTime());
}
void FpsControl_Wait() {
    int tookTime = GetNowCount() - mStartTime;  //������������
    int waitTime = mCount * 1000 / FPS - tookTime;  //�҂ׂ�����
    if (waitTime > 0) {
        Sleep(waitTime);  //�ҋ@
    }
}

int FpsControl_GetNowFrame() {
    return mFrameCount;
}



