#pragma once
#include"DxLib.h"

#define ENEMY_MAX 30

//�G�Ɋւ���\����
typedef struct {
	//�t���O�A�J�E���^�A�ړ��p�^�[���A�����A�G�̎�ށAHP�ő�l�A���Ƃ��A�C�e���A�摜
	int flag, cnt, pattern, muki, knd, hp, hp_max, item_n[6], img;
	//���W�A���xx�����A���xy�����A�X�s�[�h�A�p�x
	double x, y, vx, vy, sp, ang;
	//�e���J�n���ԁA�e���̎�ށA�e�̎�ށA�F�A��ԁA�ҋ@���ԁA��؎���
	int bltime, blknd, blknd2, col, state, wtime, wait;
}enemy_t;


class Mob_Enemy
{
private:

	enemy_t enemy[ENEMY_MAX];

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

	 //�G�f�[�^�̓o�^
	 void enemy_enter();

	 //�G�̍s������
	 void enemy_act();
};

