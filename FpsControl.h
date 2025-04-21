#pragma once
#ifndef DEF_FPSCONTROLL_H
#define DEF_FPSCONTROLL_H

void FpsControl_Initialize();
bool FpsControl_Update();
void FpsControl_Draw();
void FpsControl_Wait();

float FpsControl_GetDeltaTime();
int FpsControl_GetNowFrame(); // Å© Ç±ÇÍÇí«ãL



#endif