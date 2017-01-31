/******************************************************************************
	�^�C�g���� : CAnimetionBoard3D�N���X
	�t�@�C���� : animetionBoard3D.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/12/17
	�X�V��     : 
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#pragma once

/******************************************************************************
	�N���X�錾
******************************************************************************/
class CAnimetionBoard3D : public CBillboard
{
public:
	CAnimetionBoard3D(int Priority = NONE, OBJ_TYPE objType = OBJ_TYPE_NONE, SCENE_TYPE sceneType = SCENE_TYPE_NONE);		//	�R���X�g���N�^�B
	~CAnimetionBoard3D();		//	�f�X�g���N�^�B

protected:
	int m_UpdateUVCnt;		//	UV���W���X�V����^�C�~���O�J�E���^�B
	int m_PatternAnim;		//	�����Ă���A�j���[�V�����̃p�^�[���̔ԍ��B
	int m_AnimMaxX;			//	�e�N�X�`���ɂ���X���W��̃A�j���[�V�����̃p�^�[���̖����B
	int m_AnimMaxY;			//	�e�N�X�`���ɂ���Z���W��̃A�j���[�V�����̃p�^�[���̖����B
	int m_AnimTotalMax;		//	�e�N�X�`���ɂ���g�[�^���̃A�j���[�V�����̐��B

	void TextureUV_Update(void);		//	�A�j���[�V�����X�V�֐��B
};

