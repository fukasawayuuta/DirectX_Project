/******************************************************************************
	�^�C�g���� : SkyBox�N���X
	�t�@�C���� : skyBox.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2017/01/17
	�X�V��     :
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#pragma once

/******************************************************************************
	�N���X�錾
******************************************************************************/
class CSkyBox : public CScene3D
{
public:
	CSkyBox(int Priority = OBJECT_3D, OBJ_TYPE objType = OBJ_TYPE_FIELD, SCENE_TYPE sceneType = SCENE_TYPE_GAME);		//	�R���X�g���N�^�B
	~CSkyBox();		//	�f�X�g���N�^�B

	void Init(D3DXVECTOR3 pos, float width, float height, float depth, int textureType);		//	�������֐��B
	void Update(void);		//	�X�V�֐��B
	void Draw(void);		//	�`��֐��B

	static CSkyBox *Create(D3DXVECTOR3 pos, float width, float height, float depth, int textureType);		//	�����֐��B
};

