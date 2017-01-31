/******************************************************************************
	タイトル名 : CFixedBatteryクラス
	ファイル名 : fixedBattery.cpp
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
#include "scene3D.h"
#include "sceneX.h"
#include "billboard.h"
#include "bullet.h"
#include "model.h"
#include "motionManager.h"
#include "dynamicModel.h"
#include "Enemy.h"
#include "collision.h"
#include "capsule.h"
#include "sphere.h"
#include "collisionDetection.h"
#include "collisionList.h"
#include "Explosion.h"
#include "fixedBattery.h"
#include "Mode.h"
#include "game.h"
#include "player.h"

/******************************************************************************
	マクロ定義
******************************************************************************/
const float VIEW_DISTANCE = 1000.0f;		//	視界距離。
const float VIEW_ANGLE = 45.0f;
const int SHOT_INTERVAL_TIME = 30;		//	弾を打つ間隔。

/******************************************************************************
	関数名 : CFixedBattery::CFixedBattery(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CEnemy(Priority, objType, sceneType)
	説明   : コンストラクタ
******************************************************************************/
CFixedBattery::CFixedBattery(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CEnemy(Priority, objType, sceneType)
{
	m_Discovery = false;
	m_ViewDistance = 0.0f;
	m_ViewAngle = 0.0f;
	m_ShotInterval = 0;
}

/******************************************************************************
	関数名 : CFixedBattery::~CFixedBattery()
	説明   : デストラクタ
******************************************************************************/
CFixedBattery::~CFixedBattery()
{
}

/******************************************************************************
	関数名 : void CFixedBattery::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
	引数   : pos, rot, modelType
	戻り値 : なし
	説明   : 座標、角度の初期化。
******************************************************************************/
void CFixedBattery::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_MotionManager = CMotionManager::Create(DYNAMICMODEL_TYPE_ENEMY_00, &m_mtxWorld);
	m_MotionManager->SetMotion(MOTION_MDOE_NEUTRAL);

	m_Pos = pos;
	m_Rot = rot;
	m_Hp = 3;
	m_ViewDistance = VIEW_DISTANCE;
	m_ViewAngle = VIEW_ANGLE;
	//	あたり判定生成。
	m_pCollision = CSphere::Create(D3DXVECTOR3(0.0f, 50.0f, 0.0f), 50.0f, this);
}

/******************************************************************************
	関数名 : void CFixedBattery::Update(void)
	引数   : void
	戻り値 : なし
	説明   : プレイヤーを探して、見つけたら攻撃開始。
******************************************************************************/
void CFixedBattery::Update(void)
{
	CManager *pManager = GetManager();
	CGame	 *pGame = (CGame *)CManager::GetMode();
	CPlayer  *pPlayer = pGame->GetPlayer();

	//	視界判定。
	ViewDecision(pPlayer);
	//	発見してないとき。
	if (m_Discovery == false) {
		m_MotionManager->SetMotion(MOTION_MODE_SEARCH);
	}
	else {	//	発見したとき。
		m_MotionManager->SetMotion(MOTION_MDOE_NEUTRAL);
		CModel *model = m_MotionManager->GetModel(BODY);
		float lnegthX, lengthY;
		float rotX, rotZ;
		//	作業用変数。
		float E_To_P_Angle = 0.0f;

		//	長さを取得。
		lnegthX = sqrt(powf(m_Pos.x - pPlayer->GetPos().x, 2) + powf(m_Pos.z - pPlayer->GetPos().z, 2));
		lengthY = (m_Pos.y + m_MotionManager->GetModel(BARREL)->GetPosition().y) - (pPlayer->GetPos().y + 40.0f);

		//	求めた長さで発射角度計算。
		rotX = -atan2f(lengthY, lnegthX);
		//	求めた長さで発射角度計算。
		rotZ = atan2f(lnegthX, lengthY);
		//	自分とプレイヤーの角度取得。
		E_To_P_Angle = atan2f(m_Pos.x - pPlayer->GetPos().x, m_Pos.z - pPlayer->GetPos().z);
		//	カウンター計算。
		m_ShotInterval = (m_ShotInterval + 1) % SHOT_INTERVAL_TIME;
		//	カウンターが0になったら発射。
		if (m_ShotInterval == 0) {
			D3DXVECTOR3 shotPos = D3DXVECTOR3(m_MotionManager->GetModelMatrix(1)->_41, m_MotionManager->GetModelMatrix(1)->_42, m_MotionManager->GetModelMatrix(1)->_43);
			D3DXVECTOR3 shotAngle = m_MotionManager->GetModel(BARREL)->GetRotation();
			//	弾生成。
			CBullet::Create(shotPos, D3DXVECTOR3(-rotX, E_To_P_Angle, rotZ), 20.0f, 20.0f, CTexture::G_BULLET, m_Objtype);
		}
	}

	//	共通の更新。
	CEnemy::Update();
}

/******************************************************************************
	関数名 : void CFixedBattery::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
	引数   : pos, rot
	戻り値 : なし
	説明   : 座標、角度の初期化。
******************************************************************************/
void CFixedBattery::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CFixedBattery *obj = new CFixedBattery();
	obj->Init(pos, rot);
}

/******************************************************************************
	関数名 : void CFixedBattery::ViewDecision(void)
	引数   : void
	戻り値 : なし
	説明   : プレイヤーが視界範囲内にいるかどうか。
******************************************************************************/
void CFixedBattery::ViewDecision(CPlayer *player)
{
	D3DXVECTOR3 Direction_Facing, Direction_With_Player;
	float normal;
	float length;
	float angle;

	//	自分の向いている方向ベクトル。
	Direction_Facing.x = -sinf(m_Rot.y);
	Direction_Facing.y = 0.0f;
	Direction_Facing.z = -cosf(m_Rot.y);
	//	プレイヤーから自分への方向ベクトル。
	Direction_With_Player = player->GetPos() - m_Pos;
	//	距離取得。
	length = MATH_LENGTH_3D(Direction_With_Player.x, Direction_With_Player.y, Direction_With_Player.z);
	//	正規化。
	D3DXVec3Normalize(&Direction_Facing, &Direction_Facing);
	D3DXVec3Normalize(&Direction_With_Player, &Direction_With_Player);
	//	内積計算。
	normal = D3DXVec3Dot(&Direction_Facing, &Direction_With_Player);
	angle = acosf(normal);
	//	角度修正。
	angle = D3DXToDegree(angle);
	//	一定範囲内で一定の距離より短かったら発見。
	if (normal > 0.5f) {
		if (m_ViewDistance > length) {
			//	発見。
			m_Discovery = true;
		}
		else {
			//	発見できず。
			m_Discovery = false;
		}
	}
	else {
		//	発見できず。
		m_Discovery = false;
	}
}
