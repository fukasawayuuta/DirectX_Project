#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

/*******************************************************************************
	タイトル名：DirectX3D デバイス設定
	ファイル名：joyStick.h
	作成者    ：AT-13B-284 35 深澤佑太
	作成日    ：2017/01/31
	更新日    ：
******************************************************************************/
/******************************************************************************
	インクルードファイル
******************************************************************************/
#include "XInput.h"
#pragma comment(lib,"XInput.lib")

/******************************************************************************
	クラス宣言
******************************************************************************/

/******************************************************************************
	プロトタイプ宣言
******************************************************************************/
void InitJoyStick(void);
void UninitJoyStick(void);
void UpdateJoyStick(void);

//	JoyStickの入力を受け取る関数
bool PressJoyStick(WORD nKey);
bool TriggerJoyStick(WORD nKey);
bool ReleaseJoyStick(WORD nKey);
bool RepeatJoyStick(WORD nKey);

//	アドレスを取得する関数
XINPUT_STATE *GetPressJoyStick(void);



#endif