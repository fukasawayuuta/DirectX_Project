/******************************************************************************
	タイトル名 : タイトルクラス
	ファイル名 : title.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/07/13
	更新日     :
******************************************************************************/
/******************************************************************************
	インクルード宣言
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
	マクロ定義
******************************************************************************/
#define TITLE_TEXTURE_PRESSENTER_WIDTH (500.0f)		//	タイトルのPress_Enterの幅。
#define TITLE_TEXTURE_PRESSENTER_HEIGHT (30.0f)		//	タイトルのPress_Enterの高さ。
#define TITLE_ACTION_COL_CHANGE_TIMING ( 120 )
#define TITLE_ACT_COL_CHANGE_TIMING ( 4 )
#define CHANGE_TIME_CNT ( 1800 )

/******************************************************************************
	関数名 : CTitle::CTitle()
	説明   : コンストラクタ。
******************************************************************************/
CTitle::CTitle()
{
	m_bMultiThreed = false;
}

/******************************************************************************
	関数名 : CTitle::~CTitle()
	説明   : デストラクタ。
******************************************************************************/
CTitle::~CTitle()
{
}

/******************************************************************************
	関数名 : void CTitle::Init(void)
	引数   : void
	戻り値 : なし
	説明   : 背景の初期化。
******************************************************************************/
void CTitle::Init(void)
{
	m_pTitleTexture = CTexture::Create(TITLE);
	CTitle_BG::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CTexture::T_BACKGROUND);
}

/******************************************************************************
	関数名 : void CTitle::Uninit(void)
	引数   : void
	戻り値 : なし
	説明   : 背景の解放。
******************************************************************************/
void CTitle::Uninit(void)
{
	CTexture::UnLoad(m_pTitleTexture, TITLE);
	CScene::ReleaseAll();
}

/******************************************************************************
	関数名 : void CTitle::Update(void)
	引数   : void
	戻り値 : なし
	説明   : タイトル画面の更新。
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
	関数名 : void CTitle::Draw(void)
	引数   : void
	戻り値 : なし
	説明   : タイトル画面の描画。
******************************************************************************/
void CTitle::Draw(void)
{
	CScene::DrawAll();
}