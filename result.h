/******************************************************************************
	�^�C�g���� : ���U���g�N���X
	�t�@�C���� : result.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/07/14
	�X�V��     :
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#pragma once

/******************************************************************************
	�C���N���[�h�t�@�C��
******************************************************************************/
/******************************************************************************
	���C�u�����̃����N
******************************************************************************/
/******************************************************************************
	�}�N����`
******************************************************************************/
/******************************************************************************
	�\���̐錾
******************************************************************************/
/******************************************************************************
	�N���X
******************************************************************************/
class CResult : public CMode
{
public:
	CResult();			//	�R���X�g���N�^
	~CResult();			//	�f�X�g���N�^

	void Init(void);		//	�������֐�
	void Uninit(void);		//	�I���֐�
	void Update(void);		//	�X�V�֐�
	void Draw(void);		//	�`��֐�
	typedef enum {
		RESULT_MODE_NONE = 0,
		RESULT_MODE_ACTIVATION,
		RESULT_MODE_ACTIVE,
		RESULT_MODE_MAX
	}RESULT_MODE;


private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffPolygon;		//	���_�o�b�t�@
	LPDIRECT3DTEXTURE9      m_pTexturePolygon;		//	�e�N�X�`���o�b�t�@
	D3DXVECTOR3 m_pos;			//	���W
	D3DXCOLOR m_col;
	float m_width;				//	��
	float m_height;				//	����
	int m_stateCnt;
	int m_ChangeTime;
	RESULT_MODE m_mode;

	CTexture *m_pResultTexture;
};
