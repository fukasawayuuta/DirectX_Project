/******************************************************************************
	�^�C�g�����F�e�N�X�`���N���X
	�t�@�C�����Ftexture.h
	�쐬��    �FAT-13C-284 35 �[�V�C��
	�쐬��     : 2016/11/25
	�X�V��     :
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#pragma once

/******************************************************************************
	�N���X�錾
******************************************************************************/
class CTexture
{
public:
	CTexture();			//	�R���X�g���N�^�B
	~CTexture();		//	�f�X�g���N�^�B

	//	scene�̗񋓌^�B
	typedef enum {
		TITLE = 0,		//	�^�C�g���B
		GAME,			//	�Q�[���B
		RESULT,			//	���U���g�B
		SCENE_MAX
	};

	//	�^�C�g���Ŏg���e�N�X�`��
	typedef enum {
		T_BACKGROUND = 0,	//	�^�C�g���̔w�i�摜�B
		//T_PRESS_ENTER,		//	Press_Enter �̉摜�B
		T_MAX				//	�^�C�g���Ŏg���摜�̍ő吔�B
	}TITLE_TEXTURE;

	//	�Q�[���Ŏg���e�N�X�`��
	typedef enum {
		G_BACKGROUND = 0,	//	�Q�[���̔w�i�摜�B
		G_FLOOR,			//	���̉摜�B
		G_BULLET,			//	�e�̉摜�B
		G_EXPLOSION,		//	�����̉摜�B
		G_NUMBER,			//	0~9�̐����̉摜�B
		G_SKYBOX,			//	SkyBox�̉摜�B
		G_SMOKE,			//	���̉摜�B
		G_LIFE,				//	���C�t�Ɏg���摜�B
		G_LIVEBAR,			//	���C�t�o�[�Ɏg���摜�B
		G_DUST,				//	�����̉摜�B
		G_BOOST,			//	�u�[�X�g�̉摜�B
		G_MAX				//	�Q�[���Ŏg���摜�̍ő吔�B
	}GAME_TEXTURE;

	//	���U���g�Ŏg���e�N�X�`��
	typedef enum {
		R_BACKGROUND = 0,
		//R_PRESS_ENTER,
		R_MAX
	}RESULT_TEXTURE;

	//	�e�N�X�`���̍\���́B
	typedef struct
	{
		char pass[64];					//	FilePass������ϐ�
		LPDIRECT3DTEXTURE9 pTexture;	//	�e�N�X�`���̃|�C���^������ϐ�
	}TEXTURE;

	static CTexture *Create(int index);	//	�쐬�֐��B
	static void UnLoad(CTexture *texture, int index);		//	����֐��B
	void TextureInit(int index);		//	�����ɉ�����������������֐��B
	void TextureUninit(int index);		//	�����ɉ������j������������֐��B

	static LPDIRECT3DTEXTURE9 GetTexture(int index, int num);	//	�����������Ă���e�N�X�`���̃C���f�N�X��n���āA�t�@�C���|�C���^��Ԃ��Ă��炤�֐�

private:
	void Init_Use_In_Title(void);		//	�^�C�g���Ŏg���e�N�X�`���̏������֐��B
	void Init_Use_In_Game(void);		//	�Q�[���Ŏg���e�N�X�`���̏������֐��B
	void Init_Use_In_Result(void);		//	���U���g�Ŏg���e�N�X�`���̏������֐��B

	void Uninit_Use_In_Title(void);		//	�^�C�g���Ŏg���e�N�X�`���̏I���֐��B
	void Uninit_Use_In_Game(void);		//	�Q�[���Ŏg���e�N�X�`���̏I���֐��B
	void Uninit_Use_In_Result(void);	//	���U���g�Ŏg���e�N�X�`���̏I���֐��B

	void(CTexture::*Init[SCENE_MAX])();		//	�������֐�(�֐��|�C���^)�B
	void(CTexture::*Uninit[SCENE_MAX])();	//	�I���֐�(�֐��|�C���^)�B

	static TEXTURE m_Texture_Title[T_MAX];
	static TEXTURE m_Texture_Game[G_MAX];
	static TEXTURE m_Texture_Result[R_MAX];
};

