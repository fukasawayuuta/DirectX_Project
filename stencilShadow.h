/******************************************************************************
	ファイル名 : stencilShadow.h
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2017/01/28
	更新日     :
******************************************************************************/
/******************************************************************************
	インクルードガード
******************************************************************************/
#pragma once

/******************************************************************************
	クラス宣言
******************************************************************************/
class CScene;

class CStencilShadow : public CScene3D
{
public:
	CStencilShadow(int Priority = OBJECT_3D, OBJ_TYPE objType = OBJ_TYPE_SHADOW, SCENE_TYPE sceneType = SCENE_TYPE_GAME);		//	コンストラクタ。
	~CStencilShadow();		//	デストラクタ。

	void Init(CScene *target);		//	初期化関数。
	void Uninit(void);				//	終了関数。
	void Update(void);				//	更新関数。
	void Draw(void);				//	描画関数。

	static void Create(CScene *target);		//	生成関数。

private:
	//	シリンダーを使うのに必要な変数。
	LPD3DXMESH	 m_pMesh;
	LPD3DXBUFFER m_pBuffMat;
	DWORD		 m_NumMat;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffPolygon;		//	頂点バッファ(2D)
	CScene   *m_Target;		//	使う対象が入る。
};

