/******************************************************************************
	タイトル名 : CSphereクラス
	ファイル名 : sphere.h
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2017/01/05
	更新日     :
******************************************************************************/
/******************************************************************************
	インクルードガード
******************************************************************************/
#pragma once

/******************************************************************************
	クラス
******************************************************************************/
class CSphere : public CCollision
{
public:
	CSphere(CScene *scene = NULL, int Priority = CSPHERE);		//	コンストラクタ。
	~CSphere();			//	デストラクタ。

	void Init(D3DXVECTOR3 pos, float radius);		//	初期化関数。
	void Uninit(void);		//	終了関数。
	void Draw(void);		//	描画関数。

	float GetRadius(void) { return m_Radius; }		//	半径取得。
	
	//	ダブルディスパッチ。	
	virtual void Dispatch(CCollision* other) { other->Action(this); };
	virtual void Action(CCapsule* other);
	virtual void Action(CSphere* other);

	static CSphere *Create(D3DXVECTOR3 pos, float radius, CScene *scene);		//	生成関数。
private:
	float m_Radius;		//	半径。
};

