/******************************************************************************
	�^�C�g���� : Score�N���X
	�t�@�C���� : score.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2017/01/12
	�X�V��     : 
******************************************************************************/
/******************************************************************************
	�C���N���[�h�錾
******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "scene2D.h"
#include "texture.h"
#include "number.h"
#include "score.h"

/******************************************************************************
	�}�N����`
******************************************************************************/
const int ANIMATION_MAX_CNT = 10;	//	�\������t�@�C���ɂ��邷�ׂẴA�j���[�V������
const int ANIMATION_MAX_CNT_X = 10;	//	�\������t�@�C���ɂ���X���W�̃A�j���[�V������
const int ANIMATION_MAX_CNT_Y = 1;	//	�\������t�@�C���ɂ���Y���W�̃A�j���[�V������
const int NUMBER_OF_DIGITS = 3;		//	����
const float WIDTH = 50.0f;			//	�|���S���̕�
const float HEIGHT = 50.0f;			//	�|���S���̍���
const int SCORE_MAX = 100000000;	//	�X�R�A�̍ő�l

//	�ÓI�����o�ϐ�/////////////////////////////////////////////////////////////
int CScore::m_Score = 0;


/******************************************************************************
	�֐��� : CScore::CScore(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene2D(Priority, objType, sceneType)
	����   : �R���X�g���N�^
******************************************************************************/
CScore::CScore(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene2D(Priority, objType, sceneType)
{
	m_Score = 0;
	m_Number = NULL;
}

/******************************************************************************
	�֐��� : CScore::~CScore()
	����   : �f�X�g���N�^
******************************************************************************/
CScore::~CScore()
{
}

/******************************************************************************
	�֐��� : void CScore::Init(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �������A���W�ݒ�
******************************************************************************/
void CScore::Init(void)
{
	m_Width = WIDTH;
	m_Height = HEIGHT;
	m_DigitNumber = NUMBER_OF_DIGITS;

	m_AnimationCountX_Axis = ANIMATION_MAX_CNT_X;
	m_AnimationCountY_Axis = ANIMATION_MAX_CNT_Y;
	m_AnimationTotalNumber = m_AnimationCountX_Axis * m_AnimationCountY_Axis;
	//	�����������I�m��
	m_Number = new CNumber[m_DigitNumber];

	//	���W�ݒ�
	m_Pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.1f, 0.0f);

	//	�����Ƃ̏�����
	for (int i = 0; i < m_DigitNumber; i++) {
		m_Number[i].Init(m_Pos, m_Width, m_Height);
		//	���W�ύX
		m_Pos.x -= m_Width;
	}
}

/******************************************************************************
	�֐��� : void CScore::Uninit(void)
	����   : void
	�߂�l : �Ȃ�
	����   : ����A�I������
******************************************************************************/
void CScore::Uninit(void)
{
	//	�����̕��I������
	for (int i = 0; i < m_DigitNumber; i++) {
		m_Number[i].Uninit();
	}
	//	�폜����NULL����
	SAFE_DELETE_ARRAY(m_Number);
}

/******************************************************************************
	�֐��� : void CScore::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �X�V����
******************************************************************************/
void CScore::Update(void)
{
	//	��Ɨp�ϐ��B
	int workScore = m_Score;
	int nScore = 0;

	//	�i���o�[�X�V�B
	for (int i = 0; i < m_DigitNumber; i++) {
		nScore = workScore % 10;

		m_Number[i].Update(nScore, m_AnimationCountX_Axis, m_AnimationCountY_Axis);

		workScore = workScore / 10;
	}
}

/******************************************************************************
	�֐��� : void CScore::Draw(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �`�揈��
******************************************************************************/
void CScore::Draw(void)
{
	for (int i = 0; i < m_DigitNumber; i++) {
		m_Number[i].Draw();
	}
}

/******************************************************************************
	�֐��� : CScore *CScore::Create(void)
	����   : void
	�߂�l : obj
	����   : �����֐��B
******************************************************************************/
CScore * CScore::Create(void)
{
	CScore *obj = new CScore;
	obj->Init();
	return obj;
}
