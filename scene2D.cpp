/******************************************************************************
	タイトル名 : Scene2Dクラス
	ファイル名 : scene2D.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/04/19
	更新日     : 2016/05/31
				 2016/05/16
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

/******************************************************************************
	関数名 : CScene2D::CScene2D(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene(Priority, objType, sceneType)
	説明   : コンストラクタ
			 変数の初期化。
******************************************************************************/
CScene2D::CScene2D(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene(Priority, objType, sceneType)
{
	m_pVtxBuffPolygon = NULL;
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Width = 0.0f;
	m_Height = 0.0f;
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
	関数名 : CScene2D::~CScene2D()
	説明   : デストラクタ
			 クラスが破棄されたときに呼ばれるが、中身がないので特に記述する事なし。
******************************************************************************/
CScene2D::~CScene2D()
{
}

/******************************************************************************
	関数名 : void CScene2D::Init(D3DXVECTOR3 pos, float width, float height, int textureType)
	引数   : pos, width, height
	戻り値 : なし
	説明   : 引数の値をそれぞれ代入して、2Dポリゴンの生成。
******************************************************************************/
void CScene2D::Init(D3DXVECTOR3 pos, float width, float height, int textureType)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	VERTEX_2D *pVtx;

	//	変数の代入。
	m_Pos = pos;
	m_Width = width;
	m_Height = height;
	m_TextureType = textureType;

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffPolygon, NULL))) {
		return;
	}

	//頂点情報へのポインタを取得
	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//	座標設定
	pVtx[0].pos = D3DXVECTOR3(m_Pos.x - (m_Width * 0.5f), m_Pos.y - (m_Height * 0.5f), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_Pos.x + (m_Width * 0.5f), m_Pos.y - (m_Height * 0.5f), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_Pos.x - (m_Width * 0.5f), m_Pos.y + (m_Height * 0.5f), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_Pos.x + (m_Width * 0.5f), m_Pos.y + (m_Height * 0.5f), 0.0f);

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

	//	バッファ解放
	m_pVtxBuffPolygon->Unlock();
}

/******************************************************************************
	関数名 : void CScene2D::Uninit(void)
	引数   : void
	戻り値 : なし
	説明   : 頂点バッファの解放。
******************************************************************************/
void CScene2D::Uninit(void)
{
	//	頂点バッファの解放。
	SAFE_RELEASE(m_pVtxBuffPolygon);
}

/******************************************************************************
	関数名 : void CScene2D::Update(void)
	引数   : void
	戻り値 : なし
	説明   : 更新処理。
******************************************************************************/
void CScene2D::Update(void)
{
}

/******************************************************************************
	関数名 : void CScene2D::Draw(void)
	引数   : void
	戻り値 : なし
	説明   : 描画処理。。
******************************************************************************/
void CScene2D::Draw(void)
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
	pDevice->SetTexture(0, CTexture::GetTexture(m_SceneType, m_TextureType));

	//ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

/******************************************************************************
	関数名 : CScene2D * CScene2D::Create(D3DXVECTOR3 pos, float width, float height)
	引数   : pos, width, height
	戻り値 : obj
	説明   : 更新処理。
******************************************************************************/
CScene2D * CScene2D::Create(D3DXVECTOR3 pos, float width, float height, int textureType)
{
	CScene2D *obj = new CScene2D;
	obj->Init(pos, width, height, textureType);
	return obj;
}

/******************************************************************************
	関数名 : void CScene2D::TextureUV_Update(LPDIRECT3DVERTEXBUFFER9 vtxBuff, int patternCnt, int animContX_Axis, int animCntY_Axis)
	引数   : vtxBuff, patternCnt, animContX_Axis, animCntY_Axis
	戻り値 : なし
	説明   : テクスチャのUV更新関数。
******************************************************************************/
void CScene2D::TextureUV_Update(LPDIRECT3DVERTEXBUFFER9 vtxBuff, int patternCnt, int animContX_Axis, int animCntY_Axis)
{
	VERTEX_2D *pVtx;

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