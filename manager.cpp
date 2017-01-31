/******************************************************************************
	タイトル名 : マネージャークラス
	ファイル名 : manader.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/05/09
	更新日     :
******************************************************************************/
/******************************************************************************
	インクルードファイル
******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "Fade.h"
#include "Mode.h"
#include "title.h"

/******************************************************************************
	静的メンバ変数
******************************************************************************/
CMode *CManager::m_Mode = NULL;
CRenderer *CManager::m_pRenderer;

/******************************************************************************
	関数名 : CManager::CManager()
	説明   : コンストラクタ
******************************************************************************/
CManager::CManager()
{
	m_pRenderer = NULL;
	m_Input = NULL;
}

/******************************************************************************
	関数名 : CManager::~CManager()
	説明   : デストラクタ
******************************************************************************/
CManager::~CManager()
{
}

/******************************************************************************
	関数名 : HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
	引数   : hInstance, hWnd, bWindow
	戻り値 : S_OK, E_FAIL
	説明   : レンダラクラスの生成と初期化、シーンクラスの生成と初期化
			 カメラの生成と初期化、ライトの生成と初期化
******************************************************************************/
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//	レンダラクラス生成
	m_pRenderer = new CRenderer;
	//	初期化に失敗したらE_FAILを返す。
	if (FAILED(m_pRenderer->Init(hInstance, hWnd, bWindow))) 
		return E_FAIL;
	
	//	入力処理の作成と初期化。
	m_Input = new CInput;
	m_Input->InitKeyboard(hInstance, hWnd);			//	インプットクラスの初期化
	//	フェード処理の作成と初期化。
	m_Fade = new CFade;
	m_Fade->Init();
	//	最初のモードをタイトルに設定。
	SetMode(new CTitle);
	//	hWnd取得。
	m_hWnd = hWnd;

	return S_OK;
}

/******************************************************************************
	関数名 : void CManager::Uninit(void)
	引数   : void
	戻り値 : なし
	説明   : レンダラクラスの解放、オブジェクトの解放、
			 ポリゴンの解放処理、カメラの解放処理、ライトの解放処理
******************************************************************************/
void CManager::Uninit(void)
{
	//	レンダラクラスの終了処理
	m_pRenderer->Uninit();
	SAFE_DELETE(m_pRenderer);
	
	//	モードの終了処理
	m_Mode->Uninit();
	SAFE_DELETE(m_Mode);

	//	インプットの終了処理
	m_Input->UninitKeyboard();
	SAFE_DELETE(m_Input);
	
	//	フェードの終了処理
	m_Fade->Uninit();
	SAFE_DELETE(m_Fade);
}

/******************************************************************************
	関数名 : void CManager::Update(void)
	引数   : void
	戻り値 : なし
	説明   : オブジェクトとカメラの更新処理
******************************************************************************/
void CManager::Update(void)
{
	//	入力の更新
	m_Input->UpdateMouse();
	m_Input->UpdateKeyboard();
	m_Input->UpdateJoyStick();

	//	レンダラの更新
	m_pRenderer->Update();
	//	フェードの更新
	m_Fade->Update();
	//	モードの更新
	m_Mode->Update();
}

/******************************************************************************
	関数名 : void CManager::Draw(void)
	引数   : void
	戻り値 : なし
	説明   : オブジェクトの描画処理
******************************************************************************/
void CManager::Draw(void)
{
	//	描画開始
	if(m_pRenderer->DrawSceneFlag() == true) {
		//	モードの描画
		m_Mode->Draw();
		//	フェードの描画
		m_Fade->Draw();
	}

	//	描画終了
	m_pRenderer->ExchangeBuffer();
}

/******************************************************************************
	関数名 : D3DXVECTOR3 *CManager::CalcS_to_W(D3DXVECTOR3 *pOut,
				                               int Sx,
				                               int Sy,
				                               float ProjZ,
				                               int ScreenWidth, 
				                               int ScreenHeight, 
				                               D3DXMATRIX *ViewMatrix, 
				                               D3DXMATRIX *ProjMatrix)
	引数   : *pOut, Sx, Sy, ProjZ, ScreenWidth, ScreenHeight, *ViewMatrix, *ProjMatrix
	戻り値 : pOut
	説明   : スクリーン座標をワールド座標に変換
******************************************************************************/
D3DXVECTOR3 *CManager::CalcS_to_W(D3DXVECTOR3 *pOut,
								  int Sx,
								  int Sy,
								  float ProjZ,
								  int ScreenWidth, 
								  int ScreenHeight, 
								  D3DXMATRIX *ViewMatrix, 
								  D3DXMATRIX *ProjMatrix)
{
	//	作業用変数
	D3DXMATRIX InvView,InvProj, ViewPort, InvViewPort;

	//	各行列の逆行列を計算
	D3DXMatrixInverse( &InvView, NULL, ViewMatrix );
    D3DXMatrixInverse( &InvProj, NULL, ProjMatrix );
    D3DXMatrixIdentity( &ViewPort );

	ViewPort._11 = ScreenWidth / 2.0f; 
	ViewPort._22 = -ScreenHeight / 2.0f;
	ViewPort._41 = ScreenWidth / 2.0f; 
	ViewPort._42 = ScreenHeight / 2.0f;

    D3DXMatrixInverse( &InvViewPort, NULL, &ViewPort );

	// 2D座標を3D座標に変換
    D3DXMATRIX tmp = InvViewPort * InvProj * InvView;
    D3DXVec3TransformCoord( pOut, &D3DXVECTOR3(Sx,Sy,ProjZ), &tmp );

   return pOut;
}


/******************************************************************************
	関数名 : D3DXVECTOR3 *CManager::CalcS_To_XZ(D3DXVECTOR3 *pOut,
								   int ScreenPosX,
								   int ScreenPosY,
								   int ScreenWidth,
								   int ScreenHeight,
								   D3DXMATRIX *View,
								   D3DXMATRIX *Proj)
	引数   : *pOut, ScreenPosX, ScreenPosY, ScreenWidth, ScreenHeight, *View, *Proj
	戻り値 : なし
	説明   : オブジェクトの描画処理
******************************************************************************/
D3DXVECTOR3 *CManager::CalcS_To_XZ(D3DXVECTOR3 *pOut,
								   int ScreenPosX,
								   int ScreenPosY,
								   int ScreenWidth,
								   int ScreenHeight,
								   D3DXMATRIX *View,
								   D3DXMATRIX *Proj)
{
	//	作業用変数
	D3DXVECTOR3 nearPos, farPos, ray;

	//	カメラから一番近い座標を求める
	CalcS_to_W(&nearPos, ScreenPosX, ScreenPosY, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, View, Proj);
	//	カメラから一番遠い座標を求める
	CalcS_to_W(&farPos , ScreenPosX, ScreenPosY, 1.0f, SCREEN_WIDTH, SCREEN_HEIGHT, View, Proj);

	ray = farPos - nearPos;
	D3DXVec3Normalize( &ray, &ray );

	//	床との交差が起きている場合は交点を
    //	起きていない場合は遠くの壁との交点を出力
	if( ray.y <= 0 ) {
        //	床と交点したとき
        float Lray = D3DXVec3Dot( &ray, &D3DXVECTOR3(0, 1, 0) );
        float LP0 = D3DXVec3Dot( &(-nearPos), &D3DXVECTOR3(0, 1, 0) );
        *pOut = nearPos + (LP0 / Lray) * ray;
    }
    else {
		//	床と交点してないとき
		*pOut = farPos;
    }

	return pOut;
}

/******************************************************************************
	関数名 : void CManager::SetMode(CMode *Mode)
	引数   : *Mode
	戻り値 : なし
	説明   : オブジェクトの描画処理
******************************************************************************/
void CManager::SetMode(CMode *Mode)
{
	//	モードのポインタがnullじゃなかったら
	if (m_Mode != NULL) {
		m_Mode->Uninit();
		delete m_Mode;
		m_Mode = NULL;
	}
	
	//	モード変更。
	m_Mode = Mode;

	//	マルチスレッドしない場合、通常の初期化処理。
	if (m_Mode->GetMultiThreedFlag() == false) {
		m_Mode->Init();
	}
}

/******************************************************************************
	関数名 : void CManager::ChackRot(D3DXVECTOR3 * rot)
	引数   : * rot
	戻り値 : なし
	説明   : 角度の調整。PIを超えていたら修正。(D3DXVECTOR3型)
******************************************************************************/
void CManager::CheckRot(Vector3 * rot)
{
	if (rot->x > D3DX_PI)
		rot->x -= D3DX_PI * 2.0f;
	if (rot->x < -D3DX_PI)
		rot->x += D3DX_PI * 2.0f;

	if (rot->y > D3DX_PI)
		rot->y -= D3DX_PI * 2.0f;
	if (rot->y < -D3DX_PI)
		rot->y += D3DX_PI * 2.0f;

	if (rot->z > D3DX_PI)
		rot->z -= D3DX_PI * 2.0f;
	if (rot->z < -D3DX_PI)
		rot->z += D3DX_PI * 2.0f;

}

/******************************************************************************
	関数名 : void CManager::ChackRot(float * rot)
	引数   : * rot
	戻り値 : なし
	説明   : 角度の調整。PIを超えていたら修正。(float型)
******************************************************************************/
void CManager::CheckRot(float * rot)
{
	if (*rot > D3DX_PI)
		*rot -= D3DX_PI * 2.0f;
	if (*rot < -D3DX_PI)
		*rot += D3DX_PI * 2.0f;
}
