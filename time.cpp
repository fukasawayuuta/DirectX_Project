/******************************************************************************
	�^�C�g���� : �^�C�}�[�N���X
	�t�@�C���� : time.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2017/01/12
	�X�V��     : 
******************************************************************************/
/******************************************************************************
	�C���N���[�h�t�@�C��
******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "scene2D.h"
#include "texture.h"
#include "number.h"
#include "time.h"

/******************************************************************************
	�}�N����`
******************************************************************************/
const int ANIMATION_TIME_MAX_CNT = 10;  //	�\������t�@�C���ɂ��邷�ׂẴA�j���[�V������
const int ANIMATION_TIME_MAX_CNT_X = 10;//	�\������t�@�C���ɂ���X���W�̃A�j���[�V������
const int ANIMATION_TIME_MAX_CNT_Y = 1; //	�\������t�@�C���ɂ���Y���W�̃A�j���[�V������
const int TIME_NUMBER_OF_DIGITS = 3;	//	����
const float TIME_WIDTH = 50.0f;			//	�|���S���̕�
const float TIME_HEIGHT = 50.0f;		//	�|���S���̍���
const int TIME_MAX = 15;				//	�������Ԏ���
const int TIME_FPS = 60;				//	fps�J�E���^

//	�ÓI�����o�ϐ�/////////////////////////////////////////////////////////////
int CTime::m_Time = 0;

/******************************************************************************
	�֐��� : CTime::CTime(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene2D(Priority, objType, sceneType)
	����   : �R���X�g���N�^
******************************************************************************/
CTime::CTime(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene2D(Priority, objType, sceneType)
{
	m_Time = 0;
	m_TimerCnt = 0;
	m_Number = NULL;
}

/******************************************************************************
	�֐��� : CTime::~CTime()
	����   : �f�X�g���N�^
******************************************************************************/
CTime::~CTime()
{
}

/******************************************************************************
	�֐��� : void CTime::Init(void)
	����   : void
	�߂�l : �Ȃ�
	����   : CNumber�̏������ƁA�ϐ��̏�����
******************************************************************************/
void CTime::Init(void)
{
	m_Width = TIME_WIDTH;
	m_Height = TIME_HEIGHT;
	m_DigitNumber = TIME_NUMBER_OF_DIGITS;

	m_AnimationCountX_Axis = ANIMATION_TIME_MAX_CNT_X;
	m_AnimationCountY_Axis = ANIMATION_TIME_MAX_CNT_Y;
	m_AnimationTotalNumber = m_AnimationCountX_Axis * m_AnimationCountY_Axis;

	//	�����������I�m��
	m_Number = new CNumber[m_DigitNumber];

	//	���W�ݒ�
	m_Pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.1f, 0.0f);

	//	�����Ƃ̏�����
	for (int i = 0;i < m_DigitNumber; i++) {
		m_Number[i].Init(m_Pos, m_Width, m_Height);
		//	���W�ύX
		m_Pos.x -= m_Width;
	}
	//	�^�C���ݒ�
	m_Time = TIME_MAX;
}

/******************************************************************************
	�֐��� : void CTime::Uninit(void)
	����   : void
	�߂�l : �Ȃ�
	����   : CNumber�̏I������
******************************************************************************/
void CTime::Uninit(void)
{
	//	�����̕��I������
	for (int i = 0; i < m_DigitNumber; i++) {
		m_Number[i].Uninit();
	}
	//	�폜����NULL����
	SAFE_DELETE_ARRAY(m_Number);
}

/******************************************************************************
	�֐��� : void CTime::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �X�V����
******************************************************************************/
void CTime::Update(void)
{
	//	��Ɨp�ϐ�
	int workTime = m_Time;
	int nTime;

	//	�^�C���̌����Ƃ̍X�V
	for (int i = 0; i < m_DigitNumber; i++) {
		//	�ꌅ�擾
		nTime = workTime % 10;
		//	�����Ƃ̍X�V
		m_Number[i].Update(nTime, m_AnimationCountX_Axis, m_AnimationCountY_Axis);
		//	���̌�����
		workTime = workTime / 10;
	}

	//	�^�C���J�E���^�A�b�v����TIME_FPS����������0���A�����ĂȂ���΍��̒l��Ԃ�
	m_TimerCnt = m_TimerCnt < TIME_FPS ? m_TimerCnt + 1 : 0;

	//	�^�C���J�E���^��0�ȉ���������
	if (m_TimerCnt <= 0) {
		//	�^�C���}�C�i�X
		--m_Time;

		//	�^�C����0�ȉ���������
		if (m_Time <= 0) {
			m_Time = 0;
		}
	}
}

/******************************************************************************
	�֐��� : void CTime::Draw(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �`�揈��
******************************************************************************/
void CTime::Draw(void)
{
	for (int i = 0; i < m_DigitNumber; i++) {
		m_Number[i].Draw();
	}
}

/******************************************************************************
	�֐��� : CTime *CTime::Create(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �쐬�֐�
******************************************************************************/
CTime *CTime::Create(void)
{
	CTime *pScene = new CTime;
	pScene->Init();

	return pScene;
}