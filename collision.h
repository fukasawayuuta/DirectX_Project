/******************************************************************************
	�t�@�C���� : collision.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2017/01/05
	�X�V��     : 
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#pragma once

/******************************************************************************
	�N���X
******************************************************************************/
class CScene;
class CCapsule;
class CSphere;
class CCollisionList;

class CCollision
{
public:
	//	�D��x�̗񋓌^
	typedef enum {
		NONE = 0,
		LINE_SEGMENT,		//	�����B
		CSPHERE,			//	���́B
		PRIORITY_MAX
	}COLL_TYPE;


	CCollision(CScene *scene = NULL, int Priority = NONE);		//	�R���X�g���N�^�B
	~CCollision();		//	�f�X�g���N�^�B

	virtual void Init(void) {};			//	�������֐��B
	virtual void Uninit(void) = 0;		//	�I���֐��B
	virtual void Update(void);			//	�X�V�֐��B
	virtual void Draw(void)   = 0;		//	�`��֐��B

	//	�_�u���f�B�X�p�b�`�֐��B
	virtual void Dispatch(CCollision* other) = 0;
	virtual void Action(CCapsule* other) = 0;
	virtual void Action(CSphere* other) = 0;

	D3DXVECTOR3 GetLocalPos(void) { return m_LoaclPos; }		//	���[�J�����W�擾�B
	D3DXVECTOR3 GetWorldPos(void) { return m_WorldPos; }		//	���[���h���W�擾�B

	//	�ꊇ�֐�/////////////////////////////////////////////////////////////////////////////////////
	static void UpdateAll(void);		//	�ꊇ�X�V�֐��B
	static void DrawAll(void);			//	�ꊇ�`��֐��B
	static void ReleaseAll(void);		//	�ꊇ����֐��B
	void Release(int Priority);			//	�I������֐��B
	//	�ꊇ�֐�/////////////////////////////////////////////////////////////////////////////////////

	void SetHit(bool flag) { m_Hit = flag; }		//	�t���O��ݒ肷��֐��B
	bool GetHit(void) { return m_Hit; }				//	�����������ǂ������擾����֐��B
	void SetDelete(bool flag) { m_Delete = flag; }	//	delete�t���O��ݒ�B
	bool GetDelete(void) { return m_Delete; }		//	delete�t���O���擾�B

	CCollision *GetScenePrev(void) { return  m_Prev; }			//	����Scene�̑O�̃|�C���^��Ԃ��֐��B
	CCollision *GetSceneNext(void) { return  m_Next; }			//	����Scene�̎��̃|�C���^��Ԃ��֐��B
	static CCollision *GetListTopPointer(int priority) { return m_Top[priority]; }	//	�����̗D��x��Scene��Top�̃|�C���^��Ԃ��B
	CScene *GetScene(void) { return m_pScene; }
	CCollisionList *GetCollisionList(void) { return m_CollisionList; }
	int GetCollisionType(void) { return m_Priority; }

protected:
	D3DXVECTOR3 m_LoaclPos;	//	���W�B
	D3DXVECTOR3 m_WorldPos;	//	���[���h���W�B
	D3DXVECTOR3 m_Rot;		//	�p�x�B
	int m_Priority;			//	�D��x�B
	bool m_Delete;			//	�폜�t���O�B
	CScene *m_pScene;		//	�g�����Ώۂ�����|�C���^�B
	bool m_Hit;				//	�����������ǂ���������t���O�B
	CCollisionList *m_CollisionList;

	//	�|�C���^�֌W/////////////////////////////////////////////////////////////////////////////////
	static CCollision *m_Top[PRIORITY_MAX];		//	�擪�̃|�C���^�̔z��B
	static CCollision *m_Cur[PRIORITY_MAX];		//	���݂̃|�C���^�̔z��B
	CCollision *m_Prev;							//	�O�̃|�C���^�B
	CCollision *m_Next;							//	���̃|�C���^�B
	//	�|�C���^�֌W/////////////////////////////////////////////////////////////////////////////////
};

