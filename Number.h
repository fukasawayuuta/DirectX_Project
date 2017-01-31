#pragma once
class CNumber
{
public:
	CNumber();		//	�R���X�g���N�^�B
	~CNumber();		//	�f�X�g���N�^�B

	void Init(D3DXVECTOR3 pos, float width, float height);		//	�������֐��B
	void Uninit(void);											//	�I���֐��B
	void Update(int num, int anim_X_Max, int anim_Y_Max);		//	�X�V�֐��B
	void Draw(void);											//	�`��֐��B

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffPolygon;	//	���_�o�b�t�@�B
	D3DXCOLOR m_Color;					//	�F���B
	D3DXVECTOR3 m_pos;					//	���W�B
	float m_width;						//	���B
	float m_height;						//	�����B
	int m_TextureType;					//	�e�N�X�`���̃^�C�v�B
};

