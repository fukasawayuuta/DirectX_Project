/******************************************************************************
	�^�C�g���� : CDesertDust�N���X
	�t�@�C���� : desertDust.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2017/01/19
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
#include "desertDust.h"
#include "Mode.h"
#include "game.h"
#include "MeshField.h"

/******************************************************************************
	�}�N���K�X
******************************************************************************/
const float MOVE_SPEED = 3.0f;			//	�ړ��ʁB

/******************************************************************************
	�֐��� : CDesertDust::CDesertDust(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CBillboard(Priority, objType, sceneType)
	����   : �R���X�g���N�^
******************************************************************************/
CDesertDust::CDesertDust(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CBillboard(Priority, objType, sceneType)
{
	m_movePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_MoveSpeed = rand() % (int)MOVE_SPEED + 1;
}

/******************************************************************************
	�֐��� : CDesertDust::~CDesertDust()
	����   : �f�X�g���N�^
******************************************************************************/
CDesertDust::~CDesertDust()
{
}

/******************************************************************************
	�֐��� : void CDesertDust::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : ���W�̍X�V�B
******************************************************************************/
void CDesertDust::Update(void)
{
	// ���ꂼ��̃|�C���^�ƒl���擾
	CManager *pManager = GetManager();
	CGame *pGame = (CGame *)CManager::GetMode();
	CMeshField *pMeshField = pGame->GetMeshField();

	//	���ɒ��p�I�ɐi��
	m_movePos.x = sinf((-D3DX_PI * 0.5f) - m_Rot.y) * m_MoveSpeed;
	m_movePos.z = -cosf((-D3DX_PI * 0.5f) - m_Rot.y) * m_MoveSpeed;

	//	���W�X�V�B
	m_Pos += m_movePos;

	CheckOutsideRange(pMeshField);
}

/******************************************************************************
	�֐��� : void CDesertDust::Create(D3DXVECTOR3 pos, float width, float height, int textureType)
	����   : pos, width, height, textureType
	�߂�l : �Ȃ�
	����   : �����֐��B
******************************************************************************/
void CDesertDust::Create(D3DXVECTOR3 pos, float width, float height, int textureType)
{
	CDesertDust *obj = new CDesertDust;
	obj->Init(pos, width, height, 0.0f, textureType);
}

/******************************************************************************
	�֐��� : void CDesertDust::RendererBegin(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �`�悷��ۂ̃����_���[�̐ݒ�B
******************************************************************************/
void CDesertDust::RendererBegin(void)
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
	�֐��� : void CDesertDust::RendererEnd(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �`�悵����̃����_���[�̐ݒ�B
******************************************************************************/
void CDesertDust::RendererEnd(void)
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
	�֐��� : void CDesertDust::Reset(CMeshField *mesh)
	����   : mesh
	�߂�l : �Ȃ�
	����   : �͈͊O�ɏo�Ă��܂����I�u�W�F�N�g��͈͓��ɖ߂��B
******************************************************************************/
void CDesertDust::Reset(CMeshField *mesh)
{
	m_Pos.x = rand() % (int)mesh->GetFieldWidth();
	m_Pos.z = rand() % (int)(mesh->GetFieldDepth() * 2.0f) - (mesh->GetFieldDepth());
}

/******************************************************************************
	�֐��� : void CDesertDust::CheckOutsideRange(CMeshField *mesh)
	����   : mesh
	�߂�l : �Ȃ�
	����   : �͈͊O�ɏo�Ă��܂������ǂ��������o����B
******************************************************************************/
void CDesertDust::CheckOutsideRange(CMeshField *mesh)
{
	if (m_Pos.x > mesh->GetFieldWidth() || m_Pos.x < -mesh->GetFieldWidth() ||
		m_Pos.z > mesh->GetFieldDepth() || m_Pos.z < -mesh->GetFieldDepth()) {
		Reset(mesh);
	}
}
