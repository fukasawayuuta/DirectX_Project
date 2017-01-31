/******************************************************************************
	タイトル名 : Boost
	ファイル名 : boost.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2017/01/27
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
#include "billboard.h"
#include "texture.h"
#include "boost.h"

/******************************************************************************
	マクロ定義
******************************************************************************/
const int TEXTURE_UPDATE_TIMING_COUNT = 1;		//	テクスチャアニメーションを更新するタイミングカウンタ。

/******************************************************************************
	関数名 : CBoost::CBoost(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CBillboard(Priority, objType, sceneType)
	説明   : コンストラクタ
******************************************************************************/
CBoost::CBoost(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CBillboard(Priority, objType, sceneType)
{
	m_DuringUse = false;
	m_Magnification_Rate = 0.0f;
}

/******************************************************************************
	関数名 : CBoost::~CBoost()
	説明   : デストラクタ
******************************************************************************/
CBoost::~CBoost()
{
}

/******************************************************************************
	関数名 : void CBoost::Init(D3DXVECTOR3 pos, float width, float height, int textureType, CScene * target)
	引数   : pos, width, height, textureType, target
	戻り値 : なし
	説明   : ビルボードの設定と使う対象の設定。
******************************************************************************/
void CBoost::Init(D3DXVECTOR3 pos, float width, float height, int textureType)
{
	CBillboard::Init(pos, width, height, 0.0f, textureType);
	m_Magnification_Rate = 3.0f;

	//	アニメーションの変数設定。
	m_AnimationCountX_Axis = 8;
	m_AnimationCountY_Axis = 4;
	m_AnimationTotalNumber = m_AnimationCountX_Axis * m_AnimationCountY_Axis;
}

/******************************************************************************
関数名 : void CBoost::Uninit(void)
引数   : void
戻り値 : なし
説明   : 解放処理。
******************************************************************************/
void CBoost::Uninit(void)
{
	CBillboard::Uninit();
}

/******************************************************************************
	関数名 : void CPlayer::Update(void)
	引数   : void
	戻り値 : なし
	説明   : 更新処理。
******************************************************************************/
void CBoost::Update(void)
{
	if (m_DuringUse == true) {
		m_Scl = D3DXVECTOR3(m_Magnification_Rate, m_Magnification_Rate, m_Magnification_Rate);
		m_DuringUse = false;
	}

	//	カウンターが0だったらパターン更新。
	if (m_AnimationUpdateUVCnt == 0) {
		++m_AnimationPatternCnt;
	}

	//	パターンがアニメーションの数を超えたら削除。
	if (m_AnimationPatternCnt >= m_AnimationTotalNumber) {
		m_AnimationPatternCnt = 0;
	}

	m_Scl -= D3DXVECTOR3(0.1f, 0.1f, 0.1f);

	if (m_Scl.x < 1.0f) {
		m_Scl.x = 1.0f;
	}

	if (m_Scl.y < 1.0f) {
		m_Scl.y = 1.0f;
	}

	if (m_Scl.z < 1.0f) {
		m_Scl.z = 1.0f;
	}

	//	テクスチャのUV座標更新。
	CScene3D::TextureUV_Update(m_pVtxBuffPolygon, m_AnimationPatternCnt, m_AnimationCountX_Axis, m_AnimationCountY_Axis);
	//	カウンター更新。
	m_AnimationUpdateUVCnt = (m_AnimationUpdateUVCnt + 1) % TEXTURE_UPDATE_TIMING_COUNT;
}

/******************************************************************************
	関数名 : void CBoost::RendererBegin(void)
	引数   : void
	戻り値 : なし
	説明   : 描画する際のレンダラーの設定。
******************************************************************************/
void CBoost::RendererBegin(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//	アルファブレンド設定////////////////////////////////////////////////////
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//描画先と描画元の値を加算
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	///////////////////////////////////////////////////////////////////////////
}

/******************************************************************************
	関数名 : void CBoost::RendererEnd(void)
	引数   : void
	戻り値 : なし
	説明   : 描画した後のレンダラーの設定。
******************************************************************************/
void CBoost::RendererEnd(void)
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
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	///////////////////////////////////////////////////////////////////////////
}

/******************************************************************************
	関数名 : CBoost * CBoost::Create(D3DXVECTOR3 pos, float width, float height, int textureType, CScene * target)
	引数   :pos, width, height, textureType, target
	戻り値 : obj
	説明   : 生成関数。
******************************************************************************/
CBoost * CBoost::Create(D3DXVECTOR3 pos, float width, float height, int textureType)
{
	CBoost *obj = new CBoost;
	obj->Init(pos, width, height, textureType);

	return obj;
}
