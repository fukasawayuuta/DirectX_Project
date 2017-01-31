/******************************************************************************
	タイトル名 : Scene2Dクラス
	ファイル名 : scene2D.h
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
class CScene2D : public CScene
{
public:
	CScene2D(int Priority = NONE, OBJ_TYPE objType = OBJ_TYPE_NONE, SCENE_TYPE sceneType = SCENE_TYPE_NONE);		//	コンストラクタ。
	~CScene2D();	//	デストラクタ。

	void Init(D3DXVECTOR3 pos, float width, float height, int textureType);		//	初期化関数。
	void Uninit(void);		//	終了関数。
	void Update(void);		//	更新関数。
	void Draw(void);		//	描画関数。

	static CScene2D *Create(D3DXVECTOR3 pos, float width, float height, int textureType);		//	生成関数。
	static void TextureUV_Update(LPDIRECT3DVERTEXBUFFER9 vtxBuff, int patternCnt, int animContX_Axis, int animCntY_Axis);		//	テクスチャのUV更新用関数。

protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffPolygon;		//	頂点バッファ。
	D3DXCOLOR m_Color;	//	色情報。

	float m_Width;		//	横幅。
	float m_Height;		//	縦幅。
	float m_Length;		//	対角線の長さ。
	float m_Angle;		//	対角線の角度。

	int m_TextureType;	//	テクスチャタイプ。

	//	アニメーション変数。
	int m_AnimationPatternCnt;	//	アニメーションのパターンカウント。
	int m_AnimationTotalNumber;	//	アニメーションの合計数。
	int m_AnimationCountX_Axis;	//	X軸上のアニメーションの数。
	int m_AnimationCountY_Axis;	//	Y軸上のアニメーションの数。
	int m_AnimationUpdateUVCnt;	//	UV座標を更新するタイミングカウンタ
};

