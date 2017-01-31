/******************************************************************************
	�^�C�g���� : CCapsule�N���X
	�t�@�C���� : capsule.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2017/01/05
	�X�V��     :
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#pragma once

/******************************************************************************
	�N���X
******************************************************************************/
class CCollisionDetection;

class CCapsule : public CCollision
{
public:
	//	�J�v�Z���K�v�Œ���̍\���́B
	typedef struct {
		D3DXVECTOR3 start_pos;		//	�J�n�n�_�B
		D3DXVECTOR3 end_pos;		//	�I���n�_�B
		float radius;				//	���a�B	
	}LINESEGMENT;

	CCapsule(CScene *scene = NULL, int Priority = LINE_SEGMENT);		//	�R���X�g���N�^�B
	~CCapsule();		//	�f�X�g���N�^�B

	//	�_�u���f�B�X�p�b�`�֐��B
	virtual void Dispatch(CCollision* other) { other->Action(this); };
	virtual void Action(CCapsule* other);
	virtual void Action(CSphere* other);

	LINESEGMENT *GetSegment(void) { return &m_Segment; }		//	�\���̎擾�֐��B

	void Init(D3DXVECTOR3 pos, float radius, D3DXVECTOR3 start_pos, D3DXVECTOR3 end_pos);		//	�������֐��B
	void Uninit(void);		//	�I���֐��B
	void Draw(void);		//	�`��֐��B

	static CCapsule *Create(D3DXVECTOR3 pos, float radius, D3DXVECTOR3 start_pos, D3DXVECTOR3 end_pos, CScene *scene);		//	�����֐��B

private:
	LINESEGMENT m_Segment;
	D3DXVECTOR3 m_HitPoint;
};

