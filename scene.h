/******************************************************************************
	タイトル名 : 描画クラス
	ファイル名 : scene.h
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/04/19
	更新日     : 2016/05/31
				 2016/05/16
******************************************************************************/
/******************************************************************************
	インクルードガード
******************************************************************************/
#pragma once

/******************************************************************************
	クラス
******************************************************************************/
class CScene
{
public:
	//	オブジェクトのタイプ///////////////////////////////////////////////////
	typedef enum {
		OBJ_TYPE_NONE = 0,		//	null
		OBJ_TYPE_PLAYER,		//	player
		OBJ_TYPE_ENEMY,			//	enemy
		OBJ_TYPE_BULLET,		//	bullet
		OBJ_TYPE_EXPLOSION,		//	explosion
		OBJ_TYPE_BOOSTEFFECT,	//	boost_effect
		OBJ_TYPE_DUST,			//	dust
		OBJ_TYPE_FIELD,			//	field
		OBJ_TYPE_UI,			//	ui
		OBJ_TYPE_SHADOW,		//	shadow
		OBJ_TYPE_MAX			//	max
	}OBJ_TYPE;

	//	シーンのタイプ//////////////////////////////////////////////////////////
	typedef enum {
		SCENE_TYPE_NONE = 0,	//	none
		SCENE_TYPE_TITLE,		//	title
		SCENE_TYPE_GAME,		//	game
		SCENE_TYPE_RESULT,		//	result
		SCENE_TYPE_MAX			//	max
	}SCENE_TYPE;

	//	優先度の列挙型///////////////////////////////////////////////////////////
	typedef enum {
		NONE = 0,
		BACKGROUND,			//	ドームとか床とか動かないもの。
		STENCIL,			//	ステンシル系。
		OBJECT_3D,			//	プレイヤーとか敵とか動くもの。
		OBJECT_ENEMY,		//	エネミー。
		OBJECT_BILLBOARD,	//	ビルボードとか。
		EFFECT,				//	エフェクトとか。
		OBJECT_2D,			//	2D関連。
		PRIORITY_MAX
	}PRIORITY;

	//	基本関数/////////////////////////////////////////////////////////////////////////////////////
	CScene(int Priority = NONE, OBJ_TYPE objType = OBJ_TYPE_NONE, SCENE_TYPE sceneType = SCENE_TYPE_NONE);		//	コンストラクタ。
	~CScene();							//	デストラクタ。
	virtual void Init(void) {}			//	初期化関数。
	virtual void Uninit(void)  = 0;		//	終了関数。
	virtual void Update(void)  = 0;		//	更新関数。
	virtual void Draw(void)    = 0;		//	描画関数。
	//	基本関数/////////////////////////////////////////////////////////////////////////////////////

	//	一括関数/////////////////////////////////////////////////////////////////////////////////////
	static void UpdateAll(void);		//	一括更新関数。
	static void DrawAll(void);			//	一括描画関数。
	static void ReleaseAll(void);		//	一括解放関数。
	void Release(int Priority);			//	選択解放関数。
	//	一括関数/////////////////////////////////////////////////////////////////////////////////////

	//	取得関数/////////////////////////////////////////////////////////////////////////////////////
	OBJ_TYPE GetObjectType (void) {return m_Objtype;}			//	オブジェクトのタイプを返す関数。
	bool GetDeleteFlag(void) { return m_Delete; }				//	フラグを取得する関数。
	bool GetActive(void) { return m_Active; }					//	アクティブフラグを取得する関数。
	D3DXVECTOR3 GetPos(void) { return m_Pos; }					//	座標を返す関数。
	D3DXVECTOR3 GetRot(void) { return m_Rot; }					//	角度を返す関数。
	CScene *GetScenePrev(void) { return  m_Prev; }				//	そのSceneの前のポインタを返す関数。
	CScene *GetSceneNext(void) { return  m_Next; }				//	そのSceneの次のポインタを返す関数。
	void SetScenePrev(CScene *prev) { m_Prev = prev; }			//	そのSceneの前のポインタをセットする関数。
	void SetSceneNext(CScene *next) { m_Next = next; }			//	そのSceneの次のポインタをセットする関数。
	static CScene *GetListTopPointer(int priority) { return m_Top[priority]; }	//	引数の優先度のSceneのTopのポインタを返す。
	//	取得関数/////////////////////////////////////////////////////////////////////////////////////

	//	設定関数/////////////////////////////////////////////////////////////////////////////////////
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }				//	引数のposをメンバ変数のposに代入する関数。
	void SetDelete(bool flag) { m_Delete = flag; }				//	フラグをセットする関数。
	void SetActive(bool flag) { m_Active = flag; }				//	アクティブフラグをセットする関数。
	//	設定関数/////////////////////////////////////////////////////////////////////////////////////

protected:
	//	変数関係/////////////////////////////////////////////////////////////////////////////////////
	D3DXVECTOR3 m_Pos;			//	座標。
	D3DXVECTOR3 m_Rot;			//	角度。
	OBJ_TYPE  m_Objtype;		//	オブジェクトタイプ。
	SCENE_TYPE m_SceneType;		//	シーンタイプ。
	int m_Priority;				//	優先度。
	bool m_Delete;				//	削除フラグ。
	bool m_Active;				//	アクティブフラグ。
	//	変数関係/////////////////////////////////////////////////////////////////////////////////////
	
	//	ポインタ関係/////////////////////////////////////////////////////////////////////////////////
	static CScene *m_Top[PRIORITY_MAX];		//	先頭のポインタの配列。
	static CScene *m_Cur[PRIORITY_MAX];		//	現在のポインタの配列。
	CScene *m_Prev;							//	前のポインタ。
	CScene *m_Next;							//	次のポインタ。
	//	ポインタ関係/////////////////////////////////////////////////////////////////////////////////
};