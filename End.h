#pragma once
#include "AbstractScene.h"

class End:public AbstractScene
{
private:

public:
	//�R���X�g���N�^
	End();
	//�f�X�g���N�^
	~End();

	// �X�V����
	virtual AbstractScene* Update() override;

	// �`�揈��
	virtual void Draw() const override;

};

