/******************************************************************************
	タイトル名 : Game_BGクラス
	ファイル名 : game_BG.cpp
	作成者     : AT-13C-284 35 深澤佑太
	作成日     : 2016/12/16
	更新日     : 
******************************************************************************/
/******************************************************************************
	インクルードファイル
******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "scene2D.h"
#include "texture.h"
#include "game_BG.h"


/******************************************************************************
	関数名 : CGame_BG::CGame_BG(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene2D(Priority, objType, sceneType)
	説明   : コンストラクタ
			 変数の初期化。
******************************************************************************/
CGame_BG::CGame_BG(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene2D(Priority, objType, sceneType)
{
}

/******************************************************************************
	関数名 : CGame_BG::~CGame_BG()
	説明   : デストラクタ
			 クラスが破棄されたときに呼ばれるが、中身がないので特に記述する事なし。
******************************************************************************/
CGame_BG::~CGame_BG()
{
}

/******************************************************************************
	関数名 : CGame_BG * CGame_BG::Create(D3DXVECTOR3 pos, float width, float height, int textureType)
	引数   : pos, width, height, textureType
	戻り値 : obj
	説明   : ゲーム画面の背景を生成する関数。
******************************************************************************/
CGame_BG * CGame_BG::Create(D3DXVECTOR3 pos, float width, float height, int textureType)
{
	CGame_BG *obj = new CGame_BG;
	obj->Init(pos, width, height, textureType);
	return obj;
}
