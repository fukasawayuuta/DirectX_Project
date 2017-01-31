/******************************************************************************
	�^�C�g���� : Enemy�N���X
	�t�@�C���� : Enemy.cpp
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

/******************************************************************************
	�֐��� : CEnemy::CEnemy(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CDynamicModel(Priority, objType, sceneType)
	����   : �R���X�g���N�^
******************************************************************************/
CEnemy::CEnemy(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CDynamicModel(Priority, objType, sceneType)
{
	m_Hp = 0;
	m_pCollision = NULL;
	m_Radius = 0.0f;
}

/******************************************************************************
	�֐��� : CEnemy::~CEnemy()
	����   : �f�X�g���N�^
******************************************************************************/
CEnemy::~CEnemy()
{
}

/******************************************************************************
	�֐��� : void CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, DYNAMICMODEL_TYPE modelType)
	����   : pos, rot, modelType
	�߂�l : �Ȃ�
	����   : ���W�A�p�x�̏������B
******************************************************************************/
void CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, DYNAMICMODEL_TYPE modelType)
{
	//	���[�V�����ݒ�B
	m_MotionManager = CMotionManager::Create(modelType, &m_mtxWorld);
	m_MotionManager->SetMotion(0);

	m_Pos = pos;
	m_Rot = rot;
	m_Hp = 3;
	//	�����蔻�萶���B
	m_pCollision = CSphere::Create(D3DXVECTOR3(0.0f, 50.0f, 0.0f), 50.0f, this);
	//m_pCollision = CCapsule::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 50.0f, 0.0f), this);
}

/******************************************************************************
	�֐��� : void CEnemy::Uninit(void)
	����   : void
	�߂�l : �Ȃ�
	����   : ��������B
******************************************************************************/
void CEnemy::Uninit(void)
{
	m_MotionManager->Uninit();
	SAFE_DELETE(m_MotionManager);

	if (m_pCollision != NULL) {
		m_pCollision->SetDelete(true);
		m_pCollision->GetCollisionList()->Uninit();
		m_pCollision = NULL;
	}
}

/******************************************************************************
	�֐��� : void CEnemy::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : ��������B
******************************************************************************/
void CEnemy::Update(void)
{
	m_MotionManager->Update();

	if (m_Hp < 0) {
		m_Delete = true;
		CExplosion::Create(m_Pos, 125.0f, 125.0f, CTexture::G_EXPLOSION);
	}

	//	�����ɂȂɂ�����������B
	if (m_pCollision->GetHit()) {
		CCollisionList *collicionList = m_pCollision->GetCollisionList();
		std::list<CCollision *> *collList = collicionList->GetCollision();

		for (auto i = collList->begin(); i != collList->end();) {
			CCollision *object = (*i);

			if (object != NULL) {
				if (object->GetScene()->GetObjectType() == OBJ_TYPE_BULLET) {
					CBullet *bullet = (CBullet *)object->GetScene();

					//	�������Ώۂ�Player��������B
					if (bullet->GetTypeUsingSelf() == OBJ_TYPE_PLAYER) {
						//	HP�������B
						--m_Hp;
						CExplosion::Create(object->GetScene()->GetPos(), 125.0f, 125.0f, CTexture::G_EXPLOSION);
						object->GetScene()->SetDelete(true);
					}
				}
				//	���X�g�̂Ȃ��ݍ폜�B
				i = collList->erase(i);
			}
		}
	}
}

/******************************************************************************
	�֐��� : void CEnemy::Draw(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �`��֐��B
******************************************************************************/
void CEnemy::Draw(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//	�s�񎮂Ɏg����Ɨp�ϐ�
	D3DXMATRIX mtxScl, mtxRot, mtxTrans;

	//	���[���h�}�g���N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//	�X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, m_Scl.x, m_Scl.y, m_Scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	//	��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//	�ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	m_MotionManager->Draw();
}

/******************************************************************************
	�֐��� : CEnemy * CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, DYNAMICMODEL_TYPE modelType)
	����   : pos, rot, modelType
	�߂�l : obj
	����   : �����֐��B
******************************************************************************/
CEnemy * CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, DYNAMICMODEL_TYPE modelType)
{
	CEnemy *obj = new CEnemy;
	obj->Init(pos, rot, modelType);
	return obj;
}
