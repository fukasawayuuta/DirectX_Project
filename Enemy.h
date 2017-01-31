/******************************************************************************
	�^�C�g���� : Enemy�N���X
	�t�@�C���� : Enemy.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/12/17
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

class CEnemy : public CDynamicModel
{
public:
	CEnemy(int Priority = NONE, OBJ_TYPE objType = OBJ_TYPE_NONE, SCENE_TYPE sceneType = SCENE_TYPE_NONE);		//	�R���X�g���N�^�B
	~CEnemy();		//	�f�X�g���N�^�B

	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, DYNAMICMODEL_TYPE modelType);		//	�������֐��B
	void Uninit(void);		//	�I���֐��B
	void Update(void);		//	�X�V�֐��B
	void Draw(void);		//	�`��֐��B

	void AddHp(int power) { m_Hp += power; }		//	Hp���Z�֐��B
	void SubHp(int power) { m_Hp -= power; }		//	Hp���Z�֐��B

	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, DYNAMICMODEL_TYPE modelType);		//	�����֐��B

protected:
	CCollision *m_pCollision;		//	�����蔻��N���X�B
	int m_Hp;						//	�̗́B
	float m_Radius;					//	���a�B
};

