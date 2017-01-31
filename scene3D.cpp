/******************************************************************************
	タイトル名 : Scene3Dクラス
	ファイル名 : scene3D.cpp
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

/******************************************************************************
	関数名 : CScene3D::CScene3D(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene(Priority, objType, sceneType)
	説明   : コンストラクタ
			 変数の初期化。
******************************************************************************/
CScene3D::CScene3D(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene(Priority, objType, sceneType)
{
	m_pVtxBuffPolygon = NULL;
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Width = 0.0f;
	m_Height = 0.0f;
	m_Depth = 0.0f;
	m_Length = 0.0f;
	m_Angle = 0.0f;
	m_Delete = false;
	m_Active = true;
	m_TextureType = 0;

	m_AnimationPatternCnt = 0;
	m_AnimationTotalNumber = 0;
	m_AnimationCountX_Axis = 0;
	m_AnimationCountY_Axis = 0;
	m_AnimationUpdateUVCnt = 0;
}

/******************************************************************************
	関数名 : CScene3D::~CScene3D()
	説明   : デストラクタ
			 クラスが破棄されたときに呼ばれるが、中身がないので特に記述する事なし。
******************************************************************************/
CScene3D::~CScene3D()
{
}

/******************************************************************************
	関数名 : void CScene2D::Init(D3DXVECTOR3 pos, float width, float height, float depth, int textureType)
	引数   : pos, width, height
	戻り値 : なし
	説明   : 引数の値をそれぞれ代入して、2Dポリゴンの生成。
******************************************************************************/
void CScene3D::Init(D3DXVECTOR3 pos, float width, float height, float depth, int textureType)
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
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffPolygon, NULL))) {
		return;
	}

	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//	座標設定
	pVtx[0].pos = D3DXVECTOR3(0.0f - (m_Width * 0.5f), 0.0f + (m_Height * 0.5f), 0.0f + (m_Depth * 0.5f));
	pVtx[1].pos = D3DXVECTOR3(0.0f + (m_Width * 0.5f), 0.0f + (m_Height * 0.5f), 0.0f + (m_Depth * 0.5f));
	pVtx[2].pos = D3DXVECTOR3(0.0f - (m_Width * 0.5f), 0.0f - (m_Height * 0.5f), 0.0f - (m_Depth * 0.5f));
	pVtx[3].pos = D3DXVECTOR3(0.0f + (m_Width * 0.5f), 0.0f - (m_Height * 0.5f), 0.0f - (m_Depth * 0.5f));

	//	作業用変数
	D3DXVECTOR3 vec1 = pVtx[1].pos - pVtx[0].pos;
	D3DXVECTOR3 vec2 = pVtx[2].pos - pVtx[0].pos;

	D3DXVECTOR3 Normal1;
	D3DXVECTOR3 Normal2;
	D3DXVECTOR3 Normal3;
	D3DXVECTOR3 Normal4;

	//	外積計算
	D3DXVec3Cross(&Normal1, &vec1, &vec2);

	//	単位ベクトル化
	D3DXVec3Normalize(&Normal1, &Normal1);
	pVtx[0].nor = Normal1;

	//	ベクトル
	vec1 = pVtx[1].pos - pVtx[3].pos;
	vec2 = pVtx[2].pos - pVtx[3].pos;

	//	外積計算
	D3DXVec3Cross(&Normal4, &vec2, &vec1);

	//	単位ベクトル化
	D3DXVec3Normalize(&Normal4, &Normal4);
	pVtx[3].nor = Normal4;

	//	単位ベクトル化
	D3DXVec3Normalize(&Normal2, &(Normal1 + Normal4));
	pVtx[1].nor = Normal2;

	//	単位ベクトル化
	D3DXVec3Normalize(&Normal3, &(Normal1 + Normal4));
	pVtx[2].nor = Normal3;

	//	色設定
	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;

	//テクスチャ座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuffPolygon->Unlock();
}

/******************************************************************************
	関数名 : void CScene3D::Uninit(void)
	引数   : void
	戻り値 : なし
	説明   : 頂点バッファとインデックスバッファの解放。
******************************************************************************/
void CScene3D::Uninit(void)
{
	SAFE_RELEASE(m_pVtxBuffPolygon);
}

/******************************************************************************
	関数名 : void CScene2D::Update(void)
	引数   : void
	戻り値 : なし
	説明   : 更新処理。
******************************************************************************/
void CScene3D::Update(void)
{
}

/******************************************************************************
	関数名 : void CScene3D::Draw(void)
	引数   : void
	戻り値 : なし
	説明   : 描画処理。
******************************************************************************/
void CScene3D::Draw(void)
{
	RendererBegin();

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
	関数名 : CScene3D * CScene3D::Create(D3DXVECTOR3 pos, float width, float height, float depth, int textureType)
	引数   : pos, width, height, depth, textureType
	戻り値 : obj
	説明   : 生成処理。
******************************************************************************/
CScene3D * CScene3D::Create(D3DXVECTOR3 pos, float width, float height, float depth, int textureType)
{
	CScene3D *obj = new CScene3D;
	obj->Init(pos, width, height, depth, textureType);
	return obj;
}

/******************************************************************************
	関数名 : void CScene3D::TextureUV_Update(LPDIRECT3DVERTEXBUFFER9 vtxBuff, int patternCnt, int animContX_Axis, int animCntY_Axis)
	引数   : vtxBuff, patternCnt, animContX_Axis, animCntY_Axis
	戻り値 : なし
	説明   : テクスチャのUV更新関数。
******************************************************************************/
void CScene3D::TextureUV_Update(LPDIRECT3DVERTEXBUFFER9 vtxBuff, int patternCnt, int animContX_Axis, int animCntY_Axis)
{
	VERTEX_3D *pVtx;

	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//	テクスチャ座標更新
	pVtx[0].tex.x = (patternCnt % animContX_Axis) * (1.0f / animContX_Axis);
	pVtx[0].tex.y = ((patternCnt - (patternCnt % animContX_Axis)) / animContX_Axis) * ((float)1 / animCntY_Axis);

	pVtx[1].tex.x = (patternCnt % animContX_Axis) * (1.0f / animContX_Axis) + (1.0f / animContX_Axis);
	pVtx[1].tex.y = ((patternCnt - (patternCnt % animContX_Axis)) / animContX_Axis) * ((float)1 / animCntY_Axis);

	pVtx[2].tex.x = (patternCnt % animContX_Axis) * (1.0f / animContX_Axis);
	pVtx[2].tex.y = (((patternCnt - (patternCnt % animContX_Axis)) / animContX_Axis) * ((float)1 / animCntY_Axis)) + ((float)1 / animCntY_Axis);

	pVtx[3].tex.x = (patternCnt % animContX_Axis) * (1.0f / animContX_Axis) + (1.0f / animContX_Axis);
	pVtx[3].tex.y = (((patternCnt - (patternCnt % animContX_Axis)) / animContX_Axis) * ((float)1 / animCntY_Axis)) + ((float)1 / animCntY_Axis);

	vtxBuff->Unlock();
}
