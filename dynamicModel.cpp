/******************************************************************************
	�^�C�g���� : DynamicModel�N���X
	�t�@�C���� : dynamicModel.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/12/13
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
#include "sceneX.h"
#include "model.h"
#include "motionManager.h"
#include "dynamicModel.h"


/******************************************************************************
	�֐��� : CDynamicModel::CDynamicModel(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CSceneX(Priority, objType, sceneType)
	����   : �R���X�g���N�^
			 �ϐ��̏������B
******************************************************************************/
CDynamicModel::CDynamicModel(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CSceneX(Priority, objType, sceneType)
{
}

/******************************************************************************
	�֐��� : CDynamicModel::~CDynamicModel()
	����   : �f�X�g���N�^
			 �N���X���j�����ꂽ�Ƃ��ɌĂ΂�邪�A���g���Ȃ��̂œ��ɋL�q���鎖�Ȃ��B
******************************************************************************/
CDynamicModel::~CDynamicModel()
{
}
