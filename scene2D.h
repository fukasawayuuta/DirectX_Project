/******************************************************************************
	�^�C�g���� : Scene2D�N���X
	�t�@�C���� : scene2D.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/04/19
	�X�V��     : 2016/05/31
				 2016/05/16
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#pragma once

/******************************************************************************
	�N���X
******************************************************************************/
class CScene2D : public CScene
{
public:
	CScene2D(int Priority = NONE, OBJ_TYPE objType = OBJ_TYPE_NONE, SCENE_TYPE sceneType = SCENE_TYPE_NONE);		//	�R���X�g���N�^�B
	~CScene2D();	//	�f�X�g���N�^�B

	void Init(D3DXVECTOR3 pos, float width, float height, int textureType);		//	�������֐��B
	void Uninit(void);		//	�I���֐��B
	void Update(void);		//	�X�V�֐��B
	void Draw(void);		//	�`��֐��B

	static CScene2D *Create(D3DXVECTOR3 pos, float width, float height, int textureType);		//	�����֐��B
	static void TextureUV_Update(LPDIRECT3DVERTEXBUFFER9 vtxBuff, int patternCnt, int animContX_Axis, int animCntY_Axis);		//	�e�N�X�`����UV�X�V�p�֐��B

protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffPolygon;		//	���_�o�b�t�@�B
	D3DXCOLOR m_Color;	//	�F���B

	float m_Width;		//	�����B
	float m_Height;		//	�c���B
	float m_Length;		//	�Ίp���̒����B
	float m_Angle;		//	�Ίp���̊p�x�B

	int m_TextureType;	//	�e�N�X�`���^�C�v�B

	//	�A�j���[�V�����ϐ��B
	int m_AnimationPatternCnt;	//	�A�j���[�V�����̃p�^�[���J�E���g�B
	int m_AnimationTotalNumber;	//	�A�j���[�V�����̍��v���B
	int m_AnimationCountX_Axis;	//	X����̃A�j���[�V�����̐��B
	int m_AnimationCountY_Axis;	//	Y����̃A�j���[�V�����̐��B
	int m_AnimationUpdateUVCnt;	//	UV���W���X�V����^�C�~���O�J�E���^
};

