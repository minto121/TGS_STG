#include "PadInput.h"

char PAD_INPUT::NowKey[BUTTONS];
char PAD_INPUT::OldKey[BUTTONS];
char PAD_INPUT::HoldKey[BUTTONS];
XINPUT_STATE PAD_INPUT::Input;
Stick PAD_INPUT::Rstick;
Stick PAD_INPUT::Lstick;