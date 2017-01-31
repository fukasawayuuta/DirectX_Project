/******************************************************************************
	�^�C�g���� : Bullet�N���X
	�t�@�C���� : bullet.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/12/15
	�X�V��     : 
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#pragma once

/******************************************************************************
	�N���X�錾
******************************************************************************/
class CCollision;
class CMeshField;

class CBullet : CBillboard
{
public:
	CBullet(int Priority = OBJECT_BILLBOARD, OBJ_TYPE objType = OBJ_TYPE_BULLET, SCENE_TYPE sceneType = SCENE_TYPE_GAME);		//	�R���X�g���N�^�B
	~CBullet();		//	�f�X�g���N�^�B

	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float height, int textureType, CScene::OBJ_TYPE objType);		//	�������֐��B
	void Uninit(void);		//	�I�������B
	void Update(void);		//	�X�V�֐��B

	void RendererBegin(void);		//	�`��O�ݒ�֐��B
	void RendererEnd(void);			//	�`���ݒ�֐��B
	void CheckOutsideRange(CMeshField *mesh);		//	�͈͊O�`�F�b�N�B

	OBJ_TYPE GetTypeUsingSelf(void) { return m_TypeUsingSelf; }		//	�������g�����Ώۂ̃^�C�v���擾����֐��B
	static void Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float height, int textureType, CScene::OBJ_TYPE objType);

private:
	CCollision *m_pCollision;		//	�����蔻��̃N���X�B

	OBJ_TYPE m_TypeUsingSelf;		//	�������g�����Ώۂ̃^�C�v�B
	D3DXVECTOR3 m_GoAngle;			//	�����̐i�s�����̊p�x�B
	D3DXVECTOR3 m_Movement;

	float m_CollRadius;	//	�����蔻��̔��a�B
	float m_MovingDistance;	//	�ړ��ʁB
};

