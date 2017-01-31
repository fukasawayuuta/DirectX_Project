/*******************************************************************************
	�^�C�g�����FDirectX3D �f�o�C�X�ݒ�
	�t�@�C�����FjoyStick.h
	�쐬��    �FAT-13B-284 35 �[�V�C��
	�쐬��    �F2017/01/31
	�X�V��    �F
******************************************************************************/
/******************************************************************************
	�C���N���[�h�t�@�C��
******************************************************************************/
#include "main.h"
#include "joyStick.h"

/******************************************************************************
	�}�N����`
******************************************************************************/
#define MAX_CONTROLLERS ( 1 )			//	�R���g���[���[�̍ő吔

/******************************************************************************
	�O���[�o���ϐ�
******************************************************************************/
XINPUT_STATE g_PressState;			//	JoyStick�̃v���X���
XINPUT_STATE g_TriggerState;		//	JoyStick�̃g���K�[���
XINPUT_STATE g_ReleaseState;		//	JoyStick�̃����[�X���
XINPUT_STATE g_RepeatState;			//	JoyStick�̃��s�[�g���
int			 g_RepeatStateCnt;		//	JoyStick�̃��s�[�g�̃J�E���^

/******************************************************************************
	�֐����FHRESULT InitKeyboard(HINSTANCE hInstance , HWND hWNd)
	����  �FHINSTANCE hInstance , HWND hWNd
	�߂�l�F�Ȃ�
	����  :Keyboard�̏������A�f�o�C�X�̐����A�f�[�^�t�H�[�}�b�g�̐ݒ�A�������[�h�̐ݒ�A�A�N�Z�X���̎擾
******************************************************************************/
void InitJoyStick(void)
{
	//	��Ɨp�ϐ�
	DWORD dwResult;   

	for (DWORD i = 0; i <  MAX_CONTROLLERS; i++) 
	{
		XINPUT_STATE state;  
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState( i, &state );

		//	�f���@�C�X�����邩�ǂ���
		if( dwResult == ERROR_SUCCESS )  
		{// Controller is connected

			//	Press���̏�����
			g_PressState.Gamepad.bLeftTrigger = NULL;
			g_PressState.Gamepad.bRightTrigger = NULL;
			g_PressState.Gamepad.sThumbLX = NULL;
			g_PressState.Gamepad.sThumbLY = NULL;
			g_PressState.Gamepad.sThumbRX = NULL;
			g_PressState.Gamepad.sThumbRY = NULL;
			g_PressState.Gamepad.wButtons = NULL;

			// Trigger���̏�����
			g_TriggerState.Gamepad.bLeftTrigger = NULL;
			g_TriggerState.Gamepad.bRightTrigger = NULL;
			g_TriggerState.Gamepad.sThumbLX = NULL;
			g_TriggerState.Gamepad.sThumbLY = NULL;
			g_TriggerState.Gamepad.sThumbRX = NULL;
			g_TriggerState.Gamepad.sThumbRY = NULL;
			g_TriggerState.Gamepad.wButtons = NULL;

			//	Release���̏�����
			g_ReleaseState.Gamepad.bLeftTrigger = NULL;
			g_ReleaseState.Gamepad.bRightTrigger = NULL;
			g_ReleaseState.Gamepad.sThumbLX = NULL;
			g_ReleaseState.Gamepad.sThumbLY = NULL;
			g_ReleaseState.Gamepad.sThumbRX = NULL;
			g_ReleaseState.Gamepad.sThumbRY = NULL;
			g_ReleaseState.Gamepad.wButtons = NULL;

			//	Repeat���̏�����
			g_RepeatState.Gamepad.bLeftTrigger = NULL;
			g_RepeatState.Gamepad.bRightTrigger = NULL;
			g_RepeatState.Gamepad.sThumbLX = NULL;
			g_RepeatState.Gamepad.sThumbLY = NULL;
			g_RepeatState.Gamepad.sThumbRX = NULL;
			g_RepeatState.Gamepad.sThumbRY = NULL;
			g_RepeatState.Gamepad.wButtons = NULL;

			//	RepeatCnt�̏�����
			g_RepeatStateCnt = 0;
		}       
		else  
		{// Controller is not connected 

		}
	}
}

/******************************************************************************
	�֐����Fvoid UninitJoyStick(void)
	����  �Fvoid
	�߂�l�F�Ȃ�
	����  :�Ȃ�
******************************************************************************/
void UninitJoyStick(void)
{
	
}

/******************************************************************************
	�֐����Fvoid UpdateJoyStick(void)
	����  �Fvoid
	�߂�l�F�Ȃ�
	����  :JoyStick�̏�Ԃ��擾
******************************************************************************/
void UpdateJoyStick(void)
{
	//	��Ɨp�ϐ�
	DWORD dwResult;
	XINPUT_STATE state;

	for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
	{
		//	0�ŏ�����
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		//	�f���@�C�X���擾
		dwResult = XInputGetState(i, &state);

		//	�f���@�C�X����������
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

			//Pless����

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
	�֐����FPressJoyStick(int nKey)
	����  �FnKey
	�߂�l�F�Ȃ�
	����  :JoyStick�̏�Ԃ��擾
******************************************************************************/
bool PressJoyStick(WORD nKey)
{
	return (g_PressState.Gamepad.wButtons & nKey) ? true : false;
}


/******************************************************************************
	�֐����FTriggerJoyStick(int nKey)
	����  �FnKey
	�߂�l�F�Ȃ�
	����  :JoyStick�̏�Ԃ��擾
******************************************************************************/
bool TriggerJoyStick(WORD nKey)
{
	return (g_TriggerState.Gamepad.wButtons & nKey) ? true : false;
}


/******************************************************************************
	�֐����FReleaseJoyStick(int nKey)
	����  �FnKey
	�߂�l�F�Ȃ�
	����  :JoyStick�̏�Ԃ��擾
******************************************************************************/
bool ReleaseJoyStick(WORD nKey)
{
	return (g_ReleaseState.Gamepad.wButtons & nKey) ? true : false;
}

/******************************************************************************
	�֐����FRepeatJoyStick(int nKey)
	����  �FnKey
	�߂�l�F�Ȃ�
	����  :JoyStick�̏�Ԃ��擾
******************************************************************************/
bool RepeatJoyStick(WORD nKey)
{
	return (g_RepeatState.Gamepad.wButtons & nKey) ? true : false;
}


/******************************************************************************
	�֐����FXINPUT_STATE *GetPressJoyStick(void)
	����  �F�Ȃ�
	�߂�l�Fg_PressState�̃A�h���X
	����  :g_PressState�̃A�h���X���擾
******************************************************************************/
XINPUT_STATE *GetPressJoyStick(void)
{
	return &g_PressState;
}