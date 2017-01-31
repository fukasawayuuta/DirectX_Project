/******************************************************************************
	�^�C�g���� : Scene3D�N���X
	�t�@�C���� : scene3D.h
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
class CScene3D : public CScene
{
public:
	CScene3D(int Priority = NONE, OBJ_TYPE objType = OBJ_TYPE_NONE, SCENE_TYPE sceneType = SCENE_TYPE_NONE);		//	�R���X�g���N�^�B
	~CScene3D();		//	�f�X�g���N�^�B

	void Init(D3DXVECTOR3 pos, float width, float height, float depth, int textureType);		//	�������֐��B
	void Uninit(void);		//	�I���֐��B
	void Update(void);		//	�X�V�֐��B
	void Draw(void);		//	�`��֐��B

	virtual void RendererBegin(void) {};
	virtual void RendererEnd(void) {};

	static CScene3D *Create(D3DXVECTOR3 pos, float width, float height, float depth, int textureType);		//	�����֐��B
	static void TextureUV_Update(LPDIRECT3DVERTEXBUFFER9 vtxBuff, int patternCnt, int animContX_Axis, int animCntY_Axis);		//	�e�N�X�`����UV�X�V�p�֐��B

protected:
	//	�ʏ�ϐ��B
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffPolygon;		//	���_�o�b�t�@�B
	D3DXMATRIX m_mtxWorld;		//	���[���h�}�g���N�X�B
	D3DXVECTOR3 m_Scl;			//	�X�P�[���l�B
	D3DXCOLOR m_Color;			//	�F���B
	float m_Width;				//	�����B
	float m_Height;				//	�c���B
	float m_Depth;				//	���s�B
	float m_Length;				//	�Ίp���̒����B
	float m_Angle;				//	�Ίp���̊p�x�B
	int m_TextureType;			//	�e�N�X�`���^�C�v�B

	//	�A�j���[�V�����ϐ��B
	int m_AnimationPatternCnt;	//	�A�j���[�V�����̃p�^�[���J�E���g�B
	int m_AnimationTotalNumber;	//	�A�j���[�V�����̍��v���B
	int m_AnimationCountX_Axis;	//	X����̃A�j���[�V�����̐��B
	int m_AnimationCountY_Axis;	//	Y����̃A�j���[�V�����̐��B
	int m_AnimationUpdateUVCnt;	//	UV���W���X�V����^�C�~���O�J�E���^
};

