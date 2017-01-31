/******************************************************************************
	�^�C�g���� : �}�l�[�W���[�N���X
	�t�@�C���� : manader.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/05/09
	�X�V��     : 2016/05/16
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#pragma once

/******************************************************************************
	�C���N���[�h�t�@�C��
******************************************************************************/
/******************************************************************************
	���C�u�����̃����N
******************************************************************************/
/******************************************************************************
	�}�N����`
******************************************************************************/

/******************************************************************************
	�\���̐錾
******************************************************************************/
/******************************************************************************
	�N���X
******************************************************************************/
//	�O���錾///////////////////////////////////////////////////////////////////
class CRenderer;
class CScene;
class CCamera;
class CLight;
class CInput;
class CMode;
class CFade;
class CTexture;

//	�N���X�{��/////////////////////////////////////////////////////////////////
class CManager
{
public:
	CManager();		//	�R���X�g���N�^
	~CManager();	//	�f�X�g���N�^

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);		//	�������֐�
	void Uninit(void);		//	�I���֐�
	void Update(void);		//	�X�V�֐�
	void Draw(void);		//	�`��֐�

	//	�擾�֐��֘A
	static CRenderer  *GetRenderer(void) { return m_pRenderer; }		//	�����_���̎擾�֐�
	CInput     *GetInput(void) { return m_Input; }				//	�C���v�b�g�̎擾�֐�
	HWND        GethWnd(void) { return m_hWnd; }				//	�E�B���h�E�n���h���̎擾
	static      CMode *GetMode(void) { return m_Mode; }			//	���[�h�̎擾
	
	//	�X�N���[�����W�����[���h���W�ɕύX����֐�
	D3DXVECTOR3 *CalcS_to_W(D3DXVECTOR3 *pOut,
							int Sx,
							int Sy,
							float ProjZ,
							int ScreenWidth, 
							int ScreenHeight, 
							D3DXMATRIX *ViewMatrix, 
							D3DXMATRIX *ProjMatrix);

	//	�X�N���[�����W��XZ���ʂ̃��[���h���W��_�����߂�֐�
	D3DXVECTOR3 *CalcS_To_XZ(D3DXVECTOR3 *pOut,
							 int ScreenPosX,
							 int ScreenPosY,
							 int ScreenWidth,
							 int ScreenHeight,
							 D3DXMATRIX *View,
							 D3DXMATRIX *Proj);

	static void SetMode(CMode *Mode);			//	���[�h�̐ݒ�
	static void CheckRot(Vector3 *rot);		//	�p�x�̒���(D3DXVECTOR3�^�C�v)
	static void CheckRot(float *rot);			//	�p�x�̒���(�P�̃^�C�v)

private:
	CInput      *m_Input;				//	���͏����̏��
	HWND         m_hWnd;				//	hWnd�̎󂯎M
	CFade	     *m_Fade;				//	�t�F�[�h���
	CTexture	 *m_Texture;			//	�e�N�X�`���̏��
	static CRenderer *m_pRenderer;		//	�����_���̏��
	static CMode *m_Mode;				//	���[�h�̏��

	static HANDLE m_Threed;		//	�X���b�h���
	DWORD m_ExitCode;			//	�X���b�h�̏I������
	static unsigned int m_ThreedId;		//	�X���b�h�̔ԍ�
};

