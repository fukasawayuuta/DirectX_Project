/******************************************************************************
	タイトル名 : Billboardクラス
	ファイル名 : billboard.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/12/12
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

/******************************************************************************
	関数名 : CBillboard::CBillboard(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene3D(Priority, objType, sceneType)
	説明   : コンストラクタ
			 変数の初期化。
******************************************************************************/
CBillboard::CBillboard(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene3D(Priority, objType, sceneType)
{
	m_bY_Axis_Fixed = false;
}

/******************************************************************************
	関数名 : CBillboard::~CBillboard()
	説明   : デストラクタ
			 クラスが破棄されたときに呼ばれるが、中身がないので特に記述する事なし。
******************************************************************************/
CBillboard::~CBillboard()
{
}

/******************************************************************************
	関数名 : void CBillboard::Draw(void)
	引数   : void
	戻り値 : なし
	説明   : 通常のビルボード描画処理。
******************************************************************************/
void CBillboard::Draw(void)
{
	RendererBegin();

	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//行列式に使う作業用変数
	D3DXMATRIX mtxScl, mtxRot, mtxTrans, mtxView;

	//ワールドマトリクスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	pDevice->GetTransform(D3DTS_VIEW, &mtxView);		//	ビューマトリクスを求める

	if (m_bY_Axis_Fixed) {	//	Y軸固定のビルボード。
		//	Y軸だけ回転させる処理////////////////////////////////////////////////////////////////////////
		D3DXMATRIX matBillBoard;
		D3DXVECTOR3 vDir = CCamera::GetVecRV();

		if (vDir.x >= 0.0f) {
			D3DXMatrixRotationY(&matBillBoard, -atanf(vDir.z / vDir.x) + D3DX_PI / 2.0f);
		}
		else {
			D3DXMatrixRotationY(&matBillBoard, -atanf(vDir.z / vDir.x) - D3DX_PI / 2.0f);
		}

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &matBillBoard);
	}
	else {		//	通常のビルボード。
		D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);		//	逆行列を求める

		//	座標初期化
		m_mtxWorld._41 = 0.0f;
		m_mtxWorld._42 = 0.0f;
		m_mtxWorld._43 = 0.0f;
	}

	//	スケールを反映
	D3DXMatrixScaling(&mtxScl, m_Scl.x, m_Scl.y, m_Scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);		
	//	回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);		
	//	移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);	

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