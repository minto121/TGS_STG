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
    void UpdateInput();   // ���͏���
    void Draw();          // �`��
    int GetSelected();    // ���肳�ꂽ�� 1�i���g���C�j�� 2�i�^�C�g���j�A������Ȃ� 0
};