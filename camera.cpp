/******************************************************************************
	タイトル名 :かめらクラス
	ファイル名 : camera.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/04/26
	更新日     :
******************************************************************************/
/******************************************************************************
	インクルードファイル
******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "camera.h"
#include "input.h"
#include "Mode.h"
#include "game.h"
#include "scene3D.h"
#include "sceneX.h"
#include "model.h"
#include "motionManager.h"
#include "dynamicModel.h"
#include "player.h"

/******************************************************************************
	マクロ定義
******************************************************************************/
const float R_TO_V_DISTANCE = 300.0f;		//	視点と注視点の最初に設定する距離
const float R_TO_V_AIM_DISTANCE = 120.0f;	//	エイムしてる時の注視点と視点の距離。
const float R_TO_V_DISTANCE_MIN = 100.0f;	//	視点と注視点の一番最近距離
const float R_TO_V_DISTANCE_MAX = 700.0f;	//	視点と注視点の一番最遠距離
const float CAMERA_V_POS_MOVEMENT = 1.0f;	//	視点の移動量
const float CAMERA_R_ROT_MOVEMENT = 0.1f;	//	視点の回転量
const float CAMERA_NEAR = 1.0f;				//	カメラのNear値
const float CAMERA_FAR = 10000.0f;			//	カメラのFar値
const float CAMERA_MOVE_RATIO_X = 0.15f;	//	カメラのX軸回転の比率(マウスの感度)
const float CAMERA_MOVE_RATIO_Y = 0.15f;	//	カメラのY軸回転の比率(マウスの感度)
const float CAMERA_ROT_INERTIA = 0.03f;		//	カメラの回転の慣性値

D3DXVECTOR3 CCamera::m_VecRV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

/******************************************************************************
	関数名 : CCamera::CCamera()
	説明   : コンストラクタ。
******************************************************************************/
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_moveRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_movePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bace_point = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_moveVecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_VecRV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PurposePosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PurposePosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDistance = 0.0f;
	m_fPurposeRot = 0.0f;
	m_fVibration_Width = 0.0f;
	m_fSinCnt = 0;
	m_VecURot = 0.0f;
	m_MoveVecU = 0.0f;
}

/******************************************************************************
	関数名 : CCamera::~CCamera()
	説明   : デストラクタ。
******************************************************************************/
CCamera::~CCamera()
{

}

/******************************************************************************
	関数名 : void CCamera::Init(void)
	引数   : void
	戻り値 : なし
	説明   : カメラの座標、注視点、カメラ上部が向いている方向の設定。
******************************************************************************/
void CCamera::Init(void)
{
	m_posV = D3DXVECTOR3(0.0f, 50.0f, -R_TO_V_DISTANCE);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//	視点と注視点の距離を計算
	m_fDistance = sqrtf((m_posR.z - m_posV.z) * (m_posR.z - m_posV.z) + (m_posR.x - m_posV.x) + (m_posR.x - m_posV.x));

	m_fPurposeRot = 0.0f;

	//	視点と注視点の角度計算
	m_rot.y = atan2f(m_posR.x - m_posV.x, m_posR.z - m_posV.z);

	//	視点の座標設定
	m_posV.x = m_posR.x - sinf(m_rot.y) * m_fDistance;
	m_posV.z = m_posR.z - cosf(m_rot.y) * m_fDistance;

	//	注視点の座標設定
	m_posR.x = m_posV.x - sinf(m_rot.y + D3DX_PI) * cosf(m_rot.x) * m_fDistance;
	m_posR.y = m_posV.y - sinf(m_rot.x) * m_fDistance;
	m_posR.z = m_posV.z - cosf(m_rot.y + D3DX_PI) * cosf(m_rot.x) * m_fDistance;

	//	視点から注視点までのベクトルを求める
	m_VecRV = m_posR - m_posV;
}

/******************************************************************************
	関数名 : void CCamera::Uninit(void)
	引数   : void
	戻り値 : なし
	説明   : カメラなので特になし。
******************************************************************************/
void CCamera::Uninit(void)
{
}

/******************************************************************************
	関数名 : void CCamera::Update(void)
	引数   : void
	戻り値 : なし
	説明   : 処理が入ってないので説明なし。
******************************************************************************/
void CCamera::Update(void)
{
	// 入力処理へのポインタを取得
	CManager *pManager    = GetManager();
	CInput   *pInput      = pManager->GetInput();
	CGame	 *pGame       = (CGame *)CManager::GetMode();
	CPlayer  *pPlayer     = pGame->GetPlayer();
	D3DXVECTOR3 PlayerRot = pPlayer->GetRot();
	DIMOUSESTATE2 Mouse   = pInput->GetMouse();

	if (pPlayer->GetAlive()) {
		m_fSinCnt++;
		if (pPlayer->GetLockOn() == false) {
			//	マウスの移動量取得。
			m_rot.y += (D3DXToRadian(Mouse.lX) * CAMERA_MOVE_RATIO_X);
			m_rot.x += (D3DXToRadian(Mouse.lY) * CAMERA_MOVE_RATIO_Y);

			if (m_rot.x < -D3DX_PI * 0.5f + (D3DX_PI * 0.02f)) {
				m_rot.x = -D3DX_PI * 0.5f + (D3DX_PI * 0.02f);
			}

			if (m_rot.x > D3DX_PI * 0.5f - (D3DX_PI * 0.02f)) {
				m_rot.x = D3DX_PI * 0.5f - (D3DX_PI * 0.02f);
			}
		}

		//	注視点座標設定
		m_bace_point += (pPlayer->GetPos() - m_bace_point) * 0.175f;

		//	視点の座標設定
		m_PurposePosV.x = m_bace_point.x - sinf(m_rot.y) * cosf(m_rot.x) * m_fDistance;
		m_PurposePosV.y = m_bace_point.y + 30.0f - sinf(m_rot.x - D3DX_PI + (D3DX_PI * 0.1f)) * m_fDistance;
		m_PurposePosV.z = m_bace_point.z - cosf(m_rot.y) * cosf(m_rot.x) * m_fDistance;

		m_posV += (m_PurposePosV - m_posV) * 0.2f;

		if (pPlayer->GetLockOn() == false) {
			m_PurposePosR.x = m_bace_point.x - sinf(m_rot.y + D3DX_PI) * cosf(m_rot.x) * m_fDistance;
			m_PurposePosR.y = m_bace_point.y + 30.0f + sinf(m_rot.x - D3DX_PI + (D3DX_PI * 0.1f)) * m_fDistance;
			m_PurposePosR.z = m_bace_point.z - cosf(m_rot.y + D3DX_PI) * cosf(m_rot.x) * m_fDistance;
		}

		m_posR += (m_PurposePosR - m_posR) * 0.2f;

		m_fVibration_Width = m_fVibration_Width > 0.0f ? m_fVibration_Width - 0.5f : 0.0f;
	}

	m_MoveVecU += (m_VecURot - m_MoveVecU) * 0.8f;

	m_vecU.x = m_moveVecU.x;
	m_vecU.z = m_moveVecU.z;
}

/******************************************************************************
	関数名 : void CCamera::SetCamera(void)
	引数   : void
	戻り値 : なし
	説明   : カメラの設定
			 プロジェクションマトリクスを初期化して作成し、
			 ビューマトリクスの初期化と設定をする。
******************************************************************************/
void CCamera::SetCamera(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	float f_Screen_Width;		//アスペクト比計算用変数の画面幅幅
	float f_Screen_Height;		//アスペクト比計算用変数の画面高さ

	f_Screen_Width = SCREEN_WIDTH;
	f_Screen_Height = SCREEN_HEIGHT;

	//プロジェクションマトリクスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//プロジェクションマトリクスの作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
							    D3DX_PI * 0.25f,					//視野角
							    f_Screen_Width / f_Screen_Height,	//アスペクト比
							    CAMERA_NEAR,						//NearZ値
							    CAMERA_FAR);						//For値

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
					   &m_posV,
					   &m_posR,
					   &m_vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	//	視点から注視点までのベクトルを求める
	m_VecRV = m_posR - m_posV;
}