/******************************************************************************
	�^�C�g���� : Boost
	�t�@�C���� : boost.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2017/01/27
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
#include "billboard.h"
#include "texture.h"
#include "boost.h"

/******************************************************************************
	�}�N����`
******************************************************************************/
const int TEXTURE_UPDATE_TIMING_COUNT = 1;		//	�e�N�X�`���A�j���[�V�������X�V����^�C�~���O�J�E���^�B

/******************************************************************************
	�֐��� : CBoost::CBoost(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CBillboard(Priority, objType, sceneType)
	����   : �R���X�g���N�^
******************************************************************************/
CBoost::CBoost(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CBillboard(Priority, objType, sceneType)
{
	m_DuringUse = false;
	m_Magnification_Rate = 0.0f;
}

/******************************************************************************
	�֐��� : CBoost::~CBoost()
	����   : �f�X�g���N�^
******************************************************************************/
CBoost::~CBoost()
{
}

/******************************************************************************
	�֐��� : void CBoost::Init(D3DXVECTOR3 pos, float width, float height, int textureType, CScene * target)
	����   : pos, width, height, textureType, target
	�߂�l : �Ȃ�
	����   : �r���{�[�h�̐ݒ�Ǝg���Ώۂ̐ݒ�B
******************************************************************************/
void CBoost::Init(D3DXVECTOR3 pos, float width, float height, int textureType)
{
	CBillboard::Init(pos, width, height, 0.0f, textureType);
	m_Magnification_Rate = 3.0f;

	//	�A�j���[�V�����̕ϐ��ݒ�B
	m_AnimationCountX_Axis = 8;
	m_AnimationCountY_Axis = 4;
	m_AnimationTotalNumber = m_AnimationCountX_Axis * m_AnimationCountY_Axis;
}

/******************************************************************************
�֐��� : void CBoost::Uninit(void)
����   : void
�߂�l : �Ȃ�
����   : ��������B
******************************************************************************/
void CBoost::Uninit(void)
{
	CBillboard::Uninit();
}

/******************************************************************************
	�֐��� : void CPlayer::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �X�V�����B
******************************************************************************/
void CBoost::Update(void)
{
	if (m_DuringUse == true) {
		m_Scl = D3DXVECTOR3(m_Magnification_Rate, m_Magnification_Rate, m_Magnification_Rate);
		m_DuringUse = false;
	}

	//	�J�E���^�[��0��������p�^�[���X�V�B
	if (m_AnimationUpdateUVCnt == 0) {
		++m_AnimationPatternCnt;
	}

	//	�p�^�[�����A�j���[�V�����̐��𒴂�����폜�B
	if (m_AnimationPatternCnt >= m_AnimationTotalNumber) {
		m_AnimationPatternCnt = 0;
	}

	m_Scl -= D3DXVECTOR3(0.1f, 0.1f, 0.1f);

	if (m_Scl.x < 1.0f) {
		m_Scl.x = 1.0f;
	}

	if (m_Scl.y < 1.0f) {
		m_Scl.y = 1.0f;
	}

	if (m_Scl.z < 1.0f) {
		m_Scl.z = 1.0f;
	}

	//	�e�N�X�`����UV���W�X�V�B
	CScene3D::TextureUV_Update(m_pVtxBuffPolygon, m_AnimationPatternCnt, m_AnimationCountX_Axis, m_AnimationCountY_Axis);
	//	�J�E���^�[�X�V�B
	m_AnimationUpdateUVCnt = (m_AnimationUpdateUVCnt + 1) % TEXTURE_UPDATE_TIMING_COUNT;
}

/******************************************************************************
	�֐��� : void CBoost::RendererBegin(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �`�悷��ۂ̃����_���[�̐ݒ�B
******************************************************************************/
void CBoost::RendererBegin(void)
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
	�֐��� : void CBoost::RendererEnd(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �`�悵����̃����_���[�̐ݒ�B
******************************************************************************/
void CBoost::RendererEnd(void)
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
	�֐��� : CBoost * CBoost::Create(D3DXVECTOR3 pos, float width, float height, int textureType, CScene * target)
	����   :pos, width, height, textureType, target
	�߂�l : obj
	����   : �����֐��B
******************************************************************************/
CBoost * CBoost::Create(D3DXVECTOR3 pos, float width, float height, int textureType)
{
	CBoost *obj = new CBoost;
	obj->Init(pos, width, height, textureType);

	return obj;
}
