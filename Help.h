#pragma once
#include "AbstractScene.h"

class Help :public AbstractScene
{
private:

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