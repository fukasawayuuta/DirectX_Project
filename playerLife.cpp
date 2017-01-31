/******************************************************************************
	�^�C�g���� : �v���C���[���C�t
	�t�@�C���� : playerLife.cpp
	�쐬��     : AT-13C-284 35 �[�V�C��
	�쐬��     : 2017/01/19
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
#include "texture.h"
#include "model.h"
#include "motionManager.h"
#include "DynamicModel.h"
#include "player.h"
#include "PlayerLife.h"
#include "Mode.h"
#include "game.h"
#include "playerLife.h"

/******************************************************************************
	�}�N���錾
******************************************************************************/
const float MOVE_INETIA = 0.2f;		//	���C�t�o�[�̓����̊����l�B

/******************************************************************************
	�֐��� : CPlayerLife::CPlayerLife(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene2D(Priority, objType, sceneType)
	����   : �f�X�g���N�^
******************************************************************************/
CPlayerLife::CPlayerLife(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene2D(Priority, objType, sceneType)
{
	m_PlayerMaxHP = 0;
	m_MoveWidth = 0.0f;
	m_FixedWidth = 0.0f;
}

/******************************************************************************
	�֐��� : CPlayerLife::~CPlayerLife()
	����   : �f�X�g���N�^
******************************************************************************/
CPlayerLife::~CPlayerLife()
{
}

/******************************************************************************
	�֐��� : void CPlayerLife::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �A�j���[�V�����̍X�V�B
******************************************************************************/
void CPlayerLife::Init(D3DXVECTOR3 pos, float width, float height, int lifeTex, int barTex, int hp)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	VERTEX_2D *pVtx;

	//	�ϐ��̑���B
	m_Pos = pos;
	m_Width = width;
	m_Height = height;
	m_LifeTexture[TEX_LIFE] = lifeTex;
	m_LifeTexture[TEX_LIFE_BAR] = barTex;
	m_PlayerMaxHP = hp;
	m_FixedWidth = width;

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (4 * TEX_MAX), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffPolygon, NULL))) {
		return;
	}

	//���_���ւ̃|�C���^���擾
	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < TEX_MAX; i++, pVtx += 4) {
		//	���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(m_Pos.x, m_Pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_Pos.x, m_Pos.y + m_Height, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y + m_Height, 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//�e�N�X�`�����W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//	�F�ݒ�
		pVtx[0].col = m_Color;
		pVtx[1].col = m_Color;
		pVtx[2].col = m_Color;
		pVtx[3].col = m_Color;
	}

	//	�o�b�t�@���
	m_pVtxBuffPolygon->Unlock();
}

/******************************************************************************
	�֐��� : void CPlayerLife::Update(CPlayer *player)
	����   : player
	�߂�l : �Ȃ�
	����   : �A�j���[�V�����̍X�V�B
******************************************************************************/
void CPlayerLife::Update(CPlayer *player)
{
	VERTEX_2D *pVtx;

	m_Width = ((float)player->GetHP() / m_PlayerMaxHP) * m_FixedWidth;
	m_MoveWidth += (m_Width - m_MoveWidth) * MOVE_INETIA;

	//���_���ւ̃|�C���^���擾
	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//	���W�ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_Pos.x, m_Pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_Pos.x + m_MoveWidth, m_Pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_Pos.x, m_Pos.y + m_Height, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_Pos.x + m_MoveWidth, m_Pos.y + m_Height, 0.0f);

	//	�o�b�t�@���
	m_pVtxBuffPolygon->Unlock();
}

/******************************************************************************
	�֐��� : void CPlayerLife::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �`�揈���B
******************************************************************************/
void CPlayerLife::Draw(void)
{
	//	�f�o�C�X�擾
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

	//���_���W�ӂ��[�܂���
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < TEX_MAX; i++) {
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, CTexture::GetTexture(m_SceneType, m_LifeTexture[i]));
		//�|���S���`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}
}

/******************************************************************************
	�֐��� : CPlayerLife * CPlayerLife::Create(D3DXVECTOR3 pos, float width, float height, int lifeTex, int barTex)
	����   : pos, width, height, lifeTex, barTex
	�߂�l : �Ȃ�
	����   : �����֐��B
******************************************************************************/
CPlayerLife * CPlayerLife::Create(D3DXVECTOR3 pos, float width, float height, int lifeTex, int barTex, int hp)
{
	CPlayerLife *obj = new CPlayerLife;
	obj->Init(pos, width, height, lifeTex, barTex, hp);
	return obj;
}
