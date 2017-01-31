/******************************************************************************
	タイトル名 : SkyBoxクラス
	ファイル名 : skyBox.h
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2017/01/17
	更新日     :
******************************************************************************/
/******************************************************************************
	インクルードガード
******************************************************************************/
#pragma once

/******************************************************************************
	クラス宣言
******************************************************************************/
class CSkyBox : public CScene3D
{
public:
	CSkyBox(int Priority = OBJECT_3D, OBJ_TYPE objType = OBJ_TYPE_FIELD, SCENE_TYPE sceneType = SCENE_TYPE_GAME);		//	コンストラクタ。
	~CSkyBox();		//	デストラクタ。

	void Init(D3DXVECTOR3 pos, float width, float height, float depth, int textureType);		//	初期化関数。
	void Update(void);		//	更新関数。
	void Draw(void);		//	描画関数。

	static CSkyBox *Create(D3DXVECTOR3 pos, float width, float height, float depth, int textureType);		//	生成関数。
};

