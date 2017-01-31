/******************************************************************************
	�t�@�C���� : collision.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2017/01/05
	�X�V��     : 
******************************************************************************/
/******************************************************************************
	�C���N���[�h�錾
******************************************************************************/
#include "main.h"
#include "collision.h"
#include "collisionList.h"
#include "scene.h"

/******************************************************************************
	�ÓI�����o�ϐ�
******************************************************************************/
CCollision *CCollision::m_Top[PRIORITY_MAX];		//	�擪�̃|�C���^�̔z��B
CCollision *CCollision::m_Cur[PRIORITY_MAX];		//	���݂̃|�C���^�̔z��B

/******************************************************************************
	�֐��� : CCollision::CCollision(CScene *scene, int Priority)
	����   : �R���X�g���N�^
******************************************************************************/
CCollision::CCollision(CScene *scene, int Priority)
{
	//	�擪�������ꍇ
	if (m_Top[Priority] == NULL) {
		m_Top[Priority] = this;			//	�����̃|�C���^��ݒ�
		m_Cur[Priority] = this;			//	�����̃|�C���^��ݒ�
		m_Prev = NULL;					//	�����̑O�̃|�C���^��NULL��ݒ�
		m_Next = NULL;					//	�����̎��̃|�C���^��NULL��ݒ�
	}
	else {	//	�擪�ȊO�������ꍇ
		m_Cur[Priority]->m_Next = this;		//	���̃A�h���X�ݒ�
		m_Prev = m_Cur[Priority];			//	�O�̃A�h���X�ݒ�
		m_Cur[Priority] = this;				//	�A�h���X�X�V
		m_Next = NULL;						//	�����̎��̃A�h���X�͂܂��Ȃ��̂�NULL�ݒ�
	}

	m_pScene = scene;
	m_Priority = Priority;
}

/******************************************************************************
	�֐��� : CCollision::~CCollision()
	����   : �R���X�g���N�^
******************************************************************************/
CCollision::~CCollision()
{
}

/******************************************************************************
	�֐��� : void CCollision::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �e�̍��W���󂯎���āA���W���X�V����B
******************************************************************************/
void CCollision::Update(void)
{
	if (m_Delete == false) {
		if (m_pScene != NULL) {
			m_WorldPos = m_pScene->GetPos() + m_LoaclPos;
		}
		else {
			m_WorldPos = m_LoaclPos;
		}
	}
}
/******************************************************************************
	�֐��� : void CCollision::UpdateAll(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �����蔻��̍X�V�B
******************************************************************************/
void CCollision::UpdateAll(void)
{
	//	�D��x���������ɍX�V
	for (int i = 0; i < PRIORITY_MAX; i++) {
		CCollision *scene = m_Top[i];
		CCollision *NextScene;

		while (scene != null) {
			//	���̃|�C���^�擾
			NextScene = scene->m_Next;
			
			scene->Update();		//	�X�V
			
			scene = NextScene;		//	���̃|�C���^���
		}

		scene = m_Top[i];		//	�擪�A�h���X���

		while (scene != null) {
			//	���̃|�C���^�擾
			NextScene = scene->m_Next;
			//	�폜�t���O��true��������
			if (scene->m_Delete == true) {
				scene->Release(scene->m_Priority);		//	���X�g����폜
			}

			scene = NextScene;		//	���̃|�C���^���
		}
	}
}

/******************************************************************************
	�֐��� : void CCollision::DrawAll(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �`�揈���B
******************************************************************************/
void CCollision::DrawAll(void)
{
	//	�D��x���������ɕ`��
	for (int i = 0; i < PRIORITY_MAX; i++) {
		CCollision *scene = m_Top[i];
		CCollision *NextScene;

		while (scene != NULL) {
			//	���̃|�C���^�擾
			NextScene = scene->m_Next;
			scene->Draw();			//	�`��
			scene = NextScene;		//	���̃|�C���^���
		}
	}
}

/******************************************************************************
	�֐��� : void CCollision::ReleaseAll(void)
	����   : void
	�߂�l : �Ȃ�
	����   : ��������B
******************************************************************************/
void CCollision::ReleaseAll(void)
{
	for (int i = 0; i < PRIORITY_MAX; i++) {
		CCollision *scene = m_Top[i];
		CCollision *NextScene;

		//	NULL ����Ȃ������
		while (scene != NULL) {
			//	�����O�Ɏ��̃|�C���^�擾
			NextScene = scene->m_Next;
			scene->Release(scene->m_Priority);	//	���X�g����폜
			scene = NextScene;		//	���̃|�C���^���
		}
		m_Top[i] = null;
	}
}

/******************************************************************************
	�֐��� : void CCollision::ReleaseAll(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �����������ă��X�g���q�������B
******************************************************************************/
void CCollision::Release(int Priority)
{
	//	��O�̃|�C���^���Ȃ��āA���̃|�C���^������ꍇ (���� -> �� ~ )
	if (this->m_Prev == NULL && this->m_Next != NULL) {
		m_Top[Priority] = this->m_Next;				//	�����̎���擪��
		this->m_Next->m_Prev = NULL;		//	�������̎��ɓ����Ă��� m_Prev �ɂ͎����������Ă��邽�� NULL ������

	}//	���̃|�C���^�������āA�O�̃|�C���^���Ȃ��ꍇ (~ �O -> ����)
	else if (this->m_Prev != NULL && this->m_Next == NULL) {
		m_Cur[Priority] = this->m_Prev;				//	�����̑O���Ō��
		this->m_Prev->m_Next = NULL;		//	���������̑O�ɓ����Ă��� m_Next �ɂ͎����������Ă��邽�� NULL ������

	}//	�O�ɂ����ɂ��|�C���^������ꍇ (~ -> �O -> ���� -> �� -> ~)
	else if (this->m_Prev != NULL && this->m_Next != NULL) {
		this->m_Prev->m_Next = this->m_Next;		//	�����̑O�̃|�C���^�̎��̃|�C���^�Ɏ����̎��̃|�C���^������
		this->m_Next->m_Prev = this->m_Prev;		//	�����̎��̃|�C���^�̑O�̃|�C���^�Ɏ����̑O�̃|�C���^������

	}//	���������̏ꍇ
	else if (this == m_Top[Priority] && this == m_Cur[Priority]) {
		m_Top[Priority] = NULL;			//	m_Top �� NULL ������
		m_Cur[Priority] = NULL;			//	m_Cur �� NULL ������
	}

	//	��������ƍ폜
	this->Uninit();
	delete this;
}