/******************************************************************************
	タイトル名 : Enemyクラス
	ファイル名 : Enemy.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/12/17
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
#include "bullet.h"
#include "model.h"
#include "motionManager.h"
#include "dynamicModel.h"
#include "Enemy.h"
#include "collision.h"
#include "capsule.h"
#include "sphere.h"
#include "collisionDetection.h"
#include "collisionList.h"
#include "Explosion.h"

/******************************************************************************
	関数名 : CEnemy::CEnemy(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CDynamicModel(Priority, objType, sceneType)
	説明   : コンストラクタ
******************************************************************************/
CEnemy::CEnemy(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CDynamicModel(Priority, objType, sceneType)
{
	m_Hp = 0;
	m_pCollision = NULL;
	m_Radius = 0.0f;
}

/******************************************************************************
	関数名 : CEnemy::~CEnemy()
	説明   : デストラクタ
******************************************************************************/
CEnemy::~CEnemy()
{
}

/******************************************************************************
	関数名 : void CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, DYNAMICMODEL_TYPE modelType)
	引数   : pos, rot, modelType
	戻り値 : なし
	説明   : 座標、角度の初期化。
******************************************************************************/
void CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, DYNAMICMODEL_TYPE modelType)
{
	//	モーション設定。
	m_MotionManager = CMotionManager::Create(modelType, &m_mtxWorld);
	m_MotionManager->SetMotion(0);

	m_Pos = pos;
	m_Rot = rot;
	m_Hp = 3;
	//	あたり判定生成。
	m_pCollision = CSphere::Create(D3DXVECTOR3(0.0f, 50.0f, 0.0f), 50.0f, this);
	//m_pCollision = CCapsule::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 50.0f, 0.0f), this);
}

/******************************************************************************
	関数名 : void CEnemy::Uninit(void)
	引数   : void
	戻り値 : なし
	説明   : 解放処理。
******************************************************************************/
void CEnemy::Uninit(void)
{
	m_MotionManager->Uninit();
	SAFE_DELETE(m_MotionManager);

	if (m_pCollision != NULL) {
		m_pCollision->SetDelete(true);
		m_pCollision->GetCollisionList()->Uninit();
		m_pCollision = NULL;
	}
}

/******************************************************************************
	関数名 : void CEnemy::Update(void)
	引数   : void
	戻り値 : なし
	説明   : 解放処理。
******************************************************************************/
void CEnemy::Update(void)
{
	m_MotionManager->Update();

	if (m_Hp < 0) {
		m_Delete = true;
		CExplosion::Create(m_Pos, 125.0f, 125.0f, CTexture::G_EXPLOSION);
	}

	//	自分になにか当たったら。
	if (m_pCollision->GetHit()) {
		CCollisionList *collicionList = m_pCollision->GetCollisionList();
		std::list<CCollision *> *collList = collicionList->GetCollision();

		for (auto i = collList->begin(); i != collList->end();) {
			CCollision *object = (*i);

			if (object != NULL) {
				if (object->GetScene()->GetObjectType() == OBJ_TYPE_BULLET) {
					CBullet *bullet = (CBullet *)object->GetScene();

					//	撃った対象がPlayerだったら。
					if (bullet->GetTypeUsingSelf() == OBJ_TYPE_PLAYER) {
						//	HPが減少。
						--m_Hp;
						CExplosion::Create(object->GetScene()->GetPos(), 125.0f, 125.0f, CTexture::G_EXPLOSION);
						object->GetScene()->SetDelete(true);
					}
				}
				//	リストのなかみ削除。
				i = collList->erase(i);
			}
		}
	}
}

/******************************************************************************
	関数名 : void CEnemy::Draw(void)
	引数   : void
	戻り値 : なし
	説明   : 描画関数。
******************************************************************************/
void CEnemy::Draw(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//	行列式に使う作業用変数
	D3DXMATRIX mtxScl, mtxRot, mtxTrans;

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

	m_MotionManager->Draw();
}

/******************************************************************************
	関数名 : CEnemy * CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, DYNAMICMODEL_TYPE modelType)
	引数   : pos, rot, modelType
	戻り値 : obj
	説明   : 生成関数。
******************************************************************************/
CEnemy * CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, DYNAMICMODEL_TYPE modelType)
{
	CEnemy *obj = new CEnemy;
	obj->Init(pos, rot, modelType);
	return obj;
}
