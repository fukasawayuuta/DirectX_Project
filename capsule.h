/******************************************************************************
	タイトル名 : CCapsuleクラス
	ファイル名 : capsule.h
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
class CCollisionDetection;

class CCapsule : public CCollision
{
public:
	//	カプセル必要最低限の構造体。
	typedef struct {
		D3DXVECTOR3 start_pos;		//	開始地点。
		D3DXVECTOR3 end_pos;		//	終了地点。
		float radius;				//	半径。	
	}LINESEGMENT;

	CCapsule(CScene *scene = NULL, int Priority = LINE_SEGMENT);		//	コンストラクタ。
	~CCapsule();		//	デストラクタ。

	//	ダブルディスパッチ関数。
	virtual void Dispatch(CCollision* other) { other->Action(this); };
	virtual void Action(CCapsule* other);
	virtual void Action(CSphere* other);

	LINESEGMENT *GetSegment(void) { return &m_Segment; }		//	構造体取得関数。

	void Init(D3DXVECTOR3 pos, float radius, D3DXVECTOR3 start_pos, D3DXVECTOR3 end_pos);		//	初期化関数。
	void Uninit(void);		//	終了関数。
	void Draw(void);		//	描画関数。

	static CCapsule *Create(D3DXVECTOR3 pos, float radius, D3DXVECTOR3 start_pos, D3DXVECTOR3 end_pos, CScene *scene);		//	生成関数。

private:
	LINESEGMENT m_Segment;
	D3DXVECTOR3 m_HitPoint;
};

