/******************************************************************************
	タイトル名 : Boost
	ファイル名 : boost.h
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2017/01/27
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

class CBoost : public CBillboard
{
public:
	CBoost(int Priority = EFFECT, OBJ_TYPE objType = OBJ_TYPE_BOOSTEFFECT, SCENE_TYPE sceneType = SCENE_TYPE_GAME);		//	コンストラクタ。
	~CBoost();		//		デストラクタ。

	void Init(D3DXVECTOR3 pos, float width, float height, int textureType);		//	初期化関数。
	void Uninit(void);		//	終了関数。
	void Update(void);		//	更新関数。

	void RendererBegin(void);		//	描画前設定関数。
	void RendererEnd(void);			//	描画後設定関数。

	void SetDuringUse(bool flag) { m_DuringUse = flag; }		//	フラグ設定関数。
	static CBoost *Create(D3DXVECTOR3 pos, float width, float height, int textureType);		//	生成関数。

private:
	bool m_DuringUse;				//	使っているフラグ。
	float m_Magnification_Rate;		//	倍率のレート。
};

