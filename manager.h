/******************************************************************************
	タイトル名 : マネージャークラス
	ファイル名 : manader.h
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/05/09
	更新日     : 2016/05/16
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
/******************************************************************************
	クラス
******************************************************************************/
//	前方宣言///////////////////////////////////////////////////////////////////
class CRenderer;
class CScene;
class CCamera;
class CLight;
class CInput;
class CMode;
class CFade;
class CTexture;

//	クラス本体/////////////////////////////////////////////////////////////////
class CManager
{
public:
	CManager();		//	コンストラクタ
	~CManager();	//	デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);		//	初期化関数
	void Uninit(void);		//	終了関数
	void Update(void);		//	更新関数
	void Draw(void);		//	描画関数

	//	取得関数関連
	static CRenderer  *GetRenderer(void) { return m_pRenderer; }		//	レンダラの取得関数
	CInput     *GetInput(void) { return m_Input; }				//	インプットの取得関数
	HWND        GethWnd(void) { return m_hWnd; }				//	ウィンドウハンドルの取得
	static      CMode *GetMode(void) { return m_Mode; }			//	モードの取得
	
	//	スクリーン座標をワールド座標に変更する関数
	D3DXVECTOR3 *CalcS_to_W(D3DXVECTOR3 *pOut,
							int Sx,
							int Sy,
							float ProjZ,
							int ScreenWidth, 
							int ScreenHeight, 
							D3DXMATRIX *ViewMatrix, 
							D3DXMATRIX *ProjMatrix);

	//	スクリーン座標とXZ平面のワールド座標交点を求める関数
	D3DXVECTOR3 *CalcS_To_XZ(D3DXVECTOR3 *pOut,
							 int ScreenPosX,
							 int ScreenPosY,
							 int ScreenWidth,
							 int ScreenHeight,
							 D3DXMATRIX *View,
							 D3DXMATRIX *Proj);

	static void SetMode(CMode *Mode);			//	モードの設定
	static void CheckRot(Vector3 *rot);		//	角度の調整(D3DXVECTOR3タイプ)
	static void CheckRot(float *rot);			//	角度の調整(単体タイプ)

private:
	CInput      *m_Input;				//	入力処理の情報
	HWND         m_hWnd;				//	hWndの受け皿
	CFade	     *m_Fade;				//	フェード情報
	CTexture	 *m_Texture;			//	テクスチャの情報
	static CRenderer *m_pRenderer;		//	レンダラの情報
	static CMode *m_Mode;				//	モードの情報

	static HANDLE m_Threed;		//	スレッド情報
	DWORD m_ExitCode;			//	スレッドの終了判定
	static unsigned int m_ThreedId;		//	スレッドの番号
};

