/******************************************************************************
	�^�C�g���� : �t�F�[�h�N���X
	�t�@�C���� : fade.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/07/13
	�X�V��     :
******************************************************************************/
/******************************************************************************
	�C���N���[�h�錾
******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "Mode.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "Fade.h"

/******************************************************************************
	�}�N����`
******************************************************************************/
/******************************************************************************
	�O���[�o���ϐ��錾
******************************************************************************/
//	�ÓI�����o�ϐ��錾/////////////////////////////////////////////////////////
CMode *CFade::m_NextMode = null;
FADE   CFade::m_Fade = FADE_IN;

/******************************************************************************
	�֐��� : CFade::CFade()
	����   : �R���X�g���N�^
	�ϐ��̏������ƁA�|�C���^��NULL����
******************************************************************************/
CFade::CFade()
{
	m_pVtxBuff = null;
	m_Alfa = 1.0f;
	m_Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_Alfa);
}

/******************************************************************************
	�֐��� : CFade::~CFade()
	����   : �f�X�g���N�^
******************************************************************************/
CFade::~CFade()
{
}

/******************************************************************************
	�֐��� : void CFade::Init(void)
	����   : void
	�߂�l : �Ȃ�
	����   : ���_�o�b�t�@�̍쐬�B
******************************************************************************/
void CFade::Init(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	VERTEX_2D *pVtx;

	//	���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
		return;

	//	���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//	���W�ϊ��p�W���̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//	�|���S���̒��_�J���[�ݒ�
	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;

	//	�e�N�X�`�����W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuff->Unlock();
}

/******************************************************************************
	�֐��� : void CFade::Uninit(void)
	����   : void
	�߂�l : �Ȃ�
	����   : ���_�o�b�t�@�̉���A�|�C���^�̉���B
******************************************************************************/
void CFade::Uninit(void)
{
	//	���_�o�b�t�@�̊J��
	SAFE_RELEASE(m_pVtxBuff);
}

/******************************************************************************
	�֐��� : void CFade::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �A���t�@�l�̕ύX�A�t�F�[�h�̍X�V�A���[�h�̕ύX�B
******************************************************************************/
void CFade::Update(void)
{
	VERTEX_2D *pVtx;

	if (m_Fade == FADE_IN) {
		m_Alfa -= 0.02f;

		if (m_Alfa <= 0) {
			m_Alfa = 0.0f;
			m_Fade = FADE_NONE;
		}
	}
	else if (m_Fade == FADE_OUT) {
		m_Alfa += 0.02f;

		if (1 <= m_Alfa) {
			m_Fade = FADE_IN;
			m_Alfa = 1.0f;
			CManager::SetMode(m_NextMode);
		}
	}

	m_Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_Alfa);

	//	���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//	�|���S���̒��_�J���[�ݒ�
	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;

	m_pVtxBuff->Unlock();
}

/******************************************************************************
	�֐��� : void CFade::Draw(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �t�F�[�h�̕`��B
******************************************************************************/
void CFade::Draw(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_���W�ӂ��[�܂���
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, null);
	//�|���S���`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

/******************************************************************************
	�֐��� : void CFade::Start(FADE fade, CMode *NextMode)
	����   : fade, NextMode
	�߂�l : �Ȃ�
	����   : �t�F�[�h�̐ݒ�B
******************************************************************************/
void CFade::Start(FADE fade, CMode *NextMode)
{
	//	�t�F�[�h�̏��������ĂȂ�������
	if (m_Fade == FADE_NONE) {
		m_Fade = fade;
		m_NextMode = NextMode;
	}
}