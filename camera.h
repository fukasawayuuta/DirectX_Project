/******************************************************************************
	タイトル名 : かめらクラス
	ファイル名 : camera.h
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/04/26
	更新日     :
******************************************************************************/
/******************************************************************************
	インクルードガード
******************************************************************************/
#pragma once

/******************************************************************************
	インクルードファイル
******************************************************************************/
/******************************************************************************
	ライブラリのリンク
******************************************************************************/
/******************************************************************************
	マクロ定義
******************************************************************************/
/******************************************************************************
	構造体宣言
******************************************************************************/
typedef struct
{
	D3DXVECTOR3 posV;			//視点
	D3DXVECTOR3 posR;			//注視点
	D3DXVECTOR3 vecU;			//上方向ベクトル
	D3DXMATRIX mtxProjection;	//プロジェクションマトリクス
	D3DXMATRIX mtxView;			//ビューマトリクス
}CAMERA;

/******************************************************************************
	クラス宣言
******************************************************************************/
class CCamera
{
public:
	CCamera();					//	コンストラクタ
	~CCamera();					//	デストラクタ

	void Init(void);			//	初期化関数
	void Uninit(void);			//	終了関数
	void Update(void);			//	更新関数
	void SetCamera(void);		//	設定関数

	D3DXVECTOR3 GetCameraRot(void) { return m_rot; }		//	カメラの角度の取得

	D3DXMATRIX *GetProjectionMatrix(void) { return &m_mtxProjection; }		//	プロジェクションマトリクスの取得
	D3DXMATRIX *GetViewMatrix(void) { return &m_mtxView; }					//	ビューマトリクスの取得

	D3DXVECTOR3 GetPosR(void) { return m_posR; }		//	注視点座標取得。
	void SetPosR(D3DXVECTOR3 pos) { m_PurposePosR = pos; }		//	注視点座標設定。
	void SetVecU(float vec) { m_VecURot = vec; }		//	角度設定。
	void SetCameraRotY(float rot) { m_rot.y = rot; }	//	角度設定。
	void SetCameraRotX(float rot) { m_rot.x = rot; }	//	角度設定。
	void AddCameraRotY(float rot) { m_rot.y += rot; }
	void AddCameraRotX(float rot) { m_rot.x += rot; }

	static D3DXVECTOR3 GetVecRV(void) { return m_VecRV; }		//	注視点から視点までの距離を返す関数

private:
	D3DXVECTOR3 m_posV;				//	視点。
	D3DXVECTOR3 m_posR;				//	注視点。
	D3DXVECTOR3 m_vecU;				//	上方向ベクトル。
	D3DXVECTOR3 m_rot;				//	角度。
	D3DXVECTOR3 m_moveRot;			//	回転量。
	D3DXVECTOR3 m_movePos;			//	移動量。
	D3DXVECTOR3 m_moveVecU;			//	上方向ベクトルの移動量。
	D3DXVECTOR3 m_bace_point;		//	基準点。
	D3DXVECTOR3 m_PurposePosV;		//	視点の移動先。
	D3DXVECTOR3 m_PurposePosR;		//	注視点の移動先。

	static D3DXVECTOR3 m_VecRV;		//	視点から注視点までのベクトル。
	
	D3DXMATRIX  m_mtxProjection;	//	プロジェクションマトリクス。
	D3DXMATRIX  m_mtxView;			//	ビューマトリクス。

	float m_fDistance;			//	視点と注視点の距離。
	float m_fPurposeRot;		//	目的の角度。
	float m_fVibration_Width;	//	振動幅。
	float m_fSinCnt;			//	sin波で揺らすためのカウンター。
	float m_VecURot;			//	上方向ベクトルの回転量。
	float m_MoveVecU;	
};