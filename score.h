/******************************************************************************
	�^�C�g���� : Score�N���X
	�t�@�C���� : score.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2017/01/12
	�X�V��     : 
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#pragma once

/******************************************************************************
	�N���X�錾
******************************************************************************/
class CNumber;

class CScore : public CScene2D
{
public:
	CScore(int Priority = OBJECT_2D, OBJ_TYPE objType = OBJ_TYPE_UI, SCENE_TYPE sceneType = SCENE_TYPE_GAME);		//	�R���X�g���N�^�B
	~CScore();		//	�f�X�g���N�^�B

	void Init(void);	//	�������֐��B
	void Uninit(void);	//	�I���֐��B
	void Update(void);	//	�X�V�֐��B
	void Draw(void);	//	�`��֐��B

	static void AddScore(int point) { m_Score += point; }	//	�X�R�A���_�֐��B
	static void SubScore(int point) { m_Score -= point; }	//	�X�R�A���_�֐��B
	static int GetScore(void) { return m_Score; }			//	�X�R�A�擾�֐��B
	static CScore *Create(void);
private:
	static int m_Score;	//	���_�B
	CNumber *m_Number;	//	�i���o�[�B
	int m_DigitNumber;	//	�����B
};

