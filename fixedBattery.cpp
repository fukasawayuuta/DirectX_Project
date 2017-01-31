/******************************************************************************
	�^�C�g���� : CFixedBattery�N���X
	�t�@�C���� : fixedBattery.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/12/17
	�X�V��     :
******************************************************************************/
/******************************************************************************
	�C���N���[�h�t�@�C��
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
	�}�N����`
******************************************************************************/
const float VIEW_DISTANCE = 1000.0f;		//	���E�����B
const float VIEW_ANGLE = 45.0f;
const int SHOT_INTERVAL_TIME = 30;		//	�e��łԊu�B

/******************************************************************************
	�֐��� : CFixedBattery::CFixedBattery(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CEnemy(Priority, objType, sceneType)
	����   : �R���X�g���N�^
******************************************************************************/
CFixedBattery::CFixedBattery(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CEnemy(Priority, objType, sceneType)
{
	m_Discovery = false;
	m_ViewDistance = 0.0f;
	m_ViewAngle = 0.0f;
	m_ShotInterval = 0;
}

/******************************************************************************
	�֐��� : CFixedBattery::~CFixedBattery()
	����   : �f�X�g���N�^
******************************************************************************/
CFixedBattery::~CFixedBattery()
{
}

/******************************************************************************
	�֐��� : void CFixedBattery::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
	����   : pos, rot, modelType
	�߂�l : �Ȃ�
	����   : ���W�A�p�x�̏������B
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
	//	�����蔻�萶���B
	m_pCollision = CSphere::Create(D3DXVECTOR3(0.0f, 50.0f, 0.0f), 50.0f, this);
}

/******************************************************************************
	�֐��� : void CFixedBattery::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �v���C���[��T���āA��������U���J�n�B
******************************************************************************/
void CFixedBattery::Update(void)
{
	CManager *pManager = GetManager();
	CGame	 *pGame = (CGame *)CManager::GetMode();
	CPlayer  *pPlayer = pGame->GetPlayer();

	//	���E����B
	ViewDecision(pPlayer);
	//	�������ĂȂ��Ƃ��B
	if (m_Discovery == false) {
		m_MotionManager->SetMotion(MOTION_MODE_SEARCH);
	}
	else {	//	���������Ƃ��B
		m_MotionManager->SetMotion(MOTION_MDOE_NEUTRAL);
		CModel *model = m_MotionManager->GetModel(BODY);
		float lnegthX, lengthY;
		float rotX, rotZ;
		//	��Ɨp�ϐ��B
		float E_To_P_Angle = 0.0f;

		//	�������擾�B
		lnegthX = sqrt(powf(m_Pos.x - pPlayer->GetPos().x, 2) + powf(m_Pos.z - pPlayer->GetPos().z, 2));
		lengthY = (m_Pos.y + m_MotionManager->GetModel(BARREL)->GetPosition().y) - (pPlayer->GetPos().y + 40.0f);

		//	���߂������Ŕ��ˊp�x�v�Z�B
		rotX = -atan2f(lengthY, lnegthX);
		//	���߂������Ŕ��ˊp�x�v�Z�B
		rotZ = atan2f(lnegthX, lengthY);
		//	�����ƃv���C���[�̊p�x�擾�B
		E_To_P_Angle = atan2f(m_Pos.x - pPlayer->GetPos().x, m_Pos.z - pPlayer->GetPos().z);
		//	�J�E���^�[�v�Z�B
		m_ShotInterval = (m_ShotInterval + 1) % SHOT_INTERVAL_TIME;
		//	�J�E���^�[��0�ɂȂ����甭�ˁB
		if (m_ShotInterval == 0) {
			D3DXVECTOR3 shotPos = D3DXVECTOR3(m_MotionManager->GetModelMatrix(1)->_41, m_MotionManager->GetModelMatrix(1)->_42, m_MotionManager->GetModelMatrix(1)->_43);
			D3DXVECTOR3 shotAngle = m_MotionManager->GetModel(BARREL)->GetRotation();
			//	�e�����B
			CBullet::Create(shotPos, D3DXVECTOR3(-rotX, E_To_P_Angle, rotZ), 20.0f, 20.0f, CTexture::G_BULLET, m_Objtype);
		}
	}

	//	���ʂ̍X�V�B
	CEnemy::Update();
}

/******************************************************************************
	�֐��� : void CFixedBattery::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
	����   : pos, rot
	�߂�l : �Ȃ�
	����   : ���W�A�p�x�̏������B
******************************************************************************/
void CFixedBattery::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CFixedBattery *obj = new CFixedBattery();
	obj->Init(pos, rot);
}

/******************************************************************************
	�֐��� : void CFixedBattery::ViewDecision(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �v���C���[�����E�͈͓��ɂ��邩�ǂ����B
******************************************************************************/
void CFixedBattery::ViewDecision(CPlayer *player)
{
	D3DXVECTOR3 Direction_Facing, Direction_With_Player;
	float normal;
	float length;
	float angle;

	//	�����̌����Ă�������x�N�g���B
	Direction_Facing.x = -sinf(m_Rot.y);
	Direction_Facing.y = 0.0f;
	Direction_Facing.z = -cosf(m_Rot.y);
	//	�v���C���[���玩���ւ̕����x�N�g���B
	Direction_With_Player = player->GetPos() - m_Pos;
	//	�����擾�B
	length = MATH_LENGTH_3D(Direction_With_Player.x, Direction_With_Player.y, Direction_With_Player.z);
	//	���K���B
	D3DXVec3Normalize(&Direction_Facing, &Direction_Facing);
	D3DXVec3Normalize(&Direction_With_Player, &Direction_With_Player);
	//	���όv�Z�B
	normal = D3DXVec3Dot(&Direction_Facing, &Direction_With_Player);
	angle = acosf(normal);
	//	�p�x�C���B
	angle = D3DXToDegree(angle);
	//	���͈͓��ň��̋������Z�������甭���B
	if (normal > 0.5f) {
		if (m_ViewDistance > length) {
			//	�����B
			m_Discovery = true;
		}
		else {
			//	�����ł����B
			m_Discovery = false;
		}
	}
	else {
		//	�����ł����B
		m_Discovery = false;
	}
}
