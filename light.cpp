/******************************************************************************
	�^�C�g���� : ���C�g�N���X
	�t�@�C���� : light.cpp
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
#include "light.h"

/******************************************************************************
	�֐��� : CLight::CLight()
	����   : �R���X�g���N�^
******************************************************************************/
CLight::CLight()
{

}

/******************************************************************************
	�֐��� : CLight::~CLight()
	����   : �f�X�g���N�^
******************************************************************************/
CLight::~CLight()
{

}

/******************************************************************************
	�֐��� : void CLight::Init(void)
	����   : void
	�߂�l : �Ȃ�
	����   : ���C�g�̐ݒ�
******************************************************************************/
void CLight::Init(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	D3DXVECTOR3 vecDir;		//��Ɨp�ϐ�

	//	1�ڂ̃��C�g�̐ݒ�
	ZeroMemory(&m_Light[0], sizeof(D3DLIGHT9));		//0�ŏ�����

	m_Light[0].Type = D3DLIGHT_DIRECTIONAL;						//���s����
	m_Light[0].Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);		//���̐F
	vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);					//���̌���

	D3DXVec3Normalize((D3DXVECTOR3 *)&m_Light[0].Direction, &vecDir);

	pDevice->SetLight(0, &m_Light[0]);				//0�Ԗڂ̃��C�g�ɁA�z���0�Ԗڂ̒l��ݒ�
	pDevice->LightEnable(0, TRUE);					//0�Ԗڂ̃��C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);	//���C�e�B���O���̂�on,off�ݒ�


	//	2�ڂ̃��C�g�̐ݒ�
	ZeroMemory(&m_Light[1], sizeof(D3DLIGHT9));		//0�ŏ�����

	m_Light[1].Type = D3DLIGHT_DIRECTIONAL;		//���s����
	m_Light[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//���̐F
	vecDir = D3DXVECTOR3(-0.2f, 0.8f, 0.5f);		//���̌���

	D3DXVec3Normalize((D3DXVECTOR3 *)&m_Light[1].Direction, &vecDir);

	pDevice->SetLight(1, &m_Light[1]);		//0�Ԗڂ̃��C�g�ɁA�z���0�Ԗڂ̒l��ݒ�
	pDevice->LightEnable(1, TRUE);				//0�Ԗڂ̃��C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);		//���C�e�B���O���̂�on,off�ݒ�
}

/******************************************************************************
	�֐��� : void CLight::Uninit(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �I������
******************************************************************************/
void CLight::Uninit(void)
{

}

/******************************************************************************
	�֐��� : void CLight::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �X�V����
******************************************************************************/
void CLight::Update(void)
{

}