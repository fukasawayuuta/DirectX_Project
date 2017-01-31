/******************************************************************************
	�^�C�g���� :�V�[��X�N���X
	�t�@�C���� : sceneX.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/12/13
	�X�V��     :
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#pragma once

/******************************************************************************
	�N���X
******************************************************************************/
class CSceneX : public CScene3D
{
public:
	CSceneX(int Priority = NONE, OBJ_TYPE objType = OBJ_TYPE_NONE, SCENE_TYPE sceneType = SCENE_TYPE_NONE);		//	�R���X�g���N�^�B
	~CSceneX();		//	�f�X�g���N�^�B

	typedef enum {
		DYNAMIC_MODEL_TYPE_PLAYER = 0,		//	player
		DYNAMIC_MODEL_TYPE_ENEMY001,		//	enemy001
		DYNAMIC_MODEL_TYPE_ENEMY002,		//	enemy002	
		DYNAMIC_MODEL_TYPE_MAX				//	type_max
	}DYNAMIC_MODEL_TYPE;

	virtual void Init(void)   = 0;		//	�������֐��B
	virtual void Uninit(void) = 0;		//	�I���֐��B
	virtual void Update(void) = 0;		//	�X�V�֐��B
	virtual void Draw(void)   = 0;		//	�`��֐��B
};

