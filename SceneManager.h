#pragma once
#pragma once
#include"AbstractScene.h"

class SceneManager :
    public AbstractScene
{
private:
    AbstractScene* mScene;
    bool mSceneChanged = false; // �ǉ�: �V�[�����؂�ւ�������ǂ���
public:

    //?R???X?g???N?^
    SceneManager(AbstractScene* scene) :mScene(scene) {};

    //?f?X?g???N?^
    ~SceneManager()
    {
        delete mScene;

    }

    //?`???O??X?V????s
    AbstractScene* Update() override;

    //?`??????�??????
    void Draw() const override;

    bool IsSceneChanged() const { return mSceneChanged; } // �ǉ�
    void ResetSceneChangedFlag() { mSceneChanged = false; } // �ǉ�
};



