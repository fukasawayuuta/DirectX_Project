/******************************************************************************
	�^�C�g���� : �Q�[���N���X
	�t�@�C���� : game.h
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
const float GRAVITY = 0.5f;		//	�d�́B

/******************************************************************************
	�\���̐錾
******************************************************************************/
/******************************************************************************
	�N���X
******************************************************************************/
//	�O���錾///////////////////////////////////////////////////////////////////
class CRenderer;
class CScene;
class CCamera;
class CLight;
class CMeshField;
class CBillboard;
class CInput;
class CPlayer;
class CPause;
class CBullet;
class CDust;
class CEnemy;
class CStencilShadow;
class CDustGenerater;

//	�N���X�{��/////////////////////////////////////////////////////////////////
class CGame : public CMode
{
public:
	CGame();		//	�R���X�g���N�^
	~CGame();		//	�f�X�g���N�^

	void Init(void);		//	�����֐�
	void Uninit(void);		//	�I���֐�
	void Update(void);		//	�X�V�֐�
	void Draw(void);		//	�`��֐�

	CCamera    *GetCamera(void) { return m_pCamera; }			//	�J�����̎擾�֐��B
	CMeshField *GetMeshField(void) { return m_MeshField; }		//	���b�V���t�B�[���h�擾�֐��B
	CPlayer    *GetPlayer(void) { return m_Player; }			//	�v���C���[�̎擾�B

	CStencilShadow *GetStencilShadow(void) { return m_StencilShadow; }
	static void UnusedPause(void) { m_PauseFlag = false; }		//	�|�[�Y�t���O��off�ɂ���B
private:
	CCamera		  *m_pCamera;			//	�J�����̏��B
	CLight		  *m_Light;				//	���C�g�̏��B
	CMeshField	  *m_MeshField;			//	���b�V���t�B�[���h�̏��B
	CPlayer		  *m_Player;			//	�v���C���[�̏��B
	CPause        *m_Pause;				//	�|�[�Y�̏��B
	CTexture      *m_pGameTexture;		//	�e�N�X�`���N���X�B
	CStencilShadow *m_StencilShadow;	//	�X�e���V���V���h�E���B
	CEnemy        *m_Enemy;				//	�G�l�~�[�N���X�B
	CDustGenerater *m_DustGenerater;
	static bool    m_PauseFlag;			//	�|�[�Y���Ă邩�ǂ����B
};

