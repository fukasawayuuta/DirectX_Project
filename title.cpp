/******************************************************************************
	�^�C�g���� : �^�C�g���N���X
	�t�@�C���� : title.cpp
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
#include "scene.h"
#include "scene2D.h"
#include "texture.h"
#include "input.h"
#include "Mode.h"
#include "game.h"
#include "Fade.h"
#include "title.h"
#include "title_BG.h"

/******************************************************************************
	�}�N����`
******************************************************************************/
#define TITLE_TEXTURE_PRESSENTER_WIDTH (500.0f)		//	�^�C�g����Press_Enter�̕��B
#define TITLE_TEXTURE_PRESSENTER_HEIGHT (30.0f)		//	�^�C�g����Press_Enter�̍����B
#define TITLE_ACTION_COL_CHANGE_TIMING ( 120 )
#define TITLE_ACT_COL_CHANGE_TIMING ( 4 )
#define CHANGE_TIME_CNT ( 1800 )

/******************************************************************************
	�֐��� : CTitle::CTitle()
	����   : �R���X�g���N�^�B
******************************************************************************/
CTitle::CTitle()
{
	m_bMultiThreed = false;
}

/******************************************************************************
	�֐��� : CTitle::~CTitle()
	����   : �f�X�g���N�^�B
******************************************************************************/
CTitle::~CTitle()
{
}

/******************************************************************************
	�֐��� : void CTitle::Init(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �w�i�̏������B
******************************************************************************/
void CTitle::Init(void)
{
	m_pTitleTexture = CTexture::Create(TITLE);
	CTitle_BG::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CTexture::T_BACKGROUND);
}

/******************************************************************************
	�֐��� : void CTitle::Uninit(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �w�i�̉���B
******************************************************************************/
void CTitle::Uninit(void)
{
	CTexture::UnLoad(m_pTitleTexture, TITLE);
	CScene::ReleaseAll();
}

/******************************************************************************
	�֐��� : void CTitle::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �^�C�g����ʂ̍X�V�B
******************************************************************************/
void CTitle::Update(void)
{
	CManager *pManager = GetManager();
	CInput   *pInput = pManager->GetInput();
	
	if (CFade::GetFadeMode() == FADE_NONE) {
		m_ChangeTime = (m_ChangeTime < 0) ? CHANGE_TIME_CNT : m_ChangeTime - 1;
	}

	if (pInput->GetKeyboardTrigger(DIK_RETURN) == true || pInput->TriggerJoyStick(XINPUT_GAMEPAD_B) == true) {
		if (CFade::GetFadeMode() == FADE_NONE) {
			CFade::Start(FADE_OUT, new CGame);
			if (m_mode != TITLE_MODE_ACTIVE) {
				m_mode = TITLE_MODE_ACTIVE;
			}
		}
	}

	CScene::UpdateAll();
}

/******************************************************************************
	�֐��� : void CTitle::Draw(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �^�C�g����ʂ̕`��B
******************************************************************************/
void CTitle::Draw(void)
{
	CScene::DrawAll();
}