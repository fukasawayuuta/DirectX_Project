/******************************************************************************
	�^�C�g���� : Game_BG�N���X
	�t�@�C���� : game_BG.cpp
	�쐬��     : AT-13C-284 35 �[�V�C��
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
#include "game_BG.h"


/******************************************************************************
	�֐��� : CGame_BG::CGame_BG(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene2D(Priority, objType, sceneType)
	����   : �R���X�g���N�^
			 �ϐ��̏������B
******************************************************************************/
CGame_BG::CGame_BG(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene2D(Priority, objType, sceneType)
{
}

/******************************************************************************
	�֐��� : CGame_BG::~CGame_BG()
	����   : �f�X�g���N�^
			 �N���X���j�����ꂽ�Ƃ��ɌĂ΂�邪�A���g���Ȃ��̂œ��ɋL�q���鎖�Ȃ��B
******************************************************************************/
CGame_BG::~CGame_BG()
{
}

/******************************************************************************
	�֐��� : CGame_BG * CGame_BG::Create(D3DXVECTOR3 pos, float width, float height, int textureType)
	����   : pos, width, height, textureType
	�߂�l : obj
	����   : �Q�[����ʂ̔w�i�𐶐�����֐��B
******************************************************************************/
CGame_BG * CGame_BG::Create(D3DXVECTOR3 pos, float width, float height, int textureType)
{
	CGame_BG *obj = new CGame_BG;
	obj->Init(pos, width, height, textureType);
	return obj;
}
