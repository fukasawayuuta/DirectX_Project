/******************************************************************************
	�^�C�g���� : Billboard�N���X
	�t�@�C���� : billboard.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/12/12
	�X�V��     : 
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#pragma once

/******************************************************************************
	�N���X
******************************************************************************/
class CBillboard : public CScene3D
{
public:
	CBillboard(int Priority = NONE, OBJ_TYPE objType = OBJ_TYPE_NONE, SCENE_TYPE sceneType = SCENE_TYPE_NONE);		//	�R���X�g���N�^�B
	~CBillboard();		//	�f�X�g���N�^�B

	void Draw(void);		//	�ʏ�̕`��֐��B

	bool GetAxisFixd(void) { return m_bY_Axis_Fixed; }			//	�t���O�擾�֐��B
	void SetAxisFixd(bool flag) { m_bY_Axis_Fixed = flag; }		//	�t���O�ݒ�֐��B

protected:
	bool m_bY_Axis_Fixed;		//	Y�����Œ肷�邩�ǂ����̃t���O�B
};

