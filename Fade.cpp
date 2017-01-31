/******************************************************************************
	タイトル名 : フェードクラス
	ファイル名 : fade.cpp
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
#include "Mode.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "Fade.h"

/******************************************************************************
	マクロ定義
******************************************************************************/
/******************************************************************************
	グローバル変数宣言
******************************************************************************/
//	静的メンバ変数宣言/////////////////////////////////////////////////////////
CMode *CFade::m_NextMode = null;
FADE   CFade::m_Fade = FADE_IN;

/******************************************************************************
	関数名 : CFade::CFade()
	説明   : コンストラクタ
	変数の初期化と、ポインタにNULLを代入
******************************************************************************/
CFade::CFade()
{
	m_pVtxBuff = null;
	m_Alfa = 1.0f;
	m_Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_Alfa);
}

/******************************************************************************
	関数名 : CFade::~CFade()
	説明   : デストラクタ
******************************************************************************/
CFade::~CFade()
{
}

/******************************************************************************
	関数名 : void CFade::Init(void)
	引数   : void
	戻り値 : なし
	説明   : 頂点バッファの作成。
******************************************************************************/
void CFade::Init(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	VERTEX_2D *pVtx;

	//	頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
		return;

	//	頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//	座標変換用係数の設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//	ポリゴンの頂点カラー設定
	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;

	//	テクスチャ座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuff->Unlock();
}

/******************************************************************************
	関数名 : void CFade::Uninit(void)
	引数   : void
	戻り値 : なし
	説明   : 頂点バッファの解放、ポインタの解放。
******************************************************************************/
void CFade::Uninit(void)
{
	//	頂点バッファの開放
	SAFE_RELEASE(m_pVtxBuff);
}

/******************************************************************************
	関数名 : void CFade::Update(void)
	引数   : void
	戻り値 : なし
	説明   : アルファ値の変更、フェードの更新、モードの変更。
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

	//	頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//	ポリゴンの頂点カラー設定
	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;

	m_pVtxBuff->Unlock();
}

/******************************************************************************
	関数名 : void CFade::Draw(void)
	引数   : void
	戻り値 : なし
	説明   : フェードの描画。
******************************************************************************/
void CFade::Draw(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファをデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点座標ふぉーまっと
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, null);
	//ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

/******************************************************************************
	関数名 : void CFade::Start(FADE fade, CMode *NextMode)
	引数   : fade, NextMode
	戻り値 : なし
	説明   : フェードの設定。
******************************************************************************/
void CFade::Start(FADE fade, CMode *NextMode)
{
	//	フェードの処理をしてなかったら
	if (m_Fade == FADE_NONE) {
		m_Fade = fade;
		m_NextMode = NextMode;
	}
}