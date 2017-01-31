#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "scene2D.h"
#include "texture.h"
#include "time.h"
#include "Number.h"



CNumber::CNumber()
{
	m_pVtxBuffPolygon = NULL;
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_width = 0.0f;
	m_height = 0.0f;

	m_TextureType = CTexture::G_NUMBER;
}


CNumber::~CNumber()
{
}

void CNumber::Init(D3DXVECTOR3 pos, float width, float height)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	VERTEX_2D *pVtx;

	m_pos = pos;
	m_width = width;
	m_height = height;

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffPolygon, NULL)))
		return;

	//頂点情報へのポインタを取得
	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = m_pos.x - (m_width * 0.5f);
	pVtx[0].pos.y = m_pos.y - (m_height * 0.5f);
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + (m_width * 0.5f);
	pVtx[1].pos.y = m_pos.y - (m_height * 0.5f);
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x - (m_width * 0.5f);
	pVtx[2].pos.y = m_pos.y + (m_height * 0.5f);
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + (m_width * 0.5f);
	pVtx[3].pos.y = m_pos.y + (m_height * 0.5f);
	pVtx[3].pos.z = 0.0f;

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//テクスチャ座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	//	色設定
	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;

	//	バッファ解放
	m_pVtxBuffPolygon->Unlock();
}

void CNumber::Uninit(void)
{
	SAFE_RELEASE(m_pVtxBuffPolygon);
}

void CNumber::Update(int num, int anim_X_Max, int anim_Y_Max)
{
	VERTEX_2D *pVtx;
	//頂点情報へのポインタを取得
	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//	テクスチャ座標更新
	pVtx[0].tex.x = (float)num * 0.1f;
	pVtx[0].tex.y = ((num - (num % anim_X_Max)) / anim_X_Max) * ((float)1 / anim_Y_Max);

	pVtx[1].tex.x = (float)num * 0.1f + 0.1f;
	pVtx[1].tex.y = ((num - (num % anim_X_Max)) / anim_X_Max) * ((float)1 / anim_Y_Max);

	pVtx[2].tex.x = (float)num * 0.1f;
	pVtx[2].tex.y = (((num - (num % anim_X_Max)) / anim_X_Max) * ((float)1 / anim_Y_Max)) + ((float)1 / anim_Y_Max);

	pVtx[3].tex.x = (float)num * 0.1f + 0.1f;
	pVtx[3].tex.y = (((num - (num % anim_X_Max)) / anim_X_Max) * ((float)1 / anim_Y_Max)) + ((float)1 / anim_Y_Max);

	//	バッファ解放
	m_pVtxBuffPolygon->Unlock();
}

void CNumber::Draw(void)
{
	//	デバイス取得
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファをデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

	//頂点座標ふぉーまっと
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, CTexture::GetTexture(CScene::SCENE_TYPE_GAME, m_TextureType));

	//ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
