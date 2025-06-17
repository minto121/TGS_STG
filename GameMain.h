#pragma once
#include "AbstractScene.h"
#include"Player_Shot.h"
#include"demo_Player.h"
#include"Bullet.h"
#include "Enemy.h"

#include "Mob_Enemy.h"

class GameMain	:public AbstractScene
{
public:
    int nowtime;
    int currentPattern;
    bool isCKeyPressed;

    int UI_Img[6];
    int LifeImg;
    int BackGroundImg;

    int TextImg[13];    //数字の画像

    int score; //スコア

private:

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

public:
    //スコアの計算
    void Score_math();

    //スコアと数字画像を合わせ,描画する
    void DrawNumber(int x, int y, int value) const;
    int GetImageIndex(char ch) const;
};

