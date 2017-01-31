/******************************************************************************
	�^�C�g���� : Scene2D�N���X
	�t�@�C���� : scene2D.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/04/19
	�X�V��     : 2016/05/31
				 2016/05/16
******************************************************************************/
/******************************************************************************
	�C���N���[�h�t�@�C��
******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "scene2D.h"
#include "texture.h"

/******************************************************************************
	�֐��� : CScene2D::CScene2D(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene(Priority, objType, sceneType)
	����   : �R���X�g���N�^
			 �ϐ��̏������B
******************************************************************************/
CScene2D::CScene2D(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene(Priority, objType, sceneType)
{
	m_pVtxBuffPolygon = NULL;
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Width = 0.0f;
	m_Height = 0.0f;
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
	�֐��� : CScene2D::~CScene2D()
	����   : �f�X�g���N�^
			 �N���X���j�����ꂽ�Ƃ��ɌĂ΂�邪�A���g���Ȃ��̂œ��ɋL�q���鎖�Ȃ��B
******************************************************************************/
CScene2D::~CScene2D()
{
}

/******************************************************************************
	�֐��� : void CScene2D::Init(D3DXVECTOR3 pos, float width, float height, int textureType)
	����   : pos, width, height
	�߂�l : �Ȃ�
	����   : �����̒l�����ꂼ�������āA2D�|���S���̐����B
******************************************************************************/
void CScene2D::Init(D3DXVECTOR3 pos, float width, float height, int textureType)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	VERTEX_2D *pVtx;

	//	�ϐ��̑���B
	m_Pos = pos;
	m_Width = width;
	m_Height = height;
	m_TextureType = textureType;

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffPolygon, NULL))) {
		return;
	}

	//���_���ւ̃|�C���^���擾
	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//	���W�ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_Pos.x - (m_Width * 0.5f), m_Pos.y - (m_Height * 0.5f), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_Pos.x + (m_Width * 0.5f), m_Pos.y - (m_Height * 0.5f), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_Pos.x - (m_Width * 0.5f), m_Pos.y + (m_Height * 0.5f), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_Pos.x + (m_Width * 0.5f), m_Pos.y + (m_Height * 0.5f), 0.0f);

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
	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;

	//	�o�b�t�@���
	m_pVtxBuffPolygon->Unlock();
}

/******************************************************************************
	�֐��� : void CScene2D::Uninit(void)
	����   : void
	�߂�l : �Ȃ�
	����   : ���_�o�b�t�@�̉���B
******************************************************************************/
void CScene2D::Uninit(void)
{
	//	���_�o�b�t�@�̉���B
	SAFE_RELEASE(m_pVtxBuffPolygon);
}

/******************************************************************************
	�֐��� : void CScene2D::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �X�V�����B
******************************************************************************/
void CScene2D::Update(void)
{
}

/******************************************************************************
	�֐��� : void CScene2D::Draw(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �`�揈���B�B
******************************************************************************/
void CScene2D::Draw(void)
{
	//	�f�o�C�X�擾
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

	//���_���W�ӂ��[�܂���
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CTexture::GetTexture(m_SceneType, m_TextureType));

	//�|���S���`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

/******************************************************************************
	�֐��� : CScene2D * CScene2D::Create(D3DXVECTOR3 pos, float width, float height)
	����   : pos, width, height
	�߂�l : obj
	����   : �X�V�����B
******************************************************************************/
CScene2D * CScene2D::Create(D3DXVECTOR3 pos, float width, float height, int textureType)
{
	CScene2D *obj = new CScene2D;
	obj->Init(pos, width, height, textureType);
	return obj;
}

/******************************************************************************
	�֐��� : void CScene2D::TextureUV_Update(LPDIRECT3DVERTEXBUFFER9 vtxBuff, int patternCnt, int animContX_Axis, int animCntY_Axis)
	����   : vtxBuff, patternCnt, animContX_Axis, animCntY_Axis
	�߂�l : �Ȃ�
	����   : �e�N�X�`����UV�X�V�֐��B
******************************************************************************/
void CScene2D::TextureUV_Update(LPDIRECT3DVERTEXBUFFER9 vtxBuff, int patternCnt, int animContX_Axis, int animCntY_Axis)
{
	VERTEX_2D *pVtx;

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