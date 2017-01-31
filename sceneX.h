/******************************************************************************
	タイトル名 :シーンXクラス
	ファイル名 : sceneX.h
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/12/13
	更新日     :
******************************************************************************/
/******************************************************************************
	インクルードガード
******************************************************************************/
#pragma once

/******************************************************************************
	クラス
******************************************************************************/
class CSceneX : public CScene3D
{
public:
	CSceneX(int Priority = NONE, OBJ_TYPE objType = OBJ_TYPE_NONE, SCENE_TYPE sceneType = SCENE_TYPE_NONE);		//	コンストラクタ。
	~CSceneX();		//	デストラクタ。

	typedef enum {
		DYNAMIC_MODEL_TYPE_PLAYER = 0,		//	player
		DYNAMIC_MODEL_TYPE_ENEMY001,		//	enemy001
		DYNAMIC_MODEL_TYPE_ENEMY002,		//	enemy002	
		DYNAMIC_MODEL_TYPE_MAX				//	type_max
	}DYNAMIC_MODEL_TYPE;

	virtual void Init(void)   = 0;		//	初期化関数。
	virtual void Uninit(void) = 0;		//	終了関数。
	virtual void Update(void) = 0;		//	更新関数。
	virtual void Draw(void)   = 0;		//	描画関数。
};

