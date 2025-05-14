#include "Mob_Enemy.h"
#include"Mob_Enemy.h"

//円周率
#define PI 3.1415926535898
#define PI2 (PI*2)


Mob_Enemy::Mob_Enemy()
{
    memset(enemy, 0, sizeof(enemy_t) * ENEMY_MAX);//�G�f�[�^�̏�����

    memset(enemy_order, 0, sizeof(enemy_order_t) * ENEMY_ORDER_MAX);

    stage_count = 100;

    load_story();
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

    int t = enemy[i].cnt;

    switch (enemy[i].pattern)
    {
    case 0: //下がってきて停滞して上がっていく
        if (t == 0)
            enemy[i].vy = 3;//下がってくる
        if (t == 40)
            enemy[i].vy = 0;//止まる
        if (t == 40 + enemy[i].wait)//登録された時間だけ停滞して
            enemy[i].vy = -3;//上がっていく
        break;

    case 1: //下がってきて停滞して左下に行く
        if (t == 0)
            enemy[i].vy = 3;//下がってくる
        if (t == 40)
            enemy[i].vy = 0;//止まる
        if (t == 40 + enemy[i].wait) {//登録された時間だけ停滞して
            enemy[i].vx = -1;//左へ
            enemy[i].vy = 2;//下がっていく
            enemy[i].muki = 0;//左向きセット
        }
            break;

    case 2: //下がってきて停滞して右下に行く
        if (t == 0)
            enemy[i].vy = 3;//下がってくる
        if (t == 40)
            enemy[i].vy = 0;//止まる
        if (t == 40 + enemy[i].wait) {//登録された時間だけ停滞して
            enemy[i].vx = 1;//右へ
            enemy[i].vy = 2;//下がっていく
            enemy[i].muki = 2;//右向きセット
        }
        break;

    case 3: //すばやく降りてきて左へ
        if (t == 0)
            enemy[i].vy = 5;//下がってくる
        if (t == 30) {//途中で左向きに
            enemy[i].muki = 0;
        }
        if (t < 100) {
            enemy[i].vx -= 5 / 100.0;//左向き加速
            enemy[i].vy -= 5 / 100.0;//減速
        }
        break;

    case 4: //すばやく降りてきて右へ
        if (t == 0)
            enemy[i].vy = 5;//下がってくる
        if (t == 30) {//途中で右向きに
            enemy[i].muki = 2;
        }
        if (t < 100) {
            enemy[i].vx += 5 / 100.0;//右向き加速
            enemy[i].vy -= 5 / 100.0;//減速
        }
        break;

    case 5: //斜め左下へ
        if (t == 0) {
            enemy[i].vx -= 1;
            enemy[i].vy = 2;
            enemy[i].muki = 0;
        }
        break;

    case 6: //斜め右下へ
        if (t == 0) {
            enemy[i].vx += 1;
            enemy[i].vy = 2;
            enemy[i].muki = 2;
        }
        break;

    case 7: //停滞してそのまま左上に
        if (t == enemy[i].wait) {//登録された時間だけ停滞して
            enemy[i].vx = -0.7;//左へ
            enemy[i].vy = -0.3;//上がっていく
            enemy[i].muki = 0;//左向き
        }
        break;

    case 8: //停滞してそのまま右上に
        if (t == enemy[i].wait) {//登録された時間だけ停滞して
            enemy[i].vx = +0.7;//右へ
            enemy[i].vy = -0.3;//上がっていく
            enemy[i].muki = 2;//右向き
        }
        break;

    case 9: //停滞してそのまま上に
        if (t == enemy[i].wait)//登録された時間だけ停滞して
            enemy[i].vy = -1;//上がっていく
        break;

    case 10:    //下がってきてウロウロして上がっていく
        if (t == 0) enemy[i].vy = 4;//下がってくる
        if (t == 40)enemy[i].vy = 0;//止まる
        if (t >= 40) {
            if (t % 60 == 0) {
                int r = cos(enemy[i].ang) < 0 ? 0 : 1;
                enemy[i].sp = 6 + rang(2);
                enemy[i].ang = rang(PI / 4) + PI * r;
                enemy[i].muki = 2 - 2 * r;
            }
            enemy[i].sp *= 0.95;
        }
        if (t >= 40 + enemy[i].wait) {
            enemy[i].vy -= 0.05;
        }
        break;


    default:
        if (t == 0)
            enemy[i].vy = 2;//�������Ă���
        if (t == 60)
            enemy[i].vy = 0;//�~�܂�
        if (t == 60 + enemy[i].wait)//�o�^���ꂽ���Ԃ�����؂���
            enemy[i].vy = -2;//�オ���Ă���
        break;
        
    }
   
}

double Mob_Enemy::rang(double ang)
{
    return (-ang + ang * 2 * GetRand(10000) / 10000.0);
}

void Mob_Enemy::enemy_enter()
{

    int i, j, t;
    for (t = 0; t < ENEMY_ORDER_MAX; t++) {
        if (enemy_order[t].cnt == stage_count) {//���݂̏u�Ԃ��I�[�_�[�̏u�ԂȂ�
            if ((i = enemy_num_search()) != -1) {
                enemy[i].flag = 1;//�t���O
                enemy[i].cnt = 0;//�J�E���^
                enemy[i].pattern = enemy_order[t].pattern;//�ړ��p�^�[��
                enemy[i].muki = 1;//����
                enemy[i].knd = enemy_order[t].knd;//�G�̎��
                enemy[i].x = enemy_order[t].x;//���W
                enemy[i].y = enemy_order[t].y;
                enemy[i].sp = enemy_order[t].sp;//�X�s�[�h
                enemy[i].bltime = enemy_order[t].bltime;//�e�̔��ˎ���
                enemy[i].blknd = enemy_order[t].blknd;//�e���̎��
                enemy[i].blknd2 = enemy_order[t].blknd2;//�e�̎��
                enemy[i].col = enemy_order[t].col;//�F
                enemy[i].wait = enemy_order[t].wait;//��؎���
                enemy[i].hp = enemy_order[t].hp;//�̗�
                enemy[i].hp_max = enemy[i].hp;//�̗͍ő�l
                enemy[i].vx = 0;//���������̑��x
                enemy[i].vy = 0;//���������̑��x
                enemy[i].ang = 0;//�p�x
                for (j = 0; j < 6; j++)
                    enemy[i].item_n[j] = enemy_order[t].item_n[j];//���Ƃ��A�C�e��
            }
        }
    }
}

int Mob_Enemy::enemy_num_search()
{
    for (int i = 0; i < ENEMY_MAX; i++) {//�t���O�̂����Ė���enemy��T��
        if (enemy[i].flag == 0) {
            return i;//�g�p�\�ԍ���Ԃ�
        }
    }
    return -1;//�S�����܂��Ă�����G���[��Ԃ�
}

void Mob_Enemy::enemy_act()
{
    int i;
    for (i = 0; i < ENEMY_MAX; i++) {
        if (enemy[i].flag == 1) {//���̓G�̃t���O���I���ɂȂ��Ă���
            enemy_pattern0(i);
            enemy[i].x += cos(enemy[i].ang) * enemy[i].sp;
            enemy[i].y += sin(enemy[i].ang) * enemy[i].sp;
            enemy[i].x += enemy[i].vx;
            enemy[i].y += enemy[i].vy;
            enemy[i].cnt++;
            enemy[i].img = enemy[i].muki * 3 + (enemy[i].cnt % 18) / 6;
            //�G����ʂ���O�ꂽ�����
            if (enemy[i].x < -20 || 400 + 20 < enemy[i].x || enemy[i].y < -20 || 400 + 20 < enemy[i].y)
                enemy[i].flag = 0;
        }
    }

}

void Mob_Enemy::load_story()
{
    int n, num, i, fp;
    char fname[32] = { "Resource/date/mobenemy_date.csv" };
    int input[64];
    char inputc[64];
    fp = FileRead_open(fname);//�t�@�C���ǂݍ���
    if (fp == NULL) {
        printfDx("read error\n");
        return;
    }
    for (i = 0; i < 2; i++)//�ŏ���2�s�ǂݔ�΂�
        while (FileRead_getc(fp) != '\n');

    n = 0, num = 0;
    while (1) {
        for (i = 0; i < 64; i++) {
            inputc[i] = input[i] = FileRead_getc(fp);//1�����擾����
            if (inputc[i] == '/') {//�X���b�V���������
                while (FileRead_getc(fp) != '\n');//���s�܂Ń��[�v
                i = -1;//�J�E���^��ŏ��ɖ߂���
                continue;
            }
            if (input[i] == ',' || input[i] == '\n') {//�J���}�����s�Ȃ�
                inputc[i] = '\0';//�����܂ł𕶎���Ƃ�
                break;
            }
            if (input[i] == EOF) {//�t�@�C���̏I���Ȃ�
                goto EXFILE;//�I��
            }
        }
        switch (num) {
        case 0: enemy_order[n].cnt = atoi(inputc); break;
        case 1: enemy_order[n].pattern = atoi(inputc); break;
        case 2: enemy_order[n].knd = atoi(inputc); break;
        case 3: enemy_order[n].x = atof(inputc); break;
        case 4: enemy_order[n].y = atof(inputc); break;
        case 5: enemy_order[n].sp = atof(inputc); break;
        case 6: enemy_order[n].bltime = atoi(inputc); break;
        case 7: enemy_order[n].blknd = atoi(inputc); break;
        case 8: enemy_order[n].col = atoi(inputc); break;
        case 9: enemy_order[n].hp = atoi(inputc); break;
        case 10:enemy_order[n].blknd2 = atoi(inputc); break;
        case 11:enemy_order[n].wait = atoi(inputc); break;
        case 12:enemy_order[n].item_n[0] = atoi(inputc); break;
        case 13:enemy_order[n].item_n[1] = atoi(inputc); break;
        case 14:enemy_order[n].item_n[2] = atoi(inputc); break;
        case 15:enemy_order[n].item_n[3] = atoi(inputc); break;
        case 16:enemy_order[n].item_n[4] = atoi(inputc); break;
        case 17:enemy_order[n].item_n[5] = atoi(inputc); break;
        }
        num++;
        if (num == 18) {
            num = 0;
            n++;
        }
    }
EXFILE:
    FileRead_close(fp);
}

