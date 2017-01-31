/******************************************************************************
	�^�C�g���� : Result_BG�N���X
	�t�@�C���� : result_BG.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/12/16
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
#include "texture.h"
#include "result_BG.h"


/******************************************************************************
	�֐��� : CResult_BG::CResult_BG(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene2D(Priority, objType, sceneType)
	����   : �R���X�g���N�^
			 �ϐ��̏������B
******************************************************************************/
CResult_BG::CResult_BG(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene2D(Priority, objType, sceneType)
{
}

/******************************************************************************
	�֐��� : CResult_BG::~CResult_BG()
	����   : �f�X�g���N�^
			 �N���X���j�����ꂽ�Ƃ��ɌĂ΂�邪�A���g���Ȃ��̂œ��ɋL�q���鎖�Ȃ��B
******************************************************************************/
CResult_BG::~CResult_BG()
{
}

/******************************************************************************
	�֐��� : CResult_BG * CResult_BG::Create(D3DXVECTOR3 pos, float width, float height, int textureType)
	����   : pos, width, height, textureType
	�߂�l : obj
	����   :���U���g��ʂ̔w�i�𐶐�����֐��B
******************************************************************************/
CResult_BG * CResult_BG::Create(D3DXVECTOR3 pos, float width, float height, int textureType)
{
	CResult_BG *obj = new CResult_BG;
	obj->Init(pos, width, height, textureType);
	return obj;
}
