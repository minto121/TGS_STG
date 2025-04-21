#include "FpsControl.h"
#include <math.h>
#include "DxLib.h"
static int mStartTime;      //測定開始時刻
static int mCount;          //カウンタ
static float mFps;          //fps
static const int N = 60;  //平均を取るサンプル数
static const int FPS = 60;  //設定したFPS
static int mFrameCount = 0;

static int mPrevTime = 0;
static float mDeltaTime = 1.0f / FPS;  // 初期値は1/FPS秒

float FpsControl_GetDeltaTime(){
    return mDeltaTime;
}
//初期化
void FpsControl_Initialize() {
    mStartTime = GetNowCount();
    mCount = 0;
    mFps = 0;
}
//FPS制御
bool FpsControl_Update() {

    int now = GetNowCount();
    //if (mCount == 0) {
    //    mStartTime = now;
    //    mPrevTime = now;
    //}

    // デルタタイム計算（ミリ秒 → 秒）
    mDeltaTime = (now - mPrevTime) / 1000.0f;
    mPrevTime = now;

    if (mCount == 0) { //1フレーム目なら時刻を記憶
        mStartTime = GetNowCount();
    }
    if (mCount == N) { //60フレーム目なら平均を計算する
        int t = GetNowCount();
        mFps = 1000.f / ((t - mStartTime) / (float)N);
        mCount = 0;
        mStartTime = t;
    }
    mCount++;
    mFrameCount++;  // ← 常に加算
    return true;
}
//FPS表示
void FpsControl_Draw() {
     DrawFormatString(0, 20, GetColor(255, 255, 255), "%.1f", mFps);
     DrawFormatString(0, 40, GetColor(200, 200, 200), "DeltaTime: %.4f", FpsControl_GetDeltaTime());
}
void FpsControl_Wait() {
    int tookTime = GetNowCount() - mStartTime;  //かかった時間
    int waitTime = mCount * 1000 / FPS - tookTime;  //待つべき時間
    if (waitTime > 0) {
        Sleep(waitTime);  //待機
    }
}

int FpsControl_GetNowFrame() {
    return mFrameCount;
}



