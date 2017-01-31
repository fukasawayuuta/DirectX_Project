/******************************************************************************
	タイトル名 : MeshFieldクラス
	ファイル名 : MeshField.h
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/12/15
	更新日     : 
******************************************************************************/
/******************************************************************************
	インクルードガード
******************************************************************************/
#pragma once

/******************************************************************************
	クラス宣言
******************************************************************************/
class CMeshField : public CSceneIndex
{
public:
	CMeshField(int Priority = BACKGROUND, OBJ_TYPE objType = OBJ_TYPE_FIELD, SCENE_TYPE sceneType = SCENE_TYPE_GAME);		//	コンストラクタ。
	~CMeshField();		//	デストラクタ。

	void Init(const char *filename, int textureType);		//	初期化関数。
	void Uninit(void);	//	終了関数。
	void Update(void);	//	更新関数。
	void Draw(void);	//	描画関数。

	float GetFieldWidth(void) { return m_Width_Total_Field; }		//	フィールド全体の横幅取得。
	float GetFieldDepth(void) { return m_Depth_Total_Field; }		//	フィールド全体の奥行取得。
	D3DXVECTOR3 GetFieldNormal(D3DXVECTOR3 pos);		//	今いる座標のフィールドの法線を取得。

	static CMeshField *Create(const char *filename, int textureType);		//	生成関数。
	float Collision(D3DXVECTOR3 pos);		//	あたり判定をする関数。
private:
	int m_SelectXIndex;				//	X軸上のメッシュのインデックスの番号。
	int m_SelectZIndex;				//	Z軸上のメッシュのインデックスの番号。
	int m_SelectPolygonIndex;		//	全体のメッシュのインデックスの番号。
	int m_SelectPolygonIndexX;		//	X軸上のメッシュのポリゴンの番号。
	int m_SelectPolygonIndexZ;		//	Z軸上のメッシュのポリゴンの番号。
	int m_SelectBlockNum;			//	全体のメッシュのポリゴンの番号。

	int m_Field_Block_Num_Xline;		//	X軸にあるブロック数。
	int m_Field_Block_Num_Zline;		//	Z軸にあるブロック数。
	float m_Xline_Block_Between_Interval;		//	X軸間の頂点間の間隔。
	float m_Zline_Block_Between_Interval;		//	Z軸間の頂点間の間隔。
	int m_Field_Vertex_Num_Xline;		//	X軸上にある頂点数。
	int m_Field_Vertex_Num_Zline;		//	Z軸上にある頂点数。
	float m_Width_Total_Field;		//	メッシュフィールド全体の横幅。
	float m_Depth_Total_Field;		//	メッシュフィールド全体の奥行。
	int m_Index_Total_Field;		//	メッシュフィールドに使われているインデックスの総数。

	bool LoadField(const char *filename);		//	ファイルを読み込む関数。
	void UpdateMeshNormal(void);		//	法線更新関数
};

