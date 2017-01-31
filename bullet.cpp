/******************************************************************************
	�^�C�g���� : Bullet�N���X
	�t�@�C���� : bullet.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/12/15
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
#include "sceneIndex.h"
#include "camera.h"
#include "billboard.h"
#include "bullet.h"
#include "collision.h"
#include "capsule.h"
#include "collisionList.h"
#include "smoke.h"
#include "MeshField.h"
#include "Explosion.h"
#include "Mode.h"
#include "game.h"

/******************************************************************************
	�֐��� : CBullet::CBullet(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CBillboard(Priority, objType, sceneType)
	����   : �R���X�g���N�^
******************************************************************************/
CBullet::CBullet(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CBillboard(Priority, objType, sceneType)
{
	m_Movement = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_TypeUsingSelf = OBJ_TYPE_NONE;
}

/******************************************************************************
	�֐��� : CPlayer::~CPlayer()
	����   : �f�X�g���N�^
******************************************************************************/
CBullet::~CBullet()
{
}

/******************************************************************************
	�֐��� : void CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float height, int textureType,OBJ_TYPE objType)
	����   : pos, rot, width, height, textureType, objType
	�߂�l : �Ȃ�
	����   : ���W�Ɗp�x�̐ݒ�B
******************************************************************************/
void CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float height, int textureType, OBJ_TYPE objType)
{
	CBillboard::Init(pos, width, height, 0.0f, textureType);

	m_GoAngle = rot;
	m_TypeUsingSelf = objType;
	m_CollRadius = 1.0f;
	m_MovingDistance = 25.0f;
	//	�ړ��ʍX�V�B
	m_Movement.x = sinf(m_GoAngle.y) * cosf(m_GoAngle.x) * m_MovingDistance;
	m_Movement.y = sinf(m_GoAngle.x) * m_MovingDistance;
	m_Movement.z = cosf(m_GoAngle.y) * cosf(m_GoAngle.x) * m_MovingDistance;

	m_pCollision = CCapsule::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_CollRadius, D3DXVECTOR3(0.0f, 0.0f, 0.0f) , m_Movement, this);
}

/******************************************************************************
	�֐��� : void CBullet::Uninit(void)
	����   : void
	�߂�l : �Ȃ�
	����   : ��������B
******************************************************************************/
void CBullet::Uninit(void)
{
	CBillboard::Uninit();

	m_pCollision->SetDelete(true);
	//SAFE_DELETE(m_pCollision);
}

/******************************************************************************
	�֐��� : void CBullet::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �ړ������Ȃǂ̍X�V�����B
******************************************************************************/
void CBullet::Update(void)
{
	// ���ꂼ��̃|�C���^�ƒl���擾
	CManager *pManager = GetManager();
	CGame *pGame = (CGame *)CManager::GetMode();
	CMeshField *pMeshField = pGame->GetMeshField();

	//	�ړ��ʍX�V�B
	m_Movement.x = sinf(m_GoAngle.y) * cosf(m_GoAngle.x) * m_MovingDistance;
	m_Movement.y = sinf(m_GoAngle.x) * m_MovingDistance;
	m_Movement.z = cosf(m_GoAngle.y) * cosf(m_GoAngle.x) * m_MovingDistance;

	//	���W�X�V�B
	m_Pos -= m_Movement;

	CheckOutsideRange(pMeshField);

	float rotation = (rand() % 628) / 100.0f;

	CManager::CheckRot(&rotation);
	CSmoke::Create(m_Pos, rotation, 50.0f, 50.0f, CTexture::G_SMOKE);
}

/******************************************************************************
	�֐��� : void CBullet::RendererBegin(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �`�悷��ۂ̃����_���[�̐ݒ�B
******************************************************************************/
void CBullet::RendererBegin(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//	�A���t�@�u�����h�ݒ�////////////////////////////////////////////////////
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//�`���ƕ`�挳�̒l�����Z
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	///////////////////////////////////////////////////////////////////////////
}

/******************************************************************************
	�֐��� : void CSmoke::RendererEnd(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �`�悵����̃����_���[�̐ݒ�B
******************************************************************************/
void CBullet::RendererEnd(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//	�A���t�@�u�����h�ݒ�////////////////////////////////////////////////////
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//�`���ƕ`�挳�̒l�����Z
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);//�J�����O�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//�A���t�@�u�����h�̐ݒ�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//�\�[�X�u�����h�̐ݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//�C���o�[�X�u�����h�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	///////////////////////////////////////////////////////////////////////////
}

/******************************************************************************
	�֐��� : void CBullet::CheckOutsideRange(CMeshField * mesh)
	����   : mesh
	�߂�l : �Ȃ�
	����   : �͈͊O�ɏo����폜����B
******************************************************************************/
void CBullet::CheckOutsideRange(CMeshField * mesh)
{
	//	���b�V���t�B�[���h��ł�Y���W�擾�B
	float posY = mesh->Collision(m_Pos);

	if (m_Pos.x > mesh->GetFieldWidth() - 100.0f || m_Pos.x < -mesh->GetFieldWidth() + 100.0f ||
		m_Pos.z > mesh->GetFieldDepth() - 100.0f || m_Pos.z < -mesh->GetFieldDepth() + 100.0f) {
		m_Delete = true;
	}

	//	���߂��l���Y���W���Ⴉ������폜�B
	if (posY >= m_Pos.y) {
		m_Delete = true;
		CExplosion::Create(m_Pos, 125.0f, 125.0f, CTexture::G_EXPLOSION);
	}
}

/******************************************************************************
	�֐��� : void CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float height, int textureType, OBJ_TYPE objType)
	����   : pos, rot, width, height, textureType, objType
	�߂�l : �Ȃ�
	����   : �����֐��B
******************************************************************************/
void CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float height, int textureType, OBJ_TYPE objType)
{
	CBullet *obj = new CBullet;
	obj->Init(pos, rot, width, height, textureType, objType);
}
