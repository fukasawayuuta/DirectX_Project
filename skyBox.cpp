/******************************************************************************
	�^�C�g���� : SkyBox�N���X
	�t�@�C���� : skyBox.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2017/01/17
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
#include "scene3D.h"
#include "sceneX.h"
#include "billboard.h"
#include "model.h"
#include "motionManager.h"
#include "dynamicModel.h"
#include "skyBox.h"
#include "Mode.h"
#include "game.h"
#include "player.h"


/******************************************************************************
	�}�N����`
******************************************************************************/
const int FACE_NUM = 6;		//	�ʂ̐��B

/******************************************************************************
	�֐��� : CSkyBox::CSkyBox(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CDynamicModel(Priority, objType, sceneType)
	����   : �R���X�g���N�^
******************************************************************************/
CSkyBox::CSkyBox(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene3D(Priority, objType, sceneType)
{
}

/******************************************************************************
	�֐��� : CSkyBox::~CSkyBox()
	����   : �f�X�g���N�^
******************************************************************************/
CSkyBox::~CSkyBox()
{
}

/******************************************************************************
	�֐��� : void CSkyBox::Init(D3DXVECTOR3 pos, float width, float height, float depth, int textureType)
	����   : pos, width, height, depth, textureType
	�߂�l : �Ȃ�
	����   : ���W�Ɗp�x�̐ݒ�B
******************************************************************************/
void CSkyBox::Init(D3DXVECTOR3 pos, float width, float height, float depth, int textureType)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	VERTEX_3D *pVtx;

	//	�����ݒ�B
	m_Pos = pos;
	m_Width = width;
	m_Height = height;
	m_Depth = depth;
	m_TextureType = textureType;

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (4 * FACE_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffPolygon, NULL))) {
		return;
	}

	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//	�����̖�////////////////////////////////////////////////////////////////////////
	pVtx[0].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y + m_Height, m_Pos.z - m_Depth);
	pVtx[1].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y + m_Height, m_Pos.z + m_Depth);
	pVtx[2].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y - m_Height, m_Pos.z - m_Depth);
	pVtx[3].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y - m_Height, m_Pos.z + m_Depth);
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f / 3.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f, 1.0f / 3.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, (1.0f * 2.0f) / 3.0f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, (1.0f * 2.0f) / 3.0f);

	//	�E���̖�////////////////////////////////////////////////////////////////////////
	pVtx[4].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y + m_Height, m_Pos.z + m_Depth);
	pVtx[5].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y + m_Height, m_Pos.z - m_Depth);
	pVtx[6].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y - m_Height, m_Pos.z + m_Depth);
	pVtx[7].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y - m_Height, m_Pos.z - m_Depth);
	pVtx[4].tex = D3DXVECTOR2(0.5f, 1.0f / 3.0f);
	pVtx[5].tex = D3DXVECTOR2(0.75f, 1.0f / 3.0f);
	pVtx[6].tex = D3DXVECTOR2(0.5f, (1.0f * 2.0f) / 3.0f);
	pVtx[7].tex = D3DXVECTOR2(0.75f, (1.0f * 2.0f) / 3.0f);

	//	��O���̖�////////////////////////////////////////////////////////////////////////
	pVtx[8].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y + m_Height, m_Pos.z - m_Depth);
	pVtx[9].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y + m_Height, m_Pos.z - m_Depth);
	pVtx[10].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y - m_Height, m_Pos.z - m_Depth);
	pVtx[11].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y - m_Height, m_Pos.z - m_Depth);
	pVtx[8].tex = D3DXVECTOR2(0.75f, 1.0f / 3.0f);
	pVtx[9].tex = D3DXVECTOR2(1.0f, 1.0f / 3.0f);
	pVtx[10].tex = D3DXVECTOR2(0.75f, (1.0f * 2.0f) / 3.0f);
	pVtx[11].tex = D3DXVECTOR2(1.0f, (1.0f * 2.0f) / 3.0f);

	//	�����̖�////////////////////////////////////////////////////////////////////////
	pVtx[12].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y + m_Height, m_Pos.z + m_Depth);
	pVtx[13].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y + m_Height, m_Pos.z + m_Depth);
	pVtx[14].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y - m_Height, m_Pos.z + m_Depth);
	pVtx[15].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y - m_Height, m_Pos.z + m_Depth);
	pVtx[12].tex = D3DXVECTOR2(0.25f, 1.0f / 3.0f);
	pVtx[13].tex = D3DXVECTOR2(0.5f, 1.0f / 3.0f);
	pVtx[14].tex = D3DXVECTOR2(0.25f, (1.0f * 2.0f) / 3.0f);
	pVtx[15].tex = D3DXVECTOR2(0.5f, (1.0f * 2.0f) / 3.0f);

	//	�㑤�̖�////////////////////////////////////////////////////////////////////////
	pVtx[16].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y + m_Height, m_Pos.z - m_Depth);
	pVtx[17].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y + m_Height, m_Pos.z - m_Depth);
	pVtx[18].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y + m_Height, m_Pos.z + m_Depth);
	pVtx[19].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y + m_Height, m_Pos.z + m_Depth);
	pVtx[16].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[17].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[18].tex = D3DXVECTOR2(0.25f, 1.0f / 3.0f);
	pVtx[19].tex = D3DXVECTOR2(0.5f, 1.0f / 3.0f);

	//	�����̖�////////////////////////////////////////////////////////////////////////
	pVtx[20].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y - m_Height, m_Pos.z + m_Depth);
	pVtx[21].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y - m_Height, m_Pos.z + m_Depth);
	pVtx[22].pos = D3DXVECTOR3(m_Pos.x - m_Width, m_Pos.y - m_Height, m_Pos.z - m_Depth);
	pVtx[23].pos = D3DXVECTOR3(m_Pos.x + m_Width, m_Pos.y - m_Height, m_Pos.z - m_Depth);
	pVtx[20].tex = D3DXVECTOR2(0.25f, (1.0f * 2.0f) / 3.0f);
	pVtx[21].tex = D3DXVECTOR2(0.5f, (1.0f * 2.0f) / 3.0f);
	pVtx[22].tex = D3DXVECTOR2(0.25f, 1.0f);
	pVtx[23].tex = D3DXVECTOR2(0.5f, 1.0f);

	for (int i = 0; i < FACE_NUM; i++, pVtx += 4) {
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//�|���S���̒��_�J���[�ݒ�
		pVtx[0].col = m_Color;
		pVtx[1].col = m_Color;
		pVtx[2].col = m_Color;
		pVtx[3].col = m_Color;
	}

	m_pVtxBuffPolygon->Unlock();
}

/******************************************************************************
	�֐��� : void CSkyBox::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �X�V�֐��B
******************************************************************************/
void CSkyBox::Update(void)
{
	CManager *pManager = GetManager();
	CInput   *pInput = pManager->GetInput();
	CGame	 *pGame = (CGame *)CManager::GetMode();
	CPlayer  *pPlayer = pGame->GetPlayer();
	m_Rot.y += 0.0001f;
	m_Pos = pPlayer->GetPos();
}

/******************************************************************************
	�֐��� : void CSkyBox::Draw(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �`��֐��B
******************************************************************************/
void CSkyBox::Draw(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�s�񎮂Ɏg����Ɨp�ϐ�
	D3DXMATRIX mtxScl, mtxRot, mtxTrans;

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		//	���C�e�B���O : OFF

	//���[���h�}�g���N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXMatrixScaling(&mtxScl, m_Scl.x, m_Scl.y, m_Scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);		//�X�P�[���𔽉f

	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);		//��]�𔽉f

	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);	//�ړ��𔽉f

																//	���[���h�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//	���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

	//	���_���W�ӂ��[�܂���
	pDevice->SetFVF(FVF_VERTEX_3D);
	//	�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CTexture::GetTexture(m_SceneType, m_TextureType));

	for (int i = 0; i < FACE_NUM; i++) {
		//	�|���S���`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);		//	���C�e�B���O : OFF
}

/******************************************************************************
	�֐��� : CSkyBox * CSkyBox::Create(D3DXVECTOR3 pos, float width, float height, float depth, int textureType)
	����   : pos, width, height, depth, textureType
	�߂�l : obj
	����   : �����֐��B
******************************************************************************/
CSkyBox * CSkyBox::Create(D3DXVECTOR3 pos, float width, float height, float depth, int textureType)
{
	CSkyBox *obj = new CSkyBox;
	obj->Init(pos, width, height, depth, textureType);
	return obj;
}
