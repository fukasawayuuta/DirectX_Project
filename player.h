/******************************************************************************
	�^�C�g���� : Player�N���X
	�t�@�C���� : player.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/12/14
	�X�V��     : 
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#pragma once

/******************************************************************************
	�N���X
******************************************************************************/
class CCollision;
class CMeshField;
class CInput;
class CPlayerLife;
class CBoost;

class CPlayer : public CDynamicModel
{
public:
	CPlayer(int Priority = OBJECT_3D, OBJ_TYPE objType = OBJ_TYPE_PLAYER, SCENE_TYPE sceneType = SCENE_TYPE_GAME);		//	�R���X�g���N�^�B
	~CPlayer();		//	�f�X�g���N�^�B

	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//	�������֐��B
	void Uninit(void);		//	�I���֐��B
	void Update(void);		//	�X�V�֐��B
	void Draw(void);		//	�`��֐��B

	int GetHP(void) { return m_Hp; }	//	�̗͎擾�֐��B
	bool GetLockOn(void) { return m_LockOn; }
	bool GetAlive(void) { return m_Alive; }		//	�����󋵎擾�֐��B
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//	�����֐��B

private:
	//	Player�̃��[�V�������B
	typedef enum {
		MOTION_MDOE_NEUTRAL = 0,		//	�ҋ@���[�V�����B
		MOTION_MODE_MOVE,				//	�ړ����[�V�����B
		MOTION_MDOE_ACTION,				//	����(�A�N�V����)���[�V�����B
		MOTION_MODE_JUMP,				//	�W�����v���[�V�����B
		MOTION_MODE_LANDING,			//	���n���[�V�����B
		MOTION_MODE_DIE,				//	���S���[�V�����B
		MOTION_MODE_MAX					//	���[�V�����̍ő吔�B
	};

	D3DXVECTOR3  m_movePos;		//	�ړ��ʁB
	D3DXVECTOR3  m_moveRot;		//	��]�ʁB
	D3DXVECTOR3  m_TargetPos;	//	�Ώۂ̍��W�B
	CCollision  *m_pCollision;	//	�����蔻��̃N���X�B
	CPlayerLife *m_Life;		//	���C�t�o�[�N���X�B
	CBoost      *m_BoostEffect;	//	�u�[�X�g�N���X�B

	int m_Hp;				//	�̗͂̕ϐ��B
	int m_CoolingTimeBoost;	//	�u�[�X�g�̃N�[���^�C���B
	bool m_Alive;			//	�����Ă��邩�ǂ����̃t���O�B
	bool m_Boost;			//	�u�[�X�g���Ă��邩�ǂ����B
	bool m_Jump;			//	�W�����v���Ă��邩�ǂ����B
	bool m_AirBoost;		//	�G�A�u�[�X�g���Ă��邩�ǂ����B
	bool m_Move;			//	�����Ă��邩�ǂ����B
	bool m_Attack;			//	�U�����Ă��邩�ǂ����B
	bool m_LockOn;			//	���b�N�I���t���O�B
	float m_purposeRot;		//	�ړI�̊p�x�B
	float m_MoveSpeed;		//	�ړ��ʁB
	float m_MoveInertia;	//	�ړ��̊����l�B
	float m_RotInertia;		//	��]�̊����l�B
	float m_CollRadius;		//	�����蔻��̔��a�B

	void MoveUpdate(void);						//	�ړ������B
	void MoveUpdateJoyStick(void);				//	JoyStick�ł̈ړ������B
	void HitTest_With_Mesh(CMeshField *mesh);	//	���b�V���t�B�[���h�Ƃ̂����蔻��֐��B
	void Angle_Modification(void);				//	�p�x���C������֐��B
	void Moving_Distance_Check(void);			//	�ړ��ʂ��`�F�b�N����֐��B
	void Jump_Check(CInput *input);				//	�W�����v�������`�F�b�N����֐��B
	void Attack(CInput *input);							//	�U���֐��B
	void Boost(CInput *input);					//	�u�[�X�g�����B
	void CollisionCheck(void);					//	�����蔻��̔r�ˏ����B
	void EnemyLockOn(void);						//	�G�����b�N�I������֐��B
};

