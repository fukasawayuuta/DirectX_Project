/******************************************************************************
	タイトル名 : DynamicModelクラス
	ファイル名 : dynamicModel.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/12/13
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
#include "model.h"
#include "motionManager.h"
#include "dynamicModel.h"


/******************************************************************************
	関数名 : CDynamicModel::CDynamicModel(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CSceneX(Priority, objType, sceneType)
	説明   : コンストラクタ
			 変数の初期化。
******************************************************************************/
CDynamicModel::CDynamicModel(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CSceneX(Priority, objType, sceneType)
{
}

/******************************************************************************
	関数名 : CDynamicModel::~CDynamicModel()
	説明   : デストラクタ
			 クラスが破棄されたときに呼ばれるが、中身がないので特に記述する事なし。
******************************************************************************/
CDynamicModel::~CDynamicModel()
{
}
