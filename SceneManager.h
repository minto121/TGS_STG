#pragma once
#pragma once
#include"AbstractScene.h"

class SceneManager :
    public AbstractScene
{
private:
    AbstractScene* mScene;
    bool mSceneChanged = false; // ’Ç‰Á: ƒV[ƒ“‚ªØ‚è‘Ö‚í‚Á‚½‚©‚Ç‚¤‚©
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

    //?`??????ç·??????
    void Draw() const override;

    bool IsSceneChanged() const { return mSceneChanged; } // ’Ç‰Á
    void ResetSceneChangedFlag() { mSceneChanged = false; } // ’Ç‰Á
};



