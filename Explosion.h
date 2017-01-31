/******************************************************************************
	�^�C�g���� : Explosion�N���X
	�t�@�C���� : Explosion.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2017/01/06
	�X�V��     : 
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#pragma once

/******************************************************************************
	�N���X�錾
******************************************************************************/
class CExplosion : public CBillboard
{
public:
	CExplosion(int Priority = OBJECT_BILLBOARD, OBJ_TYPE objType = OBJ_TYPE_EXPLOSION, SCENE_TYPE sceneType = SCENE_TYPE_GAME);		//	�R���X�g���N�^�B
	~CExplosion();		//	�f�X�g���N�^�B

	void Init(D3DXVECTOR3 pos, float width, float height, int textureType);		//	�������֐��B
	void Update(void);		//	�X�V�֐��B
	void Draw(void);		//	�`��֐��B

	void RendererBegin(void);		//	�`��O�ݒ�֐��B
	void RendererEnd(void);			//	�`���ݒ�֐��B

	static void Create(D3DXVECTOR3 pos, float width, float height, int textureType);		//	�����֐��B
};

