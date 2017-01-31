/******************************************************************************
	�^�C�g���� : MeshField�N���X
	�t�@�C���� : MeshField.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/12/15
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
#include "sceneIndex.h"
#include "MeshField.h"

/******************************************************************************
	�֐��� : CMeshField::CMeshField(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CSceneIndex(Priority, objType, sceneType)
	����   : �R���X�g���N�^�B
******************************************************************************/
CMeshField::CMeshField(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CSceneIndex(Priority, objType, sceneType)
{
	m_SelectXIndex = 0;
	m_SelectZIndex = 0;
	m_SelectPolygonIndex = 0;
	m_SelectPolygonIndexX = 0;
	m_SelectPolygonIndexZ = 0;
	m_SelectBlockNum = 0;
}

/******************************************************************************
	�֐��� : CMeshField::~CMeshField()
	����   : �f�X�g���N�^�B
******************************************************************************/
CMeshField::~CMeshField()
{
}

/******************************************************************************
	�֐��� : void CMeshField::Init(const char *filename, int textureType)
	����   : filename, textureType
	�߂�l : �Ȃ�
	����   : �g���t�@�C����ǂݍ���ŁA�n�C�g�}�b�v��ǂݍ��ށB
******************************************************************************/
void CMeshField::Init(const char *filename, int textureType)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	VERTEX_3D *pVtx;
	WORD *pIdx;

	m_Field_Block_Num_Xline = 128;		//	X����̃u���b�N���B
	m_Field_Block_Num_Zline = 128;		//	Z����̃u���b�N���B
	m_Xline_Block_Between_Interval = 50.0f;		//	X����̒��_�Ԃ̊Ԋu�B
	m_Zline_Block_Between_Interval = 50.0f;		//	Z����̒��_�Ԃ̊Ԋu�B
	m_Field_Vertex_Num_Xline = m_Field_Block_Num_Xline + 1;		//	X����̒��_���B	
	m_Field_Vertex_Num_Zline = m_Field_Block_Num_Zline + 1;		//	Z����̒��_���B
	m_Width_Total_Field = (m_Field_Block_Num_Xline / 2) * m_Xline_Block_Between_Interval;		//	���b�V���t�B�[���h�̑S�̂̉����B
	m_Depth_Total_Field = (m_Field_Block_Num_Zline / 2) * m_Zline_Block_Between_Interval;		//	���b�V���t�B�[���h�̑S�̂̉��s�B
	m_VertexNum = m_Field_Vertex_Num_Xline * m_Field_Vertex_Num_Zline;		//	�S�̂̒��_���B
	m_PolygonNum = ((m_Field_Block_Num_Xline * m_Field_Block_Num_Zline) * 2) + ((m_Field_Block_Num_Zline - 1) * 4);		//	�S�̂̃|���S�����B
	m_Index_Total_Field = m_PolygonNum + 2;		//	�S�̂̃C���f�b�N�X���B
	m_TextureType = textureType;		//	�e�N�X�`���̃^�C�v

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_VertexNum, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffPolygon, NULL))) {
		return;
	}

	//�C���f�b�N�X�o�b�t�@�̊m��
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) *  m_Index_Total_Field, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL))) {
		return;
	}

	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 PosMeshField;

	//���W(0 , 0 , 0)�𒆐S�ɕ`��
	for (int z = 0; z < m_Field_Vertex_Num_Zline; z++) {
		for (int x = 0; x < m_Field_Vertex_Num_Xline; x++) {
			PosMeshField = D3DXVECTOR3(x * m_Xline_Block_Between_Interval - m_Xline_Block_Between_Interval * (m_Field_Block_Num_Xline * 0.5f),
									   0.0f,
									   -z * m_Zline_Block_Between_Interval + m_Zline_Block_Between_Interval * (m_Field_Block_Num_Zline * 0.5f));

			pVtx[x + (m_Field_Vertex_Num_Xline * z)].pos = PosMeshField;
			pVtx[x + (m_Field_Vertex_Num_Xline * z)].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[x + (m_Field_Vertex_Num_Xline * z)].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[x + (m_Field_Vertex_Num_Xline * z)].tex = D3DXVECTOR2((float)x, (float)z);
		}
	}

	m_pVtxBuffPolygon->Unlock();

	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	int ZIdxCnt = 0;
	int IdxNum = 0;
	int XLineMax = (m_Field_Vertex_Num_Xline * 2) - 1;		//	�E�[�̏k�ރ|���S���̂���C���f�b�N�X�����߂�
	int XLineMin = XLineMax + 2;						//	���[�̏k�ރ|���S���̂���C���f�b�N�X�����߂�

	for (int IdxCnt = 0; IdxCnt < m_Index_Total_Field; IdxCnt++, IdxNum++) {
		pIdx[IdxCnt] = IdxNum % m_Field_Vertex_Num_Xline + ((ZIdxCnt + 1) * m_Field_Vertex_Num_Xline);

		//	�����[�̏k�ރ|���S���̂���C���f�b�N�X��������
		if (IdxCnt == XLineMin) {
			XLineMin = XLineMax + 2;		//	���̍��[�̏k�ރ|���S���̂���C���f�b�N�X�����߂�
			pIdx[IdxCnt + 1] = pIdx[IdxCnt];
			IdxCnt++;
		}

		IdxCnt++;

		pIdx[IdxCnt] = IdxNum % m_Field_Vertex_Num_Xline + ZIdxCnt * m_Field_Vertex_Num_Xline;

		//	�����E�[�̏k�ރ|���S���̂���C���f�b�N�X���C���f�b�N�X���ő傶��Ȃ�������
		if (IdxCnt == XLineMax && XLineMax != m_Index_Total_Field - 1) {
			XLineMax += (m_Field_Block_Num_Xline + 2) * 2;		//	���̉E�[�̏k�ރ|���S���̂���C���f�b�N�X�����߂�

			++ZIdxCnt;
			++IdxCnt;

			pIdx[IdxCnt] = pIdx[IdxCnt - 1];
		}
	}

	//	�t�@�C����ǂݍ��߂���B
	if (LoadField(filename) != NULL) {
		//	�@�����X�V�B
		UpdateMeshNormal();
	}

	m_pIdxBuff->Unlock();
}

/******************************************************************************
	�֐��� : void CMeshField::Uninit(void)
	����   : void
	�߂�l : �Ȃ�
	����   : ��������B
******************************************************************************/
void CMeshField::Uninit(void)
{
	CSceneIndex::Uninit();
}

/******************************************************************************
	�֐��� : void CMeshField::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �X�V�����B
******************************************************************************/
void CMeshField::Update(void)
{
}

/******************************************************************************
	�֐��� : void CMeshField::Draw(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �`�揈���B
******************************************************************************/
void CMeshField::Draw(void)
{
	CSceneIndex::Draw();
}

/******************************************************************************
	�֐��� : D3DXVECTOR3 CMeshField::GetFieldNormal(D3DXVECTOR3 pos)
	����   : pos
	�߂�l : �Ȃ�
	����   : �Ώۂ�������W����@���x�N�g�����v�Z���A���ʂ�Ԃ��B
******************************************************************************/
D3DXVECTOR3 CMeshField::GetFieldNormal(D3DXVECTOR3 pos)
{
	m_SelectPolygonIndexX = (pos.x / m_Xline_Block_Between_Interval) + (m_Field_Block_Num_Xline * 0.5f);		//	�v���C���[������Ă���X���ɂ���u���_�̃C���f�b�N�X�𒲂ׂ�
	m_SelectPolygonIndexZ = (m_Field_Block_Num_Zline * 0.5f) - (pos.z / m_Zline_Block_Between_Interval);		//	�v���C���[������Ă���Z���ɂ���u���_�̃C���f�b�N�X�𒲂ׂ�
	m_SelectPolygonIndex = m_SelectPolygonIndexX + (m_Field_Vertex_Num_Xline * m_SelectPolygonIndexZ);			//	���߂�X��Z�Ƃ̃C���f�b�N�X���g���āA���Ԗڂ̒��_�̃C���f�b�N�X�����ׂ�
	m_SelectBlockNum = ((m_Field_Block_Num_Xline * 2) * m_SelectPolygonIndexZ) + (m_SelectPolygonIndexX * 2);	//	���߂�X��Z�Ƃ̃C���f�b�N�X���g���āA���ڂ̃|���S���ɏ���Ă���̂����ׂ�
	VERTEX_3D *pVtx;

	//	��Ɨp�ϐ�
	D3DXVECTOR3 vec1, vec2;
	D3DXVECTOR3 nor1, nor2, nor3;
	float fNor1;
	D3DXVECTOR3 Normal;

	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
	m_pVtxBuffPolygon->Unlock();

	//	�ŏ��̃x�N�g�������߂�
	vec1 = pVtx[m_SelectPolygonIndex].pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline + 1].pos;
	vec2 = pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline + 1].pos;

	//	�񎟌��̊O��
	fNor1 = vec2.x * vec1.z - vec2.z * vec1.x;
	//	�m�[�}���C�Y
	D3DXVec3Cross(&nor1, &vec1, &vec2);
	D3DXVec3Normalize(&nor1, &nor1);

	//	�l�p�`�|���S���̂ǂ����̃|���S���ɏ���Ă��邩�̔���
	if (fNor1 > 0) {
		//	�C���f�N�X�����Z
		m_SelectPolygonIndex += 1;

		vec1 = pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos - pVtx[m_SelectPolygonIndex].pos;
		vec2 = pVtx[m_SelectPolygonIndex - 1].pos - pVtx[m_SelectPolygonIndex].pos;

		D3DXVec3Cross(&nor1, &vec1, &vec2);
		D3DXVec3Normalize(&nor1, &nor1);
	}
	else {
		vec1 = pVtx[m_SelectPolygonIndex].pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos;
		vec2 = pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline + 1].pos - pVtx[m_SelectPolygonIndex].pos;

		D3DXVec3Cross(&nor1, &vec1, &vec2);
		D3DXVec3Normalize(&nor1, &nor1);
	}

	Normal = nor1;
	return Normal;
}

/******************************************************************************
	�֐��� : CMeshField * CMeshField::Create(const char * filename, int textureType)
	����   : filename, textureType
	�߂�l : obj
	����   : ���������B
******************************************************************************/
CMeshField * CMeshField::Create(const char * filename, int textureType)
{
	CMeshField *obj = new CMeshField;
	obj->Init(filename, textureType);
	return obj;
}

/******************************************************************************
	�֐��� : float CMeshField::Collision(D3DXVECTOR3 pos)
	����   : pos
	�߂�l : pos.y
	����   : �|���S���ɏ���Ă���Ƃ���Y���W�����߂Ė߂�l�ŕԂ��֐��B
******************************************************************************/
float CMeshField::Collision(D3DXVECTOR3 pos)
{
	m_SelectPolygonIndexX = (pos.x / m_Xline_Block_Between_Interval) + (m_Field_Block_Num_Xline * 0.5f);		//	�v���C���[������Ă���X���ɂ���u���_�̃C���f�b�N�X�𒲂ׂ�
	m_SelectPolygonIndexZ = (m_Field_Block_Num_Zline * 0.5f) - (pos.z / m_Zline_Block_Between_Interval);		//	�v���C���[������Ă���Z���ɂ���u���_�̃C���f�b�N�X�𒲂ׂ�
	m_SelectPolygonIndex = m_SelectPolygonIndexX + (m_Field_Vertex_Num_Xline * m_SelectPolygonIndexZ);			//	���߂�X��Z�Ƃ̃C���f�b�N�X���g���āA���Ԗڂ̒��_�̃C���f�b�N�X�����ׂ�
	m_SelectBlockNum = ((m_Field_Block_Num_Xline * 2) * m_SelectPolygonIndexZ) + (m_SelectPolygonIndexX * 2);	//	���߂�X��Z�Ƃ̃C���f�b�N�X���g���āA���ڂ̃|���S���ɏ���Ă���̂����ׂ�

	VERTEX_3D *pVtx;

	//	��Ɨp�ϐ�
	D3DXVECTOR3 vec1, vec2;
	D3DXVECTOR3 nor1, nor2, nor3;
	float fNor1;

	D3DXVECTOR3 mathPos;

	//	�͈͊O�Ȃ�NULL��Ԃ��B
	if (m_SelectPolygonIndex > m_Index_Total_Field || m_SelectPolygonIndex < 0) {
		return NULL;
	}

	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
	//	�ŏ��̃x�N�g�������߂�
	vec1 = pVtx[m_SelectPolygonIndex].pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline + 1].pos;
	vec2 = pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline + 1].pos;

	//	�񎟌��̊O��
	fNor1 = vec2.x * vec1.z - vec2.z * vec1.x;
	//	�m�[�}���C�Y
	D3DXVec3Cross(&nor1, &vec1, &vec2);
	D3DXVec3Normalize(&nor1, &nor1);

	//	�l�p�`�|���S���̂ǂ����̃|���S���ɏ���Ă��邩�̔���
	if (fNor1 > 0) {
		//	�C���f�N�X�����Z
		m_SelectPolygonIndex += 1;

		//	�x�N�g�����߂ĊO�όv�Z
		vec1 = pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos - pVtx[m_SelectPolygonIndex].pos;
		vec2 = pos - pVtx[m_SelectPolygonIndex].pos;
		//	�m�[�}���C�Y
		D3DXVec3Cross(&nor1, &vec1, &vec2);
		D3DXVec3Normalize(&nor1, &nor1);

		//	�x�N�g�����߂ĊO�όv�Z
		vec1 = pVtx[m_SelectPolygonIndex - 1].pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos;
		vec2 = pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos;
		//	�m�[�}���C�Y
		D3DXVec3Cross(&nor2, &vec1, &vec2);
		D3DXVec3Normalize(&nor2, &nor2);

		//	�x�N�g�����߂ĊO�όv�Z
		vec1 = pVtx[m_SelectPolygonIndex].pos - pVtx[m_SelectPolygonIndex - 1].pos;
		vec2 = pos - pVtx[m_SelectPolygonIndex - 1].pos;
		//	�m�[�}���C�Y
		D3DXVec3Cross(&nor3, &vec1, &vec2);
		D3DXVec3Normalize(&nor3, &nor3);

		//	�O�p�`�̒��ɓ����Ă��邩�ǂ���
		if (nor1 > 0 && nor2 > 0 && nor3 > 0) {		//	�����Ă�
			//	�x�N�g�����߂�
			vec1 = pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos - pVtx[m_SelectPolygonIndex].pos;
			vec2 = pVtx[m_SelectPolygonIndex - 1].pos - pVtx[m_SelectPolygonIndex].pos;

			//	�́[�܂炢��
			D3DXVec3Cross(&nor1, &vec1, &vec2);
			D3DXVec3Normalize(&nor1, &nor1);

			//	���������߂�
			mathPos.y = pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos.y - ((nor1.x * (pos.x - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos.x) +
				nor1.z * (pos.z - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos.z)) / nor1.y);

			pos.y = mathPos.y;
		}
		else {		//	�����ĂȂ�
			//	�v���C���[��Y���W�ɒ��ڑ��
			pos.y = 0.0f;
		}
	}
	else {
		//	�x�N�g�������߂�
		vec1 = pVtx[m_SelectPolygonIndex].pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos;
		vec2 = pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos;
		//	�O�ϋ��߂ăm�[�}���C�Y
		D3DXVec3Cross(&nor1, &vec1, &vec2);
		D3DXVec3Normalize(&nor1, &nor1);

		//	�x�N�g�������߂�
		vec1 = pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline + 1].pos - pVtx[m_SelectPolygonIndex].pos;
		vec2 = pos - pVtx[m_SelectPolygonIndex].pos;
		//	�O�ϋ��߂ăm�[�}���C�Y
		D3DXVec3Cross(&nor2, &vec1, &vec2);
		D3DXVec3Normalize(&nor2, &nor2);

		//	�x�N�g�������߂�
		vec1 = pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline + 1].pos;
		vec2 = pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos;
		//	�O�ϋ��߂ăm�[�}���C�Y
		D3DXVec3Cross(&nor3, &vec1, &vec2);
		D3DXVec3Normalize(&nor3, &nor3);

		//	�O�p�`�̒��ɓ����Ă��邩�ǂ���
		if (nor1 > 0 && nor2 > 0 && nor3 > 0) {		//	�����Ă���
			//	�x�N�g�����߂�
			vec1 = pVtx[m_SelectPolygonIndex].pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos;
			vec2 = pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline + 1].pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos;

			//	�́[�܂炢��
			D3DXVec3Cross(&nor1, &vec1, &vec2);
			D3DXVec3Normalize(&nor1, &nor1);

			//	���������߂�
			mathPos.y = pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos.y - ((nor1.x * (pos.x - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos.x) +
				nor1.z * (pos.z - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos.z)) / nor1.y);

			pos.y = mathPos.y;
		}
		else {		//	�����ĂȂ�
			//	�v���C���[��Y���W�ɒ��ڑ��
			pos.y = 0.0f;
		}
	}

	m_pVtxBuffPolygon->Unlock();

	return pos.y;
}

/******************************************************************************
	�֐��� : bool CMeshField::LoadField(const char *filename)
	����   : filename
	�߂�l : true, false
	����   : �t�@�C�����ǂݍ��߂���n�C�g�}�b�v��ǂݍ����true��Ԃ��āA
			 ������Ή�������false��Ԃ��B
******************************************************************************/
bool CMeshField::LoadField(const char *filename)
{
	VERTEX_3D *pVtx;
	FILE *fp;
	char str[32];

	//	�t�@�C����������
	fp = fopen(filename, "r");

	if (fp != NULL) {		//	�t�@�C�������݂���B
		m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
		//	�e���_��Y���W�̒��_�ǂݍ���
		for (int i = 0; i < m_VertexNum; i++) {
			fscanf(fp, "%s", str);
			fscanf(fp, "%f", &pVtx[i].pos.y);
		}
		m_pVtxBuffPolygon->Unlock();
		fclose(fp);
		//	�t�@�C�������
		return true;
	}
	else {		//	�t�@�C�������݂��Ȃ��B
		fclose(fp);
		//	�t�@�C�������
		return false;
	}
}

/******************************************************************************
	�֐��� : void CMeshField::UpdateMeshNormal(void)
	����   : void
	�߂�l : �Ȃ�
	����   : ���b�V���t�B�[���h�̖@�����X�V����֐��B
******************************************************************************/
void CMeshField::UpdateMeshNormal(void)
{
	VERTEX_3D *pVtx;
	//	��Ɨp�ϐ�
	D3DXVECTOR3 vec1, vec2, vec3, vec4, vec5, vec6;			//	�x�N�g��������ϐ�	
	D3DXVECTOR3 nor1, nor2, nor3, nor4, nor5, nor6;			//	�O�ς̌v�Z���ʂ�����ϐ�
	int Idx;		//	���_�ԍ�������ϐ�

	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
	for (int i = 0; i < m_Field_Vertex_Num_Zline; i++) {
		for (int j = 0; j < m_Field_Vertex_Num_Xline; j++) {
			//	���̒��_�ԍ�������
			Idx = j + (m_Field_Vertex_Num_Xline * i);

			pVtx[Idx].col = D3DXCOLOR(255, 255, 255, 255);

			//	�����̒��_�̖@������
			if ((i != 0 && i != m_Field_Vertex_Num_Zline - 1) && (j != 0 && j != m_Field_Vertex_Num_Xline - 1)) {

				//	���ꂼ��̃x�N�g���̒��������߂�
				vec1 = pVtx[(Idx - m_Field_Vertex_Num_Xline) - 1].pos - pVtx[Idx].pos;		//	����
				vec2 = pVtx[Idx - m_Field_Vertex_Num_Xline].pos - pVtx[Idx].pos;			//	��
				vec3 = pVtx[Idx + 1].pos - pVtx[Idx].pos;									//	�E
				vec4 = pVtx[(Idx + m_Field_Vertex_Num_Xline) + 1].pos - pVtx[Idx].pos;		//	�E��
				vec5 = pVtx[(Idx)+m_Field_Vertex_Num_Xline].pos - pVtx[Idx].pos;			//	��
				vec6 = pVtx[(Idx)-1].pos - pVtx[Idx].pos;									//	��

				//	���ꂼ��̃x�N�g���̊O�ς̌v�Z���ʂ����ꂼ��̕ϐ��ɑ��
				D3DXVec3Cross(&nor1, &vec1, &vec2);
				D3DXVec3Normalize(&nor1, &nor1);

				D3DXVec3Cross(&nor2, &vec2, &vec3);
				D3DXVec3Normalize(&nor2, &nor2);

				D3DXVec3Cross(&nor3, &vec3, &vec4);
				D3DXVec3Normalize(&nor3, &nor3);

				D3DXVec3Cross(&nor4, &vec4, &vec5);
				D3DXVec3Normalize(&nor4, &nor4);

				D3DXVec3Cross(&nor5, &vec5, &vec6);
				D3DXVec3Normalize(&nor5, &nor5);

				D3DXVec3Cross(&nor6, &vec6, &vec1);
				D3DXVec3Normalize(&nor6, &nor6);

				//	���̒��_�ɋ��߂��@���x�N�g���̕��ς�ݒ�
				D3DXVec3Normalize(&pVtx[Idx].nor, &(nor1 + nor2 + nor3 + nor4 + nor5 + nor6));
			}
			else if (i == 0 && j == 0) {	//	����
				vec1 = pVtx[Idx + 1].pos - pVtx[Idx].pos;
				vec2 = pVtx[Idx + m_Field_Vertex_Num_Xline + 1].pos - pVtx[Idx].pos;
				vec3 = pVtx[Idx + m_Field_Vertex_Num_Xline].pos - pVtx[Idx].pos;

				//	���ꂼ��̃x�N�g���̊O�ς̌v�Z���ʂ����ꂼ��̕ϐ��ɑ��
				D3DXVec3Cross(&nor1, &vec1, &vec2);
				D3DXVec3Normalize(&nor1, &nor1);

				D3DXVec3Cross(&nor2, &vec2, &vec3);
				D3DXVec3Normalize(&nor2, &nor2);

				//	���̒��_�ɋ��߂��@���x�N�g���̕��ς�ݒ�
				D3DXVec3Normalize(&pVtx[Idx].nor, &(nor1 + nor2));
			}
			else if (i == 0 && j == m_Field_Vertex_Num_Xline - 1) {		//	�E��
				vec1 = pVtx[Idx + m_Field_Vertex_Num_Xline].pos - pVtx[Idx].pos;
				vec2 = pVtx[Idx - 1].pos - pVtx[Idx].pos;

				//	���ꂼ��̃x�N�g���̊O�ς̌v�Z���ʂ����ꂼ��̕ϐ��ɑ��
				D3DXVec3Cross(&nor1, &vec1, &vec2);
				D3DXVec3Normalize(&nor1, &nor1);

				//	���ꂼ��̃x�N�g���̊O�ς̌v�Z���ʂ����ꂼ��̕ϐ��ɑ��
				D3DXVec3Normalize(&pVtx[Idx].nor, &(nor1));
			}
			else if (i == m_Field_Vertex_Num_Zline - 1 && j == 0) {		//	����
				vec1 = pVtx[Idx - m_Field_Vertex_Num_Xline].pos - pVtx[Idx].pos;
				vec2 = pVtx[Idx + 1].pos - pVtx[Idx].pos;

				//	���ꂼ��̃x�N�g���̊O�ς̌v�Z���ʂ����ꂼ��̕ϐ��ɑ��
				D3DXVec3Cross(&nor1, &vec1, &vec2);
				D3DXVec3Normalize(&nor1, &nor1);

				//	���ꂼ��̃x�N�g���̊O�ς̌v�Z���ʂ����ꂼ��̕ϐ��ɑ��
				D3DXVec3Normalize(&pVtx[Idx].nor, &(nor1));
			}
			else if (i == m_Field_Vertex_Num_Zline - 1 && j == m_Field_Vertex_Num_Xline - 1) {		//	�E��
				vec1 = pVtx[Idx - 1].pos - pVtx[Idx].pos;
				vec2 = pVtx[(Idx - (m_Field_Vertex_Num_Xline)) - 1].pos - pVtx[Idx].pos;
				vec3 = pVtx[Idx - m_Field_Vertex_Num_Xline].pos - pVtx[Idx].pos;

				//	���ꂼ��̃x�N�g���̊O�ς̌v�Z���ʂ����ꂼ��̕ϐ��ɑ��
				D3DXVec3Cross(&nor1, &vec1, &vec2);
				D3DXVec3Normalize(&nor1, &nor1);

				D3DXVec3Cross(&nor2, &vec2, &vec3);
				D3DXVec3Normalize(&nor2, &nor2);

				//	���ꂼ��̃x�N�g���̊O�ς̌v�Z���ʂ����ꂼ��̕ϐ��ɑ��
				D3DXVec3Normalize(&pVtx[Idx].nor, &(nor1 + nor2));
			}
			else if ((i != 0 && i != m_Field_Vertex_Num_Zline - 1) && j == 0) {		//	���[
				vec1 = pVtx[Idx - m_Field_Vertex_Num_Xline].pos - pVtx[Idx].pos;
				vec2 = pVtx[Idx + 1].pos - pVtx[Idx].pos;
				vec3 = pVtx[Idx + (m_Field_Vertex_Num_Xline + 1)].pos - pVtx[Idx].pos;
				vec4 = pVtx[Idx + m_Field_Vertex_Num_Xline].pos - pVtx[Idx].pos;

				//	���ꂼ��̃x�N�g���̊O�ς̌v�Z���ʂ����ꂼ��̕ϐ��ɑ��
				D3DXVec3Cross(&nor1, &vec1, &vec2);
				D3DXVec3Normalize(&nor1, &nor1);

				D3DXVec3Cross(&nor2, &vec2, &vec3);
				D3DXVec3Normalize(&nor2, &nor2);

				D3DXVec3Cross(&nor3, &vec3, &vec4);
				D3DXVec3Normalize(&nor3, &nor3);

				//	���ꂼ��̃x�N�g���̊O�ς̌v�Z���ʂ����ꂼ��̕ϐ��ɑ��
				D3DXVec3Normalize(&pVtx[Idx].nor, &(nor1 + nor2 + nor3));
			}
			else if ((i != 0 && i != m_Field_Vertex_Num_Zline - 1) && j == m_Field_Vertex_Num_Xline - 1) {		//	�E�[
				vec1 = pVtx[Idx + m_Field_Vertex_Num_Xline].pos - pVtx[j + (m_Field_Vertex_Num_Xline * i)].pos;	//	��
				vec2 = pVtx[Idx - 1].pos - pVtx[Idx].pos;	//	��
				vec3 = pVtx[(Idx - m_Field_Vertex_Num_Xline) - 1].pos - pVtx[Idx].pos;//	����
				vec4 = pVtx[Idx - m_Field_Vertex_Num_Xline].pos - pVtx[Idx].pos;//	��

																			//	���ꂼ��̃x�N�g���̊O�ς̌v�Z���ʂ����ꂼ��̕ϐ��ɑ��
				D3DXVec3Cross(&nor1, &vec1, &vec2);
				D3DXVec3Normalize(&nor1, &nor1);

				D3DXVec3Cross(&nor2, &vec2, &vec3);
				D3DXVec3Normalize(&nor2, &nor2);

				D3DXVec3Cross(&nor3, &vec3, &vec4);
				D3DXVec3Normalize(&nor3, &nor3);

				//	���ꂼ��̃x�N�g���̊O�ς̌v�Z���ʂ����ꂼ��̕ϐ��ɑ��
				D3DXVec3Normalize(&pVtx[Idx].nor, &(nor1 + nor2 + nor3));
			}
			else if (i == 0 && (j != 0 && j != m_Field_Vertex_Num_Xline - 1)) {		//	��[
				vec1 = pVtx[Idx + 1].pos - pVtx[Idx].pos;
				vec2 = pVtx[Idx + (m_Field_Vertex_Num_Xline + 1)].pos - pVtx[Idx].pos;
				vec3 = pVtx[Idx + m_Field_Vertex_Num_Xline].pos - pVtx[Idx].pos;
				vec4 = pVtx[Idx - 1].pos - pVtx[Idx].pos;

				//	���ꂼ��̃x�N�g���̊O�ς̌v�Z���ʂ����ꂼ��̕ϐ��ɑ��
				D3DXVec3Cross(&nor1, &vec1, &vec2);
				D3DXVec3Normalize(&nor1, &nor1);

				D3DXVec3Cross(&nor2, &vec2, &vec3);
				D3DXVec3Normalize(&nor2, &nor2);

				D3DXVec3Cross(&nor3, &vec3, &vec4);
				D3DXVec3Normalize(&nor3, &nor3);

				//	���ꂼ��̃x�N�g���̊O�ς̌v�Z���ʂ����ꂼ��̕ϐ��ɑ��
				D3DXVec3Normalize(&pVtx[Idx].nor, &(nor1 + nor2 + nor3));

			}
			else if (i == m_Field_Vertex_Num_Zline - 1 && (j != 0 && j != m_Field_Vertex_Num_Xline - 1)) {		//	���[
				vec1 = pVtx[Idx - 1].pos - pVtx[Idx].pos;
				vec2 = pVtx[(Idx - m_Field_Vertex_Num_Xline) - 1].pos - pVtx[Idx].pos;
				vec3 = pVtx[(Idx - m_Field_Vertex_Num_Xline)].pos - pVtx[Idx].pos;
				vec4 = pVtx[Idx + 1].pos - pVtx[Idx].pos;

				//	���ꂼ��̃x�N�g���̊O�ς̌v�Z���ʂ����ꂼ��̕ϐ��ɑ��
				D3DXVec3Cross(&nor1, &vec1, &vec2);
				D3DXVec3Normalize(&nor1, &nor1);

				D3DXVec3Cross(&nor2, &vec2, &vec3);
				D3DXVec3Normalize(&nor2, &nor2);

				D3DXVec3Cross(&nor3, &vec3, &vec4);
				D3DXVec3Normalize(&nor3, &nor3);

				//	���ꂼ��̃x�N�g���̊O�ς̌v�Z���ʂ����ꂼ��̕ϐ��ɑ��
				D3DXVec3Normalize(&pVtx[Idx].nor, &(nor1 + nor2 + nor3));
			}
		}
	}
	m_pVtxBuffPolygon->Unlock();
}
