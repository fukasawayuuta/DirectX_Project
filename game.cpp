/******************************************************************************
	�^�C�g���� : �Q�[���N���X
	�t�@�C���� : game.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/07/14
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
#include "scene3D.h"
#include "sceneX.h"
#include "sceneIndex.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "Mode.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "Fade.h"
#include "texture.h"
#include "game_BG.h"
#include "dynamicModel.h"
#include "motionManager.h"
#include "player.h"
#include "MeshField.h"
#include "Enemy.h"
#include "collision.h"
#include "sphere.h"
#include "collisionDetection.h"
#include "time.h"
#include "skyBox.h"
#include "dustGenerate.h"
#include "EnemyManager.h"
#include "fixedBattery.h"
#include "score.h"

/******************************************************************************
	�}�N����`
******************************************************************************/
/******************************************************************************
	�\���̐錾
******************************************************************************/
//	�ÓI�����o�ϐ��錾/////////////////////////////////////////////////////////
bool CGame::m_PauseFlag = false;

/******************************************************************************
	�֐��� : CGame::CGame()
	����   : �R���X�g���N�^
******************************************************************************/
CGame::CGame()
{
	m_pCamera = NULL;
	m_Light = NULL;

	m_bMultiThreed = false;		//	�}���`�X���b�h�p�t���O�B
}

/******************************************************************************
	�֐��� : CGame::~CGame()
	����   : �f�X�g���N�^
******************************************************************************/
CGame::~CGame()
{
}

/******************************************************************************
	�֐��� : void CGame::Init(void)
	����   : void
	�߂�l : �Ȃ�
	����   : ���b�V���t�B�[���h�̏������A�v���C���[�̏������A���b�V���h�[���̏������A
			 �t�B�[���h�I�u�W�F�N�g�̏������A�X�R�A�̏������A�^�C���̏������A
			 ���e�B�N���̏������A���f���}�l�[�W���̏������A�G�̏������A�J�����̏������A
			 ���C�g�̏������B
******************************************************************************/
void CGame::Init(void)
{
	//	�N���X�̎��̉�
	m_pCamera = new CCamera;		//	�J�����̍쐬�B
	m_Light = new CLight;			//	���C�g�̍쐬�B

	m_pCamera->Init();		//	�J�����̏������B
	m_Light->Init();		//	���C�g�̏������B

	CMotionManager::Load();
	m_pGameTexture = CTexture::Create(GAME);
	m_Player = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_MeshField = CMeshField::Create("data/field_data/MeshFiled_Postision.txt", CTexture::G_FLOOR);
	//m_Enemy = CEnemy::Create(D3DXVECTOR3(300.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//CFixedBattery::Create(D3DXVECTOR3(300.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
	CSkyBox::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2500.0f, 2500.0f, 2500.0f, CTexture::G_SKYBOX);
	m_DustGenerater = CDustGenerater::Create(300);
	CEnemyManager::Load("EnemyInfo.txt");
	CScore::Create();
	//CGame_BG::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CTexture::G_BACKGROUND);
	
	m_PauseFlag = false;
}

/******************************************************************************
	�֐��� : void CGame::Uninit(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �V�[���̉���A�J�����̉���A���C�g�̉���A���f���}�l�[�W���̉���B
******************************************************************************/
void CGame::Uninit(void)
{
	CScene::ReleaseAll();

	//	�J�����̏I������
	m_pCamera->Uninit();
	SAFE_DELETE(m_pCamera);
	
	//	���C�g�̏I������
	m_Light->Uninit();
	SAFE_DELETE(m_Light);

	SAFE_DELETE(m_DustGenerater);

	CTexture::UnLoad(m_pGameTexture, GAME);	//	�e�N�X�`���̉���B
	CMotionManager::Unload();				//	���[�V�����}�l�[�W���̉���B
	CCollision::ReleaseAll();				//	�����蔻��̉���B
	CCollisionDetection::Release();			//	�����蔻��Ǘ��N���X�̉���B
}

/******************************************************************************
	�֐��� : void CGame::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �V�[���̍X�V�A�J�����̍X�V�A���͏����n���̍X�V�B
******************************************************************************/
void CGame::Update(void)
{
	CManager *pManager = GetManager();
	CInput   *pInput = pManager->GetInput();

	if (pInput->GetKeyboardTrigger(DIK_P) == true) {
		if (CFade::GetFadeMode() == FADE_NONE) {
			m_PauseFlag = m_PauseFlag ? false : true;
		}
		if (m_PauseFlag == true) {
			
		}
	}

	if (m_PauseFlag == false) {
		//	�J�����̍X�V
		m_pCamera->Update();

		CScene::UpdateAll();
		CCollision::UpdateAll();
		CCollisionDetection::GetInstance()->CollisionDetectionManager();
	}
	else {
		
	}

	if (pInput->GetKeyboardTrigger(DIK_RETURN) == true) {
		if (CFade::GetFadeMode() == FADE_NONE) {
			CFade::Start(FADE_OUT, new CResult);
		}
	}
}

/******************************************************************************
	�֐��� : void CGame::Draw(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �V�[���̕`��B
******************************************************************************/
void CGame::Draw(void)
{
	m_pCamera->SetCamera();

	CScene::DrawAll();
	if (m_PauseFlag == true) {
	}
}