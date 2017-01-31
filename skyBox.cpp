/******************************************************************************
	タイトル名 : SkyBoxクラス
	ファイル名 : skyBox.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2017/01/17
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
#include "sceneX.h"
#include "billboard.h"
#include "model.h"
#include "motionManager.h"
#include "dynamicModel.h"
#include "skyBox.h"
#include "Mode.h"
#include "game.h"
#include "player.h"


/******************************************************************************
	マクロ定義
******************************************************************************/
const int FACE_NUM = 6;		//	面の数。

/******************************************************************************
	関数名 : CSkyBox::CSkyBox(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CDynamicModel(Priority, objType, sceneType)
	説明   : コンストラクタ
******************************************************************************/
CSkyBox::CSkyBox(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene3D(Priority, objType, sceneType)
{
}

/******************************************************************************
	関数名 : CSkyBox::~CSkyBox()
	説明   : デストラクタ
******************************************************************************/
CSkyBox::~CSkyBox()
{
}

/******************************************************************************
	関数名 : void CSkyBox::Init(D3DXVECTOR3 pos, float width, float height, float depth, int textureType)
	引数   : pos, width, height, depth, textureType
	戻り値 : なし
	説明   : 座標と角度の設定。
******************************************************************************/
void CSkyBox::Init(D3DXVECTOR3 pos, float width, float height, float depth, int textureType)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	VERTEX_3D *pVtx;

	//	引数設定。
	m_Pos = pos;
	m_Width = width;
	m_Height = height;
	m_Depth = depth;
	m_TextureType = textureType;

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (4 * FACE_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffPolygon, NULL))) {
		return;
	}

	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//	左側の面////////////////////////////////////////////////////////////////////////
	pVtx[0].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y + m_Height, m_Pos.z - m_Depth);
	pVtx[1].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y + m_Height, m_Pos.z + m_Depth);
	pVtx[2].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y - m_Height, m_Pos.z - m_Depth);
	pVtx[3].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y - m_Height, m_Pos.z + m_Depth);
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f / 3.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f, 1.0f / 3.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, (1.0f * 2.0f) / 3.0f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, (1.0f * 2.0f) / 3.0f);

	//	右側の面////////////////////////////////////////////////////////////////////////
	pVtx[4].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y + m_Height, m_Pos.z + m_Depth);
	pVtx[5].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y + m_Height, m_Pos.z - m_Depth);
	pVtx[6].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y - m_Height, m_Pos.z + m_Depth);
	pVtx[7].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y - m_Height, m_Pos.z - m_Depth);
	pVtx[4].tex = D3DXVECTOR2(0.5f, 1.0f / 3.0f);
	pVtx[5].tex = D3DXVECTOR2(0.75f, 1.0f / 3.0f);
	pVtx[6].tex = D3DXVECTOR2(0.5f, (1.0f * 2.0f) / 3.0f);
	pVtx[7].tex = D3DXVECTOR2(0.75f, (1.0f * 2.0f) / 3.0f);

	//	手前側の面////////////////////////////////////////////////////////////////////////
	pVtx[8].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y + m_Height, m_Pos.z - m_Depth);
	pVtx[9].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y + m_Height, m_Pos.z - m_Depth);
	pVtx[10].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y - m_Height, m_Pos.z - m_Depth);
	pVtx[11].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y - m_Height, m_Pos.z - m_Depth);
	pVtx[8].tex = D3DXVECTOR2(0.75f, 1.0f / 3.0f);
	pVtx[9].tex = D3DXVECTOR2(1.0f, 1.0f / 3.0f);
	pVtx[10].tex = D3DXVECTOR2(0.75f, (1.0f * 2.0f) / 3.0f);
	pVtx[11].tex = D3DXVECTOR2(1.0f, (1.0f * 2.0f) / 3.0f);

	//	奥側の面////////////////////////////////////////////////////////////////////////
	pVtx[12].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y + m_Height, m_Pos.z + m_Depth);
	pVtx[13].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y + m_Height, m_Pos.z + m_Depth);
	pVtx[14].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y - m_Height, m_Pos.z + m_Depth);
	pVtx[15].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y - m_Height, m_Pos.z + m_Depth);
	pVtx[12].tex = D3DXVECTOR2(0.25f, 1.0f / 3.0f);
	pVtx[13].tex = D3DXVECTOR2(0.5f, 1.0f / 3.0f);
	pVtx[14].tex = D3DXVECTOR2(0.25f, (1.0f * 2.0f) / 3.0f);
	pVtx[15].tex = D3DXVECTOR2(0.5f, (1.0f * 2.0f) / 3.0f);

	//	上側の面////////////////////////////////////////////////////////////////////////
	pVtx[16].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y + m_Height, m_Pos.z - m_Depth);
	pVtx[17].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y + m_Height, m_Pos.z - m_Depth);
	pVtx[18].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y + m_Height, m_Pos.z + m_Depth);
	pVtx[19].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y + m_Height, m_Pos.z + m_Depth);
	pVtx[16].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[17].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[18].tex = D3DXVECTOR2(0.25f, 1.0f / 3.0f);
	pVtx[19].tex = D3DXVECTOR2(0.5f, 1.0f / 3.0f);

	//	下側の面////////////////////////////////////////////////////////////////////////
	pVtx[20].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y - m_Height, m_Pos.z + m_Depth);
	pVtx[21].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y - m_Height, m_Pos.z + m_Depth);
	pVtx[22].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y - m_Height, m_Pos.z - m_Depth);
	pVtx[23].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y - m_Height, m_Pos.z - m_Depth);
	pVtx[20].tex = D3DXVECTOR2(0.25f, (1.0f * 2.0f) / 3.0f);
	pVtx[21].tex = D3DXVECTOR2(0.5f, (1.0f * 2.0f) / 3.0f);
	pVtx[22].tex = D3DXVECTOR2(0.25f, 1.0f);
	pVtx[23].tex = D3DXVECTOR2(0.5f, 1.0f);

	for (int i = 0; i < FACE_NUM; i++, pVtx += 4) {
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//ポリゴンの頂点カラー設定
		pVtx[0].col = m_Color;
		pVtx[1].col = m_Color;
		pVtx[2].col = m_Color;
		pVtx[3].col = m_Color;
	}

	m_pVtxBuffPolygon->Unlock();
}

/******************************************************************************
	関数名 : void CSkyBox::Update(void)
	引数   : void
	戻り値 : なし
	説明   : 更新関数。
******************************************************************************/
void CSkyBox::Update(void)
{
	CManager *pManager = GetManager();
	CInput   *pInput = pManager->GetInput();
	CGame	 *pGame = (CGame *)CManager::GetMode();
	CPlayer  *pPlayer = pGame->GetPlayer();
	m_Rot.y += 0.0001f;
	m_Pos = pPlayer->GetPos();
}

/******************************************************************************
	関数名 : void CSkyBox::Draw(void)
	引数   : void
	戻り値 : なし
	説明   : 描画関数。
******************************************************************************/
void CSkyBox::Draw(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//行列式に使う作業用変数
	D3DXMATRIX mtxScl, mtxRot, mtxTrans;

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		//	ライティング : OFF

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

	//	頂点バッファをデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

	//	頂点座標ふぉーまっと
	pDevice->SetFVF(FVF_VERTEX_3D);
	//	テクスチャの設定
	pDevice->SetTexture(0, CTexture::GetTexture(m_SceneType, m_TextureType));

	for (int i = 0; i < FACE_NUM; i++) {
		//	ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);		//	ライティング : OFF
}

/******************************************************************************
	関数名 : CSkyBox * CSkyBox::Create(D3DXVECTOR3 pos, float width, float height, float depth, int textureType)
	引数   : pos, width, height, depth, textureType
	戻り値 : obj
	説明   : 生成関数。
******************************************************************************/
CSkyBox * CSkyBox::Create(D3DXVECTOR3 pos, float width, float height, float depth, int textureType)
{
	CSkyBox *obj = new CSkyBox;
	obj->Init(pos, width, height, depth, textureType);
	return obj;
}
