#pragma once
#include "AbstractScene.h"

class Help :public AbstractScene
{
private:
	int Help_img;

	int Title_BGM;
	int Select_SE;

public:
	//�R���X�g���N�^
	Help();
	//�f�X�g���N�^
	~Help();

	// �X�V����
	virtual AbstractScene* Update() override;

	// �`�揈��
	virtual void Draw() const override;

};