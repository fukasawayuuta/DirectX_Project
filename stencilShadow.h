/******************************************************************************
	�t�@�C���� : stencilShadow.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2017/01/28
	�X�V��     :
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#pragma once

/******************************************************************************
	�N���X�錾
******************************************************************************/
class CScene;

class CStencilShadow : public CScene3D
{
public:
	CStencilShadow(int Priority = OBJECT_3D, OBJ_TYPE objType = OBJ_TYPE_SHADOW, SCENE_TYPE sceneType = SCENE_TYPE_GAME);		//	�R���X�g���N�^�B
	~CStencilShadow();		//	�f�X�g���N�^�B

	void Init(CScene *target);		//	�������֐��B
	void Uninit(void);				//	�I���֐��B
	void Update(void);				//	�X�V�֐��B
	void Draw(void);				//	�`��֐��B

	static void Create(CScene *target);		//	�����֐��B

private:
	//	�V�����_�[���g���̂ɕK�v�ȕϐ��B
	LPD3DXMESH	 m_pMesh;
	LPD3DXBUFFER m_pBuffMat;
	DWORD		 m_NumMat;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffPolygon;		//	���_�o�b�t�@(2D)
	CScene   *m_Target;		//	�g���Ώۂ�����B
};

