/******************************************************************************
	�^�C�g���� : Boost
	�t�@�C���� : boost.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2017/01/27
	�X�V��     :
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#pragma once

/******************************************************************************
	�N���X
******************************************************************************/
class CScene;

class CBoost : public CBillboard
{
public:
	CBoost(int Priority = EFFECT, OBJ_TYPE objType = OBJ_TYPE_BOOSTEFFECT, SCENE_TYPE sceneType = SCENE_TYPE_GAME);		//	�R���X�g���N�^�B
	~CBoost();		//		�f�X�g���N�^�B

	void Init(D3DXVECTOR3 pos, float width, float height, int textureType);		//	�������֐��B
	void Uninit(void);		//	�I���֐��B
	void Update(void);		//	�X�V�֐��B

	void RendererBegin(void);		//	�`��O�ݒ�֐��B
	void RendererEnd(void);			//	�`���ݒ�֐��B

	void SetDuringUse(bool flag) { m_DuringUse = flag; }		//	�t���O�ݒ�֐��B
	static CBoost *Create(D3DXVECTOR3 pos, float width, float height, int textureType);		//	�����֐��B

private:
	bool m_DuringUse;				//	�g���Ă���t���O�B
	float m_Magnification_Rate;		//	�{���̃��[�g�B
};

