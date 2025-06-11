#pragma once
#include "DxLib.h"

#define BUTTONS 16

#define PRESS   1 // 押された瞬間
#define PRESSED 2 // 押されている間
#define RELEASE 3 // 離した瞬間

//�X�e�B�b�N
struct Stick
{
	short ThumbX;	//�����l
	short ThumbY;	//�c���l
};

class PAD_INPUT
{
private:
	static char NowKey[BUTTONS]; //����̓��̓L�[
	static char OldKey[BUTTONS]; //�O��̓��̓L�[
	static char HoldKey[BUTTONS];	//ホールド
	static XINPUT_STATE Input; //�p�b�h
	static Stick Rstick; //�E�X�e�B�b�N
	static Stick Lstick; //���X�e�B�b�N
private:
	//�R���X�g���N�^
	PAD_INPUT() = default;
public:
	//�p�b�h���̍X�V
	static void UpdateKey()
	{
		// ���̓L�[�擾
		GetJoypadXInputState(DX_INPUT_KEY_PAD1, &Input);
		for (int i = 0; i < BUTTONS; i++)
		{
			OldKey[i] = NowKey[i];
			NowKey[i] = Input.Buttons[i];
			HoldKey[i] = Input.Buttons[i];
		}

		//�E�X�e�B�b�N
		Rstick.ThumbX = Input.ThumbRX;
		Rstick.ThumbY = Input.ThumbRY;

		//���X�e�B�b�N
		Lstick.ThumbX = Input.ThumbLX;
		Lstick.ThumbY = Input.ThumbLY;
	}

	//�{�^��������ꂽ�u��
	static bool OnButton(int button)
	{
		bool ret = (NowKey[button] == 1 && OldKey[button] == 0);
		return ret;
	}

	//�{�^���𗣂����u��
	static bool OnRelease(int button)
	{
		bool ret = (NowKey[button] == 0 && OldKey[button] == 1);
		return ret;
	}

	static bool OnHold(int button)
	{
		bool ret = (HoldKey[button] == 1);
		return ret;
	}

	//�E�X�e�B�b�N�̎擾
	static Stick GetRStick()
	{
		return Rstick;
	}

	//���X�e�B�b�N�̎擾
	static Stick GetLStick()
	{
		return Lstick;
	}
};

