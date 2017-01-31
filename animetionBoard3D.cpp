/******************************************************************************
	�^�C�g���� : CAnimetionBoard3D�N���X
	�t�@�C���� : animetionBoard3D.cpp
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
#include "camera.h"
#include "billboard.h"
#include "animetionBoard3D.h"


/******************************************************************************
	�֐��� : CAnimetionBoard3D::CAnimetionBoard3D(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CBillboard(Priority, objType, sceneType)
	����   : �R���X�g���N�^
			 �ϐ��̏������B
******************************************************************************/
CAnimetionBoard3D::CAnimetionBoard3D(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CBillboard(Priority, objType, sceneType)
{
	m_UpdateUVCnt = 0;
	m_PatternAnim = 0;
	m_AnimMaxX = 0;
	m_AnimMaxY = 0;
	m_AnimTotalMax = 0;
}

/******************************************************************************
	�֐��� : CAnimetionBoard3D::~CAnimetionBoard3D()
	����   : �f�X�g���N�^
			 �N���X���j�����ꂽ�Ƃ��ɌĂ΂�邪�A���g���Ȃ��̂œ��ɋL�q���鎖�Ȃ��B
******************************************************************************/
CAnimetionBoard3D::~CAnimetionBoard3D()
{
}

/******************************************************************************
	�֐��� :void CAnimetionBoard3D::TextureUV_Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �A�j���[�V�����̏����B
******************************************************************************/
void CAnimetionBoard3D::TextureUV_Update(void)
{
	VERTEX_3D *pVtx;

	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//	�e�N�X�`�����W�X�V
	pVtx[0].tex.x = (m_PatternAnim % m_AnimMaxX) * (1.0f / m_AnimMaxX);
	pVtx[0].tex.y = ((m_PatternAnim - (m_PatternAnim % m_AnimMaxX)) / m_AnimMaxX) * ((float)1 / m_AnimMaxY);

	pVtx[1].tex.x = (m_PatternAnim % m_AnimMaxX) * (1.0f / m_AnimMaxX) + (1.0f / m_AnimMaxX);
	pVtx[1].tex.y = ((m_PatternAnim - (m_PatternAnim % m_AnimMaxX)) / m_AnimMaxX) * ((float)1 / m_AnimMaxY);

	pVtx[2].tex.x = (m_PatternAnim % m_AnimMaxX) * (1.0f / m_AnimMaxX);
	pVtx[2].tex.y = (((m_PatternAnim - (m_PatternAnim % m_AnimMaxX)) / m_AnimMaxX) * ((float)1 / m_AnimMaxY)) + ((float)1 / m_AnimMaxY);

	pVtx[3].tex.x = (m_PatternAnim % m_AnimMaxX) * (1.0f / m_AnimMaxX) + (1.0f / m_AnimMaxX);
	pVtx[3].tex.y = (((m_PatternAnim - (m_PatternAnim % m_AnimMaxX)) / m_AnimMaxX) * ((float)1 / m_AnimMaxY)) + ((float)1 / m_AnimMaxY);

	m_pVtxBuffPolygon->Unlock();
}
