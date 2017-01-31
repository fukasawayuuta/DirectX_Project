/******************************************************************************
	�^�C�g���� : Title_BG�N���X
	�t�@�C���� : Title_BG.cpp
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
#include "title_BG.h"


/******************************************************************************
	�֐��� : CTitle_BG::CTitle_BG(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene2D(Priority, objType, sceneType)
	����   : �R���X�g���N�^
			 �ϐ��̏������B
******************************************************************************/
CTitle_BG::CTitle_BG(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene2D(Priority, objType, sceneType)
{
}

/******************************************************************************
	�֐��� : CTitle_BG::~CTitle_BG()
	����   : �f�X�g���N�^
			 �N���X���j�����ꂽ�Ƃ��ɌĂ΂�邪�A���g���Ȃ��̂œ��ɋL�q���鎖�Ȃ��B
******************************************************************************/
CTitle_BG::~CTitle_BG()
{
}

/******************************************************************************
	�֐��� : CTitle_BG * CTitle_BG::Create(D3DXVECTOR3 pos, float width, float height, int textureType)
	����   : pos, width, height, textureType
	�߂�l : obj
	����   : �^�C�g����ʂ̔w�i�𐶐�����֐��B
******************************************************************************/
CTitle_BG * CTitle_BG::Create(D3DXVECTOR3 pos, float width, float height, int textureType)
{
	CTitle_BG *obj = new CTitle_BG;
	obj->Init(pos, width, height, textureType);
	return obj;
}
