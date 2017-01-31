/******************************************************************************
	�^�C�g���� : CFixedBattery�N���X
	�t�@�C���� : fixedBattery.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/12/17
	�X�V��     :
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#pragma once

/******************************************************************************
	�N���X�錾
******************************************************************************/
class CPlayer;

class CFixedBattery : public CEnemy
{
public:
	CFixedBattery(int Priority = OBJECT_ENEMY, OBJ_TYPE objType = OBJ_TYPE_ENEMY, SCENE_TYPE sceneType = SCENE_TYPE_GAME);		//	�R���X�g���N�^�B
	~CFixedBattery();		//	�f�X�g���N�^�B

	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Update(void);

	static void Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void ViewDecision(CPlayer *player);

private:
	//	���[�V�����̗񋓌^
	typedef enum {
		MOTION_MDOE_NEUTRAL = 0,	//	�ҋ@���[�V�����B
		MOTION_MODE_SEARCH,			//	���G���[�V�����B
		MOTION_MODE_MAX				//	���[�V�����ő吔�B
	};

	//	���f���̗񋓌^
	typedef enum {
		FOOT = 0,		//	���̃��f���B
		BODY,			//	�̂̃��f���B
		BARREL,			//	�e�g�̃��f���B
		MAX				//	���f���̍ő吔�B
	};

	int m_ShotInterval;		//	�e��łԊu�B
	bool m_Discovery;		//	���������ǂ����B
	float m_ViewDistance;	//	���E�����B
	float m_ViewAngle;		//	���E�p�x�B
};

