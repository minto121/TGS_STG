#pragma once
#include "DxLib.h"
#include "math.h"

#define ENEMY_MAX 30        // 敵の最大数
#define ENEMY_ORDER_MAX 500 // 敵の出現指示の最大数

// 敵に関する構造体
typedef struct {
    // フラグ、カウンタ、移動用パターン、向き、種類、敵の種類、HPと最大HP、ドロップアイテム、画像
    int flag, cnt, pattern, muki, knd, hp, hp_max, item_n[6], img;
    // 座標、速度(x/y)、スピード、角度
    double x, y, vx, vy, sp, ang;
    // 弾の開始時間、弾の種類1・2、当たり判定カラー、状態、待機時間、出現までの待ち時間
    int bltime, blknd, blknd2, col, state, wtime, wait;
} enemy_t;

// 敵出現指示の構造体
typedef struct {
    // カウンタ、移動用パターン、敵の種類
    int cnt, pattern, knd;
    // 初期位置と移動スピード
    double x, y, sp;
    // 弾開始時間、弾の種類、当たり判定カラー、HP、弾種2、待機時間、アイテム種別（最大6つ）
    int bltime, blknd, col, hp, blknd2, wait, item_n[6];
} enemy_order_t;

class Mob_Enemy
{
private:
    enemy_t enemy[ENEMY_MAX];                  // 実際の敵キャラ情報
    enemy_order_t enemy_order[ENEMY_ORDER_MAX]; // 敵出現指示情報
    int stage_count;                           // ステージの進行カウント

public:
    // コンストラクタ
    Mob_Enemy();

    // デストラクタ
    ~Mob_Enemy();

    // 更新処理
    void Update();

    // 描画処理
    void Draw() const;

    // 敵の移動パターン0：シンプルな直進など
    void enemy_pattern0(int i);

    // 渡された ±ang の範囲内でランダムな角度を返す
    double rang(double ang);

    // 空いている敵番号を探して返す
    int enemy_num_search();

    // 敵データの出現処理
    void enemy_enter();

    // 敵の行動処理
    void enemy_act();

    // 敵の出現パターンを外部ファイル（CSV等）から読み込む関数
    void load_story();
};
