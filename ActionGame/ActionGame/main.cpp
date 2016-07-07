#include <DxLib.h>
#include<math.h>
#include "GameMain.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SetMainWindowText("1401349‡FíJçhãP");
	GameMain& gm = GameMain::Instance();


	gm.GameLoop();

	gm.Terminate();

	return 0;
}