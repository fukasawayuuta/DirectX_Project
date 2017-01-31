/******************************************************************************
	�^�C�g���� : CDesertDust�N���X
	�t�@�C���� : desertDust.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2017/01/19
	�X�V��     : 
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#pragma once

/******************************************************************************
	�N���X�錾
******************************************************************************/
class CMeshField;

class CDesertDust : public CBillboard
{
public:
	CDesertDust(int Priority = OBJECT_BILLBOARD, OBJ_TYPE objType = OBJ_TYPE_DUST, SCENE_TYPE sceneType = SCENE_TYPE_GAME);		//	�R���X�g���N�^�B
	~CDesertDust();			//	�f�X�g���N�^�B

	void Update(void);		//	�X�V�֐��B
	static void Create(D3DXVECTOR3 pos, float width, float height, int textureType);		//	�����֐��B

	void RendererBegin(void);		//	�`��O�ݒ�֐��B
	void RendererEnd(void);			//	�`���ݒ�֐��B

private:
	D3DXVECTOR3 m_movePos;		//	�ړ��ʁB
	float m_MoveSpeed;			//	�ړ��ʁB

	void Reset(CMeshField *mesh);		//	���Z�b�g�֐��B
	void CheckOutsideRange(CMeshField *mesh);		//	�͈͊O�`�F�b�N�B
};

