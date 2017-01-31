/******************************************************************************
	�^�C�g���� : CDustGenerater�N���X
	�t�@�C���� : dustGenerater.cpp
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
#include "dustGenerate.h"
#include "MeshField.h"
#include "Mode.h"
#include "game.h"

/******************************************************************************
	�֐��� : CDustGenerater::CDustGenerater()
	����   : �R���X�g���N�^
******************************************************************************/
CDustGenerater::CDustGenerater()
{
	m_DustNum = 0;
}

/******************************************************************************
	�֐��� : CDustGenerater::~CDustGenerater()
	����   : �f�X�g���N�^
******************************************************************************/
CDustGenerater::~CDustGenerater()
{
}

/******************************************************************************
	�֐��� : void CDustGenerater::Init(int num)
	����   : void
	�߂�l : �Ȃ�
	����   : �����𐶐��B
******************************************************************************/
void CDustGenerater::Init(int num)
{
	// ���ꂼ��̃|�C���^�ƒl���擾
	CManager *pManager = GetManager();
	CGame *pGame = (CGame *)CManager::GetMode();
	CMeshField *pMeshField = pGame->GetMeshField();

	D3DXVECTOR3 setPos;
	float setSize;
	//	�����擾�B
	m_DustNum = num;
	//	���������𐶐��B
	for (int i = 0; i < m_DustNum; i++) {
		setPos.x = rand() % (int)pMeshField->GetFieldWidth();
		setPos.y = 0.0f;
		setPos.z = rand() % (int)(pMeshField->GetFieldDepth() * 2.0f) - (pMeshField->GetFieldDepth());
		setSize = rand() % 100 + 35.0f;

		CDesertDust::Create(setPos, setSize, setSize, CTexture::G_DUST);
	}
}

/******************************************************************************
	�֐��� : CDustGenerater * CDustGenerater::Create(int num)
	����   : void
	�߂�l : �Ȃ�
	����   : �����֐��B
******************************************************************************/
CDustGenerater * CDustGenerater::Create(int num)
{
	CDustGenerater *obj = new CDustGenerater;
	obj->Init(num);
	return obj;
}
