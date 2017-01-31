/******************************************************************************
	タイトル名 : CDesertDustクラス
	ファイル名 : desertDust.h
	作成者     : AT-13C-284 36 深澤佑太
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
class CMeshField;

class CDesertDust : public CBillboard
{
public:
	CDesertDust(int Priority = OBJECT_BILLBOARD, OBJ_TYPE objType = OBJ_TYPE_DUST, SCENE_TYPE sceneType = SCENE_TYPE_GAME);		//	コンストラクタ。
	~CDesertDust();			//	デストラクタ。

	void Update(void);		//	更新関数。
	static void Create(D3DXVECTOR3 pos, float width, float height, int textureType);		//	生成関数。

	void RendererBegin(void);		//	描画前設定関数。
	void RendererEnd(void);			//	描画後設定関数。

private:
	D3DXVECTOR3 m_movePos;		//	移動量。
	float m_MoveSpeed;			//	移動量。

	void Reset(CMeshField *mesh);		//	リセット関数。
	void CheckOutsideRange(CMeshField *mesh);		//	範囲外チェック。
};

