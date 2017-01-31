/******************************************************************************
	タイトル名 : Bulletクラス
	ファイル名 : bullet.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/12/15
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
#include "bullet.h"
#include "collision.h"
#include "capsule.h"
#include "collisionList.h"
#include "smoke.h"
#include "MeshField.h"
#include "Explosion.h"
#include "Mode.h"
#include "game.h"

/******************************************************************************
	関数名 : CBullet::CBullet(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CBillboard(Priority, objType, sceneType)
	説明   : コンストラクタ
******************************************************************************/
CBullet::CBullet(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CBillboard(Priority, objType, sceneType)
{
	m_Movement = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_TypeUsingSelf = OBJ_TYPE_NONE;
}

/******************************************************************************
	関数名 : CPlayer::~CPlayer()
	説明   : デストラクタ
******************************************************************************/
CBullet::~CBullet()
{
}

/******************************************************************************
	関数名 : void CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float height, int textureType,OBJ_TYPE objType)
	引数   : pos, rot, width, height, textureType, objType
	戻り値 : なし
	説明   : 座標と角度の設定。
******************************************************************************/
void CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float height, int textureType, OBJ_TYPE objType)
{
	CBillboard::Init(pos, width, height, 0.0f, textureType);

	m_GoAngle = rot;
	m_TypeUsingSelf = objType;
	m_CollRadius = 1.0f;
	m_MovingDistance = 25.0f;
	//	移動量更新。
	m_Movement.x = sinf(m_GoAngle.y) * cosf(m_GoAngle.x) * m_MovingDistance;
	m_Movement.y = sinf(m_GoAngle.x) * m_MovingDistance;
	m_Movement.z = cosf(m_GoAngle.y) * cosf(m_GoAngle.x) * m_MovingDistance;

	m_pCollision = CCapsule::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_CollRadius, D3DXVECTOR3(0.0f, 0.0f, 0.0f) , m_Movement, this);
}

/******************************************************************************
	関数名 : void CBullet::Uninit(void)
	引数   : void
	戻り値 : なし
	説明   : 解放処理。
******************************************************************************/
void CBullet::Uninit(void)
{
	CBillboard::Uninit();

	m_pCollision->SetDelete(true);
	//SAFE_DELETE(m_pCollision);
}

/******************************************************************************
	関数名 : void CBullet::Update(void)
	引数   : void
	戻り値 : なし
	説明   : 移動処理などの更新処理。
******************************************************************************/
void CBullet::Update(void)
{
	// それぞれのポインタと値を取得
	CManager *pManager = GetManager();
	CGame *pGame = (CGame *)CManager::GetMode();
	CMeshField *pMeshField = pGame->GetMeshField();

	//	移動量更新。
	m_Movement.x = sinf(m_GoAngle.y) * cosf(m_GoAngle.x) * m_MovingDistance;
	m_Movement.y = sinf(m_GoAngle.x) * m_MovingDistance;
	m_Movement.z = cosf(m_GoAngle.y) * cosf(m_GoAngle.x) * m_MovingDistance;

	//	座標更新。
	m_Pos -= m_Movement;

	CheckOutsideRange(pMeshField);

	float rotation = (rand() % 628) / 100.0f;

	CManager::CheckRot(&rotation);
	CSmoke::Create(m_Pos, rotation, 50.0f, 50.0f, CTexture::G_SMOKE);
}

/******************************************************************************
	関数名 : void CBullet::RendererBegin(void)
	引数   : void
	戻り値 : なし
	説明   : 描画する際のレンダラーの設定。
******************************************************************************/
void CBullet::RendererBegin(void)
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
	関数名 : void CSmoke::RendererEnd(void)
	引数   : void
	戻り値 : なし
	説明   : 描画した後のレンダラーの設定。
******************************************************************************/
void CBullet::RendererEnd(void)
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
	関数名 : void CBullet::CheckOutsideRange(CMeshField * mesh)
	引数   : mesh
	戻り値 : なし
	説明   : 範囲外に出たら削除する。
******************************************************************************/
void CBullet::CheckOutsideRange(CMeshField * mesh)
{
	//	メッシュフィールド上でのY座標取得。
	float posY = mesh->Collision(m_Pos);

	if (m_Pos.x > mesh->GetFieldWidth() - 100.0f || m_Pos.x < -mesh->GetFieldWidth() + 100.0f ||
		m_Pos.z > mesh->GetFieldDepth() - 100.0f || m_Pos.z < -mesh->GetFieldDepth() + 100.0f) {
		m_Delete = true;
	}

	//	求めた値よりY座標が低かったら削除。
	if (posY >= m_Pos.y) {
		m_Delete = true;
		CExplosion::Create(m_Pos, 125.0f, 125.0f, CTexture::G_EXPLOSION);
	}
}

/******************************************************************************
	関数名 : void CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float height, int textureType, OBJ_TYPE objType)
	引数   : pos, rot, width, height, textureType, objType
	戻り値 : なし
	説明   : 生成関数。
******************************************************************************/
void CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float height, int textureType, OBJ_TYPE objType)
{
	CBullet *obj = new CBullet;
	obj->Init(pos, rot, width, height, textureType, objType);
}
