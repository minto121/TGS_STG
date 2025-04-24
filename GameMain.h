#pragma once
#include "AbstractScene.h"
#include"Player_Shot.h"
#include"demo_Player.h"
#include"Bullet.h"
#include "Enemy.h"

class GameMain	:public AbstractScene
{
public:

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

};

