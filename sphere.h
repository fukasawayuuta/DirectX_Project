/******************************************************************************
	�^�C�g���� : CSphere�N���X
	�t�@�C���� : sphere.h
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
class CSphere : public CCollision
{
public:
	CSphere(CScene *scene = NULL, int Priority = CSPHERE);		//	�R���X�g���N�^�B
	~CSphere();			//	�f�X�g���N�^�B

	void Init(D3DXVECTOR3 pos, float radius);		//	�������֐��B
	void Uninit(void);		//	�I���֐��B
	void Draw(void);		//	�`��֐��B

	float GetRadius(void) { return m_Radius; }		//	���a�擾�B
	
	//	�_�u���f�B�X�p�b�`�B	
	virtual void Dispatch(CCollision* other) { other->Action(this); };
	virtual void Action(CCapsule* other);
	virtual void Action(CSphere* other);

	static CSphere *Create(D3DXVECTOR3 pos, float radius, CScene *scene);		//	�����֐��B
private:
	float m_Radius;		//	���a�B
};

