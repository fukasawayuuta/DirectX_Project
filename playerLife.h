/******************************************************************************
	タイトル名 : プレイヤーライフ
	ファイル名 : playerLife.h
	作成者     : AT-13C-284 35 深澤佑太
	作成日     : 2017/01/19
	更新日     :
******************************************************************************/
/******************************************************************************
	インクルードガード
******************************************************************************/
#pragma once

/******************************************************************************
	クラス宣言
******************************************************************************/
class CPlayerLife : public CScene2D
{
public:
	CPlayerLife(int Priority = OBJECT_2D, OBJ_TYPE objType = OBJ_TYPE_UI, SCENE_TYPE sceneType = SCENE_TYPE_GAME);		//	コンストラクタ。
	~CPlayerLife();			//	デストラクタ。

	void Init(D3DXVECTOR3 pos, float width, float height, int lifeTex, int barTex, int hp);		//	初期化関数。
	void Update(CPlayer *player);		//	更新関数。
	void Draw(void);		//	描画関数。

	static CPlayerLife *Create(D3DXVECTOR3 pos, float width, float height, int lifeTex, int barTex, int hp);		//	生成関数。

private:
	typedef enum {
		TEX_LIFE = 0,		//	ライフのテクスチャ。
		TEX_LIFE_BAR,		//	ライフバーのテクスチャ。
		TEX_MAX				//	最大数。
	};

	int m_PlayerMaxHP;				//	プレイヤーの最大HP。
	int m_LifeTexture[TEX_MAX];		//	使うテクスチャを設定する変数。
	float m_MoveWidth;				//	幅の移動量。
	float m_FixedWidth;				//	固定幅。
};

