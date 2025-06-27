#include"DxLib.h"
#include"SceneManager.h"
#include"FPS.h"
#include"Title.h"
#include"PadInput.h"
#include"GameMain.h"

#include"FpsControl.h"


#define FRAMERATE 60.0 //?t???[?????[?g

#define HEIGHT 720
#define WIDTH 1280 
#define REFRESHRATE 32

/***********************************************
 * ?v???O??????J?n
 ***********************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	SetMainWindowText("STG");

	ChangeWindowMode(TRUE);		// ?E?B???h?E???[?h??N??

	SetGraphMode(WIDTH, HEIGHT, REFRESHRATE);	//???T?C?Y????

	// ����������؂�
	SetWaitVSyncFlag(0);

	if (DxLib_Init() == -1) return -1;	// DX???C?u???????????????

	SetDrawScreen(DX_SCREEN_BACK);	// ?`???????????

	SceneManager* sceneMng;

	try
	{
		sceneMng = new SceneManager((AbstractScene*)new GameMain() /*Title()*/);

	}
	catch (const char* err)
	{
		FILE* fp = NULL;

		DATEDATA data;

		GetDateTime(&data);
		//?t?@?C???I?[?v??
		fopen_s(&fp, "ErrLog.txt", "a");
		//?G???[?f?[?^?????????
		fprintf_s(fp, "%02d?N %02d??R %02d?? %02d?? %02d?? %02d?b : %s??????????B\n", data.Year, data.Mon, data.Day, data.Hour, data.Min, data.Sec, err);
		fclose(fp);

		return 0;
	}
	FpsController* FPSC = new FpsController(FRAMERATE, 800);

	// ?Q?[?????[?v
	while ((ProcessMessage() == 0) && (sceneMng->Update() != nullptr)) {

		ClearDrawScreen();		// ?????????
		PAD_INPUT::UpdateKey();
		//sceneMng->Draw();
		FPSC->All();
		FPSC->Disp();

		if (sceneMng->IsSceneChanged()) {
			sceneMng->ResetSceneChangedFlag();
			continue; // �V�[���؂�ւ������ Draw ��X�L�b�v
		}
		sceneMng->Draw();

	
		//?????I??
		if (PAD_INPUT::OnButton(XINPUT_BUTTON_BACK))
		{
			break;
		}

		ScreenFlip();			// ???????e??\??????f
	}
	return 0;
}