/******************************************************************************
	�^�C�g���� : CDynamicModel�N���X
	�t�@�C���� : dynamicModel.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/12/14
	�X�V��     : 
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#pragma once

/******************************************************************************
	�N���X
******************************************************************************/
class CMotionManager;

class CDynamicModel : public CSceneX
{
public:
	CDynamicModel(int Priority = NONE, OBJ_TYPE objType = OBJ_TYPE_NONE, SCENE_TYPE sceneType = SCENE_TYPE_NONE);		//	�R���X�g���N�^�B
	~CDynamicModel();		//	�f�X�g���N�^�B

	virtual void Init(void) {};		//	�������֐��B
	virtual void Uninit(void) = 0;		//	�I���֐��B
	virtual void Update(void) = 0;		//	�X�V�֐��B
	virtual void Draw(void) = 0;		//	�`��֐��B

protected:
	CMotionManager *m_MotionManager;		//	���[�V�������邽�߂̕ϐ��B
	DYNAMIC_MODEL_TYPE m_DynamicModelType;		//	�g�����[�V�����ƃ��f���̃^�C�v�B
};

