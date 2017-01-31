/******************************************************************************
	�t�@�C���� : stencilShadow.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2017/01/28
	�X�V��     :
******************************************************************************/
/******************************************************************************
	�C���N���[�h�t�@�C��
******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "stencilShadow.h"


/******************************************************************************
	�֐��� : CStencilShadow::CStencilShadow(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene3D(Priority, objType, sceneType)
	����   : �R���X�g���N�^�B
******************************************************************************/
CStencilShadow::CStencilShadow(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene3D(Priority, objType, sceneType)
{
	m_Target = NULL;
	m_pMesh = NULL;
	m_pBuffMat = NULL;
	m_pVtxBuffPolygon = NULL;
	m_NumMat = 0;
}

/******************************************************************************
	�֐��� : CStencilShadow::~CStencilShadow()
	����   : �f�X�g���N�^�B
******************************************************************************/
CStencilShadow::~CStencilShadow()
{
}

/******************************************************************************
	�֐��� : void CStencilShadow::Init(CScene * target)
	����   : target
	�߂�l : �Ȃ�
	����   : 2D�|���S���쐬�B�V�����_�[�̃��f���ǂݍ��݁B
******************************************************************************/
void CStencilShadow::Init(CScene * target)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	VERTEX_2D *pVtx;

	m_Target = target;

	if (FAILED(D3DXLoadMeshFromX("data/MODEL/stencil_shadow.x",		//	�t�@�C��������ꏊ
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,	//	�}�e���A�����
		NULL,
		&m_NumMat,		//	�}�e���A���̐�
		&m_pMesh))) {	//	���b�V�����
		return;
	}

	//	���_�o�b�t�@�̐���(2D)
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffPolygon, NULL))) {
		return;
	}

	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//�e�N�X�`�����W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//	�F�ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(0, 0, 0, 128);
	pVtx[1].col = D3DCOLOR_RGBA(0, 0, 0, 128);
	pVtx[2].col = D3DCOLOR_RGBA(0, 0, 0, 128);
	pVtx[3].col = D3DCOLOR_RGBA(0, 0, 0, 128);

	m_pVtxBuffPolygon->Unlock();
}

/******************************************************************************
	�֐��� : void CStencilShadow::Uninit(void)
	����   : void
	�߂�l : �Ȃ�
	����   : ��������B
******************************************************************************/
void CStencilShadow::Uninit(void)
{
	m_Target = NULL;
	if (m_pMesh != NULL)
		SAFE_RELEASE(m_pMesh)
	if (m_pBuffMat != NULL)
		SAFE_RELEASE(m_pBuffMat)
	if (m_pVtxBuffPolygon != NULL) 
		SAFE_RELEASE(m_pVtxBuffPolygon);
}

/******************************************************************************
	�֐��� : void CStencilShadow::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �V�����_�[�̍��W�X�V�B
******************************************************************************/
void CStencilShadow::Update(void)
{
	m_Pos = m_Target->GetPos();
	//m_Pos.y = 0.0f;
}

/******************************************************************************
	�֐��� : void CStencilShadow::Draw(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �`�揈��
******************************************************************************/
void CStencilShadow::Draw(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	//	�s�񎮂Ɏg����Ɨp�ϐ�
	D3DXMATRIX mtxScl, mtxRot, mtxTrans;
	D3DXMATERIAL *pMat;
	D3DMATERIAL9 matDef;		//	�ޔ�p�}�e���A��

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

	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);	//	�t�H�O�FOFF
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		//	���C�e�B���O
	pDevice->SetTexture(0, NULL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0);	//	�F���͏����Ȃ�
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);		//	��r�p�̂P: OFF

	/* -- �~���\ -- */
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);	//	INCR = �C���N�������g�̗���1�ŕ`��
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	for (int nCntMat = 0; nCntMat < m_NumMat; nCntMat++) {
		m_pMesh->DrawSubset(nCntMat);
	}
	/* -- �~���\ -- */
	/* -- �~���� -- */
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_DECR);	//	DECR = �f�N�������g�̗���-1�ŕ`��
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	for (int nCntMat = 0; nCntMat < m_NumMat; nCntMat++) {
		m_pMesh->DrawSubset(nCntMat);
	}
	/* -- �~���� -- */

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xf);
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	/* -- �S���2D�|���S���`�� -- */
	//���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));
	//���_���W�ӂ��[�܂���
	pDevice->SetFVF(FVF_VERTEX_2D);
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);
	//�|���S���`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);		//�t�H�O�FON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);		//	���C�e�B���O : OFF
}

/******************************************************************************
	�֐��� : void CStencilShadow::Create(CScene * target)
	����   : target
	�߂�l : �Ȃ�
	����   : �����֐��B
******************************************************************************/
void CStencilShadow::Create(CScene * target)
{
	CStencilShadow *obj = new CStencilShadow;
	obj->Init(target);
}
