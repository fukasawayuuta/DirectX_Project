/******************************************************************************
	�^�C�g���� : �v���C���[���C�t
	�t�@�C���� : playerLife.h
	�쐬��     : AT-13C-284 35 �[�V�C��
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
class CPlayerLife : public CScene2D
{
public:
	CPlayerLife(int Priority = OBJECT_2D, OBJ_TYPE objType = OBJ_TYPE_UI, SCENE_TYPE sceneType = SCENE_TYPE_GAME);		//	�R���X�g���N�^�B
	~CPlayerLife();			//	�f�X�g���N�^�B

	void Init(D3DXVECTOR3 pos, float width, float height, int lifeTex, int barTex, int hp);		//	�������֐��B
	void Update(CPlayer *player);		//	�X�V�֐��B
	void Draw(void);		//	�`��֐��B

	static CPlayerLife *Create(D3DXVECTOR3 pos, float width, float height, int lifeTex, int barTex, int hp);		//	�����֐��B

private:
	typedef enum {
		TEX_LIFE = 0,		//	���C�t�̃e�N�X�`���B
		TEX_LIFE_BAR,		//	���C�t�o�[�̃e�N�X�`���B
		TEX_MAX				//	�ő吔�B
	};

	int m_PlayerMaxHP;				//	�v���C���[�̍ő�HP�B
	int m_LifeTexture[TEX_MAX];		//	�g���e�N�X�`����ݒ肷��ϐ��B
	float m_MoveWidth;				//	���̈ړ��ʁB
	float m_FixedWidth;				//	�Œ蕝�B
};

