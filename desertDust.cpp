/******************************************************************************
	タイトル名 : CDesertDustクラス
	ファイル名 : desertDust.cpp
	作成者     : AT-13C-284 36 深澤佑太
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
#include "texture.h"
#include "scene3D.h"
#include "sceneIndex.h"
#include "camera.h"
#include "billboard.h"
#include "desertDust.h"
#include "Mode.h"
#include "game.h"
#include "MeshField.h"

/******************************************************************************
	マクロ適宜
******************************************************************************/
const float MOVE_SPEED = 3.0f;			//	移動量。

/******************************************************************************
	関数名 : CDesertDust::CDesertDust(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CBillboard(Priority, objType, sceneType)
	説明   : コンストラクタ
******************************************************************************/
CDesertDust::CDesertDust(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CBillboard(Priority, objType, sceneType)
{
	m_movePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_MoveSpeed = rand() % (int)MOVE_SPEED + 1;
}

/******************************************************************************
	関数名 : CDesertDust::~CDesertDust()
	説明   : デストラクタ
******************************************************************************/
CDesertDust::~CDesertDust()
{
}

/******************************************************************************
	関数名 : void CDesertDust::Update(void)
	引数   : void
	戻り値 : なし
	説明   : 座標の更新。
******************************************************************************/
void CDesertDust::Update(void)
{
	// それぞれのポインタと値を取得
	CManager *pManager = GetManager();
	CGame *pGame = (CGame *)CManager::GetMode();
	CMeshField *pMeshField = pGame->GetMeshField();

	//	左に直角的に進む
	m_movePos.x = sinf((-D3DX_PI * 0.5f) - m_Rot.y) * m_MoveSpeed;
	m_movePos.z = -cosf((-D3DX_PI * 0.5f) - m_Rot.y) * m_MoveSpeed;

	//	座標更新。
	m_Pos += m_movePos;

	CheckOutsideRange(pMeshField);
}

/******************************************************************************
	関数名 : void CDesertDust::Create(D3DXVECTOR3 pos, float width, float height, int textureType)
	引数   : pos, width, height, textureType
	戻り値 : なし
	説明   : 生成関数。
******************************************************************************/
void CDesertDust::Create(D3DXVECTOR3 pos, float width, float height, int textureType)
{
	CDesertDust *obj = new CDesertDust;
	obj->Init(pos, width, height, 0.0f, textureType);
}

/******************************************************************************
	関数名 : void CDesertDust::RendererBegin(void)
	引数   : void
	戻り値 : なし
	説明   : 描画する際のレンダラーの設定。
******************************************************************************/
void CDesertDust::RendererBegin(void)
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
	関数名 : void CDesertDust::RendererEnd(void)
	引数   : void
	戻り値 : なし
	説明   : 描画した後のレンダラーの設定。
******************************************************************************/
void CDesertDust::RendererEnd(void)
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
	関数名 : void CDesertDust::Reset(CMeshField *mesh)
	引数   : mesh
	戻り値 : なし
	説明   : 範囲外に出てしまったオブジェクトを範囲内に戻す。
******************************************************************************/
void CDesertDust::Reset(CMeshField *mesh)
{
	m_Pos.x = rand() % (int)mesh->GetFieldWidth();
	m_Pos.z = rand() % (int)(mesh->GetFieldDepth() * 2.0f) - (mesh->GetFieldDepth());
}

/******************************************************************************
	関数名 : void CDesertDust::CheckOutsideRange(CMeshField *mesh)
	引数   : mesh
	戻り値 : なし
	説明   : 範囲外に出てしまったかどうかを検出する。
******************************************************************************/
void CDesertDust::CheckOutsideRange(CMeshField *mesh)
{
	if (m_Pos.x > mesh->GetFieldWidth() || m_Pos.x < -mesh->GetFieldWidth() ||
		m_Pos.z > mesh->GetFieldDepth() || m_Pos.z < -mesh->GetFieldDepth()) {
		Reset(mesh);
	}
}
