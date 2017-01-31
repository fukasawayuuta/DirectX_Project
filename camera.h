/******************************************************************************
	�^�C�g���� : ���߂�N���X
	�t�@�C���� : camera.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/04/26
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
typedef struct
{
	D3DXVECTOR3 posV;			//���_
	D3DXVECTOR3 posR;			//�����_
	D3DXVECTOR3 vecU;			//������x�N�g��
	D3DXMATRIX mtxProjection;	//�v���W�F�N�V�����}�g���N�X
	D3DXMATRIX mtxView;			//�r���[�}�g���N�X
}CAMERA;

/******************************************************************************
	�N���X�錾
******************************************************************************/
class CCamera
{
public:
	CCamera();					//	�R���X�g���N�^
	~CCamera();					//	�f�X�g���N�^

	void Init(void);			//	�������֐�
	void Uninit(void);			//	�I���֐�
	void Update(void);			//	�X�V�֐�
	void SetCamera(void);		//	�ݒ�֐�

	D3DXVECTOR3 GetCameraRot(void) { return m_rot; }		//	�J�����̊p�x�̎擾

	D3DXMATRIX *GetProjectionMatrix(void) { return &m_mtxProjection; }		//	�v���W�F�N�V�����}�g���N�X�̎擾
	D3DXMATRIX *GetViewMatrix(void) { return &m_mtxView; }					//	�r���[�}�g���N�X�̎擾

	D3DXVECTOR3 GetPosR(void) { return m_posR; }		//	�����_���W�擾�B
	void SetPosR(D3DXVECTOR3 pos) { m_PurposePosR = pos; }		//	�����_���W�ݒ�B
	void SetVecU(float vec) { m_VecURot = vec; }		//	�p�x�ݒ�B
	void SetCameraRotY(float rot) { m_rot.y = rot; }	//	�p�x�ݒ�B
	void SetCameraRotX(float rot) { m_rot.x = rot; }	//	�p�x�ݒ�B
	void AddCameraRotY(float rot) { m_rot.y += rot; }
	void AddCameraRotX(float rot) { m_rot.x += rot; }

	static D3DXVECTOR3 GetVecRV(void) { return m_VecRV; }		//	�����_���王�_�܂ł̋�����Ԃ��֐�

private:
	D3DXVECTOR3 m_posV;				//	���_�B
	D3DXVECTOR3 m_posR;				//	�����_�B
	D3DXVECTOR3 m_vecU;				//	������x�N�g���B
	D3DXVECTOR3 m_rot;				//	�p�x�B
	D3DXVECTOR3 m_moveRot;			//	��]�ʁB
	D3DXVECTOR3 m_movePos;			//	�ړ��ʁB
	D3DXVECTOR3 m_moveVecU;			//	������x�N�g���̈ړ��ʁB
	D3DXVECTOR3 m_bace_point;		//	��_�B
	D3DXVECTOR3 m_PurposePosV;		//	���_�̈ړ���B
	D3DXVECTOR3 m_PurposePosR;		//	�����_�̈ړ���B

	static D3DXVECTOR3 m_VecRV;		//	���_���璍���_�܂ł̃x�N�g���B
	
	D3DXMATRIX  m_mtxProjection;	//	�v���W�F�N�V�����}�g���N�X�B
	D3DXMATRIX  m_mtxView;			//	�r���[�}�g���N�X�B

	float m_fDistance;			//	���_�ƒ����_�̋����B
	float m_fPurposeRot;		//	�ړI�̊p�x�B
	float m_fVibration_Width;	//	�U�����B
	float m_fSinCnt;			//	sin�g�ŗh�炷���߂̃J�E���^�[�B
	float m_VecURot;			//	������x�N�g���̉�]�ʁB
	float m_MoveVecU;	
};