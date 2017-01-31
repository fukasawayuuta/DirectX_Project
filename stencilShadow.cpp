/******************************************************************************
	ファイル名 : stencilShadow.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2017/01/28
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
#include "stencilShadow.h"


/******************************************************************************
	関数名 : CStencilShadow::CStencilShadow(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene3D(Priority, objType, sceneType)
	説明   : コンストラクタ。
******************************************************************************/
CStencilShadow::CStencilShadow(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene3D(Priority, objType, sceneType)
{
	m_Target = NULL;
	m_pMesh = NULL;
	m_pBuffMat = NULL;
	m_pVtxBuffPolygon = NULL;
	m_NumMat = 0;
}

/******************************************************************************
	関数名 : CStencilShadow::~CStencilShadow()
	説明   : デストラクタ。
******************************************************************************/
CStencilShadow::~CStencilShadow()
{
}

/******************************************************************************
	関数名 : void CStencilShadow::Init(CScene * target)
	引数   : target
	戻り値 : なし
	説明   : 2Dポリゴン作成。シリンダーのモデル読み込み。
******************************************************************************/
void CStencilShadow::Init(CScene * target)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	VERTEX_2D *pVtx;

	m_Target = target;

	if (FAILED(D3DXLoadMeshFromX("data/MODEL/stencil_shadow.x",		//	ファイルがある場所
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,	//	マテリアル情報
		NULL,
		&m_NumMat,		//	マテリアルの数
		&m_pMesh))) {	//	メッシュ情報
		return;
	}

	//	頂点バッファの生成(2D)
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffPolygon, NULL))) {
		return;
	}

	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
	pVtx[0].col = D3DCOLOR_RGBA(0, 0, 0, 128);
	pVtx[1].col = D3DCOLOR_RGBA(0, 0, 0, 128);
	pVtx[2].col = D3DCOLOR_RGBA(0, 0, 0, 128);
	pVtx[3].col = D3DCOLOR_RGBA(0, 0, 0, 128);

	m_pVtxBuffPolygon->Unlock();
}

/******************************************************************************
	関数名 : void CStencilShadow::Uninit(void)
	引数   : void
	戻り値 : なし
	説明   : 解放処理。
******************************************************************************/
void CStencilShadow::Uninit(void)
{
	m_Target = NULL;
	if (m_pMesh != NULL)
		SAFE_RELEASE(m_pMesh)
	if (m_pBuffMat != NULL)
		SAFE_RELEASE(m_pBuffMat)
	if (m_pVtxBuffPolygon != NULL) 
		SAFE_RELEASE(m_pVtxBuffPolygon);
}

/******************************************************************************
	関数名 : void CStencilShadow::Update(void)
	引数   : void
	戻り値 : なし
	説明   : シリンダーの座標更新。
******************************************************************************/
void CStencilShadow::Update(void)
{
	m_Pos = m_Target->GetPos();
	//m_Pos.y = 0.0f;
}

/******************************************************************************
	関数名 : void CStencilShadow::Draw(void)
	引数   : void
	戻り値 : なし
	説明   : 描画処理
******************************************************************************/
void CStencilShadow::Draw(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	//	行列式に使う作業用変数
	D3DXMATRIX mtxScl, mtxRot, mtxTrans;
	D3DXMATERIAL *pMat;
	D3DMATERIAL9 matDef;		//	退避用マテリアル

	//	ワールドマトリクスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//	スケールを反映
	D3DXMatrixScaling(&mtxScl, m_Scl.x, m_Scl.y, m_Scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	//	回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//	移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);	//	フォグ：OFF
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		//	ライティング
	pDevice->SetTexture(0, NULL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0);	//	色情報は書かない
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);		//	比較用の１: OFF

	/* -- 円柱表 -- */
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);	//	INCR = インクリメントの略で1で描画
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	for (int nCntMat = 0; nCntMat < m_NumMat; nCntMat++) {
		m_pMesh->DrawSubset(nCntMat);
	}
	/* -- 円柱表 -- */
	/* -- 円柱裏 -- */
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_DECR);	//	DECR = デクリメントの略で-1で描画
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	for (int nCntMat = 0; nCntMat < m_NumMat; nCntMat++) {
		m_pMesh->DrawSubset(nCntMat);
	}
	/* -- 円柱裏 -- */

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xf);
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	/* -- 全画面2Dポリゴン描画 -- */
	//頂点バッファをデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));
	//頂点座標ふぉーまっと
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャの設定
	pDevice->SetTexture(0, NULL);
	//ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);		//フォグ：ON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);		//	ライティング : OFF
}

/******************************************************************************
	関数名 : void CStencilShadow::Create(CScene * target)
	引数   : target
	戻り値 : なし
	説明   : 生成関数。
******************************************************************************/
void CStencilShadow::Create(CScene * target)
{
	CStencilShadow *obj = new CStencilShadow;
	obj->Init(target);
}
