/******************************************************************************
	ファイル名 : collision.h
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
class CScene;
class CCapsule;
class CSphere;
class CCollisionList;

class CCollision
{
public:
	//	優先度の列挙型
	typedef enum {
		NONE = 0,
		LINE_SEGMENT,		//	線分。
		CSPHERE,			//	球体。
		PRIORITY_MAX
	}COLL_TYPE;


	CCollision(CScene *scene = NULL, int Priority = NONE);		//	コンストラクタ。
	~CCollision();		//	デストラクタ。

	virtual void Init(void) {};			//	初期化関数。
	virtual void Uninit(void) = 0;		//	終了関数。
	virtual void Update(void);			//	更新関数。
	virtual void Draw(void)   = 0;		//	描画関数。

	//	ダブルディスパッチ関数。
	virtual void Dispatch(CCollision* other) = 0;
	virtual void Action(CCapsule* other) = 0;
	virtual void Action(CSphere* other) = 0;

	D3DXVECTOR3 GetLocalPos(void) { return m_LoaclPos; }		//	ローカル座標取得。
	D3DXVECTOR3 GetWorldPos(void) { return m_WorldPos; }		//	ワールド座標取得。

	//	一括関数/////////////////////////////////////////////////////////////////////////////////////
	static void UpdateAll(void);		//	一括更新関数。
	static void DrawAll(void);			//	一括描画関数。
	static void ReleaseAll(void);		//	一括解放関数。
	void Release(int Priority);			//	選択解放関数。
	//	一括関数/////////////////////////////////////////////////////////////////////////////////////

	void SetHit(bool flag) { m_Hit = flag; }		//	フラグを設定する関数。
	bool GetHit(void) { return m_Hit; }				//	当たったかどうかを取得する関数。
	void SetDelete(bool flag) { m_Delete = flag; }	//	deleteフラグを設定。
	bool GetDelete(void) { return m_Delete; }		//	deleteフラグを取得。

	CCollision *GetScenePrev(void) { return  m_Prev; }			//	そのSceneの前のポインタを返す関数。
	CCollision *GetSceneNext(void) { return  m_Next; }			//	そのSceneの次のポインタを返す関数。
	static CCollision *GetListTopPointer(int priority) { return m_Top[priority]; }	//	引数の優先度のSceneのTopのポインタを返す。
	CScene *GetScene(void) { return m_pScene; }
	CCollisionList *GetCollisionList(void) { return m_CollisionList; }
	int GetCollisionType(void) { return m_Priority; }

protected:
	D3DXVECTOR3 m_LoaclPos;	//	座標。
	D3DXVECTOR3 m_WorldPos;	//	ワールド座標。
	D3DXVECTOR3 m_Rot;		//	角度。
	int m_Priority;			//	優先度。
	bool m_Delete;			//	削除フラグ。
	CScene *m_pScene;		//	使った対象が入るポインタ。
	bool m_Hit;				//	当たったかどうかが入るフラグ。
	CCollisionList *m_CollisionList;

	//	ポインタ関係/////////////////////////////////////////////////////////////////////////////////
	static CCollision *m_Top[PRIORITY_MAX];		//	先頭のポインタの配列。
	static CCollision *m_Cur[PRIORITY_MAX];		//	現在のポインタの配列。
	CCollision *m_Prev;							//	前のポインタ。
	CCollision *m_Next;							//	次のポインタ。
	//	ポインタ関係/////////////////////////////////////////////////////////////////////////////////
};

