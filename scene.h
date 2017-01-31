/******************************************************************************
	�^�C�g���� : �`��N���X
	�t�@�C���� : scene.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/04/19
	�X�V��     : 2016/05/31
				 2016/05/16
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#pragma once

/******************************************************************************
	�N���X
******************************************************************************/
class CScene
{
public:
	//	�I�u�W�F�N�g�̃^�C�v///////////////////////////////////////////////////
	typedef enum {
		OBJ_TYPE_NONE = 0,		//	null
		OBJ_TYPE_PLAYER,		//	player
		OBJ_TYPE_ENEMY,			//	enemy
		OBJ_TYPE_BULLET,		//	bullet
		OBJ_TYPE_EXPLOSION,		//	explosion
		OBJ_TYPE_BOOSTEFFECT,	//	boost_effect
		OBJ_TYPE_DUST,			//	dust
		OBJ_TYPE_FIELD,			//	field
		OBJ_TYPE_UI,			//	ui
		OBJ_TYPE_SHADOW,		//	shadow
		OBJ_TYPE_MAX			//	max
	}OBJ_TYPE;

	//	�V�[���̃^�C�v//////////////////////////////////////////////////////////
	typedef enum {
		SCENE_TYPE_NONE = 0,	//	none
		SCENE_TYPE_TITLE,		//	title
		SCENE_TYPE_GAME,		//	game
		SCENE_TYPE_RESULT,		//	result
		SCENE_TYPE_MAX			//	max
	}SCENE_TYPE;

	//	�D��x�̗񋓌^///////////////////////////////////////////////////////////
	typedef enum {
		NONE = 0,
		BACKGROUND,			//	�h�[���Ƃ����Ƃ������Ȃ����́B
		STENCIL,			//	�X�e���V���n�B
		OBJECT_3D,			//	�v���C���[�Ƃ��G�Ƃ��������́B
		OBJECT_ENEMY,		//	�G�l�~�[�B
		OBJECT_BILLBOARD,	//	�r���{�[�h�Ƃ��B
		EFFECT,				//	�G�t�F�N�g�Ƃ��B
		OBJECT_2D,			//	2D�֘A�B
		PRIORITY_MAX
	}PRIORITY;

	//	��{�֐�/////////////////////////////////////////////////////////////////////////////////////
	CScene(int Priority = NONE, OBJ_TYPE objType = OBJ_TYPE_NONE, SCENE_TYPE sceneType = SCENE_TYPE_NONE);		//	�R���X�g���N�^�B
	~CScene();							//	�f�X�g���N�^�B
	virtual void Init(void) {}			//	�������֐��B
	virtual void Uninit(void)  = 0;		//	�I���֐��B
	virtual void Update(void)  = 0;		//	�X�V�֐��B
	virtual void Draw(void)    = 0;		//	�`��֐��B
	//	��{�֐�/////////////////////////////////////////////////////////////////////////////////////

	//	�ꊇ�֐�/////////////////////////////////////////////////////////////////////////////////////
	static void UpdateAll(void);		//	�ꊇ�X�V�֐��B
	static void DrawAll(void);			//	�ꊇ�`��֐��B
	static void ReleaseAll(void);		//	�ꊇ����֐��B
	void Release(int Priority);			//	�I������֐��B
	//	�ꊇ�֐�/////////////////////////////////////////////////////////////////////////////////////

	//	�擾�֐�/////////////////////////////////////////////////////////////////////////////////////
	OBJ_TYPE GetObjectType (void) {return m_Objtype;}			//	�I�u�W�F�N�g�̃^�C�v��Ԃ��֐��B
	bool GetDeleteFlag(void) { return m_Delete; }				//	�t���O���擾����֐��B
	bool GetActive(void) { return m_Active; }					//	�A�N�e�B�u�t���O���擾����֐��B
	D3DXVECTOR3 GetPos(void) { return m_Pos; }					//	���W��Ԃ��֐��B
	D3DXVECTOR3 GetRot(void) { return m_Rot; }					//	�p�x��Ԃ��֐��B
	CScene *GetScenePrev(void) { return  m_Prev; }				//	����Scene�̑O�̃|�C���^��Ԃ��֐��B
	CScene *GetSceneNext(void) { return  m_Next; }				//	����Scene�̎��̃|�C���^��Ԃ��֐��B
	void SetScenePrev(CScene *prev) { m_Prev = prev; }			//	����Scene�̑O�̃|�C���^���Z�b�g����֐��B
	void SetSceneNext(CScene *next) { m_Next = next; }			//	����Scene�̎��̃|�C���^���Z�b�g����֐��B
	static CScene *GetListTopPointer(int priority) { return m_Top[priority]; }	//	�����̗D��x��Scene��Top�̃|�C���^��Ԃ��B
	//	�擾�֐�/////////////////////////////////////////////////////////////////////////////////////

	//	�ݒ�֐�/////////////////////////////////////////////////////////////////////////////////////
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }				//	������pos�������o�ϐ���pos�ɑ������֐��B
	void SetDelete(bool flag) { m_Delete = flag; }				//	�t���O���Z�b�g����֐��B
	void SetActive(bool flag) { m_Active = flag; }				//	�A�N�e�B�u�t���O���Z�b�g����֐��B
	//	�ݒ�֐�/////////////////////////////////////////////////////////////////////////////////////

protected:
	//	�ϐ��֌W/////////////////////////////////////////////////////////////////////////////////////
	D3DXVECTOR3 m_Pos;			//	���W�B
	D3DXVECTOR3 m_Rot;			//	�p�x�B
	OBJ_TYPE  m_Objtype;		//	�I�u�W�F�N�g�^�C�v�B
	SCENE_TYPE m_SceneType;		//	�V�[���^�C�v�B
	int m_Priority;				//	�D��x�B
	bool m_Delete;				//	�폜�t���O�B
	bool m_Active;				//	�A�N�e�B�u�t���O�B
	//	�ϐ��֌W/////////////////////////////////////////////////////////////////////////////////////
	
	//	�|�C���^�֌W/////////////////////////////////////////////////////////////////////////////////
	static CScene *m_Top[PRIORITY_MAX];		//	�擪�̃|�C���^�̔z��B
	static CScene *m_Cur[PRIORITY_MAX];		//	���݂̃|�C���^�̔z��B
	CScene *m_Prev;							//	�O�̃|�C���^�B
	CScene *m_Next;							//	���̃|�C���^�B
	//	�|�C���^�֌W/////////////////////////////////////////////////////////////////////////////////
};