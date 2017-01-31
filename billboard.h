/******************************************************************************
	タイトル名 : Billboardクラス
	ファイル名 : billboard.h
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
class CBillboard : public CScene3D
{
public:
	CBillboard(int Priority = NONE, OBJ_TYPE objType = OBJ_TYPE_NONE, SCENE_TYPE sceneType = SCENE_TYPE_NONE);		//	コンストラクタ。
	~CBillboard();		//	デストラクタ。

	void Draw(void);		//	通常の描画関数。

	bool GetAxisFixd(void) { return m_bY_Axis_Fixed; }			//	フラグ取得関数。
	void SetAxisFixd(bool flag) { m_bY_Axis_Fixed = flag; }		//	フラグ設定関数。

protected:
	bool m_bY_Axis_Fixed;		//	Y軸を固定するかどうかのフラグ。
};

