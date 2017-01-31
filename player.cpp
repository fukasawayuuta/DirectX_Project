/******************************************************************************
	タイトル名 : Playerクラス
	ファイル名 : player.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/12/17
	更新日     :
******************************************************************************/
/******************************************************************************
	インクルードファイル
******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "texture.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneIndex.h"
#include "sceneX.h"
#include "billboard.h"
#include "model.h"
#include "motionManager.h"
#include "dynamicModel.h"
#include "player.h"
#include "collision.h"
#include "collisionList.h"
#include "sphere.h"
#include "Mode.h"
#include "game.h"
#include "camera.h"
#include "input.h"
#include "bullet.h"
#include "MeshField.h"
#include "playerLife.h"
#include "Explosion.h"
#include "capsule.h"
#include "boost.h"
#include "stencilShadow.h"

/******************************************************************************
	マクロ適宜
******************************************************************************/
const float MOVE_SPEED = 1.0f;			//	移動量。
const float MOVE_INERTIA = 0.23f;		//	移動量の慣性値。
const float COLLISION_RADIUS = 50.0f;	//	あたり判定の半径。
const float JUMP_POWER = 5.0f;			//	ジャンプ量。
const float PLAYER_MOVEMENT_LOWEST_VALUE = 0.01f;	//	プレイヤーがまだ動いていると認める最低移動量。
const float CERTAIN_DISTANCE = 800.0f;
const float BOOST_MAGNIFICATION = 15.0f;		//	ブーストする際の移動量に掛ける倍率。
const int   BOOST_COOLING_TIME = 30;		//	ブーストのクールタイム。


/******************************************************************************
	関数名 : CPlayer::CPlayer(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CDynamicModel(Priority, objType, sceneType)
	説明   : コンストラクタ
******************************************************************************/
CPlayer::CPlayer(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CDynamicModel(Priority, objType, sceneType)
{
	m_Hp = 0;
	m_CoolingTimeBoost = 0;
	m_Boost = false;
	m_Jump = false;
	m_AirBoost = false;
	m_Alive = true;
	m_Move = false;
	m_LockOn = false;
	m_pCollision = NULL;
	m_Life = NULL;
	m_BoostEffect = NULL;
	m_purposeRot = 0.0f;
	m_MoveSpeed = 0.0f;
	m_MoveInertia = 0.0f;
	m_RotInertia = 0.0f;
	m_CollRadius = 0.0f;
	m_movePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_moveRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

/******************************************************************************
	関数名 : CPlayer::~CPlayer()
	説明   : デストラクタ
******************************************************************************/
CPlayer::~CPlayer()
{
}

/******************************************************************************
	関数名 : void CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
	引数   : pos, rot
	戻り値 : なし
	説明   : 座標と角度の設定。
******************************************************************************/
void CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//	モーション設定。
	m_MotionManager = CMotionManager::Create(DYNAMICMODEL_TYPE_PLAYER_00, &m_mtxWorld);
	m_MotionManager->SetMotion(MOTION_MDOE_NEUTRAL);

	//	初期値設定。
	m_Pos = pos;
	m_Rot = rot;
	m_Scl *= 0.5f;
	m_MoveSpeed = MOVE_SPEED;
	m_MoveInertia = MOVE_INERTIA;
	m_CollRadius = COLLISION_RADIUS;
	m_Hp = 500;

	//CSphere::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_CollRadius, this);
	//	あたり判定生成。
	m_pCollision = CCapsule::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 50.0f, 0.0f), this);
	//	ライフバー生成。
	m_Life = CPlayerLife::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.018f, SCREEN_HEIGHT * 0.88f, 0.0f), 500.0f, 75, CTexture::G_LIFE, CTexture::G_LIVEBAR, m_Hp);
	//	エフェクト生成。
	m_BoostEffect = CBoost::Create(m_Pos, 100.0f, 100.0f, CTexture::G_BOOST);
	//	影を生成。
	CStencilShadow::Create(this);
}

/******************************************************************************
	関数名 : void CPlayer::Uninit(void)
	引数   : void
	戻り値 : なし
	説明   : 解放処理。
******************************************************************************/
void CPlayer::Uninit(void)
{
	m_MotionManager->Uninit();
	SAFE_DELETE(m_MotionManager);
	//	あたり判定解放。
	if (m_pCollision != NULL) {
		m_pCollision->SetDelete(true);
		m_pCollision->GetCollisionList()->Uninit();
		m_pCollision = NULL;
	}

	m_Life->SetDelete(true);
	m_Life = NULL;

	m_BoostEffect->SetDelete(true);
	m_BoostEffect = NULL;
}

/******************************************************************************
	関数名 : void CPlayer::Update(void)
	引数   : void
	戻り値 : なし
	説明   : 更新処理。
******************************************************************************/
void CPlayer::Update(void)
{
	// それぞれのポインタと値を取得
	CManager *pManager = GetManager();
	CInput *pInput = pManager->GetInput();
	CGame *pGame = (CGame *)CManager::GetMode();
	D3DXVECTOR3 CameraRot = pGame->GetCamera()->GetCameraRot();
	CMeshField *pMeshField = pGame->GetMeshField();
	bool bJumpOld = m_Jump;

	//	移動処理。
	MoveUpdate();
	//	ジャンプチェック。
	Jump_Check(pInput);
	//	ブースト処理。
	Boost(pInput);
	//	あたり判定処理。
	CollisionCheck();
	//	重力加算。
	m_movePos.y -= GRAVITY;

	//	角度修正。
	Angle_Modification();

	//	角度慣性処理。
	m_moveRot.y += (m_purposeRot - m_moveRot.y) * m_RotInertia;

	m_Rot.y = CameraRot.y + D3DX_PI;

	//	移動量の慣性処理。
	m_movePos.x += (0.0f - m_movePos.x) * m_MoveInertia;
	m_movePos.z += (0.0f - m_movePos.z) * m_MoveInertia;

	//	座標更新。
	m_Pos += m_movePos;

	//	メッシュフィールドとのあたり判定。
	HitTest_With_Mesh(pMeshField);
	//	移動量チェック。
	Moving_Distance_Check();

	m_BoostEffect->SetPos(D3DXVECTOR3(m_Pos.x, m_Pos.y + 60.0f, m_Pos.z));

	Attack(pInput);

	if (m_LockOn == true) {
		EnemyLockOn();
	}

	//	ライフの更新。
	m_Life->Update(this);

	//	モーション更新。
	m_MotionManager->Update();
}

/******************************************************************************
	関数名 : void CPlayer::Draw(void)
	引数   : void
	戻り値 : なし
	説明   : 描画処理。
******************************************************************************/
void CPlayer::Draw(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//	行列式に使う作業用変数
	D3DXMATRIX mtxScl, mtxRot, mtxTrans;

	//	ワールドマトリクスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//	スケールを反映
	D3DXMatrixScaling(&mtxScl, m_Scl.x, m_Scl.y, m_Scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	//	回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//	移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	//	モデル描画開始。
	m_MotionManager->Draw();
}

/******************************************************************************
	関数名 : CPlayer * CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
	引数   : pos, rot
	戻り値 : なし
	説明   : 生成関数。
******************************************************************************/
CPlayer * CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer *obj = new CPlayer;
	obj->Init(pos, rot);
	return obj;
}

/******************************************************************************
	関数名 : void CPlayer::PlayerMoveUpdate(void)
	引数   : void
	戻り値 : なし
	説明   : 移動関連の処理をまとめた関数。
******************************************************************************/
void CPlayer::MoveUpdate(void)
{
	// それぞれのポインタと値を取得
	CManager    *pManager = GetManager();
	CInput      *pInput = pManager->GetInput();
	CGame		*pGame = (CGame *)CManager::GetMode();
	D3DXVECTOR3  CameraRot = pGame->GetCamera()->GetCameraRot();
	CCamera		*pCamera = pGame->GetCamera();

	bool button_push = false;

	if (pInput->GetJoyStickConnected() == true) {
		MoveUpdateJoyStick();
		return;
	}

	//	Wキーが押されている
	if (pInput->GetKeyboardPress(DIK_W) == true) {
		//	Aキーが押されている
		if (pInput->GetKeyboardPress(DIK_A) == true) {
			//カメラが向いている方向に対してまっすぐ前に進む
			m_movePos.x += sinf((-D3DX_PI * 0.75f) - CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf((-D3DX_PI * 0.75f) - CameraRot.y) * m_MoveSpeed;

			//	目標の角度設定
			m_purposeRot = (D3DX_PI * 0.75f) - CameraRot.y;
			pCamera->SetVecU(-0.5f);
		}
		//	Dキーが押されている
		if (pInput->GetKeyboardPress(DIK_D) == true) {
			//カメラが向いている方向に対してまっすぐ前に進む
			m_movePos.x += sinf((D3DX_PI * 0.75f) - CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf((D3DX_PI * 0.75f) - CameraRot.y) * m_MoveSpeed;

			//	目標の角度設定
			m_purposeRot = (-D3DX_PI * 0.75f) - CameraRot.y;
			pCamera->SetVecU(0.5f);
		}
		//	Wキーだけ押されている
		if (pInput->GetKeyboardPress(DIK_D) == false && pInput->GetKeyboardPress(DIK_A) == false) {
			//カメラが向いている方向に対してまっすぐ前に進む
			m_movePos.x += sinf(D3DX_PI - CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf(D3DX_PI - CameraRot.y) * m_MoveSpeed;

			//	目標の角度設定
			m_purposeRot = D3DX_PI + CameraRot.y;
		}
		m_Move = true;
		button_push = true;
	}
	if (pInput->GetKeyboardPress(DIK_S) == true) {

		//	Aキーが押されている
		if (pInput->GetKeyboardPress(DIK_A) == true) {
			//カメラが向いている方向に対してまっすぐ前に進む
			m_movePos.x += sinf((-D3DX_PI * 0.25f) - CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf((-D3DX_PI * 0.25f) - CameraRot.y) * m_MoveSpeed;

			//	目標の角度設定
			m_purposeRot = (D3DX_PI * 0.25f) - CameraRot.y;
			pCamera->SetVecU(-0.5f);
		}
		//	Dキーが押されている
		if (pInput->GetKeyboardPress(DIK_D) == true) {
			//カメラが向いている方向に対してまっすぐ前に進む
			m_movePos.x += sinf((D3DX_PI * 0.25f) - CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf((D3DX_PI * 0.25f) - CameraRot.y) * m_MoveSpeed;

			//	目標の角度設定
			m_purposeRot = (-D3DX_PI * 0.25f) - CameraRot.y;
			pCamera->SetVecU(0.5f);
		}
		//	Sキーだけ押されている
		if (pInput->GetKeyboardPress(DIK_D) == false && pInput->GetKeyboardPress(DIK_A) == false) {
			//カメラが向いている方向に対してまっすぐ手前進む
			m_movePos.x += sinf(-CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf(-CameraRot.y) * m_MoveSpeed;

			//	目標の角度設定
			m_purposeRot = D3DX_PI + CameraRot.y;
		}
		m_Move = true;
		button_push = true;
	}
	if ((pInput->GetKeyboardPress(DIK_S) == false && pInput->GetKeyboardPress(DIK_W) == false) && pInput->GetKeyboardPress(DIK_A) == true) {
		//カメラが向いている方向に対して左に直角的に進む
		m_movePos.x += sinf((-D3DX_PI * 0.5f) - CameraRot.y) * m_MoveSpeed;
		m_movePos.z -= cosf((-D3DX_PI * 0.5f) - CameraRot.y) * m_MoveSpeed;

		//	目標の角度設定
		m_purposeRot = (D3DX_PI * 0.5f) + CameraRot.y;
		pCamera->SetVecU(0.01f);

		button_push = true;
		m_Move = true;
	}
	if ((pInput->GetKeyboardPress(DIK_S) == false && pInput->GetKeyboardPress(DIK_W) == false) && pInput->GetKeyboardPress(DIK_D) == true) {
		//カメラが向いている方向に対して右に直角的に進む
		m_movePos.x += sinf((D3DX_PI * 0.5f) - CameraRot.y) * m_MoveSpeed;
		m_movePos.z -= cosf((D3DX_PI * 0.5f) - CameraRot.y) * m_MoveSpeed;

		//	目標の角度設定
		m_purposeRot = (-D3DX_PI * 0.5f) + CameraRot.y;
		pCamera->SetVecU(-0.01f);

		button_push = true;
		m_Move = true;
	}

	if (m_Jump == false && m_Move) {
		m_MotionManager->SetMotion(MOTION_MODE_MOVE);
	}

	if (button_push == false) {
		pCamera->SetVecU(0.0f);
	}
}

/******************************************************************************
	関数名 : void CPlayer::MoveUpdateJoyStick(void)
	引数   : void
	戻り値 : なし
	説明   : JoyStickを使った移動処理。
******************************************************************************/
void CPlayer::MoveUpdateJoyStick(void)
{
	// それぞれのポインタと値を取得
	CManager    *pManager = GetManager();
	CInput      *pInput = pManager->GetInput();
	CGame		*pGame = (CGame *)CManager::GetMode();
	D3DXVECTOR3  CameraRot = pGame->GetCamera()->GetCameraRot();
	CCamera		*pCamera = pGame->GetCamera();
	XINPUT_STATE *state = pInput->GetPressState();

	bool button_push = false;
	m_Move = true;
	button_push = true;

	if (state->Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
		pCamera->AddCameraRotY(0.03f);
	}
	else if (state->Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
		pCamera->AddCameraRotY(-0.03f);
	}

	if (state->Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
		pCamera->AddCameraRotX(-0.01f);
	}
	else if (state->Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
		pCamera->AddCameraRotX(0.01f);
	}

	//	Wキーが押されている
	if (state->Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
		//	Aキーが押されている
		if (state->Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
			//カメラが向いている方向に対してまっすぐ前に進む
			m_movePos.x += sinf((-D3DX_PI * 0.75f) - CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf((-D3DX_PI * 0.75f) - CameraRot.y) * m_MoveSpeed;

			//	目標の角度設定
			m_purposeRot = (D3DX_PI * 0.75f) - CameraRot.y;
			pCamera->SetVecU(-0.5f);
		}
		//	Dキーが押されている
		else if (state->Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
			//カメラが向いている方向に対してまっすぐ前に進む
			m_movePos.x += sinf((D3DX_PI * 0.75f) - CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf((D3DX_PI * 0.75f) - CameraRot.y) * m_MoveSpeed;

			//	目標の角度設定
			m_purposeRot = (-D3DX_PI * 0.75f) - CameraRot.y;
			pCamera->SetVecU(0.5f);
		}
		//	Wキーだけ押されている
		else if (state->Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
			//カメラが向いている方向に対してまっすぐ前に進む
			m_movePos.x += sinf(D3DX_PI - CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf(D3DX_PI - CameraRot.y) * m_MoveSpeed;

			//	目標の角度設定
			m_purposeRot = D3DX_PI + CameraRot.y;
		}

		m_Move = true;
		button_push = true;
	}
	if (state->Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {

		//	Aキーが押されている
		if (state->Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
			//カメラが向いている方向に対してまっすぐ前に進む
			m_movePos.x += sinf((-D3DX_PI * 0.25f) - CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf((-D3DX_PI * 0.25f) - CameraRot.y) * m_MoveSpeed;

			//	目標の角度設定
			m_purposeRot = (D3DX_PI * 0.25f) - CameraRot.y;
			pCamera->SetVecU(-0.5f);
		}
		//	Dキーが押されている
		else if (state->Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
			//カメラが向いている方向に対してまっすぐ前に進む
			m_movePos.x += sinf((D3DX_PI * 0.25f) - CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf((D3DX_PI * 0.25f) - CameraRot.y) * m_MoveSpeed;

			//	目標の角度設定
			m_purposeRot = (-D3DX_PI * 0.25f) - CameraRot.y;
			pCamera->SetVecU(0.5f);
		}
		//	Sキーだけ押されている
		else if (state->Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
			//カメラが向いている方向に対してまっすぐ手前進む
			m_movePos.x += sinf(-CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf(-CameraRot.y) * m_MoveSpeed;

			//	目標の角度設定
			m_purposeRot = D3DX_PI + CameraRot.y;
		}
		m_Move = true;
		button_push = true;
	}
	if ((state->Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && 
		 state->Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		 state->Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
		//カメラが向いている方向に対して左に直角的に進む
		m_movePos.x += sinf((-D3DX_PI * 0.5f) - CameraRot.y) * m_MoveSpeed;
		m_movePos.z -= cosf((-D3DX_PI * 0.5f) - CameraRot.y) * m_MoveSpeed;

		//	目標の角度設定
		m_purposeRot = (D3DX_PI * 0.5f) + CameraRot.y;
		pCamera->SetVecU(0.01f);

		button_push = true;
		m_Move = true;
	}
	if ((state->Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && 
		 state->Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) && 
		 state->Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
		//カメラが向いている方向に対して右に直角的に進む
		m_movePos.x += sinf((D3DX_PI * 0.5f) - CameraRot.y) * m_MoveSpeed;
		m_movePos.z -= cosf((D3DX_PI * 0.5f) - CameraRot.y) * m_MoveSpeed;

		//	目標の角度設定
		m_purposeRot = (-D3DX_PI * 0.5f) + CameraRot.y;
		pCamera->SetVecU(-0.01f);

		button_push = true;
		m_Move = true;
	}

	if (m_Jump == false && m_Move) {
		m_MotionManager->SetMotion(MOTION_MODE_MOVE);
	}

	if (button_push == false) {
		pCamera->SetVecU(0.0f);
	}
}

/******************************************************************************
	関数名 : void CPlayer::HitTest_With_Mesh(CMeshField *mesh)
	引数   : mesh
	戻り値 : なし
	説明   : メッシュフィールドとのあたり判定をしたときに、
			 メッシュフィールドより下にいた場合のみ、求めたY座標を
			 自分のY座標に代入する関数。
			 そして地面にいた際に地面の法線を取得してプレイヤーの移動量に
			 掛け合わせる処理。
******************************************************************************/
void CPlayer::HitTest_With_Mesh(CMeshField *mesh)
{
	D3DXVECTOR3 fieldNormal = mesh->GetFieldNormal(m_Pos);

	//	メッシュフィールド上でのY座標取得。
	float posY = mesh->Collision(m_Pos);
	//	求めた値よりY座標が低かったら座標更新して、ジャンプフラグをoffに戻す。
	if (posY >= m_Pos.y) {
		m_Pos.y = posY;
		m_Jump = false;
		if (m_Move == false) {
			m_MotionManager->SetMotion(MOTION_MDOE_NEUTRAL);
		}
	}

	//	ジャンプ中じゃなかったら移動に補正をかける。
	if (m_Jump == false) {
		//	x値の補正。
		if (fieldNormal.x > 0.5f || fieldNormal.x < -0.5f)
			m_movePos.x += fieldNormal.x;
		//	z値の補正。
		if (fieldNormal.z > 0.5f || fieldNormal.z < -0.5f)
			m_movePos.z += fieldNormal.z;
	}

	if (m_Pos.x > mesh->GetFieldWidth() - 100.0f) {
		m_Pos.x = mesh->GetFieldWidth() - 100.0f;
	}

	if (m_Pos.x < -mesh->GetFieldWidth() + 100.0f) {
		m_Pos.x = -mesh->GetFieldWidth() + 100.0f;
	}

	if (m_Pos.z > mesh->GetFieldDepth() - 100.0f) {
		m_Pos.z = mesh->GetFieldDepth() - 100.0f;
	}

	if (m_Pos.z < -mesh->GetFieldDepth() + 100.0f) {
		m_Pos.z = -mesh->GetFieldDepth() + 100.0f;
	}
}

/******************************************************************************
	関数名 : void CPlayer::Angle_Modification(void)
	引数   : void
	戻り値 : なし
	説明   : 角度を修正する関数。
******************************************************************************/
void CPlayer::Angle_Modification(void)
{
	//-3.14から3.14の領域に入った時の処理
	if (m_moveRot.y - m_purposeRot < -D3DX_PI)
		m_moveRot.y = D3DX_PI + (m_moveRot.y + D3DX_PI);

	//3.14から-3.14の領域に入った時の処理
	if (D3DX_PI < m_moveRot.y - m_purposeRot)
		m_moveRot.y = -D3DX_PI + (m_moveRot.y - D3DX_PI);
}

/******************************************************************************
	関数名 : void CPlayer::Moving_Distance_Check(void)
	引数   : void
	戻り値 : なし
	説明   : 角度を修正する関数。
******************************************************************************/
void CPlayer::Moving_Distance_Check(void)
{
	if (m_Jump == false) {
		//	移動量が一定値以下だったら移動フラグをoffにする。
		if (PLAYER_MOVEMENT_LOWEST_VALUE > m_movePos.x && -PLAYER_MOVEMENT_LOWEST_VALUE < m_movePos.x) {
			if (PLAYER_MOVEMENT_LOWEST_VALUE > m_movePos.z && -PLAYER_MOVEMENT_LOWEST_VALUE < m_movePos.z) {
				m_Move = false;		//	移動フラグをoffにする。
				m_MotionManager->SetMotion(MOTION_MDOE_NEUTRAL);
			}
			else {
				m_Move = true;			//	移動フラグをonにする。
			}
		}
		else {
			m_Move = true;		//	移動フラグをonにする。
		}
	}
}

/******************************************************************************
	関数名 : void CPlayer::Jump_Check(CInput *input)
	引数   : input
	戻り値 : なし
	説明   : ジャンプボタンを押しているかどうかを調べる関数。
			 押していたらジャンプして、ジャンプ中にまた押されたら
			 エアブースト状態にする。
			 エアブーストしているときにもう一度押すとエアブーストが解除される。
******************************************************************************/
void CPlayer::Jump_Check(CInput *input)
{
	//	スペースキーが押されたときにジャンプしてなかったらジャンプ。
	if (input->GetKeyboardTrigger(DIK_SPACE) == true || 
		input->TriggerJoyStick(XINPUT_GAMEPAD_A) == true) {
		if (m_Jump == false) {
			m_Jump = true;
			m_movePos.y = JUMP_POWER;
			m_MotionManager->SetMotion(MOTION_MODE_JUMP);
		}
		else {		//	すでにジャンプしていたらエアブースト状態に。すでにしていたら解除する。
			if (input->GetKeyboardTrigger(DIK_SPACE) == true) {
				m_AirBoost = m_AirBoost ? false : true;
			}
		}
	}
}

/******************************************************************************
	関数名 : void CPlayer::Attack(void)
	引数   : void
	戻り値 : なし
	説明   : 攻撃する際の処理が入った関数。
******************************************************************************/
void CPlayer::Attack(CInput *input)
{
	// それぞれのポインタと値を取得
	CManager *pManager = GetManager();
	CGame *pGame = (CGame *)CManager::GetMode();
	D3DXVECTOR3 CameraRot = pGame->GetCamera()->GetCameraRot();

	//	弾発射。
	if (input->GetMouseTrigger(MOUSE_INPUT_LEFT) == true ||
		input->TriggerJoyStick(XINPUT_GAMEPAD_RIGHT_SHOULDER) == true) {
		CBullet::Create(D3DXVECTOR3(m_Pos.x, m_Pos.y + 30.0f, m_Pos.z), D3DXVECTOR3(CameraRot.x, m_Rot.y, m_Rot.z), 20.0f, 20.0f, CTexture::G_BULLET, m_Objtype);
	}

	if (input->GetKeyboardTrigger(DIK_Q) == true ||
		input->TriggerJoyStick(XINPUT_GAMEPAD_LEFT_SHOULDER) == true) {
		m_LockOn = m_LockOn == true ? false : true;
	}
}

/******************************************************************************
	関数名 : void CPlayer::Boost(CInput * input)
	引数   : input
	戻り値 : なし
	説明   : ブースト処理。
******************************************************************************/
void CPlayer::Boost(CInput * input)
{
	//	マウスの右クリックが押されたらブースト処理。
	if (input->GetMouseTrigger(MOUSE_INPUT_RIGHT) == true || 
		input->TriggerJoyStick(XINPUT_GAMEPAD_X)) {
		if (m_Boost == false) {
			m_Boost = true;
			m_movePos.x *= m_MoveSpeed * BOOST_MAGNIFICATION;
			m_movePos.z *= m_MoveSpeed * BOOST_MAGNIFICATION;
			m_BoostEffect->SetDuringUse(true);
		}
	}
	//	ブーストしてたらクールダウンチェック。
	if (m_Boost == true) {
		m_CoolingTimeBoost = (m_CoolingTimeBoost + 1) % BOOST_COOLING_TIME;
		m_Boost = m_CoolingTimeBoost == 0 ? false : true;
	}
}

/******************************************************************************
	関数名 : void CPlayer::CollisionCheck(void)
	引数   : void
	戻り値 : なし
	説明   : あたり判定の排斥処理。
******************************************************************************/
void CPlayer::CollisionCheck(void)
{
	//	自分になにか当たったら。
	if (m_pCollision->GetHit()) {
		CCollisionList *collicionList = m_pCollision->GetCollisionList();
		std::list<CCollision *> *collList = collicionList->GetCollision();

		for (auto i = collList->begin(); i != collList->end();) {
			CCollision *object = (*i);

			if (object != NULL) {
				if (object->GetScene()->GetObjectType() == OBJ_TYPE_BULLET) {
					CBullet *bullet = (CBullet *)object->GetScene();

					//	撃った対象がPlayerだったら。
					if (bullet->GetTypeUsingSelf() == OBJ_TYPE_ENEMY) {
						//	HPが減少。
						--m_Hp;
						CExplosion::Create(object->GetScene()->GetPos(), 125.0f, 125.0f, CTexture::G_EXPLOSION);
						object->GetScene()->SetDelete(true);
					}
				}
				//	リストのなかみ削除。
				i = collList->erase(i);
			}
		}
	}
}

/******************************************************************************
	関数名 : void CPlayer::EnemyLockOn(void)
	引数   : void
	戻り値 : なし
	説明   : 敵のロックオン処理。
******************************************************************************/
void CPlayer::EnemyLockOn(void)
{
	// それぞれのポインタと値を取得
	CManager *pManager = GetManager();
	CGame *pGame = (CGame *)CManager::GetMode();
	CCamera *camera = pGame->GetCamera();

	CScene *pScene = CScene::GetListTopPointer(CScene::OBJECT_ENEMY);
	CScene *pSceneNext;
	
	float distance = 0.0f;
	float minLength = CERTAIN_DISTANCE;
	float angle = 0.0f;
	float rotX = 0.0f;
	float lnegthX, lengthY;
	int cnt = 0;
	bool lockOn = false;
	D3DXVECTOR3 targetPos = m_TargetPos;

	//	nullじゃなければ回す。
	while (pScene != null) {
		//	次のポインタ取得。
		pSceneNext = pScene->GetSceneNext();
		//	タイプがOBJ_TYPE_ENEMYだったら入る。
		if (pScene->GetObjectType() == CScene::OBJ_TYPE_ENEMY) {
			if (pScene->GetDeleteFlag() == false) {
				distance = sqrtf(((pScene->GetPos().x - m_Pos.x) * (pScene->GetPos().x - m_Pos.x)) +
								 ((pScene->GetPos().y - m_Pos.y) * (pScene->GetPos().y - m_Pos.y)) +
								 ((pScene->GetPos().z - m_Pos.z) * (pScene->GetPos().z - m_Pos.z)));

				if (distance < minLength || cnt == 0) {
					if (distance < CERTAIN_DISTANCE) {
						minLength = distance;
						m_TargetPos = pScene->GetPos();
						lockOn = true;
					}
				}
			}
		}
		//	次のポインタ設定。
		pScene = pSceneNext;
		++cnt;
	}

	if (lockOn == false) {
		if (targetPos == m_TargetPos) {
			m_LockOn = false;
			return;
		}
	}

	//	長さを取得。
	lnegthX = sqrt(powf(m_Pos.x - m_TargetPos.x, 2) + powf(m_Pos.z - m_TargetPos.z, 2));
	lengthY = (m_Pos.y + 50.0f) - (m_TargetPos.y + 30.0f);

	//	求めた長さで発射角度計算。
	rotX = atan2f(lengthY, lnegthX);

	angle = MATH_ANGLE(m_TargetPos.x - m_Pos.x, m_TargetPos.z - m_Pos.z);
	camera->SetPosR(m_TargetPos);
	camera->SetCameraRotY(angle);
	camera->SetCameraRotX(rotX);
}
