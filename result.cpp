/******************************************************************************
	�^�C�g���� : ���U���g�N���X
	�t�@�C���� : result.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/07/14
	�X�V��     :
******************************************************************************/
/******************************************************************************
	�C���N���[�h�t�@�C��
******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "scene2D.h"
#include "texture.h"
#include "input.h"
#include "Mode.h"
#include "Fade.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "result_BG.h"

/******************************************************************************
	�}�N����`
******************************************************************************/
#define RESULT_TEXTURE_PRESSENTER_WIDTH (500.0f)		//	�^�C�g����Press_Enter�̕��B
#define RESULT_TEXTURE_PRESSENTER_HEIGHT (30.0f)		//	�^�C�g����Press_Enter�̍����B
#define RESULT_ACTION_COL_CHANGE_TIMING ( 120 )
#define RESULT_ACT_COL_CHANGE_TIMING ( 4 )
#define CHANGE_TIME_CNT ( 1800 )

/******************************************************************************
	�\���̐錾
******************************************************************************/
/******************************************************************************
	�֐��� : CResult::CResult()
	����   : �R���X�g���N�^
******************************************************************************/
CResult::CResult()
{
	m_bMultiThreed = false;
}

/******************************************************************************
	�֐��� : CResult::~CResult()
	����   : �f�X�g���N�^
******************************************************************************/
CResult::~CResult()
{

}

/******************************************************************************
	�֐��� : void CResult::Init(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �w�i�摜�̐ݒ�B
******************************************************************************/
void CResult::Init(void)
{
	m_pResultTexture = CTexture::Create(RESULT);
	CResult_BG::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CTexture::R_BACKGROUND);
}

/******************************************************************************
	�֐��� : void CResult::Uninit(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �w�i�摜�̉���B
******************************************************************************/
void CResult::Uninit(void)
{
	CTexture::UnLoad(m_pResultTexture, RESULT);
	CScene::ReleaseAll();
}

/******************************************************************************
	�֐��� : void CResult::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �摜�̏����̍X�V�B
			 Enter�������ꂽ��^�C�g���ɑJ�ځB
******************************************************************************/
void CResult::Update(void)
{
	CManager *pManager = GetManager();
	CInput   *pInput = pManager->GetInput();

	if (CFade::GetFadeMode() == FADE_NONE) {
		m_ChangeTime = (m_ChangeTime < 0) ? CHANGE_TIME_CNT : m_ChangeTime - 1;
	}

	if (pInput->GetKeyboardTrigger(DIK_RETURN) == true) {
		if (CFade::GetFadeMode() == FADE_NONE) {
			CFade::Start(FADE_OUT, new CTitle);
		}
	}

	CScene::UpdateAll();
}

/******************************************************************************
	�֐��� : void CResult::Draw(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �摜�̕`��B
******************************************************************************/
void CResult::Draw(void)
{
	CScene::DrawAll();
}