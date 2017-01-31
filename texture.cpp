/******************************************************************************
	�^�C�g�����F�e�N�X�`���N���X
	�t�@�C�����Ftexture.cpp
	�쐬��    �FAT-13C-284 35 �[�V�C��
	�쐬��     : 2016/11/25
	�X�V��     :
******************************************************************************/
/******************************************************************************
	�C���N���[�h�t�@�C��
******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

/******************************************************************************
	�ÓI�����o�ϐ�
******************************************************************************/
//	�^�C�g����ʂŎg���e�N�X�`���̈ꗗ
CTexture::TEXTURE CTexture::m_Texture_Title[T_MAX] = {
	{"data/texture/title_BG001.jpg"}		//	2D�w�i�摜�B
};

//	�Q�[����ʂŎg���e�N�X�`���̈ꗗ�B
CTexture::TEXTURE CTexture::m_Texture_Game[G_MAX] = {
	{"data/texture/game_BG001.jpg"},		//	2D�w�i�摜�B
	{"data/texture/ground002.jpg"},			//	�n�ʂ̉摜�B
	{"data/texture/block.png"},				//	�e�̉摜
	{"data/texture/explosion002.png"},		//	�����̉摜�B
	{"data/texture/number000.png"},			//	�����̉摜�B
	{"data/texture/skybox004.png"},			//	skybox�̉摜�B
	{"data/texture/smoke001.png"},			//	���̉摜�B
	{"data/texture/player_hp.png"},			//	���C�t�̉摜�B
	{"data/texture/player_hp_var.png"},		//	���C�t�o�[�̉摜�B
	{"data/texture/Dust.png"},				//	�����̉摜�B
	{"data/texture/boost.png"}				//	�u�[�X�g�̉摜�B
};

//	���U���g��ʂŎg���e�N�X�`���̈ꗗ�B
CTexture::TEXTURE CTexture::m_Texture_Result[R_MAX] = {
	{"data/texture/result_BG001.jpg"}		//	2D�w�i�摜�B
};

/******************************************************************************
	�֐��� : CTexture::CTexture()
	����   : �֐��|�C���^��ݒ�B
******************************************************************************/
CTexture::CTexture()
{
	//	�֐��|�C���^�ݒ�B
	Init[TITLE] = &CTexture::Init_Use_In_Title;
	Init[GAME] = &CTexture::Init_Use_In_Game;
	Init[RESULT] = &CTexture::Init_Use_In_Result;
	//	�֐��|�C���^�ݒ�B
	Uninit[TITLE] = &CTexture::Uninit_Use_In_Title;
	Uninit[GAME] = &CTexture::Uninit_Use_In_Game;
	Uninit[RESULT] = &CTexture::Uninit_Use_In_Result;
}

/******************************************************************************
	�֐��� : CTexture::~CTexture()
	����   : �f�X�g���N�^�B
******************************************************************************/
CTexture::~CTexture()
{
}

/******************************************************************************
	�֐��� : void CTexture::Init_Use_In_Title(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �^�C�g���Ŏg���e�N�X�`���̏������B
******************************************************************************/
void CTexture::Init_Use_In_Title(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	//	������
	for (int i = 0; i < T_MAX; i++) {
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, m_Texture_Title[i].pass, &m_Texture_Title[i].pTexture);
	}
}

/******************************************************************************
	�֐��� : void CTexture::Init_Use_In_Game(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �Q�[���Ŏg���e�N�X�`���̏������B
******************************************************************************/
void CTexture::Init_Use_In_Game(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	//	������
	for (int i = 0; i < G_MAX; i++) {
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, m_Texture_Game[i].pass, &m_Texture_Game[i].pTexture);
	}
}

/******************************************************************************
	�֐��� : void CTexture::Init_Use_In_Result(void)
	����   : void
	�߂�l : �Ȃ�
	����   : ���U���g�Ŏg���e�N�X�`���̏������B
******************************************************************************/
void CTexture::Init_Use_In_Result(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	//	������
	for (int i = 0; i < R_MAX; i++) {
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, m_Texture_Result[i].pass, &m_Texture_Result[i].pTexture);
	}
}

/******************************************************************************
	�֐��� : void CTexture::Uninit_Use_In_Title(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �e�N�X�`���̉�������B
******************************************************************************/
void CTexture::Uninit_Use_In_Title(void)
{
	//	�e�N�X�`���̐�������
	for (int i = 0; i < T_MAX; i++) {
		SAFE_RELEASE(m_Texture_Title[i].pTexture);
	}
}

/******************************************************************************
	�֐��� : void CTexture::Uninit_Use_In_Game(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �e�N�X�`���̉�������B
******************************************************************************/
void CTexture::Uninit_Use_In_Game(void)
{
	//	�e�N�X�`���̐�������
	for (int i = 0; i < G_MAX; i++) {
		SAFE_RELEASE(m_Texture_Game[i].pTexture);
	}
}

/******************************************************************************
	�֐��� : void CTexture::Uninit_Use_In_Result(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �e�N�X�`���̉�������B
******************************************************************************/
void CTexture::Uninit_Use_In_Result(void)
{
	//	�e�N�X�`���̐�������
	for (int i = 0; i < R_MAX; i++) {
		SAFE_RELEASE(m_Texture_Result[i].pTexture);
	}
}

/******************************************************************************
	�֐��� : CTexture * CTexture::Create(int index)
	����   : index
	�߂�l : �Ȃ�
	����   : �����֐��B
******************************************************************************/
CTexture * CTexture::Create(int index)
{
	CTexture *obj = new CTexture;
	obj->TextureInit(index);

	return obj;
}

/******************************************************************************
	�֐��� : void CTexture::UnLoad(CTexture * texture, int index)
	����   : texture
	�߂�l : �Ȃ�
	����   : ����֐��B
******************************************************************************/
void CTexture::UnLoad(CTexture * texture, int index)
{
	texture->TextureUninit(index);
	SAFE_DELETE(texture);
}

/******************************************************************************
	�֐��� : void CTexture::TextureInit(int index)
	����   : index
	�߂�l : �Ȃ�
	����   : �����̃|�C���^�̒��̏������������s���B
******************************************************************************/
void CTexture::TextureInit(int index)
{
	if (index < SCENE_MAX)
		(this->*Init[index])();
}

/******************************************************************************
	�֐��� : void CTexture::TextureUninit(int index)
	����   : index
	�߂�l : �Ȃ�
	����   : �����̃|�C���^�̒��̉���������s���B
******************************************************************************/
void CTexture::TextureUninit(int index)
{
	if (index < SCENE_MAX)
		(this->*Uninit[index])();
}

/******************************************************************************
	�֐��� : LPDIRECT3DTEXTURE9 CTexture::GetTexture(int index, int num)
	����   : index, num
	�߂�l : �Ȃ�
	����   : ������index��num�Ԗڂ̃e�N�X�`���̃|�C���^��Ԃ��B
******************************************************************************/
LPDIRECT3DTEXTURE9 CTexture::GetTexture(int index, int num)
{
	if ((index - 1) < SCENE_MAX) {
		switch (index - 1) {
		case 0:
			if (num < T_MAX)
				return m_Texture_Title[num].pTexture;
			else 
				return NULL;
			break;
		case 1:
			if (num < G_MAX)
				return m_Texture_Game[num].pTexture;
			else 
				return NULL;
			break;
		case 2:
			if (num < R_MAX)
				return m_Texture_Result[num].pTexture;
			else
				return NULL;
			break;
		default:
			return NULL;
			break;
		}
	}
	else {
		return NULL;
	}
	return NULL;
}
