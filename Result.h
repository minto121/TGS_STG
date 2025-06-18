#pragma once
#include <DxLib.h>
#include "AbstractScene.h"

enum class ResultOption {
    Retry,
    Title
};

class Result : 
    public AbstractScene
{
private:
    ResultOption selectedOption = ResultOption::Retry;
    bool decisionMade = false;
    int inputCooldown = 0;

public:
    Result();
    ~Result();

    AbstractScene* Update()override;
    void Draw() const override;
    void UpdateInput();   // 入力処理
    void Draw();          // 描画
    int GetSelected();    // 決定されたら 1（リトライ）か 2（タイトル）、未決定なら 0
};