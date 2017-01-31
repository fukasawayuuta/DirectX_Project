/******************************************************************************
	タイトル名 : CDynamicModelクラス
	ファイル名 : dynamicModel.h
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/12/14
	更新日     : 
******************************************************************************/
/******************************************************************************
	インクルードガード
******************************************************************************/
#pragma once

/******************************************************************************
	クラス
******************************************************************************/
class CMotionManager;

class CDynamicModel : public CSceneX
{
public:
	CDynamicModel(int Priority = NONE, OBJ_TYPE objType = OBJ_TYPE_NONE, SCENE_TYPE sceneType = SCENE_TYPE_NONE);		//	コンストラクタ。
	~CDynamicModel();		//	デストラクタ。

	virtual void Init(void) {};		//	初期化関数。
	virtual void Uninit(void) = 0;		//	終了関数。
	virtual void Update(void) = 0;		//	更新関数。
	virtual void Draw(void) = 0;		//	描画関数。

protected:
	CMotionManager *m_MotionManager;		//	モーションするための変数。
	DYNAMIC_MODEL_TYPE m_DynamicModelType;		//	使うモーションとモデルのタイプ。
};

