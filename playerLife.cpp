/******************************************************************************
	タイトル名 : プレイヤーライフ
	ファイル名 : playerLife.cpp
	作成者     : AT-13C-284 35 深澤佑太
	作成日     : 2017/01/19
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
#include "scene3D.h"
#include "sceneX.h"
#include "texture.h"
#include "model.h"
#include "motionManager.h"
#include "DynamicModel.h"
#include "player.h"
#include "PlayerLife.h"
#include "Mode.h"
#include "game.h"
#include "playerLife.h"

/******************************************************************************
	マクロ宣言
******************************************************************************/
const float MOVE_INETIA = 0.2f;		//	ライフバーの動きの慣性値。

/******************************************************************************
	関数名 : CPlayerLife::CPlayerLife(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene2D(Priority, objType, sceneType)
	説明   : デストラクタ
******************************************************************************/
CPlayerLife::CPlayerLife(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene2D(Priority, objType, sceneType)
{
	m_PlayerMaxHP = 0;
	m_MoveWidth = 0.0f;
	m_FixedWidth = 0.0f;
}

/******************************************************************************
	関数名 : CPlayerLife::~CPlayerLife()
	説明   : デストラクタ
******************************************************************************/
CPlayerLife::~CPlayerLife()
{
}

/******************************************************************************
	関数名 : void CPlayerLife::Update(void)
	引数   : void
	戻り値 : なし
	説明   : アニメーションの更新。
******************************************************************************/
void CPlayerLife::Init(D3DXVECTOR3 pos, float width, float height, int lifeTex, int barTex, int hp)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	VERTEX_2D *pVtx;

	//	変数の代入。
	m_Pos = pos;
	m_Width = width;
	m_Height = height;
	m_LifeTexture[TEX_LIFE] = lifeTex;
	m_LifeTexture[TEX_LIFE_BAR] = barTex;
	m_PlayerMaxHP = hp;
	m_FixedWidth = width;

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (4 * TEX_MAX), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffPolygon, NULL))) {
		return;
	}

	//頂点情報へのポインタを取得
	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < TEX_MAX; i++, pVtx += 4) {
		//	座標設定
		pVtx[0].pos = D3DXVECTOR3(m_Pos.x, m_Pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_Pos.x, m_Pos.y + m_Height, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y + m_Height, 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//テクスチャ座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//	色設定
		pVtx[0].col = m_Color;
		pVtx[1].col = m_Color;
		pVtx[2].col = m_Color;
		pVtx[3].col = m_Color;
	}

	//	バッファ解放
	m_pVtxBuffPolygon->Unlock();
}

/******************************************************************************
	関数名 : void CPlayerLife::Update(CPlayer *player)
	引数   : player
	戻り値 : なし
	説明   : アニメーションの更新。
******************************************************************************/
void CPlayerLife::Update(CPlayer *player)
{
	VERTEX_2D *pVtx;

	m_Width = ((float)player->GetHP() / m_PlayerMaxHP) * m_FixedWidth;
	m_MoveWidth += (m_Width - m_MoveWidth) * MOVE_INETIA;

	//頂点情報へのポインタを取得
	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//	座標設定
	pVtx[0].pos = D3DXVECTOR3(m_Pos.x, m_Pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_Pos.x + m_MoveWidth, m_Pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_Pos.x, m_Pos.y + m_Height, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_Pos.x + m_MoveWidth, m_Pos.y + m_Height, 0.0f);

	//	バッファ解放
	m_pVtxBuffPolygon->Unlock();
}

/******************************************************************************
	関数名 : void CPlayerLife::Update(void)
	引数   : void
	戻り値 : なし
	説明   : 描画処理。
******************************************************************************/
void CPlayerLife::Draw(void)
{
	//	デバイス取得
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファをデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

	//頂点座標ふぉーまっと
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < TEX_MAX; i++) {
		//テクスチャの設定
		pDevice->SetTexture(0, CTexture::GetTexture(m_SceneType, m_LifeTexture[i]));
		//ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}
}

/******************************************************************************
	関数名 : CPlayerLife * CPlayerLife::Create(D3DXVECTOR3 pos, float width, float height, int lifeTex, int barTex)
	引数   : pos, width, height, lifeTex, barTex
	戻り値 : なし
	説明   : 生成関数。
******************************************************************************/
CPlayerLife * CPlayerLife::Create(D3DXVECTOR3 pos, float width, float height, int lifeTex, int barTex, int hp)
{
	CPlayerLife *obj = new CPlayerLife;
	obj->Init(pos, width, height, lifeTex, barTex, hp);
	return obj;
}
