/******************************************************************************
	�^�C�g���� : Player�N���X
	�t�@�C���� : player.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/12/17
	�X�V��     :
******************************************************************************/
/******************************************************************************
	�C���N���[�h�t�@�C��
******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "texture.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneIndex.h"
#include "sceneX.h"
#include "billboard.h"
#include "model.h"
#include "motionManager.h"
#include "dynamicModel.h"
#include "player.h"
#include "collision.h"
#include "collisionList.h"
#include "sphere.h"
#include "Mode.h"
#include "game.h"
#include "camera.h"
#include "input.h"
#include "bullet.h"
#include "MeshField.h"
#include "playerLife.h"
#include "Explosion.h"
#include "capsule.h"
#include "boost.h"
#include "stencilShadow.h"

/******************************************************************************
	�}�N���K�X
******************************************************************************/
const float MOVE_SPEED = 1.0f;			//	�ړ��ʁB
const float MOVE_INERTIA = 0.23f;		//	�ړ��ʂ̊����l�B
const float COLLISION_RADIUS = 50.0f;	//	�����蔻��̔��a�B
const float JUMP_POWER = 5.0f;			//	�W�����v�ʁB
const float PLAYER_MOVEMENT_LOWEST_VALUE = 0.01f;	//	�v���C���[���܂������Ă���ƔF�߂�Œ�ړ��ʁB
const float CERTAIN_DISTANCE = 800.0f;
const float BOOST_MAGNIFICATION = 15.0f;		//	�u�[�X�g����ۂ̈ړ��ʂɊ|����{���B
const int   BOOST_COOLING_TIME = 30;		//	�u�[�X�g�̃N�[���^�C���B


/******************************************************************************
	�֐��� : CPlayer::CPlayer(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CDynamicModel(Priority, objType, sceneType)
	����   : �R���X�g���N�^
******************************************************************************/
CPlayer::CPlayer(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CDynamicModel(Priority, objType, sceneType)
{
	m_Hp = 0;
	m_CoolingTimeBoost = 0;
	m_Boost = false;
	m_Jump = false;
	m_AirBoost = false;
	m_Alive = true;
	m_Move = false;
	m_LockOn = false;
	m_pCollision = NULL;
	m_Life = NULL;
	m_BoostEffect = NULL;
	m_purposeRot = 0.0f;
	m_MoveSpeed = 0.0f;
	m_MoveInertia = 0.0f;
	m_RotInertia = 0.0f;
	m_CollRadius = 0.0f;
	m_movePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_moveRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

/******************************************************************************
	�֐��� : CPlayer::~CPlayer()
	����   : �f�X�g���N�^
******************************************************************************/
CPlayer::~CPlayer()
{
}

/******************************************************************************
	�֐��� : void CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
	����   : pos, rot
	�߂�l : �Ȃ�
	����   : ���W�Ɗp�x�̐ݒ�B
******************************************************************************/
void CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//	���[�V�����ݒ�B
	m_MotionManager = CMotionManager::Create(DYNAMICMODEL_TYPE_PLAYER_00, &m_mtxWorld);
	m_MotionManager->SetMotion(MOTION_MDOE_NEUTRAL);

	//	�����l�ݒ�B
	m_Pos = pos;
	m_Rot = rot;
	m_Scl *= 0.5f;
	m_MoveSpeed = MOVE_SPEED;
	m_MoveInertia = MOVE_INERTIA;
	m_CollRadius = COLLISION_RADIUS;
	m_Hp = 500;

	//CSphere::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_CollRadius, this);
	//	�����蔻�萶���B
	m_pCollision = CCapsule::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 50.0f, 0.0f), this);
	//	���C�t�o�[�����B
	m_Life = CPlayerLife::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.018f, SCREEN_HEIGHT * 0.88f, 0.0f), 500.0f, 75, CTexture::G_LIFE, CTexture::G_LIVEBAR, m_Hp);
	//	�G�t�F�N�g�����B
	m_BoostEffect = CBoost::Create(m_Pos, 100.0f, 100.0f, CTexture::G_BOOST);
	//	�e�𐶐��B
	CStencilShadow::Create(this);
}

/******************************************************************************
	�֐��� : void CPlayer::Uninit(void)
	����   : void
	�߂�l : �Ȃ�
	����   : ��������B
******************************************************************************/
void CPlayer::Uninit(void)
{
	m_MotionManager->Uninit();
	SAFE_DELETE(m_MotionManager);
	//	�����蔻�����B
	if (m_pCollision != NULL) {
		m_pCollision->SetDelete(true);
		m_pCollision->GetCollisionList()->Uninit();
		m_pCollision = NULL;
	}

	m_Life->SetDelete(true);
	m_Life = NULL;

	m_BoostEffect->SetDelete(true);
	m_BoostEffect = NULL;
}

/******************************************************************************
	�֐��� : void CPlayer::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �X�V�����B
******************************************************************************/
void CPlayer::Update(void)
{
	// ���ꂼ��̃|�C���^�ƒl���擾
	CManager *pManager = GetManager();
	CInput *pInput = pManager->GetInput();
	CGame *pGame = (CGame *)CManager::GetMode();
	D3DXVECTOR3 CameraRot = pGame->GetCamera()->GetCameraRot();
	CMeshField *pMeshField = pGame->GetMeshField();
	bool bJumpOld = m_Jump;

	//	�ړ������B
	MoveUpdate();
	//	�W�����v�`�F�b�N�B
	Jump_Check(pInput);
	//	�u�[�X�g�����B
	Boost(pInput);
	//	�����蔻�菈���B
	CollisionCheck();
	//	�d�͉��Z�B
	m_movePos.y -= GRAVITY;

	//	�p�x�C���B
	Angle_Modification();

	//	�p�x���������B
	m_moveRot.y += (m_purposeRot - m_moveRot.y) * m_RotInertia;

	m_Rot.y = CameraRot.y + D3DX_PI;

	//	�ړ��ʂ̊��������B
	m_movePos.x += (0.0f - m_movePos.x) * m_MoveInertia;
	m_movePos.z += (0.0f - m_movePos.z) * m_MoveInertia;

	//	���W�X�V�B
	m_Pos += m_movePos;

	//	���b�V���t�B�[���h�Ƃ̂����蔻��B
	HitTest_With_Mesh(pMeshField);
	//	�ړ��ʃ`�F�b�N�B
	Moving_Distance_Check();

	m_BoostEffect->SetPos(D3DXVECTOR3(m_Pos.x, m_Pos.y + 60.0f, m_Pos.z));

	Attack(pInput);

	if (m_LockOn == true) {
		EnemyLockOn();
	}

	//	���C�t�̍X�V�B
	m_Life->Update(this);

	//	���[�V�����X�V�B
	m_MotionManager->Update();
}

/******************************************************************************
	�֐��� : void CPlayer::Draw(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �`�揈���B
******************************************************************************/
void CPlayer::Draw(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//	�s�񎮂Ɏg����Ɨp�ϐ�
	D3DXMATRIX mtxScl, mtxRot, mtxTrans;

	//	���[���h�}�g���N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//	�X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, m_Scl.x, m_Scl.y, m_Scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	//	��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//	�ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	//	���f���`��J�n�B
	m_MotionManager->Draw();
}

/******************************************************************************
	�֐��� : CPlayer * CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
	����   : pos, rot
	�߂�l : �Ȃ�
	����   : �����֐��B
******************************************************************************/
CPlayer * CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer *obj = new CPlayer;
	obj->Init(pos, rot);
	return obj;
}

/******************************************************************************
	�֐��� : void CPlayer::PlayerMoveUpdate(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �ړ��֘A�̏������܂Ƃ߂��֐��B
******************************************************************************/
void CPlayer::MoveUpdate(void)
{
	// ���ꂼ��̃|�C���^�ƒl���擾
	CManager    *pManager = GetManager();
	CInput      *pInput = pManager->GetInput();
	CGame		*pGame = (CGame *)CManager::GetMode();
	D3DXVECTOR3  CameraRot = pGame->GetCamera()->GetCameraRot();
	CCamera		*pCamera = pGame->GetCamera();

	bool button_push = false;

	if (pInput->GetJoyStickConnected() == true) {
		MoveUpdateJoyStick();
		return;
	}

	//	W�L�[��������Ă���
	if (pInput->GetKeyboardPress(DIK_W) == true) {
		//	A�L�[��������Ă���
		if (pInput->GetKeyboardPress(DIK_A) == true) {
			//�J�����������Ă�������ɑ΂��Ă܂������O�ɐi��
			m_movePos.x += sinf((-D3DX_PI * 0.75f) - CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf((-D3DX_PI * 0.75f) - CameraRot.y) * m_MoveSpeed;

			//	�ڕW�̊p�x�ݒ�
			m_purposeRot = (D3DX_PI * 0.75f) - CameraRot.y;
			pCamera->SetVecU(-0.5f);
		}
		//	D�L�[��������Ă���
		if (pInput->GetKeyboardPress(DIK_D) == true) {
			//�J�����������Ă�������ɑ΂��Ă܂������O�ɐi��
			m_movePos.x += sinf((D3DX_PI * 0.75f) - CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf((D3DX_PI * 0.75f) - CameraRot.y) * m_MoveSpeed;

			//	�ڕW�̊p�x�ݒ�
			m_purposeRot = (-D3DX_PI * 0.75f) - CameraRot.y;
			pCamera->SetVecU(0.5f);
		}
		//	W�L�[����������Ă���
		if (pInput->GetKeyboardPress(DIK_D) == false && pInput->GetKeyboardPress(DIK_A) == false) {
			//�J�����������Ă�������ɑ΂��Ă܂������O�ɐi��
			m_movePos.x += sinf(D3DX_PI - CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf(D3DX_PI - CameraRot.y) * m_MoveSpeed;

			//	�ڕW�̊p�x�ݒ�
			m_purposeRot = D3DX_PI + CameraRot.y;
		}
		m_Move = true;
		button_push = true;
	}
	if (pInput->GetKeyboardPress(DIK_S) == true) {

		//	A�L�[��������Ă���
		if (pInput->GetKeyboardPress(DIK_A) == true) {
			//�J�����������Ă�������ɑ΂��Ă܂������O�ɐi��
			m_movePos.x += sinf((-D3DX_PI * 0.25f) - CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf((-D3DX_PI * 0.25f) - CameraRot.y) * m_MoveSpeed;

			//	�ڕW�̊p�x�ݒ�
			m_purposeRot = (D3DX_PI * 0.25f) - CameraRot.y;
			pCamera->SetVecU(-0.5f);
		}
		//	D�L�[��������Ă���
		if (pInput->GetKeyboardPress(DIK_D) == true) {
			//�J�����������Ă�������ɑ΂��Ă܂������O�ɐi��
			m_movePos.x += sinf((D3DX_PI * 0.25f) - CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf((D3DX_PI * 0.25f) - CameraRot.y) * m_MoveSpeed;

			//	�ڕW�̊p�x�ݒ�
			m_purposeRot = (-D3DX_PI * 0.25f) - CameraRot.y;
			pCamera->SetVecU(0.5f);
		}
		//	S�L�[����������Ă���
		if (pInput->GetKeyboardPress(DIK_D) == false && pInput->GetKeyboardPress(DIK_A) == false) {
			//�J�����������Ă�������ɑ΂��Ă܂�������O�i��
			m_movePos.x += sinf(-CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf(-CameraRot.y) * m_MoveSpeed;

			//	�ڕW�̊p�x�ݒ�
			m_purposeRot = D3DX_PI + CameraRot.y;
		}
		m_Move = true;
		button_push = true;
	}
	if ((pInput->GetKeyboardPress(DIK_S) == false && pInput->GetKeyboardPress(DIK_W) == false) && pInput->GetKeyboardPress(DIK_A) == true) {
		//�J�����������Ă�������ɑ΂��č��ɒ��p�I�ɐi��
		m_movePos.x += sinf((-D3DX_PI * 0.5f) - CameraRot.y) * m_MoveSpeed;
		m_movePos.z -= cosf((-D3DX_PI * 0.5f) - CameraRot.y) * m_MoveSpeed;

		//	�ڕW�̊p�x�ݒ�
		m_purposeRot = (D3DX_PI * 0.5f) + CameraRot.y;
		pCamera->SetVecU(0.01f);

		button_push = true;
		m_Move = true;
	}
	if ((pInput->GetKeyboardPress(DIK_S) == false && pInput->GetKeyboardPress(DIK_W) == false) && pInput->GetKeyboardPress(DIK_D) == true) {
		//�J�����������Ă�������ɑ΂��ĉE�ɒ��p�I�ɐi��
		m_movePos.x += sinf((D3DX_PI * 0.5f) - CameraRot.y) * m_MoveSpeed;
		m_movePos.z -= cosf((D3DX_PI * 0.5f) - CameraRot.y) * m_MoveSpeed;

		//	�ڕW�̊p�x�ݒ�
		m_purposeRot = (-D3DX_PI * 0.5f) + CameraRot.y;
		pCamera->SetVecU(-0.01f);

		button_push = true;
		m_Move = true;
	}

	if (m_Jump == false && m_Move) {
		m_MotionManager->SetMotion(MOTION_MODE_MOVE);
	}

	if (button_push == false) {
		pCamera->SetVecU(0.0f);
	}
}

/******************************************************************************
	�֐��� : void CPlayer::MoveUpdateJoyStick(void)
	����   : void
	�߂�l : �Ȃ�
	����   : JoyStick���g�����ړ������B
******************************************************************************/
void CPlayer::MoveUpdateJoyStick(void)
{
	// ���ꂼ��̃|�C���^�ƒl���擾
	CManager    *pManager = GetManager();
	CInput      *pInput = pManager->GetInput();
	CGame		*pGame = (CGame *)CManager::GetMode();
	D3DXVECTOR3  CameraRot = pGame->GetCamera()->GetCameraRot();
	CCamera		*pCamera = pGame->GetCamera();
	XINPUT_STATE *state = pInput->GetPressState();

	bool button_push = false;
	m_Move = true;
	button_push = true;

	if (state->Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
		pCamera->AddCameraRotY(0.03f);
	}
	else if (state->Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
		pCamera->AddCameraRotY(-0.03f);
	}

	if (state->Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
		pCamera->AddCameraRotX(-0.01f);
	}
	else if (state->Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
		pCamera->AddCameraRotX(0.01f);
	}

	//	W�L�[��������Ă���
	if (state->Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
		//	A�L�[��������Ă���
		if (state->Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
			//�J�����������Ă�������ɑ΂��Ă܂������O�ɐi��
			m_movePos.x += sinf((-D3DX_PI * 0.75f) - CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf((-D3DX_PI * 0.75f) - CameraRot.y) * m_MoveSpeed;

			//	�ڕW�̊p�x�ݒ�
			m_purposeRot = (D3DX_PI * 0.75f) - CameraRot.y;
			pCamera->SetVecU(-0.5f);
		}
		//	D�L�[��������Ă���
		else if (state->Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
			//�J�����������Ă�������ɑ΂��Ă܂������O�ɐi��
			m_movePos.x += sinf((D3DX_PI * 0.75f) - CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf((D3DX_PI * 0.75f) - CameraRot.y) * m_MoveSpeed;

			//	�ڕW�̊p�x�ݒ�
			m_purposeRot = (-D3DX_PI * 0.75f) - CameraRot.y;
			pCamera->SetVecU(0.5f);
		}
		//	W�L�[����������Ă���
		else if (state->Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
			//�J�����������Ă�������ɑ΂��Ă܂������O�ɐi��
			m_movePos.x += sinf(D3DX_PI - CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf(D3DX_PI - CameraRot.y) * m_MoveSpeed;

			//	�ڕW�̊p�x�ݒ�
			m_purposeRot = D3DX_PI + CameraRot.y;
		}

		m_Move = true;
		button_push = true;
	}
	if (state->Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {

		//	A�L�[��������Ă���
		if (state->Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
			//�J�����������Ă�������ɑ΂��Ă܂������O�ɐi��
			m_movePos.x += sinf((-D3DX_PI * 0.25f) - CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf((-D3DX_PI * 0.25f) - CameraRot.y) * m_MoveSpeed;

			//	�ڕW�̊p�x�ݒ�
			m_purposeRot = (D3DX_PI * 0.25f) - CameraRot.y;
			pCamera->SetVecU(-0.5f);
		}
		//	D�L�[��������Ă���
		else if (state->Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
			//�J�����������Ă�������ɑ΂��Ă܂������O�ɐi��
			m_movePos.x += sinf((D3DX_PI * 0.25f) - CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf((D3DX_PI * 0.25f) - CameraRot.y) * m_MoveSpeed;

			//	�ڕW�̊p�x�ݒ�
			m_purposeRot = (-D3DX_PI * 0.25f) - CameraRot.y;
			pCamera->SetVecU(0.5f);
		}
		//	S�L�[����������Ă���
		else if (state->Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
			//�J�����������Ă�������ɑ΂��Ă܂�������O�i��
			m_movePos.x += sinf(-CameraRot.y) * m_MoveSpeed;
			m_movePos.z -= cosf(-CameraRot.y) * m_MoveSpeed;

			//	�ڕW�̊p�x�ݒ�
			m_purposeRot = D3DX_PI + CameraRot.y;
		}
		m_Move = true;
		button_push = true;
	}
	if ((state->Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && 
		 state->Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		 state->Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
		//�J�����������Ă�������ɑ΂��č��ɒ��p�I�ɐi��
		m_movePos.x += sinf((-D3DX_PI * 0.5f) - CameraRot.y) * m_MoveSpeed;
		m_movePos.z -= cosf((-D3DX_PI * 0.5f) - CameraRot.y) * m_MoveSpeed;

		//	�ڕW�̊p�x�ݒ�
		m_purposeRot = (D3DX_PI * 0.5f) + CameraRot.y;
		pCamera->SetVecU(0.01f);

		button_push = true;
		m_Move = true;
	}
	if ((state->Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && 
		 state->Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) && 
		 state->Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
		//�J�����������Ă�������ɑ΂��ĉE�ɒ��p�I�ɐi��
		m_movePos.x += sinf((D3DX_PI * 0.5f) - CameraRot.y) * m_MoveSpeed;
		m_movePos.z -= cosf((D3DX_PI * 0.5f) - CameraRot.y) * m_MoveSpeed;

		//	�ڕW�̊p�x�ݒ�
		m_purposeRot = (-D3DX_PI * 0.5f) + CameraRot.y;
		pCamera->SetVecU(-0.01f);

		button_push = true;
		m_Move = true;
	}

	if (m_Jump == false && m_Move) {
		m_MotionManager->SetMotion(MOTION_MODE_MOVE);
	}

	if (button_push == false) {
		pCamera->SetVecU(0.0f);
	}
}

/******************************************************************************
	�֐��� : void CPlayer::HitTest_With_Mesh(CMeshField *mesh)
	����   : mesh
	�߂�l : �Ȃ�
	����   : ���b�V���t�B�[���h�Ƃ̂����蔻��������Ƃ��ɁA
			 ���b�V���t�B�[���h��艺�ɂ����ꍇ�̂݁A���߂�Y���W��
			 ������Y���W�ɑ������֐��B
			 �����Ēn�ʂɂ����ۂɒn�ʂ̖@�����擾���ăv���C���[�̈ړ��ʂ�
			 �|�����킹�鏈���B
******************************************************************************/
void CPlayer::HitTest_With_Mesh(CMeshField *mesh)
{
	D3DXVECTOR3 fieldNormal = mesh->GetFieldNormal(m_Pos);

	//	���b�V���t�B�[���h��ł�Y���W�擾�B
	float posY = mesh->Collision(m_Pos);
	//	���߂��l���Y���W���Ⴉ��������W�X�V���āA�W�����v�t���O��off�ɖ߂��B
	if (posY >= m_Pos.y) {
		m_Pos.y = posY;
		m_Jump = false;
		if (m_Move == false) {
			m_MotionManager->SetMotion(MOTION_MDOE_NEUTRAL);
		}
	}

	//	�W�����v������Ȃ�������ړ��ɕ␳��������B
	if (m_Jump == false) {
		//	x�l�̕␳�B
		if (fieldNormal.x > 0.5f || fieldNormal.x < -0.5f)
			m_movePos.x += fieldNormal.x;
		//	z�l�̕␳�B
		if (fieldNormal.z > 0.5f || fieldNormal.z < -0.5f)
			m_movePos.z += fieldNormal.z;
	}

	if (m_Pos.x > mesh->GetFieldWidth() - 100.0f) {
		m_Pos.x = mesh->GetFieldWidth() - 100.0f;
	}

	if (m_Pos.x < -mesh->GetFieldWidth() + 100.0f) {
		m_Pos.x = -mesh->GetFieldWidth() + 100.0f;
	}

	if (m_Pos.z > mesh->GetFieldDepth() - 100.0f) {
		m_Pos.z = mesh->GetFieldDepth() - 100.0f;
	}

	if (m_Pos.z < -mesh->GetFieldDepth() + 100.0f) {
		m_Pos.z = -mesh->GetFieldDepth() + 100.0f;
	}
}

/******************************************************************************
	�֐��� : void CPlayer::Angle_Modification(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �p�x���C������֐��B
******************************************************************************/
void CPlayer::Angle_Modification(void)
{
	//-3.14����3.14�̗̈�ɓ��������̏���
	if (m_moveRot.y - m_purposeRot < -D3DX_PI)
		m_moveRot.y = D3DX_PI + (m_moveRot.y + D3DX_PI);

	//3.14����-3.14�̗̈�ɓ��������̏���
	if (D3DX_PI < m_moveRot.y - m_purposeRot)
		m_moveRot.y = -D3DX_PI + (m_moveRot.y - D3DX_PI);
}

/******************************************************************************
	�֐��� : void CPlayer::Moving_Distance_Check(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �p�x���C������֐��B
******************************************************************************/
void CPlayer::Moving_Distance_Check(void)
{
	if (m_Jump == false) {
		//	�ړ��ʂ����l�ȉ���������ړ��t���O��off�ɂ���B
		if (PLAYER_MOVEMENT_LOWEST_VALUE > m_movePos.x && -PLAYER_MOVEMENT_LOWEST_VALUE < m_movePos.x) {
			if (PLAYER_MOVEMENT_LOWEST_VALUE > m_movePos.z && -PLAYER_MOVEMENT_LOWEST_VALUE < m_movePos.z) {
				m_Move = false;		//	�ړ��t���O��off�ɂ���B
				m_MotionManager->SetMotion(MOTION_MDOE_NEUTRAL);
			}
			else {
				m_Move = true;			//	�ړ��t���O��on�ɂ���B
			}
		}
		else {
			m_Move = true;		//	�ړ��t���O��on�ɂ���B
		}
	}
}

/******************************************************************************
	�֐��� : void CPlayer::Jump_Check(CInput *input)
	����   : input
	�߂�l : �Ȃ�
	����   : �W�����v�{�^���������Ă��邩�ǂ����𒲂ׂ�֐��B
			 �����Ă�����W�����v���āA�W�����v���ɂ܂������ꂽ��
			 �G�A�u�[�X�g��Ԃɂ���B
			 �G�A�u�[�X�g���Ă���Ƃ��ɂ�����x�����ƃG�A�u�[�X�g�����������B
******************************************************************************/
void CPlayer::Jump_Check(CInput *input)
{
	//	�X�y�[�X�L�[�������ꂽ�Ƃ��ɃW�����v���ĂȂ�������W�����v�B
	if (input->GetKeyboardTrigger(DIK_SPACE) == true || 
		input->TriggerJoyStick(XINPUT_GAMEPAD_A) == true) {
		if (m_Jump == false) {
			m_Jump = true;
			m_movePos.y = JUMP_POWER;
			m_MotionManager->SetMotion(MOTION_MODE_JUMP);
		}
		else {		//	���łɃW�����v���Ă�����G�A�u�[�X�g��ԂɁB���łɂ��Ă������������B
			if (input->GetKeyboardTrigger(DIK_SPACE) == true) {
				m_AirBoost = m_AirBoost ? false : true;
			}
		}
	}
}

/******************************************************************************
	�֐��� : void CPlayer::Attack(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �U������ۂ̏������������֐��B
******************************************************************************/
void CPlayer::Attack(CInput *input)
{
	// ���ꂼ��̃|�C���^�ƒl���擾
	CManager *pManager = GetManager();
	CGame *pGame = (CGame *)CManager::GetMode();
	D3DXVECTOR3 CameraRot = pGame->GetCamera()->GetCameraRot();

	//	�e���ˁB
	if (input->GetMouseTrigger(MOUSE_INPUT_LEFT) == true ||
		input->TriggerJoyStick(XINPUT_GAMEPAD_RIGHT_SHOULDER) == true) {
		CBullet::Create(D3DXVECTOR3(m_Pos.x, m_Pos.y + 30.0f, m_Pos.z), D3DXVECTOR3(CameraRot.x, m_Rot.y, m_Rot.z), 20.0f, 20.0f, CTexture::G_BULLET, m_Objtype);
	}

	if (input->GetKeyboardTrigger(DIK_Q) == true ||
		input->TriggerJoyStick(XINPUT_GAMEPAD_LEFT_SHOULDER) == true) {
		m_LockOn = m_LockOn == true ? false : true;
	}
}

/******************************************************************************
	�֐��� : void CPlayer::Boost(CInput * input)
	����   : input
	�߂�l : �Ȃ�
	����   : �u�[�X�g�����B
******************************************************************************/
void CPlayer::Boost(CInput * input)
{
	//	�}�E�X�̉E�N���b�N�������ꂽ��u�[�X�g�����B
	if (input->GetMouseTrigger(MOUSE_INPUT_RIGHT) == true || 
		input->TriggerJoyStick(XINPUT_GAMEPAD_X)) {
		if (m_Boost == false) {
			m_Boost = true;
			m_movePos.x *= m_MoveSpeed * BOOST_MAGNIFICATION;
			m_movePos.z *= m_MoveSpeed * BOOST_MAGNIFICATION;
			m_BoostEffect->SetDuringUse(true);
		}
	}
	//	�u�[�X�g���Ă���N�[���_�E���`�F�b�N�B
	if (m_Boost == true) {
		m_CoolingTimeBoost = (m_CoolingTimeBoost + 1) % BOOST_COOLING_TIME;
		m_Boost = m_CoolingTimeBoost == 0 ? false : true;
	}
}

/******************************************************************************
	�֐��� : void CPlayer::CollisionCheck(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �����蔻��̔r�ˏ����B
******************************************************************************/
void CPlayer::CollisionCheck(void)
{
	//	�����ɂȂɂ�����������B
	if (m_pCollision->GetHit()) {
		CCollisionList *collicionList = m_pCollision->GetCollisionList();
		std::list<CCollision *> *collList = collicionList->GetCollision();

		for (auto i = collList->begin(); i != collList->end();) {
			CCollision *object = (*i);

			if (object != NULL) {
				if (object->GetScene()->GetObjectType() == OBJ_TYPE_BULLET) {
					CBullet *bullet = (CBullet *)object->GetScene();

					//	�������Ώۂ�Player��������B
					if (bullet->GetTypeUsingSelf() == OBJ_TYPE_ENEMY) {
						//	HP�������B
						--m_Hp;
						CExplosion::Create(object->GetScene()->GetPos(), 125.0f, 125.0f, CTexture::G_EXPLOSION);
						object->GetScene()->SetDelete(true);
					}
				}
				//	���X�g�̂Ȃ��ݍ폜�B
				i = collList->erase(i);
			}
		}
	}
}

/******************************************************************************
	�֐��� : void CPlayer::EnemyLockOn(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �G�̃��b�N�I�������B
******************************************************************************/
void CPlayer::EnemyLockOn(void)
{
	// ���ꂼ��̃|�C���^�ƒl���擾
	CManager *pManager = GetManager();
	CGame *pGame = (CGame *)CManager::GetMode();
	CCamera *camera = pGame->GetCamera();

	CScene *pScene = CScene::GetListTopPointer(CScene::OBJECT_ENEMY);
	CScene *pSceneNext;
	
	float distance = 0.0f;
	float minLength = CERTAIN_DISTANCE;
	float angle = 0.0f;
	float rotX = 0.0f;
	float lnegthX, lengthY;
	int cnt = 0;
	bool lockOn = false;
	D3DXVECTOR3 targetPos = m_TargetPos;

	//	null����Ȃ���Ή񂷁B
	while (pScene != null) {
		//	���̃|�C���^�擾�B
		pSceneNext = pScene->GetSceneNext();
		//	�^�C�v��OBJ_TYPE_ENEMY�����������B
		if (pScene->GetObjectType() == CScene::OBJ_TYPE_ENEMY) {
			if (pScene->GetDeleteFlag() == false) {
				distance = sqrtf(((pScene->GetPos().x - m_Pos.x) * (pScene->GetPos().x - m_Pos.x)) +
								 ((pScene->GetPos().y - m_Pos.y) * (pScene->GetPos().y - m_Pos.y)) +
								 ((pScene->GetPos().z - m_Pos.z) * (pScene->GetPos().z - m_Pos.z)));

				if (distance < minLength || cnt == 0) {
					if (distance < CERTAIN_DISTANCE) {
						minLength = distance;
						m_TargetPos = pScene->GetPos();
						lockOn = true;
					}
				}
			}
		}
		//	���̃|�C���^�ݒ�B
		pScene = pSceneNext;
		++cnt;
	}

	if (lockOn == false) {
		if (targetPos == m_TargetPos) {
			m_LockOn = false;
			return;
		}
	}

	//	�������擾�B
	lnegthX = sqrt(powf(m_Pos.x - m_TargetPos.x, 2) + powf(m_Pos.z - m_TargetPos.z, 2));
	lengthY = (m_Pos.y + 50.0f) - (m_TargetPos.y + 30.0f);

	//	���߂������Ŕ��ˊp�x�v�Z�B
	rotX = atan2f(lengthY, lnegthX);

	angle = MATH_ANGLE(m_TargetPos.x - m_Pos.x, m_TargetPos.z - m_Pos.z);
	camera->SetPosR(m_TargetPos);
	camera->SetCameraRotY(angle);
	camera->SetCameraRotX(rotX);
}
