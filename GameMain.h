﻿#pragma once
#include "AbstractScene.h"
#include"Player_Shot.h"
#include"demo_Player.h"
#include"Bullet.h"
#include "Enemy.h"
#include "Result.h"
#include"Mob_Enemy.h"

class GameMain	:public AbstractScene
{
public:

    bool isCKeyPressed = false;        
    int nowtime;
    int currentPattern;

    int UI_Img[6];
    int LifeImg;
    int BackGroundImg;
    int EnemyPhase;
    int GameMain_BGM;
    const int MaxEnemyPhase = 2;  // 0,1,2 の 3フェーズ

    int clearTimer = 0;            // クリア演出のタイマー
    bool clearBulletStopped = false; // 弾を止めたかどうか

    bool isGameOver = false;      // ゲームオーバー中かどうか
    int gameOverTimer = 0;        // ゲームオーバー経過時間（フレーム）


    Result* result = nullptr;
private:

    bool isGameClear = false;      // ゲームクリア状態
public:
    //�R���X�g���N�^
    GameMain();

    //�f�X�g���N�^
    ~GameMain();

    //�`��ȊO�̍X�V����s
    AbstractScene* Update() override;

    //�`��Ɋւ��邱�Ƃ����
    void Draw() const override;

    Player_Shot* P_SHOT;
    demo_Player* D_PLAYER;
    Bullet* BULLET_DATE;

    Enemy* enemy;

    Mob_Enemy* MOB_ENEMY;
};

