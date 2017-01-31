/******************************************************************************
	タイトル名 : CSmokeクラス
	ファイル名 : smoke.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2017/01/18
	更新日     : 
******************************************************************************/
/******************************************************************************
	インクルードファイル
******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "texture.h"
#include "scene3D.h"
#include "camera.h"
#include "billboard.h"
#include "smoke.h"

/******************************************************************************
	マクロ定義
******************************************************************************/
const int TEXTURE_UPDATE_TIMING_COUNT = 3;		//	テクスチャアニメーションを更新するタイミングカウンタ。

/******************************************************************************
	関数名 : CSmoke::CSmoke(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CBillboard(Priority, objType, sceneType)
	説明   : コンストラクタ
******************************************************************************/
CSmoke::CSmoke(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CBillboard(Priority, objType, sceneType)
{
}

/******************************************************************************
	関数名 : CSmoke::~CSmoke()
	説明   : デストラクタ
******************************************************************************/
CSmoke::~CSmoke()
{
}

/******************************************************************************
	関数名 : void CSmoke::Init(D3DXVECTOR3 pos, float width, float height, int textureType)
	引数   : pos, width, height, textureType
	戻り値 : なし
	説明   : 座標、角度、テクスチャーの初期化。
******************************************************************************/
void CSmoke::Init(D3DXVECTOR3 pos, float rot, float width, float height, int textureType)
{
	//	初期化。
	CBillboard::Init(pos, width, height, 0.0f, textureType);
	//	アニメーションの変数設定。
	m_AnimationCountX_Axis = 8;
	m_AnimationCountY_Axis = 4;
	m_AnimationTotalNumber = m_AnimationCountX_Axis * m_AnimationCountY_Axis;

	m_UpdateTimeCnt = rand() % TEXTURE_UPDATE_TIMING_COUNT + 2;
	int magnification = rand() % 10 + 1;
	m_Scl *= (magnification * 0.1f);
	m_Rot.z += rot;

	CManager::CheckRot(&m_Rot.z);
}

/******************************************************************************
	関数名 : void CSmoke::Update(void)
	引数   : void
	戻り値 : なし
	説明   : アニメーションの更新。
******************************************************************************/
void CSmoke::Update(void)
{
	VERTEX_3D *pVtx;

	m_Color.a -= 0.01f;		//	アルファ値を減少。
	
	m_Scl += D3DXVECTOR3(0.01f, 0.01f, 0.01f);		//	スケール値を増加

	//	カウンターが0だったらパターン更新。
	if (m_AnimationUpdateUVCnt == 0) {
		++m_AnimationPatternCnt;
	}

	//	パターンがアニメーションの数を超えたら削除。
	if (m_AnimationPatternCnt >= m_AnimationTotalNumber) {
		m_Delete = true;
		return;
	}

	//	テクスチャのUV座標更新。
	CScene3D::TextureUV_Update(m_pVtxBuffPolygon, m_AnimationPatternCnt, m_AnimationCountX_Axis, m_AnimationCountY_Axis);
	//	カウンター更新。
	m_AnimationUpdateUVCnt = (m_AnimationUpdateUVCnt + 1) % m_UpdateTimeCnt;

	
	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//	色設定
	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;

	m_pVtxBuffPolygon->Unlock();
}

/******************************************************************************
	関数名 : void CSmoke::Draw(void)
	引数   : void
	戻り値 : なし
	説明   : 描画処理だが、通常のビルボードの描画処理ではなく、回転と拡縮ができる描画処理。
******************************************************************************/
void CSmoke::Draw(void)
{
	RendererBegin();

	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//行列式に使う作業用変数
	D3DXMATRIX mtxScl, mtxRot, mtxTrans, mtxView, invMatrix;

	//ワールドマトリクスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXMatrixScaling(&mtxScl, m_Scl.x, m_Scl.y, m_Scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);		//スケールを反映

	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);		//回転を反映

	pDevice->GetTransform(D3DTS_VIEW, &mtxView);		//	ビューマトリクスを求める

	D3DXMatrixInverse(&invMatrix, NULL, &mtxView);		//	逆行列を求める
	//	座標初期化
	invMatrix._41 = 0.0f;
	invMatrix._42 = 0.0f;
	invMatrix._43 = 0.0f;

	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&invMatrix, &invMatrix, &mtxTrans);	//移動を反映

	//	座標用のマトリクスともともとのマトリクスを掛け合わせる。
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &invMatrix);

	//	ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//	頂点バッファをデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

	//	頂点座標ふぉーまっと
	pDevice->SetFVF(FVF_VERTEX_3D);
	//	テクスチャの設定
	pDevice->SetTexture(0, CTexture::GetTexture(m_SceneType, m_TextureType));
	//	ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	RendererEnd();
}

/******************************************************************************
	関数名 : void CSmoke::RendererBegin(void)
	引数   : void
	戻り値 : なし
	説明   : 描画する際のレンダラーの設定。
******************************************************************************/
void CSmoke::RendererBegin(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//	アルファブレンド設定////////////////////////////////////////////////////
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//描画先と描画元の値を加算
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	///////////////////////////////////////////////////////////////////////////
}

/******************************************************************************
	関数名 : void CSmoke::RendererBegin(void)
	引数   : void
	戻り値 : なし
	説明   : 描画した後のレンダラーの設定。
******************************************************************************/
void CSmoke::RendererEnd(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//	アルファブレンド設定////////////////////////////////////////////////////
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//描画先と描画元の値を加算
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);//カリングの設定
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//アルファブレンドの設定
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//ソースブレンドの設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//インバースブレンドの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	///////////////////////////////////////////////////////////////////////////
}

/******************************************************************************
	関数名 : void CSmoke::Create(D3DXVECTOR3 pos, float width, float height, int textureType)
	引数   : pos, width, height, textureType
	戻り値 : なし
	説明   : 生成関数。
******************************************************************************/
void CSmoke::Create(D3DXVECTOR3 pos, float rot, float width, float height, int textureType)
{
	CSmoke *obj = new CSmoke;
	obj->Init(pos, rot, width, height, textureType);
}
