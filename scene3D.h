/******************************************************************************
	タイトル名 : Scene3Dクラス
	ファイル名 : scene3D.h
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/12/12
	更新日     : 
				 
******************************************************************************/
/******************************************************************************
	インクルードガード
******************************************************************************/
#pragma once

/******************************************************************************
	クラス
******************************************************************************/
class CScene3D : public CScene
{
public:
	CScene3D(int Priority = NONE, OBJ_TYPE objType = OBJ_TYPE_NONE, SCENE_TYPE sceneType = SCENE_TYPE_NONE);		//	コンストラクタ。
	~CScene3D();		//	デストラクタ。

	void Init(D3DXVECTOR3 pos, float width, float height, float depth, int textureType);		//	初期化関数。
	void Uninit(void);		//	終了関数。
	void Update(void);		//	更新関数。
	void Draw(void);		//	描画関数。

	virtual void RendererBegin(void) {};
	virtual void RendererEnd(void) {};

	static CScene3D *Create(D3DXVECTOR3 pos, float width, float height, float depth, int textureType);		//	生成関数。
	static void TextureUV_Update(LPDIRECT3DVERTEXBUFFER9 vtxBuff, int patternCnt, int animContX_Axis, int animCntY_Axis);		//	テクスチャのUV更新用関数。

protected:
	//	通常変数。
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffPolygon;		//	頂点バッファ。
	D3DXMATRIX m_mtxWorld;		//	ワールドマトリクス。
	D3DXVECTOR3 m_Scl;			//	スケール値。
	D3DXCOLOR m_Color;			//	色情報。
	float m_Width;				//	横幅。
	float m_Height;				//	縦幅。
	float m_Depth;				//	奥行。
	float m_Length;				//	対角線の長さ。
	float m_Angle;				//	対角線の角度。
	int m_TextureType;			//	テクスチャタイプ。

	//	アニメーション変数。
	int m_AnimationPatternCnt;	//	アニメーションのパターンカウント。
	int m_AnimationTotalNumber;	//	アニメーションの合計数。
	int m_AnimationCountX_Axis;	//	X軸上のアニメーションの数。
	int m_AnimationCountY_Axis;	//	Y軸上のアニメーションの数。
	int m_AnimationUpdateUVCnt;	//	UV座標を更新するタイミングカウンタ
};

