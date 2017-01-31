/******************************************************************************
	タイトル名 : CSphereクラス
	ファイル名 : sphere.cpp
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
#include "sphere.h"
#include "collisionDetection.h"
#include "collisionList.h"

/******************************************************************************
	関数名 : CSphere::CSphere(CScene *scene, int Priority) : CCollision(scene, Priority)
	説明   : コンストラクタ
******************************************************************************/
CSphere::CSphere(CScene *scene, int Priority) : CCollision(scene, Priority)
{
	m_Delete = false;
	m_Hit = false;
	m_Radius = 0.0f;
}

/******************************************************************************
	関数名 : CSphere::~CSphere()
	説明   : デストラクタ
******************************************************************************/
CSphere::~CSphere()
{
}

/******************************************************************************
	関数名 : void CSphere::Init(D3DXVECTOR3 pos, float radius)
	引数   : pos, radius
	戻り値 : なし
	説明   : 座標と半径の設定。
******************************************************************************/
void CSphere::Init(D3DXVECTOR3 pos, float radius)
{
	m_LoaclPos = pos;
	m_Radius = radius;
	m_CollisionList = new CCollisionList;
}

/******************************************************************************
	関数名 : void CSphere::Uninit(void)
	引数   : void
	戻り値 : なし
	説明   : リストの解放。
******************************************************************************/
void CSphere::Uninit(void)
{
	m_CollisionList->Uninit();
	SAFE_DELETE(m_CollisionList);
}

/******************************************************************************
	関数名 : void CSphere::Draw(void)
	引数   : void
	戻り値 : なし
	説明   : 描画関数。
******************************************************************************/
void CSphere::Draw(void)
{
}

/******************************************************************************
	関数名 : void CSphere::Action(CCapsule * other)
	引数   : other
	戻り値 : なし
	説明   : otherと自分とで球体とカプセルのあたり判定をする。
******************************************************************************/
void CSphere::Action(CCapsule * other)
{
	CCollisionDetection::GetInstance()->Sphere_To_Capsule(this, other);
}

/******************************************************************************
	関数名 : void CSphere::Action(CSphere * other)
	引数   : other
	戻り値 : なし
	説明   : otherと自分とで球体同士のあたり判定をする。
******************************************************************************/
void CSphere::Action(CSphere * other)
{
	CCollisionDetection::GetInstance()->Sphere_To_Sphere(this, other);
}

/******************************************************************************
	関数名 : CSphere * CSphere::Create(D3DXVECTOR3 pos, float radius, CScene *scene)
	引数   : pos, radius, scene
	戻り値 : なし
	説明   : 生成関数。
******************************************************************************/
CSphere * CSphere::Create(D3DXVECTOR3 pos, float radius, CScene *scene)
{
	CSphere *obj = new CSphere(scene);
	obj->Init(pos, radius);
	return obj;
}
