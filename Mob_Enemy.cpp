#include "Mob_Enemy.h"



Mob_Enemy::Mob_Enemy()
{
    memset(enemy, 0, sizeof(enemy_t) * ENEMY_MAX);//敵データの初期化

    memset(enemy_order, 0, sizeof(enemy_order_t) * ENEMY_ORDER_MAX);

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
    int t = enemy[i].cnt;
    if (t == 0)
        enemy[i].vy = 2;//下がってくる
    if (t == 60)
        enemy[i].vy = 0;//止まる
    if (t == 60 + enemy[i].wait)//登録された時間だけ停滞して
        enemy[i].vy = -2;//上がっていく
}

void Mob_Enemy::enemy_enter()
{

    int i, j, t;
    for (t = 0; t < ENEMY_ORDER_MAX; t++) {
        if (enemy_order[t].cnt == stage_count) {//現在の瞬間がオーダーの瞬間なら
            if ((i = enemy_num_search()) != -1) {
                enemy[i].flag = 1;//フラグ
                enemy[i].cnt = 0;//カウンタ
                enemy[i].pattern = enemy_order[t].pattern;//移動パターン
                enemy[i].muki = 1;//向き
                enemy[i].knd = enemy_order[t].knd;//敵の種類
                enemy[i].x = enemy_order[t].x;//座標
                enemy[i].y = enemy_order[t].y;
                enemy[i].sp = enemy_order[t].sp;//スピード
                enemy[i].bltime = enemy_order[t].bltime;//弾の発射時間
                enemy[i].blknd = enemy_order[t].blknd;//弾幕の種類
                enemy[i].blknd2 = enemy_order[t].blknd2;//弾の種類
                enemy[i].col = enemy_order[t].col;//色
                enemy[i].wait = enemy_order[t].wait;//停滞時間
                enemy[i].hp = enemy_order[t].hp;//体力
                enemy[i].hp_max = enemy[i].hp;//体力最大値
                enemy[i].vx = 0;//水平成分の速度
                enemy[i].vy = 0;//鉛直成分の速度
                enemy[i].ang = 0;//角度
                for (j = 0; j < 6; j++)
                    enemy[i].item_n[j] = enemy_order[t].item_n[j];//落とすアイテム
            }
        }
    }
}

int Mob_Enemy::enemy_num_search()
{
    for (int i = 0; i < ENEMY_MAX; i++) {//フラグのたって無いenemyを探す
        if (enemy[i].flag == 0) {
            return i;//使用可能番号を返す
        }
    }
    return -1;//全部埋まっていたらエラーを返す
}

void Mob_Enemy::enemy_act()
{
    int i;
    for (i = 0; i < ENEMY_MAX; i++) {
        if (enemy[i].flag == 1) {//その敵のフラグがオンになってたら
            enemy_pattern0(i);
            enemy[i].x += cos(enemy[i].ang) * enemy[i].sp;
            enemy[i].y += sin(enemy[i].ang) * enemy[i].sp;
            enemy[i].x += enemy[i].vx;
            enemy[i].y += enemy[i].vy;
            enemy[i].cnt++;
            enemy[i].img = enemy[i].muki * 3 + (enemy[i].cnt % 18) / 6;
            //敵が画面から外れたら消す
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
    fp = FileRead_open(fname);//ファイル読み込み
    if (fp == NULL) {
        printfDx("read error\n");
        return;
    }
    for (i = 0; i < 2; i++)//最初の2行読み飛ばす
        while (FileRead_getc(fp) != '\n');

    n = 0, num = 0;
    while (1) {
        for (i = 0; i < 64; i++) {
            inputc[i] = input[i] = FileRead_getc(fp);//1文字取得する
            if (inputc[i] == '/') {//スラッシュがあれば
                while (FileRead_getc(fp) != '\n');//改行までループ
                i = -1;//カウンタを最初に戻して
                continue;
            }
            if (input[i] == ',' || input[i] == '\n') {//カンマか改行なら
                inputc[i] = '\0';//そこまでを文字列とし
                break;
            }
            if (input[i] == EOF) {//ファイルの終わりなら
                goto EXFILE;//終了
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

