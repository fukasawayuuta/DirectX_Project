/******************************************************************************
	�^�C�g���� : CSmoke�N���X
	�t�@�C���� : smoke.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2017/01/18
	�X�V��     : 
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#pragma once

/******************************************************************************
	�N���X�錾
******************************************************************************/
class CSmoke : public CBillboard
{
public:
	CSmoke(int Priority = OBJECT_BILLBOARD, OBJ_TYPE objType = OBJ_TYPE_EXPLOSION, SCENE_TYPE sceneType = SCENE_TYPE_GAME);
	~CSmoke();

	void Init(D3DXVECTOR3 pos, float rot, float width, float height, int textureType);		//	�������֐��B
	void Update(void);			//	�X�V�֐��B
	void Draw(void);

	void RendererBegin(void);		//	�`��O�ݒ�֐��B
	void RendererEnd(void);			//	�`���ݒ�֐��B

	static void Create(D3DXVECTOR3 pos, float rot, float width, float height, int textureType);		//	�����֐��B

private:
	int m_UpdateTimeCnt;		//	�e�N�X�`���A�j���[�V�������X�V����^�C�~���O�J�E���^�B
};

