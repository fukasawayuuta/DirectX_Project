/******************************************************************************
	�^�C�g���� : Explosion�N���X
	�t�@�C���� : Explosion.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2017/01/06
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
#include "Explosion.h"

/******************************************************************************
	�}�N����`
******************************************************************************/
const int TEXTURE_UPDATE_TIMING_COUNT = 2;		//	�e�N�X�`���A�j���[�V�������X�V����^�C�~���O�J�E���^�B

/******************************************************************************
	�֐��� : CExplosion::CExplosion(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CBillboard(Priority, objType, sceneType)
	����   : �R���X�g���N�^
******************************************************************************/
CExplosion::CExplosion(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CBillboard(Priority, objType, sceneType)
{
}

/******************************************************************************
	�֐��� : CExplosion::~CExplosion()
	����   : �f�X�g���N�^
******************************************************************************/
CExplosion::~CExplosion()
{
}

/******************************************************************************
	�֐��� : void CExplosion::Init(D3DXVECTOR3 pos, float width, float height, float depth, int textureType)
	����   : pos, width, height, depth, textureType
	�߂�l : �Ȃ�
	����   : ���W�A�p�x�A�e�N�X�`���[�̏������B
******************************************************************************/
void CExplosion::Init(D3DXVECTOR3 pos, float width, float height, int textureType)
{
	//	�������B
	CBillboard::Init(pos, width, height, 0.0f, textureType);
	float rotation = (rand() % 628) / 100.0f;

	CManager::CheckRot(&rotation);
	m_Rot.z = rotation;

	//	�A�j���[�V�����̕ϐ��ݒ�B
	m_AnimationCountX_Axis = 8;
	m_AnimationCountY_Axis = 6;
	m_AnimationTotalNumber = m_AnimationCountX_Axis * m_AnimationCountY_Axis;
}

/******************************************************************************
	�֐��� : void CExplosion::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �A�j���[�V�����̍X�V�B
******************************************************************************/
void CExplosion::Update(void)
{
	//	�J�E���^�[��0��������p�^�[���X�V�B
	if (m_AnimationUpdateUVCnt == 0) {
		++m_AnimationPatternCnt;
	}

	//	�p�^�[�����A�j���[�V�����̐��𒴂�����폜�B
	if (m_AnimationPatternCnt >= m_AnimationTotalNumber) {
		m_Delete = true;
		return;
	}

	//	�e�N�X�`����UV���W�X�V�B
	CScene3D::TextureUV_Update(m_pVtxBuffPolygon, m_AnimationPatternCnt, m_AnimationCountX_Axis, m_AnimationCountY_Axis);
	//	�J�E���^�[�X�V�B
	m_AnimationUpdateUVCnt = (m_AnimationUpdateUVCnt + 1) % TEXTURE_UPDATE_TIMING_COUNT;
}

/******************************************************************************
	�֐��� : void CExplosion::Draw(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �`��֐��B
******************************************************************************/
void CExplosion::Draw(void)
{
	RendererBegin();

	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�s�񎮂Ɏg����Ɨp�ϐ�
	D3DXMATRIX mtxScl, mtxRot, mtxTrans, mtxView, invMatrix;

	//���[���h�}�g���N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXMatrixScaling(&mtxScl, m_Scl.x, m_Scl.y, m_Scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);		//�X�P�[���𔽉f

	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);		//��]�𔽉f

	pDevice->GetTransform(D3DTS_VIEW, &mtxView);		//	�r���[�}�g���N�X�����߂�

	D3DXMatrixInverse(&invMatrix, NULL, &mtxView);		//	�t�s������߂�
	//	���W������
	invMatrix._41 = 0.0f;
	invMatrix._42 = 0.0f;
	invMatrix._43 = 0.0f;

	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&invMatrix, &invMatrix, &mtxTrans);	//�ړ��𔽉f

	//	���W�p�̃}�g���N�X�Ƃ��Ƃ��Ƃ̃}�g���N�X���|�����킹��B
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &invMatrix);

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
	�֐��� : void CExplosion::RendererBegin(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �`�悷��ۂ̃����_���[�̐ݒ�B
******************************************************************************/
void CExplosion::RendererBegin(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//	�A���t�@�u�����h�ݒ�////////////////////////////////////////////////////
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//�`���ƕ`�挳�̒l�����Z
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	///////////////////////////////////////////////////////////////////////////
}

/******************************************************************************
	�֐��� : void CExplosion::RendererEnd(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �`�悵����̃����_���[�̐ݒ�B
******************************************************************************/
void CExplosion::RendererEnd(void)
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
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	///////////////////////////////////////////////////////////////////////////
}

/******************************************************************************
	�֐��� : void CExplosion::Create(D3DXVECTOR3 pos, float width, float height, int textureType)
	����   : pos, width, height, textureType
	�߂�l : �Ȃ�
	����   : �����֐��B
******************************************************************************/
void CExplosion::Create(D3DXVECTOR3 pos, float width, float height, int textureType)
{
	CExplosion *obj = new CExplosion;
	obj->Init(pos, width, height, textureType);
}
