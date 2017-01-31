/******************************************************************************
	�^�C�g���� : SceneIndex�N���X
	�t�@�C���� : sceneIndex.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/12/12
	�X�V��     : 
				 
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#pragma once

/******************************************************************************
	�N���X
******************************************************************************/
class CSceneIndex : public CScene3D
{
public:
	CSceneIndex(int Priority = NONE, OBJ_TYPE objType = OBJ_TYPE_NONE, SCENE_TYPE sceneType = SCENE_TYPE_NONE);		//	�R���X�g���N�^�B
	~CSceneIndex();		//	�f�X�g���N�^�B

	void Uninit(void);		//	�I���֐��B
	void Draw(void);		//	�`��֐��B

protected:
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;				//	�C���f�N�X�o�b�t�@�B
	int m_VertexNum;		//	���_��
	int m_PolygonNum;		//	�|���S����
};

