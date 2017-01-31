/******************************************************************************
	�^�C�g���� : �}�l�[�W���[�N���X
	�t�@�C���� : manader.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/05/09
	�X�V��     :
******************************************************************************/
/******************************************************************************
	�C���N���[�h�t�@�C��
******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "Fade.h"
#include "Mode.h"
#include "title.h"

/******************************************************************************
	�ÓI�����o�ϐ�
******************************************************************************/
CMode *CManager::m_Mode = NULL;
CRenderer *CManager::m_pRenderer;

/******************************************************************************
	�֐��� : CManager::CManager()
	����   : �R���X�g���N�^
******************************************************************************/
CManager::CManager()
{
	m_pRenderer = NULL;
	m_Input = NULL;
}

/******************************************************************************
	�֐��� : CManager::~CManager()
	����   : �f�X�g���N�^
******************************************************************************/
CManager::~CManager()
{
}

/******************************************************************************
	�֐��� : HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
	����   : hInstance, hWnd, bWindow
	�߂�l : S_OK, E_FAIL
	����   : �����_���N���X�̐����Ə������A�V�[���N���X�̐����Ə�����
			 �J�����̐����Ə������A���C�g�̐����Ə�����
******************************************************************************/
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//	�����_���N���X����
	m_pRenderer = new CRenderer;
	//	�������Ɏ��s������E_FAIL��Ԃ��B
	if (FAILED(m_pRenderer->Init(hInstance, hWnd, bWindow))) 
		return E_FAIL;
	
	//	���͏����̍쐬�Ə������B
	m_Input = new CInput;
	m_Input->InitKeyboard(hInstance, hWnd);			//	�C���v�b�g�N���X�̏�����
	//	�t�F�[�h�����̍쐬�Ə������B
	m_Fade = new CFade;
	m_Fade->Init();
	//	�ŏ��̃��[�h���^�C�g���ɐݒ�B
	SetMode(new CTitle);
	//	hWnd�擾�B
	m_hWnd = hWnd;

	return S_OK;
}

/******************************************************************************
	�֐��� : void CManager::Uninit(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �����_���N���X�̉���A�I�u�W�F�N�g�̉���A
			 �|���S���̉�������A�J�����̉�������A���C�g�̉������
******************************************************************************/
void CManager::Uninit(void)
{
	//	�����_���N���X�̏I������
	m_pRenderer->Uninit();
	SAFE_DELETE(m_pRenderer);
	
	//	���[�h�̏I������
	m_Mode->Uninit();
	SAFE_DELETE(m_Mode);

	//	�C���v�b�g�̏I������
	m_Input->UninitKeyboard();
	SAFE_DELETE(m_Input);
	
	//	�t�F�[�h�̏I������
	m_Fade->Uninit();
	SAFE_DELETE(m_Fade);
}

/******************************************************************************
	�֐��� : void CManager::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �I�u�W�F�N�g�ƃJ�����̍X�V����
******************************************************************************/
void CManager::Update(void)
{
	//	���͂̍X�V
	m_Input->UpdateMouse();
	m_Input->UpdateKeyboard();
	m_Input->UpdateJoyStick();

	//	�����_���̍X�V
	m_pRenderer->Update();
	//	�t�F�[�h�̍X�V
	m_Fade->Update();
	//	���[�h�̍X�V
	m_Mode->Update();
}

/******************************************************************************
	�֐��� : void CManager::Draw(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �I�u�W�F�N�g�̕`�揈��
******************************************************************************/
void CManager::Draw(void)
{
	//	�`��J�n
	if(m_pRenderer->DrawSceneFlag() == true) {
		//	���[�h�̕`��
		m_Mode->Draw();
		//	�t�F�[�h�̕`��
		m_Fade->Draw();
	}

	//	�`��I��
	m_pRenderer->ExchangeBuffer();
}

/******************************************************************************
	�֐��� : D3DXVECTOR3 *CManager::CalcS_to_W(D3DXVECTOR3 *pOut,
				                               int Sx,
				                               int Sy,
				                               float ProjZ,
				                               int ScreenWidth, 
				                               int ScreenHeight, 
				                               D3DXMATRIX *ViewMatrix, 
				                               D3DXMATRIX *ProjMatrix)
	����   : *pOut, Sx, Sy, ProjZ, ScreenWidth, ScreenHeight, *ViewMatrix, *ProjMatrix
	�߂�l : pOut
	����   : �X�N���[�����W�����[���h���W�ɕϊ�
******************************************************************************/
D3DXVECTOR3 *CManager::CalcS_to_W(D3DXVECTOR3 *pOut,
								  int Sx,
								  int Sy,
								  float ProjZ,
								  int ScreenWidth, 
								  int ScreenHeight, 
								  D3DXMATRIX *ViewMatrix, 
								  D3DXMATRIX *ProjMatrix)
{
	//	��Ɨp�ϐ�
	D3DXMATRIX InvView,InvProj, ViewPort, InvViewPort;

	//	�e�s��̋t�s����v�Z
	D3DXMatrixInverse( &InvView, NULL, ViewMatrix );
    D3DXMatrixInverse( &InvProj, NULL, ProjMatrix );
    D3DXMatrixIdentity( &ViewPort );

	ViewPort._11 = ScreenWidth / 2.0f; 
	ViewPort._22 = -ScreenHeight / 2.0f;
	ViewPort._41 = ScreenWidth / 2.0f; 
	ViewPort._42 = ScreenHeight / 2.0f;

    D3DXMatrixInverse( &InvViewPort, NULL, &ViewPort );

	// 2D���W��3D���W�ɕϊ�
    D3DXMATRIX tmp = InvViewPort * InvProj * InvView;
    D3DXVec3TransformCoord( pOut, &D3DXVECTOR3(Sx,Sy,ProjZ), &tmp );

   return pOut;
}


/******************************************************************************
	�֐��� : D3DXVECTOR3 *CManager::CalcS_To_XZ(D3DXVECTOR3 *pOut,
								   int ScreenPosX,
								   int ScreenPosY,
								   int ScreenWidth,
								   int ScreenHeight,
								   D3DXMATRIX *View,
								   D3DXMATRIX *Proj)
	����   : *pOut, ScreenPosX, ScreenPosY, ScreenWidth, ScreenHeight, *View, *Proj
	�߂�l : �Ȃ�
	����   : �I�u�W�F�N�g�̕`�揈��
******************************************************************************/
D3DXVECTOR3 *CManager::CalcS_To_XZ(D3DXVECTOR3 *pOut,
								   int ScreenPosX,
								   int ScreenPosY,
								   int ScreenWidth,
								   int ScreenHeight,
								   D3DXMATRIX *View,
								   D3DXMATRIX *Proj)
{
	//	��Ɨp�ϐ�
	D3DXVECTOR3 nearPos, farPos, ray;

	//	�J���������ԋ߂����W�����߂�
	CalcS_to_W(&nearPos, ScreenPosX, ScreenPosY, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, View, Proj);
	//	�J���������ԉ������W�����߂�
	CalcS_to_W(&farPos , ScreenPosX, ScreenPosY, 1.0f, SCREEN_WIDTH, SCREEN_HEIGHT, View, Proj);

	ray = farPos - nearPos;
	D3DXVec3Normalize( &ray, &ray );

	//	���Ƃ̌������N���Ă���ꍇ�͌�_��
    //	�N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��
	if( ray.y <= 0 ) {
        //	���ƌ�_�����Ƃ�
        float Lray = D3DXVec3Dot( &ray, &D3DXVECTOR3(0, 1, 0) );
        float LP0 = D3DXVec3Dot( &(-nearPos), &D3DXVECTOR3(0, 1, 0) );
        *pOut = nearPos + (LP0 / Lray) * ray;
    }
    else {
		//	���ƌ�_���ĂȂ��Ƃ�
		*pOut = farPos;
    }

	return pOut;
}

/******************************************************************************
	�֐��� : void CManager::SetMode(CMode *Mode)
	����   : *Mode
	�߂�l : �Ȃ�
	����   : �I�u�W�F�N�g�̕`�揈��
******************************************************************************/
void CManager::SetMode(CMode *Mode)
{
	//	���[�h�̃|�C���^��null����Ȃ�������
	if (m_Mode != NULL) {
		m_Mode->Uninit();
		delete m_Mode;
		m_Mode = NULL;
	}
	
	//	���[�h�ύX�B
	m_Mode = Mode;

	//	�}���`�X���b�h���Ȃ��ꍇ�A�ʏ�̏����������B
	if (m_Mode->GetMultiThreedFlag() == false) {
		m_Mode->Init();
	}
}

/******************************************************************************
	�֐��� : void CManager::ChackRot(D3DXVECTOR3 * rot)
	����   : * rot
	�߂�l : �Ȃ�
	����   : �p�x�̒����BPI�𒴂��Ă�����C���B(D3DXVECTOR3�^)
******************************************************************************/
void CManager::CheckRot(Vector3 * rot)
{
	if (rot->x > D3DX_PI)
		rot->x -= D3DX_PI * 2.0f;
	if (rot->x < -D3DX_PI)
		rot->x += D3DX_PI * 2.0f;

	if (rot->y > D3DX_PI)
		rot->y -= D3DX_PI * 2.0f;
	if (rot->y < -D3DX_PI)
		rot->y += D3DX_PI * 2.0f;

	if (rot->z > D3DX_PI)
		rot->z -= D3DX_PI * 2.0f;
	if (rot->z < -D3DX_PI)
		rot->z += D3DX_PI * 2.0f;

}

/******************************************************************************
	�֐��� : void CManager::ChackRot(float * rot)
	����   : * rot
	�߂�l : �Ȃ�
	����   : �p�x�̒����BPI�𒴂��Ă�����C���B(float�^)
******************************************************************************/
void CManager::CheckRot(float * rot)
{
	if (*rot > D3DX_PI)
		*rot -= D3DX_PI * 2.0f;
	if (*rot < -D3DX_PI)
		*rot += D3DX_PI * 2.0f;
}
