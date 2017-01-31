/******************************************************************************
	タイトル名 : CSmokeクラス
	ファイル名 : smoke.h
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2017/01/18
	更新日     : 
******************************************************************************/
/******************************************************************************
	インクルードガード
******************************************************************************/
#pragma once

/******************************************************************************
	クラス宣言
******************************************************************************/
class CSmoke : public CBillboard
{
public:
	CSmoke(int Priority = OBJECT_BILLBOARD, OBJ_TYPE objType = OBJ_TYPE_EXPLOSION, SCENE_TYPE sceneType = SCENE_TYPE_GAME);
	~CSmoke();

	void Init(D3DXVECTOR3 pos, float rot, float width, float height, int textureType);		//	初期化関数。
	void Update(void);			//	更新関数。
	void Draw(void);

	void RendererBegin(void);		//	描画前設定関数。
	void RendererEnd(void);			//	描画後設定関数。

	static void Create(D3DXVECTOR3 pos, float rot, float width, float height, int textureType);		//	生成関数。

private:
	int m_UpdateTimeCnt;		//	テクスチャアニメーションを更新するタイミングカウンタ。
};

