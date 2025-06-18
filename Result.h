#pragma once
#include <DxLib.h>

enum class ResultOption {
    Retry,
    Title
};

class Result
{
private:
    ResultOption selectedOption = ResultOption::Retry;
    bool decisionMade = false;
    int inputCooldown = 0;

public:
    void UpdateInput();   // 入力処理
    void Draw();          // 描画
    int GetSelected();    // 決定されたら 1（リトライ）か 2（タイトル）、未決定なら 0
};