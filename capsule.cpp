/******************************************************************************
	タイトル名 : CCapsuleクラス
	ファイル名 : capsule.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2017/01/05
	更新日     :
******************************************************************************/
/******************************************************************************
	インクルードファイル
******************************************************************************/
#include "main.h"
#include "scene.h"
#include "collision.h"
#include "capsule.h"
#include "collisionDetection.h"
#include "collisionList.h"


/******************************************************************************
	関数名 : CCapsule::CCapsule(CScene *scene, int Priority) : CCollision(scene, Priority)
	説明   : コンストラクタ
******************************************************************************/
CCapsule::CCapsule(CScene *scene, int Priority) : CCollision(scene, Priority)
{
	m_Hit = false;
	m_Delete = false;
	m_Segment.start_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Segment.end_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Segment.radius = 0.0f;
	m_HitPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

/******************************************************************************
	関数名 : CSphere::~CSphere()
	説明   : デストラクタ
******************************************************************************/
CCapsule::~CCapsule()
{
}

/******************************************************************************
	関数名 : void CCapsule::Action(CCapsule * other)
	引数   : other
	戻り値 : なし
	説明   : otherと自分とでカプセル同士のあたり判定をする。
******************************************************************************/
void CCapsule::Action(CCapsule * other)
{
	CCollisionDetection::GetInstance()->Capsule_To_Capsule(other, this);
}

/******************************************************************************
	関数名 : void CCapsule::Action(CSphere * other)
	引数   : other
	戻り値 : なし
	説明   : otherと自分とで球体とカプセルのあたり判定をする。
******************************************************************************/
void CCapsule::Action(CSphere * other)
{
	CCollisionDetection::GetInstance()->Sphere_To_Capsule(other, this);
}

/******************************************************************************
	関数名 : void CSphere::Init(D3DXVECTOR3 pos, float radius)
	引数   : pos, radius
	戻り値 : なし
	説明   : 座標と半径と始点と終点の設定。
******************************************************************************/
void CCapsule::Init(D3DXVECTOR3 pos, float radius, D3DXVECTOR3 start_pos, D3DXVECTOR3 end_pos)
{
	m_Segment.start_pos = start_pos;
	m_Segment.end_pos = end_pos;
	m_Segment.radius = radius;
	m_LoaclPos = pos;

	m_CollisionList = new CCollisionList;
}

/******************************************************************************
	関数名 : void CSphere::Uninit(void)
	引数   : void
	戻り値 : なし
	説明   : リストの解放。
******************************************************************************/
void CCapsule::Uninit(void)
{
	m_CollisionList->Uninit();
	SAFE_DELETE(m_CollisionList);
}

/******************************************************************************
	関数名 : void CCapsule::Draw(void)
	引数   : void
	戻り値 : なし
	説明   : 描画関数。
******************************************************************************/
void CCapsule::Draw(void)
{
}

/******************************************************************************
	関数名 : CCapsule * CCapsule::Create(D3DXVECTOR3 pos, float radius, D3DXVECTOR3 start_pos, D3DXVECTOR3 end_pos, CScene *scene)
	引数   : pos, radius, start_pos, end_pos, scene
	戻り値 : なし
	説明   : 生成関数。
******************************************************************************/
CCapsule * CCapsule::Create(D3DXVECTOR3 pos, float radius, D3DXVECTOR3 start_pos, D3DXVECTOR3 end_pos, CScene *scene)
{
	CCapsule *obj = new CCapsule(scene);
	obj->Init(pos, radius, start_pos, end_pos);
	return obj;
}
