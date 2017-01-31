/******************************************************************************
	タイトル名 : collisionDetectionクラス
	ファイル名 : collisionDetection.h
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2017/01/05
	更新日     : 
******************************************************************************/
/******************************************************************************
	インクルードガード
******************************************************************************/
#pragma once

/******************************************************************************
	クラス宣言
******************************************************************************/
class CScene;
class CCapsule;
class CSphere;

class CCollisionDetection
{
public:
	//	シングルトン生成。
	static CCollisionDetection * GetInstance() { return m_pInstance == NULL ? m_pInstance = new CCollisionDetection() : m_pInstance; }
	void CollisionDetectionManager(void);		//	あたり判定する関数。
	void Sphere_To_Sphere(CSphere *sphere1, CSphere *sphere2);		//	球体と球体のあたり判定関数。
	void Capsule_To_Capsule(CCapsule *capsule1, CCapsule *capsule2);//	カプセルとカプセルのあたり判定関数。
	void Sphere_To_Capsule(CSphere *sphere, CCapsule *capsule);		//	球体とカプセルのあたり判定。
	D3DXVECTOR3 NearPoint(CSphere * sphere, CCapsule * capsule);	//	球体とカプセルの近似点を求める関数。
	void NearPoint(CCapsule *capsule1, CCapsule *capsule2, D3DXVECTOR3 &pOut1, D3DXVECTOR3 &pOut2);	//	カプセル同士の近似点を求める関数。
	static void Release(void) { SAFE_DELETE(m_pInstance); }			//	解放処理。
private:
	static CCollisionDetection *m_pInstance;	//	インスタンス。

	CCollisionDetection();		//	コンストラクタ。
	~CCollisionDetection();		//	デストラクタ。
};

