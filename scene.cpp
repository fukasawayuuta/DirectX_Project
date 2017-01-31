/******************************************************************************
	�^�C�g���� : �`��N���X
	�t�@�C���� : scene.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/04/19
	�X�V��     : 2016/06/21
******************************************************************************/
/******************************************************************************
	�C���N���[�h�t�@�C��
******************************************************************************/
#include "main.h"
#include "scene.h"
#include "renderer.h"
#include "manager.h"

/******************************************************************************
	�O���[�o���ϐ��錾
******************************************************************************/
//	�ÓI�����o�ϐ��錾/////////////////////////////////////////////////////////
CScene *CScene::m_Top[PRIORITY_MAX] = {NULL};		//	NULL�ŏ�����
CScene *CScene::m_Cur[PRIORITY_MAX] = {NULL};		//	NULL�ŏ�����

/******************************************************************************
	�֐��� : CScene::CScene()
	����   : �R���X�g���N�^
			 ���X�g�\������
			 �A�h���X�̑��
******************************************************************************/
CScene::CScene(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType)
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

	m_Objtype = objType;		//	�I�u�W�F�N�g�^�C�v��ݒ�
	m_Priority = Priority;		//	�D��x��ݒ�
	m_SceneType = sceneType;	//	�V�[����ݒ�
}

/******************************************************************************
	�֐��� : CScene::~CScene()
	����   : �f�X�g���N�^
			 �j�����ꂽ�Ƃ��ɌĂ΂�邪�A���g���Ȃ��̂œ��ɋL�q���鎖�Ȃ�
******************************************************************************/
CScene::~CScene()
{
}

/******************************************************************************
	�֐��� : void CScene::ReleaseAll(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �o�^����Ă���I�u�W�F�N�g����C�ɉ��
******************************************************************************/
void CScene::ReleaseAll(void)
{
	for (int i = 0 ; i < PRIORITY_MAX; i++) {
		CScene *scene = m_Top[i];
		CScene *NextScene;

		//	NULL ����Ȃ������
		while (scene != NULL) {
			//	�����O�Ɏ��̃|�C���^�擾
			NextScene = scene->m_Next;

			scene->Uninit();		//	�������
			delete scene;			//	�폜
			scene = NextScene;		//	���̃|�C���^���
		}
		m_Top[i] = null;
	}
}
/******************************************************************************
	�֐��� : void CScene::Release(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �o�^����Ă���I�u�W�F�N�g���������
			 ���X�g�\�����Ȃ��Ȃ���
******************************************************************************/
void CScene::Release(int Priority)
{
	//	��O�̃|�C���^���Ȃ��āA���̃|�C���^������ꍇ (���� -> �� ~ )
	if (this->m_Prev == NULL && this->m_Next != NULL) {
		m_Top[Priority] = this->m_Next;				//	�����̎���擪��
		this->m_Next->m_Prev = NULL;		//	�������̎��ɓ����Ă��� m_Prev �ɂ͎����������Ă��邽�� NULL ������

	}//	���̃|�C���^�������āA�O�̃|�C���^���Ȃ��ꍇ (~ �O -> ����)
	else if(this->m_Prev != NULL && this->m_Next == NULL) {
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
/******************************************************************************
	�֐��� : void CScene::UpdateAll(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �o�^����Ă���I�u�W�F�N�g���X�V
			 �폜�t���O��true�ɂȂ��Ă�����̂��폜����
******************************************************************************/
void CScene::UpdateAll(void)
{
	//	�D��x���������ɍX�V
	for (int i = 0; i < PRIORITY_MAX; i++) {
		CScene *scene = m_Top[i];
		CScene *NextScene;

		while (scene != null) {
			//	���̃|�C���^�擾
			NextScene = scene->m_Next;
			//	�A�N�e�B�u��������B
			if (scene->GetActive()) {
				scene->Update();		//	�X�V
			}
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
	�֐��� : void CScene::DrawAll(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �o�^����Ă���I�u�W�F�N�g��`��
******************************************************************************/
void CScene::DrawAll(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//	�D��x���������ɕ`��
	for (int i = 0; i < PRIORITY_MAX; i++) {
		CScene *scene = m_Top[i];
		CScene *NextScene;

		if (i >= OBJECT_BILLBOARD) {
			pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);	//	�t�H�O�FOFF
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		//	���C�e�B���O : OFF
		}

		while (scene != NULL) {
			//	���̃|�C���^�擾
			NextScene = scene->m_Next;

			//	�A�N�e�B�u��������B
			if (scene->GetActive()) {
				scene->Draw();			//	�`��
			}
			//}
			scene = NextScene;		//	���̃|�C���^���
		}

		if (i >= OBJECT_BILLBOARD) {
			pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);		//	�t�H�O�FON
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);		//	���C�e�B���O : OFF
		}
	}
}