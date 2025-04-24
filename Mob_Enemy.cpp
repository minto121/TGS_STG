#include "Mob_Enemy.h"



Mob_Enemy::Mob_Enemy()
{
    stage_count = 100;
	
}

Mob_Enemy::~Mob_Enemy()
{
    
}

void Mob_Enemy::Update()
{
    enemy_enter();
    stage_count += 1;
    enemy_act();
}

void Mob_Enemy::Draw() const
{
    for (int i = 0; i < ENEMY_MAX; i++) {
        if (enemy[i].flag == 1) {
            DrawBox(enemy[i].x - 10, enemy[i].y - 10, enemy[i].x + 10, enemy[i].y + 10,0xff00ff, TRUE);
        }
    }
}


void Mob_Enemy::enemy_pattern0(int i)
{
    if (enemy[i].cnt < 60) {
        enemy[i].y += 2.0;
    }
    if (enemy[i].cnt > 60 + 240) {
        enemy[i].y -= 2.0;
    }
}

void Mob_Enemy::enemy_enter()
{

	if(stage_count==100){//ƒQ[ƒ€‚ªn‚Ü‚Á‚Ä100ƒJƒEƒ“ƒg‚Å“o˜^
        enemy[0].cnt    =0;
        enemy[0].muki   =1;
        enemy[0].flag   =1;
        enemy[0].bltime =150;
        enemy[0].hp     =1000;
        enemy[0].hp_max =enemy[0].hp;
        enemy[0].pattern=0;
        enemy[0].x      =100;
        enemy[0].y      =200;
    }
}

void Mob_Enemy::enemy_act()
{
    int i;
    for (i = 0; i < ENEMY_MAX; i++) {
        if (enemy[i].flag == 1) {//‚»‚Ì“G‚Ìƒtƒ‰ƒO‚ªƒIƒ“‚É‚È‚Á‚Ä‚½‚ç
            enemy_pattern0(i);
            enemy[i].cnt++;
            enemy[i].img = enemy[i].muki * 3 + (enemy[i].cnt % 18) / 6;
            //“G‚ª‰æ–Ê‚©‚çŠO‚ê‚½‚çÁ‚·
            if (enemy[i].x < -50 || 200 + 50 < enemy[i].x || enemy[i].y < -50 || 600 + 50 < enemy[i].y)
                enemy[i].flag = 0;
        }
    }

}
