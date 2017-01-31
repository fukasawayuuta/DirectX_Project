/******************************************************************************
	タイトル名 : SceneIndexクラス
	ファイル名 : sceneIndex.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/12/13
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
#include "sceneIndex.h"

/******************************************************************************
	関数名 : CSceneIndex::CSceneIndex(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene3D(Priority, objType, sceneType)
	説明   : コンストラクタ
			 変数の初期化。
******************************************************************************/
CSceneIndex::CSceneIndex(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene3D(Priority, objType, sceneType)
{
	m_pIdxBuff = NULL;
	m_VertexNum = 0;
	m_PolygonNum = 0;
}

/******************************************************************************
	関数名 : CSceneIndex::~CSceneIndex()
	説明   : デストラクタ
			 クラスが破棄されたときに呼ばれるが、中身がないので特に記述する事なし。
******************************************************************************/
CSceneIndex::~CSceneIndex()
{
}

/******************************************************************************
	関数名 : void CSceneIndex::Uninit(void)
	引数   : void
	戻り値 : なし
	説明   : インデックスバッファの解放処理。
******************************************************************************/
void CSceneIndex::Uninit(void)
{
	CScene3D::Uninit();
	SAFE_RELEASE(m_pIdxBuff);
}

/******************************************************************************
	関数名 : void CScene3D::DrawIndex(void)
	引数   : void
	戻り値 : なし
	説明   : インデックスバッファを使った描画処理。
******************************************************************************/
void CSceneIndex::Draw(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//行列式に使う作業用変数
	D3DXMATRIX mtxScl, mtxRot, mtxTrans;
	//ワールドマトリクスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXMatrixScaling(&mtxScl, m_Scl.x, m_Scl.y, m_Scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);		//スケールを反映

	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);		//回転を反映

	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);	//移動を反映

																//	ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

	pDevice->SetIndices(m_pIdxBuff);

	//頂点座標ふぉーまっと
	pDevice->SetFVF(FVF_VERTEX_3D);
	//	テクスチャの設定
	pDevice->SetTexture(0, CTexture::GetTexture(m_SceneType, m_TextureType));
	//ポリゴン描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_VertexNum, 0, m_PolygonNum);
}
