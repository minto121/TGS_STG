#pragma once
#include"DxLib.h"
#include"math.h"

#define ENEMY_MAX 30

#define ENEMY_ORDER_MAX 500

//円周率
#ifndef PI
#define PI 3.1415926535898
#define PI2 (PI*2)
#endif

//�G�Ɋւ���\����
typedef struct {
	//�t���O�A�J�E���^�A�ړ��p�^�[���A�����A�G�̎�ށAHP�ő�l�A���Ƃ��A�C�e���A�摜
	int flag, cnt, pattern, muki, knd, hp, hp_max, item_n[6], img;
	//���W�A���xx�����A���xy�����A�X�s�[�h�A�p�x
	double x, y, vx, vy, sp, ang;
	//�e���J�n���ԁA�e���̎�ށA�e�̎�ށA�F�A��ԁA�ҋ@���ԁA��؎���
	int bltime, blknd, blknd2, col, state, wtime, wait;
}enemy_t;


typedef struct {
	//�J�E���^�A�ړ��p�^�[���A�G�̎��
	int cnt, pattern, knd;
	//�������W�ƈړ��X�s�[�h
	double x, y, sp;
	//�e���J�n���ԁA�e���̎�ށA�F�A�̗́A�e�̎�ށA��؎��ԁA�A�C�e��(6���)
	int bltime, blknd, col, hp, blknd2, wait, item_n[6];
}enemy_order_t;



class Mob_Enemy
{
private:

	enemy_t enemy[ENEMY_MAX];

	enemy_order_t enemy_order[ENEMY_ORDER_MAX];

	int stage_count;

public:

	//�R���X�g���N�^
	Mob_Enemy();

	//�f�X�g���N�^
	~Mob_Enemy();

	// �X�V����
	void  Update();

	// �`�揈��
	 void Draw() const;


	 //�G�̈ړ��p�^�[��0�ł̈ړ�����
	 void enemy_pattern0(int i);

	 // 渡された - ang～angまでのランダムな角度を返す
	 double rang(double ang);

	 //�󂢂Ă���G�ԍ������
	 int enemy_num_search();

	 //�G�f�[�^�̓o�^
	 void enemy_enter();

	 //�G�̍s������
	 void enemy_act();

	 //�G�̏o������G�N�Z������ǂݍ���Ŋi�[����֐�
	 void load_story();
};

