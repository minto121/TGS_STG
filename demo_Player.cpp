#include "demo_Player.h"
#include"Player_Shot.h"
//#include"Bullet.h"
#include"DxLib.h"
#include"Title.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

Bullet* BulletManager = nullptr;

demo_Player::demo_Player()
{
    x = 400;
    y = 650;
    radius = 5.0f;
    Alive = true;
    Respawn = false;
    RespawnTimer = 0;
    Zanki = 3;

    LoadDivGraph("Resource/image/Character_image/sample006.png",12, 3, 4, 48, 48, player_img);

    //BGM・SE読込
    Hit_SE = LoadSoundMem("Resource/bgm/hit_SE.wav");

 /*   int result = LoadDivGraph("Resource/image/Character_image/sample006.png", 12, 3, 4, 32, 48, player_img);
    if (result == -1) {
        printfDx("プレイヤー画像の読み込みに失敗しました\n");
    }*/

}

void demo_Player::Update(const std::vector<BulletInstance>& bullets)
{
    if (Alive) {
        for (const auto& b : bullets) {
            if (b.active && CheckHit(x, y, radius, b.x, b.y, 8.0f)) {
                PlaySoundMem(Hit_SE, DX_PLAYTYPE_BACK, TRUE);
                Hit(); // 被弾時の処理
                break;
            }
        }
    }
    if (Respawn) {
        if (RespawnTimer > 0) {
            y -= 2.0f;
            RespawnTimer--;
        }
        else {
            Alive = true;
            Respawn = false;
        }
    }

}

void demo_Player::move()
{
    //if (CheckHitKey(KEY_INPUT_W)) y -= 5.0f;  // 上
    //if (CheckHitKey(KEY_INPUT_S)) y += 5.0f;  // 下
    //if (CheckHitKey(KEY_INPUT_A)) x -= 5.0f;  // 左
    //if (CheckHitKey(KEY_INPUT_D)) x += 5.0f;  // 右

    bool slow = PAD_INPUT::OnHold(XINPUT_BUTTON_A) || CheckHitKey(KEY_INPUT_SPACE);

    // 上  0.2f
    if (CheckHitKey(KEY_INPUT_W) || PAD_INPUT::OnHold(XINPUT_BUTTON_DPAD_UP) == 1) {
        if (slow == true) {
            y -= 2.0f;
        }
        else {
            y -= 4.0f;
        }

        if (y < 0.0f)y = 0.0f;
    }

    // 下
    if (CheckHitKey(KEY_INPUT_S) || PAD_INPUT::OnHold(XINPUT_BUTTON_DPAD_DOWN) == 1) {
        if (slow == true) {
            y += 2.0f;
        }
        else {
            y += 4.0f;
        }
        if (y > SCREEN_HEIGHT)y = SCREEN_HEIGHT;
    }

    // 左
    if (CheckHitKey(KEY_INPUT_A) || PAD_INPUT::OnHold(XINPUT_BUTTON_DPAD_LEFT) == 1) {
        if (slow == true) {
            x -= 2.0f;
        }
        else {
            x -= 4.0f;
        }
        if (x < 0.0f)x = 0.0f;
    }

    // 右
    if (CheckHitKey(KEY_INPUT_D) || PAD_INPUT::OnHold(XINPUT_BUTTON_DPAD_RIGHT) == 1) {
        if (slow == true) {
            x += 2.0f;
        }
        else {
            x += 4.0f;
        }
        if (x > 850)x = 850;
    }
}

void demo_Player::Hit()
{
    Zanki--;

    //// --- 弾を消す処理を呼ぶ ---
    //if (BulletManager != nullptr) {
    //    BulletManager->TriggerRippleEffect(x, y, 500.0f); // 半径100以内の弾を削除
    //}

    if (Zanki <= 0) {
        Alive = false;
        Respawn = false;
        return;
    }

    Alive = false;
    Respawn = true;
    RespawnTimer = 120;
    x = 400;
    y = SCREEN_HEIGHT + 30;
 
}

bool demo_Player::CheckHit(float x1, float y1, float r1, float x2, float y2, float r2)
{
    float dx = x1 - x2;
    float dy = y1 - y2;
    float distanceSq = dx * dx + dy * dy;
    float radiusSum = r1 + r2;

    return distanceSq <= radiusSum * radiusSum;
    
}

bool demo_Player::GameOver() const
{
    return (Zanki <= 0);
}

bool demo_Player::IsAlive() const
{
    return Alive;
}

bool demo_Player::IsRespawn() const
{
    return Respawn;
}

void demo_Player::SetBulletManager(Bullet* manager)
{
    BulletManager = manager;
}

AbstractScene* demo_Player::Update()
{
    return this;
}

//void demo_Player::fire(Player_Shot*P_SHOT)
//{
//    int now = GetNowCount();
//    if (now - lastShotTime >= shotInterval) {
//        P_SHOT->FireBullet(x, y);
//        lastShotTime = now;
//    }
//}


void demo_Player::Draw()const 
{
    // プレイヤーを白い四角で描画
    //DrawBox((int)(x - 10), (int)(y - 10), (int)(x + 10), (int)(y + 10), GetColor(255, 255, 255), TRUE);
    if (Alive || Respawn) {
        // --- 点滅判定 ---
        bool visible = true;
        if (Respawn) {
            // 10フレームごとに表示・非表示を切り替える
            if ((RespawnTimer / 5) % 2 == 0) {
                visible = false;
            }
        }

        if (visible) {
            int drawIndex = 10; // 通常状態（中央）

            if (CheckHitKey(KEY_INPUT_A) || PAD_INPUT::OnHold(XINPUT_BUTTON_DPAD_LEFT) == 1) {
                drawIndex = 9; // 左
            }
            else if (CheckHitKey(KEY_INPUT_D) || PAD_INPUT::OnHold(XINPUT_BUTTON_DPAD_RIGHT) == 1) {
                drawIndex = 11; // 右
            }

            const int imgCenterX = 24; // 48 / 2
            const int imgCenterY = 24;

            DrawGraph((int)(x - imgCenterX), (int)(y - imgCenterY), player_img[drawIndex], TRUE);
        }

        // ヒットボックスは常に描画（必要に応じて非表示にしてもよい）
        DrawCircle((int)x, (int)y, (int)radius, GetColor(255, 0, 0), TRUE);
    }


   

    //DrawFormatString(0, 20, 0xffffff, "x,4:%d,%d", x, y);
    //DrawFormatString(0, 40, 0xffffff, "ReSpawnTimer:%d", RespawnTimer);
    //DrawFormatString(0, 80, 0xffffff, "Zanki:%d", Zanki);
 

}

