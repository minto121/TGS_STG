#pragma once
#pragma once
#include"AbstractScene.h"

class SceneManager :
    public AbstractScene
{
private:
    AbstractScene* mScene;
    bool mSceneChanged = false; // 追加: シーンが切り替わったかどうか
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

    //?`??????邱??????
    void Draw() const override;

    bool IsSceneChanged() const { return mSceneChanged; } // 追加
    void ResetSceneChangedFlag() { mSceneChanged = false; } // 追加
};



