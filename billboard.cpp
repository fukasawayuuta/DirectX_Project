/******************************************************************************
	�^�C�g���� : Billboard�N���X
	�t�@�C���� : billboard.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/12/12
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
#include "camera.h"
#include "billboard.h"

/******************************************************************************
	�֐��� : CBillboard::CBillboard(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene3D(Priority, objType, sceneType)
	����   : �R���X�g���N�^
			 �ϐ��̏������B
******************************************************************************/
CBillboard::CBillboard(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene3D(Priority, objType, sceneType)
{
	m_bY_Axis_Fixed = false;
}

/******************************************************************************
	�֐��� : CBillboard::~CBillboard()
	����   : �f�X�g���N�^
			 �N���X���j�����ꂽ�Ƃ��ɌĂ΂�邪�A���g���Ȃ��̂œ��ɋL�q���鎖�Ȃ��B
******************************************************************************/
CBillboard::~CBillboard()
{
}

/******************************************************************************
	�֐��� : void CBillboard::Draw(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �ʏ�̃r���{�[�h�`�揈���B
******************************************************************************/
void CBillboard::Draw(void)
{
	RendererBegin();

	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�s�񎮂Ɏg����Ɨp�ϐ�
	D3DXMATRIX mtxScl, mtxRot, mtxTrans, mtxView;

	//���[���h�}�g���N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	pDevice->GetTransform(D3DTS_VIEW, &mtxView);		//	�r���[�}�g���N�X�����߂�

	if (m_bY_Axis_Fixed) {	//	Y���Œ�̃r���{�[�h�B
		//	Y��������]�����鏈��////////////////////////////////////////////////////////////////////////
		D3DXMATRIX matBillBoard;
		D3DXVECTOR3 vDir = CCamera::GetVecRV();

		if (vDir.x >= 0.0f) {
			D3DXMatrixRotationY(&matBillBoard, -atanf(vDir.z / vDir.x) + D3DX_PI / 2.0f);
		}
		else {
			D3DXMatrixRotationY(&matBillBoard, -atanf(vDir.z / vDir.x) - D3DX_PI / 2.0f);
		}

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &matBillBoard);
	}
	else {		//	�ʏ�̃r���{�[�h�B
		D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);		//	�t�s������߂�

		//	���W������
		m_mtxWorld._41 = 0.0f;
		m_mtxWorld._42 = 0.0f;
		m_mtxWorld._43 = 0.0f;
	}

	//	�X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, m_Scl.x, m_Scl.y, m_Scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);		
	//	��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);		
	//	�ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);	

	//	���[���h�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//	���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

	//	���_���W�ӂ��[�܂���
	pDevice->SetFVF(FVF_VERTEX_3D);
	//	�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CTexture::GetTexture(m_SceneType, m_TextureType));
	//	�|���S���`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	RendererEnd();
}