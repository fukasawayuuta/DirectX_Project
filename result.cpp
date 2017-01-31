/******************************************************************************
	タイトル名 : リザルトクラス
	ファイル名 : result.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/07/14
	更新日     :
******************************************************************************/
/******************************************************************************
	インクルードファイル
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
	マクロ定義
******************************************************************************/
#define RESULT_TEXTURE_PRESSENTER_WIDTH (500.0f)		//	タイトルのPress_Enterの幅。
#define RESULT_TEXTURE_PRESSENTER_HEIGHT (30.0f)		//	タイトルのPress_Enterの高さ。
#define RESULT_ACTION_COL_CHANGE_TIMING ( 120 )
#define RESULT_ACT_COL_CHANGE_TIMING ( 4 )
#define CHANGE_TIME_CNT ( 1800 )

/******************************************************************************
	構造体宣言
******************************************************************************/
/******************************************************************************
	関数名 : CResult::CResult()
	説明   : コンストラクタ
******************************************************************************/
CResult::CResult()
{
	m_bMultiThreed = false;
}

/******************************************************************************
	関数名 : CResult::~CResult()
	説明   : デストラクタ
******************************************************************************/
CResult::~CResult()
{

}

/******************************************************************************
	関数名 : void CResult::Init(void)
	引数   : void
	戻り値 : なし
	説明   : 背景画像の設定。
******************************************************************************/
void CResult::Init(void)
{
	m_pResultTexture = CTexture::Create(RESULT);
	CResult_BG::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CTexture::R_BACKGROUND);
}

/******************************************************************************
	関数名 : void CResult::Uninit(void)
	引数   : void
	戻り値 : なし
	説明   : 背景画像の解放。
******************************************************************************/
void CResult::Uninit(void)
{
	CTexture::UnLoad(m_pResultTexture, RESULT);
	CScene::ReleaseAll();
}

/******************************************************************************
	関数名 : void CResult::Update(void)
	引数   : void
	戻り値 : なし
	説明   : 画像の処理の更新。
			 Enterが押されたらタイトルに遷移。
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
	関数名 : void CResult::Draw(void)
	引数   : void
	戻り値 : なし
	説明   : 画像の描画。
******************************************************************************/
void CResult::Draw(void)
{
	CScene::DrawAll();
}