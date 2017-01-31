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
#include "main.h"
#include "joyStick.h"

/******************************************************************************
	マクロ定義
******************************************************************************/
#define MAX_CONTROLLERS ( 1 )			//	コントローラーの最大数

/******************************************************************************
	グローバル変数
******************************************************************************/
XINPUT_STATE g_PressState;			//	JoyStickのプレス情報
XINPUT_STATE g_TriggerState;		//	JoyStickのトリガー情報
XINPUT_STATE g_ReleaseState;		//	JoyStickのリリース情報
XINPUT_STATE g_RepeatState;			//	JoyStickのリピート情報
int			 g_RepeatStateCnt;		//	JoyStickのリピートのカウンタ

/******************************************************************************
	関数名：HRESULT InitKeyboard(HINSTANCE hInstance , HWND hWNd)
	引数  ：HINSTANCE hInstance , HWND hWNd
	戻り値：なし
	説明  :Keyboardの初期化、デバイスの生成、データフォーマットの設定、協調モードの設定、アクセス権の取得
******************************************************************************/
void InitJoyStick(void)
{
	//	作業用変数
	DWORD dwResult;   

	for (DWORD i = 0; i <  MAX_CONTROLLERS; i++) 
	{
		XINPUT_STATE state;  
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState( i, &state );

		//	デヴァイスがあるかどうか
		if( dwResult == ERROR_SUCCESS )  
		{// Controller is connected

			//	Press情報の初期化
			g_PressState.Gamepad.bLeftTrigger = NULL;
			g_PressState.Gamepad.bRightTrigger = NULL;
			g_PressState.Gamepad.sThumbLX = NULL;
			g_PressState.Gamepad.sThumbLY = NULL;
			g_PressState.Gamepad.sThumbRX = NULL;
			g_PressState.Gamepad.sThumbRY = NULL;
			g_PressState.Gamepad.wButtons = NULL;

			// Trigger情報の初期化
			g_TriggerState.Gamepad.bLeftTrigger = NULL;
			g_TriggerState.Gamepad.bRightTrigger = NULL;
			g_TriggerState.Gamepad.sThumbLX = NULL;
			g_TriggerState.Gamepad.sThumbLY = NULL;
			g_TriggerState.Gamepad.sThumbRX = NULL;
			g_TriggerState.Gamepad.sThumbRY = NULL;
			g_TriggerState.Gamepad.wButtons = NULL;

			//	Release情報の初期化
			g_ReleaseState.Gamepad.bLeftTrigger = NULL;
			g_ReleaseState.Gamepad.bRightTrigger = NULL;
			g_ReleaseState.Gamepad.sThumbLX = NULL;
			g_ReleaseState.Gamepad.sThumbLY = NULL;
			g_ReleaseState.Gamepad.sThumbRX = NULL;
			g_ReleaseState.Gamepad.sThumbRY = NULL;
			g_ReleaseState.Gamepad.wButtons = NULL;

			//	Repeat情報の初期化
			g_RepeatState.Gamepad.bLeftTrigger = NULL;
			g_RepeatState.Gamepad.bRightTrigger = NULL;
			g_RepeatState.Gamepad.sThumbLX = NULL;
			g_RepeatState.Gamepad.sThumbLY = NULL;
			g_RepeatState.Gamepad.sThumbRX = NULL;
			g_RepeatState.Gamepad.sThumbRY = NULL;
			g_RepeatState.Gamepad.wButtons = NULL;

			//	RepeatCntの初期化
			g_RepeatStateCnt = 0;
		}       
		else  
		{// Controller is not connected 

		}
	}
}

/******************************************************************************
	関数名：void UninitJoyStick(void)
	引数  ：void
	戻り値：なし
	説明  :なし
******************************************************************************/
void UninitJoyStick(void)
{
	
}

/******************************************************************************
	関数名：void UpdateJoyStick(void)
	引数  ：void
	戻り値：なし
	説明  :JoyStickの状態を取得
******************************************************************************/
void UpdateJoyStick(void)
{
	//	作業用変数
	DWORD dwResult;
	XINPUT_STATE state;

	for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
	{
		//	0で初期化
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		//	デヴァイス情報取得
		dwResult = XInputGetState(i, &state);

		//	デヴァイスがあったら
		if (dwResult == ERROR_SUCCESS)
		{
			//Trigger
			g_TriggerState.Gamepad.wButtons = (g_PressState.Gamepad.wButtons | state.Gamepad.wButtons) ^ g_PressState.Gamepad.wButtons;

			//Release
			g_ReleaseState.Gamepad.wButtons = (g_PressState.Gamepad.wButtons | state.Gamepad.wButtons) ^ state.Gamepad.wButtons;
			if (ReleaseJoyStick((WORD)i) == true)
			{
				g_RepeatStateCnt = 0;
			}

			//Pless処理

			if (state.Gamepad.sThumbLY <= 0)
			{
				g_PressState = state;
			}

			g_PressState = state;
			if (PressJoyStick((WORD)i) == true)
			{
				g_RepeatStateCnt += 1;
			}

			//Repeat
			g_RepeatState.Gamepad.wButtons = (g_PressState.Gamepad.wButtons | state.Gamepad.wButtons) ^ g_PressState.Gamepad.wButtons;
			if (TriggerJoyStick((WORD)i) == true | g_RepeatStateCnt > 40)
			{
				g_RepeatState = state;
			}
			
		}
	}
}

/******************************************************************************
	関数名：PressJoyStick(int nKey)
	引数  ：nKey
	戻り値：なし
	説明  :JoyStickの状態を取得
******************************************************************************/
bool PressJoyStick(WORD nKey)
{
	return (g_PressState.Gamepad.wButtons & nKey) ? true : false;
}


/******************************************************************************
	関数名：TriggerJoyStick(int nKey)
	引数  ：nKey
	戻り値：なし
	説明  :JoyStickの状態を取得
******************************************************************************/
bool TriggerJoyStick(WORD nKey)
{
	return (g_TriggerState.Gamepad.wButtons & nKey) ? true : false;
}


/******************************************************************************
	関数名：ReleaseJoyStick(int nKey)
	引数  ：nKey
	戻り値：なし
	説明  :JoyStickの状態を取得
******************************************************************************/
bool ReleaseJoyStick(WORD nKey)
{
	return (g_ReleaseState.Gamepad.wButtons & nKey) ? true : false;
}

/******************************************************************************
	関数名：RepeatJoyStick(int nKey)
	引数  ：nKey
	戻り値：なし
	説明  :JoyStickの状態を取得
******************************************************************************/
bool RepeatJoyStick(WORD nKey)
{
	return (g_RepeatState.Gamepad.wButtons & nKey) ? true : false;
}


/******************************************************************************
	関数名：XINPUT_STATE *GetPressJoyStick(void)
	引数  ：なし
	戻り値：g_PressStateのアドレス
	説明  :g_PressStateのアドレスを取得
******************************************************************************/
XINPUT_STATE *GetPressJoyStick(void)
{
	return &g_PressState;
}