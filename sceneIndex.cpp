/******************************************************************************
	�^�C�g���� : SceneIndex�N���X
	�t�@�C���� : sceneIndex.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/12/13
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

/******************************************************************************
	�֐��� : CSceneIndex::CSceneIndex(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene3D(Priority, objType, sceneType)
	����   : �R���X�g���N�^
			 �ϐ��̏������B
******************************************************************************/
CSceneIndex::CSceneIndex(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene3D(Priority, objType, sceneType)
{
	m_pIdxBuff = NULL;
	m_VertexNum = 0;
	m_PolygonNum = 0;
}

/******************************************************************************
	�֐��� : CSceneIndex::~CSceneIndex()
	����   : �f�X�g���N�^
			 �N���X���j�����ꂽ�Ƃ��ɌĂ΂�邪�A���g���Ȃ��̂œ��ɋL�q���鎖�Ȃ��B
******************************************************************************/
CSceneIndex::~CSceneIndex()
{
}

/******************************************************************************
	�֐��� : void CSceneIndex::Uninit(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �C���f�b�N�X�o�b�t�@�̉�������B
******************************************************************************/
void CSceneIndex::Uninit(void)
{
	CScene3D::Uninit();
	SAFE_RELEASE(m_pIdxBuff);
}

/******************************************************************************
	�֐��� : void CScene3D::DrawIndex(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �C���f�b�N�X�o�b�t�@���g�����`�揈���B
******************************************************************************/
void CSceneIndex::Draw(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�s�񎮂Ɏg����Ɨp�ϐ�
	D3DXMATRIX mtxScl, mtxRot, mtxTrans;
	//���[���h�}�g���N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXMatrixScaling(&mtxScl, m_Scl.x, m_Scl.y, m_Scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);		//�X�P�[���𔽉f

	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);		//��]�𔽉f

	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);	//�ړ��𔽉f

																//	���[���h�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

	pDevice->SetIndices(m_pIdxBuff);

	//���_���W�ӂ��[�܂���
	pDevice->SetFVF(FVF_VERTEX_3D);
	//	�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CTexture::GetTexture(m_SceneType, m_TextureType));
	//�|���S���`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_VertexNum, 0, m_PolygonNum);
}
