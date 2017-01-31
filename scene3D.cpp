/******************************************************************************
	�^�C�g���� : Scene3D�N���X
	�t�@�C���� : scene3D.cpp
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

/******************************************************************************
	�֐��� : CScene3D::CScene3D(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene(Priority, objType, sceneType)
	����   : �R���X�g���N�^
			 �ϐ��̏������B
******************************************************************************/
CScene3D::CScene3D(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene(Priority, objType, sceneType)
{
	m_pVtxBuffPolygon = NULL;
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Width = 0.0f;
	m_Height = 0.0f;
	m_Depth = 0.0f;
	m_Length = 0.0f;
	m_Angle = 0.0f;
	m_Delete = false;
	m_Active = true;
	m_TextureType = 0;

	m_AnimationPatternCnt = 0;
	m_AnimationTotalNumber = 0;
	m_AnimationCountX_Axis = 0;
	m_AnimationCountY_Axis = 0;
	m_AnimationUpdateUVCnt = 0;
}

/******************************************************************************
	�֐��� : CScene3D::~CScene3D()
	����   : �f�X�g���N�^
			 �N���X���j�����ꂽ�Ƃ��ɌĂ΂�邪�A���g���Ȃ��̂œ��ɋL�q���鎖�Ȃ��B
******************************************************************************/
CScene3D::~CScene3D()
{
}

/******************************************************************************
	�֐��� : void CScene2D::Init(D3DXVECTOR3 pos, float width, float height, float depth, int textureType)
	����   : pos, width, height
	�߂�l : �Ȃ�
	����   : �����̒l�����ꂼ�������āA2D�|���S���̐����B
******************************************************************************/
void CScene3D::Init(D3DXVECTOR3 pos, float width, float height, float depth, int textureType)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	VERTEX_3D *pVtx;

	//	�����ݒ�B
	m_Pos = pos;
	m_Width = width;
	m_Height = height;
	m_Depth = depth;
	m_TextureType = textureType;

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffPolygon, NULL))) {
		return;
	}

	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//	���W�ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f - (m_Width * 0.5f), 0.0f + (m_Height * 0.5f), 0.0f + (m_Depth * 0.5f));
	pVtx[1].pos = D3DXVECTOR3(0.0f + (m_Width * 0.5f), 0.0f + (m_Height * 0.5f), 0.0f + (m_Depth * 0.5f));
	pVtx[2].pos = D3DXVECTOR3(0.0f - (m_Width * 0.5f), 0.0f - (m_Height * 0.5f), 0.0f - (m_Depth * 0.5f));
	pVtx[3].pos = D3DXVECTOR3(0.0f + (m_Width * 0.5f), 0.0f - (m_Height * 0.5f), 0.0f - (m_Depth * 0.5f));

	//	��Ɨp�ϐ�
	D3DXVECTOR3 vec1 = pVtx[1].pos - pVtx[0].pos;
	D3DXVECTOR3 vec2 = pVtx[2].pos - pVtx[0].pos;

	D3DXVECTOR3 Normal1;
	D3DXVECTOR3 Normal2;
	D3DXVECTOR3 Normal3;
	D3DXVECTOR3 Normal4;

	//	�O�όv�Z
	D3DXVec3Cross(&Normal1, &vec1, &vec2);

	//	�P�ʃx�N�g����
	D3DXVec3Normalize(&Normal1, &Normal1);
	pVtx[0].nor = Normal1;

	//	�x�N�g��
	vec1 = pVtx[1].pos - pVtx[3].pos;
	vec2 = pVtx[2].pos - pVtx[3].pos;

	//	�O�όv�Z
	D3DXVec3Cross(&Normal4, &vec2, &vec1);

	//	�P�ʃx�N�g����
	D3DXVec3Normalize(&Normal4, &Normal4);
	pVtx[3].nor = Normal4;

	//	�P�ʃx�N�g����
	D3DXVec3Normalize(&Normal2, &(Normal1 + Normal4));
	pVtx[1].nor = Normal2;

	//	�P�ʃx�N�g����
	D3DXVec3Normalize(&Normal3, &(Normal1 + Normal4));
	pVtx[2].nor = Normal3;

	//	�F�ݒ�
	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;

	//�e�N�X�`�����W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuffPolygon->Unlock();
}

/******************************************************************************
	�֐��� : void CScene3D::Uninit(void)
	����   : void
	�߂�l : �Ȃ�
	����   : ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̉���B
******************************************************************************/
void CScene3D::Uninit(void)
{
	SAFE_RELEASE(m_pVtxBuffPolygon);
}

/******************************************************************************
	�֐��� : void CScene2D::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �X�V�����B
******************************************************************************/
void CScene3D::Update(void)
{
}

/******************************************************************************
	�֐��� : void CScene3D::Draw(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �`�揈���B
******************************************************************************/
void CScene3D::Draw(void)
{
	RendererBegin();

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


/******************************************************************************
	�֐��� : CScene3D * CScene3D::Create(D3DXVECTOR3 pos, float width, float height, float depth, int textureType)
	����   : pos, width, height, depth, textureType
	�߂�l : obj
	����   : ���������B
******************************************************************************/
CScene3D * CScene3D::Create(D3DXVECTOR3 pos, float width, float height, float depth, int textureType)
{
	CScene3D *obj = new CScene3D;
	obj->Init(pos, width, height, depth, textureType);
	return obj;
}

/******************************************************************************
	�֐��� : void CScene3D::TextureUV_Update(LPDIRECT3DVERTEXBUFFER9 vtxBuff, int patternCnt, int animContX_Axis, int animCntY_Axis)
	����   : vtxBuff, patternCnt, animContX_Axis, animCntY_Axis
	�߂�l : �Ȃ�
	����   : �e�N�X�`����UV�X�V�֐��B
******************************************************************************/
void CScene3D::TextureUV_Update(LPDIRECT3DVERTEXBUFFER9 vtxBuff, int patternCnt, int animContX_Axis, int animCntY_Axis)
{
	VERTEX_3D *pVtx;

	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//	�e�N�X�`�����W�X�V
	pVtx[0].tex.x = (patternCnt % animContX_Axis) * (1.0f / animContX_Axis);
	pVtx[0].tex.y = ((patternCnt - (patternCnt % animContX_Axis)) / animContX_Axis) * ((float)1 / animCntY_Axis);

	pVtx[1].tex.x = (patternCnt % animContX_Axis) * (1.0f / animContX_Axis) + (1.0f / animContX_Axis);
	pVtx[1].tex.y = ((patternCnt - (patternCnt % animContX_Axis)) / animContX_Axis) * ((float)1 / animCntY_Axis);

	pVtx[2].tex.x = (patternCnt % animContX_Axis) * (1.0f / animContX_Axis);
	pVtx[2].tex.y = (((patternCnt - (patternCnt % animContX_Axis)) / animContX_Axis) * ((float)1 / animCntY_Axis)) + ((float)1 / animCntY_Axis);

	pVtx[3].tex.x = (patternCnt % animContX_Axis) * (1.0f / animContX_Axis) + (1.0f / animContX_Axis);
	pVtx[3].tex.y = (((patternCnt - (patternCnt % animContX_Axis)) / animContX_Axis) * ((float)1 / animCntY_Axis)) + ((float)1 / animCntY_Axis);

	vtxBuff->Unlock();
}
