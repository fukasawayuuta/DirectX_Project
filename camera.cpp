/******************************************************************************
	�^�C�g���� :���߂�N���X
	�t�@�C���� : camera.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/04/26
	�X�V��     :
******************************************************************************/
/******************************************************************************
	�C���N���[�h�t�@�C��
******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "camera.h"
#include "input.h"
#include "Mode.h"
#include "game.h"
#include "scene3D.h"
#include "sceneX.h"
#include "model.h"
#include "motionManager.h"
#include "dynamicModel.h"
#include "player.h"

/******************************************************************************
	�}�N����`
******************************************************************************/
const float R_TO_V_DISTANCE = 300.0f;		//	���_�ƒ����_�̍ŏ��ɐݒ肷�鋗��
const float R_TO_V_AIM_DISTANCE = 120.0f;	//	�G�C�����Ă鎞�̒����_�Ǝ��_�̋����B
const float R_TO_V_DISTANCE_MIN = 100.0f;	//	���_�ƒ����_�̈�ԍŋߋ���
const float R_TO_V_DISTANCE_MAX = 700.0f;	//	���_�ƒ����_�̈�ԍŉ�����
const float CAMERA_V_POS_MOVEMENT = 1.0f;	//	���_�̈ړ���
const float CAMERA_R_ROT_MOVEMENT = 0.1f;	//	���_�̉�]��
const float CAMERA_NEAR = 1.0f;				//	�J������Near�l
const float CAMERA_FAR = 10000.0f;			//	�J������Far�l
const float CAMERA_MOVE_RATIO_X = 0.15f;	//	�J������X����]�̔䗦(�}�E�X�̊��x)
const float CAMERA_MOVE_RATIO_Y = 0.15f;	//	�J������Y����]�̔䗦(�}�E�X�̊��x)
const float CAMERA_ROT_INERTIA = 0.03f;		//	�J�����̉�]�̊����l

D3DXVECTOR3 CCamera::m_VecRV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

/******************************************************************************
	�֐��� : CCamera::CCamera()
	����   : �R���X�g���N�^�B
******************************************************************************/
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_moveRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_movePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bace_point = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_moveVecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_VecRV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PurposePosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PurposePosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDistance = 0.0f;
	m_fPurposeRot = 0.0f;
	m_fVibration_Width = 0.0f;
	m_fSinCnt = 0;
	m_VecURot = 0.0f;
	m_MoveVecU = 0.0f;
}

/******************************************************************************
	�֐��� : CCamera::~CCamera()
	����   : �f�X�g���N�^�B
******************************************************************************/
CCamera::~CCamera()
{

}

/******************************************************************************
	�֐��� : void CCamera::Init(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �J�����̍��W�A�����_�A�J�����㕔�������Ă�������̐ݒ�B
******************************************************************************/
void CCamera::Init(void)
{
	m_posV = D3DXVECTOR3(0.0f, 50.0f, -R_TO_V_DISTANCE);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//	���_�ƒ����_�̋������v�Z
	m_fDistance = sqrtf((m_posR.z - m_posV.z) * (m_posR.z - m_posV.z) + (m_posR.x - m_posV.x) + (m_posR.x - m_posV.x));

	m_fPurposeRot = 0.0f;

	//	���_�ƒ����_�̊p�x�v�Z
	m_rot.y = atan2f(m_posR.x - m_posV.x, m_posR.z - m_posV.z);

	//	���_�̍��W�ݒ�
	m_posV.x = m_posR.x - sinf(m_rot.y) * m_fDistance;
	m_posV.z = m_posR.z - cosf(m_rot.y) * m_fDistance;

	//	�����_�̍��W�ݒ�
	m_posR.x = m_posV.x - sinf(m_rot.y + D3DX_PI) * cosf(m_rot.x) * m_fDistance;
	m_posR.y = m_posV.y - sinf(m_rot.x) * m_fDistance;
	m_posR.z = m_posV.z - cosf(m_rot.y + D3DX_PI) * cosf(m_rot.x) * m_fDistance;

	//	���_���璍���_�܂ł̃x�N�g�������߂�
	m_VecRV = m_posR - m_posV;
}

/******************************************************************************
	�֐��� : void CCamera::Uninit(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �J�����Ȃ̂œ��ɂȂ��B
******************************************************************************/
void CCamera::Uninit(void)
{
}

/******************************************************************************
	�֐��� : void CCamera::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �����������ĂȂ��̂Ő����Ȃ��B
******************************************************************************/
void CCamera::Update(void)
{
	// ���͏����ւ̃|�C���^���擾
	CManager *pManager    = GetManager();
	CInput   *pInput      = pManager->GetInput();
	CGame	 *pGame       = (CGame *)CManager::GetMode();
	CPlayer  *pPlayer     = pGame->GetPlayer();
	D3DXVECTOR3 PlayerRot = pPlayer->GetRot();
	DIMOUSESTATE2 Mouse   = pInput->GetMouse();

	if (pPlayer->GetAlive()) {
		m_fSinCnt++;
		if (pPlayer->GetLockOn() == false) {
			//	�}�E�X�̈ړ��ʎ擾�B
			m_rot.y += (D3DXToRadian(Mouse.lX) * CAMERA_MOVE_RATIO_X);
			m_rot.x += (D3DXToRadian(Mouse.lY) * CAMERA_MOVE_RATIO_Y);

			if (m_rot.x < -D3DX_PI * 0.5f + (D3DX_PI * 0.02f)) {
				m_rot.x = -D3DX_PI * 0.5f + (D3DX_PI * 0.02f);
			}

			if (m_rot.x > D3DX_PI * 0.5f - (D3DX_PI * 0.02f)) {
				m_rot.x = D3DX_PI * 0.5f - (D3DX_PI * 0.02f);
			}
		}

		//	�����_���W�ݒ�
		m_bace_point += (pPlayer->GetPos() - m_bace_point) * 0.175f;

		//	���_�̍��W�ݒ�
		m_PurposePosV.x = m_bace_point.x - sinf(m_rot.y) * cosf(m_rot.x) * m_fDistance;
		m_PurposePosV.y = m_bace_point.y + 30.0f - sinf(m_rot.x - D3DX_PI + (D3DX_PI * 0.1f)) * m_fDistance;
		m_PurposePosV.z = m_bace_point.z - cosf(m_rot.y) * cosf(m_rot.x) * m_fDistance;

		m_posV += (m_PurposePosV - m_posV) * 0.2f;

		if (pPlayer->GetLockOn() == false) {
			m_PurposePosR.x = m_bace_point.x - sinf(m_rot.y + D3DX_PI) * cosf(m_rot.x) * m_fDistance;
			m_PurposePosR.y = m_bace_point.y + 30.0f + sinf(m_rot.x - D3DX_PI + (D3DX_PI * 0.1f)) * m_fDistance;
			m_PurposePosR.z = m_bace_point.z - cosf(m_rot.y + D3DX_PI) * cosf(m_rot.x) * m_fDistance;
		}

		m_posR += (m_PurposePosR - m_posR) * 0.2f;

		m_fVibration_Width = m_fVibration_Width > 0.0f ? m_fVibration_Width - 0.5f : 0.0f;
	}

	m_MoveVecU += (m_VecURot - m_MoveVecU) * 0.8f;

	m_vecU.x = m_moveVecU.x;
	m_vecU.z = m_moveVecU.z;
}

/******************************************************************************
	�֐��� : void CCamera::SetCamera(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �J�����̐ݒ�
			 �v���W�F�N�V�����}�g���N�X�����������č쐬���A
			 �r���[�}�g���N�X�̏������Ɛݒ������B
******************************************************************************/
void CCamera::SetCamera(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	float f_Screen_Width;		//�A�X�y�N�g��v�Z�p�ϐ��̉�ʕ���
	float f_Screen_Height;		//�A�X�y�N�g��v�Z�p�ϐ��̉�ʍ���

	f_Screen_Width = SCREEN_WIDTH;
	f_Screen_Height = SCREEN_HEIGHT;

	//�v���W�F�N�V�����}�g���N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	//�v���W�F�N�V�����}�g���N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
							    D3DX_PI * 0.25f,					//����p
							    f_Screen_Width / f_Screen_Height,	//�A�X�y�N�g��
							    CAMERA_NEAR,						//NearZ�l
							    CAMERA_FAR);						//For�l

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView,
					   &m_posV,
					   &m_posR,
					   &m_vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	//	���_���璍���_�܂ł̃x�N�g�������߂�
	m_VecRV = m_posR - m_posV;
}