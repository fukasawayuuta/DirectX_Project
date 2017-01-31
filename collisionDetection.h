/******************************************************************************
	�^�C�g���� : collisionDetection�N���X
	�t�@�C���� : collisionDetection.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2017/01/05
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
class CCapsule;
class CSphere;

class CCollisionDetection
{
public:
	//	�V���O���g�������B
	static CCollisionDetection * GetInstance() { return m_pInstance == NULL ? m_pInstance = new CCollisionDetection() : m_pInstance; }
	void CollisionDetectionManager(void);		//	�����蔻�肷��֐��B
	void Sphere_To_Sphere(CSphere *sphere1, CSphere *sphere2);		//	���̂Ƌ��̂̂����蔻��֐��B
	void Capsule_To_Capsule(CCapsule *capsule1, CCapsule *capsule2);//	�J�v�Z���ƃJ�v�Z���̂����蔻��֐��B
	void Sphere_To_Capsule(CSphere *sphere, CCapsule *capsule);		//	���̂ƃJ�v�Z���̂����蔻��B
	D3DXVECTOR3 NearPoint(CSphere * sphere, CCapsule * capsule);	//	���̂ƃJ�v�Z���̋ߎ��_�����߂�֐��B
	void NearPoint(CCapsule *capsule1, CCapsule *capsule2, D3DXVECTOR3 &pOut1, D3DXVECTOR3 &pOut2);	//	�J�v�Z�����m�̋ߎ��_�����߂�֐��B
	static void Release(void) { SAFE_DELETE(m_pInstance); }			//	��������B
private:
	static CCollisionDetection *m_pInstance;	//	�C���X�^���X�B

	CCollisionDetection();		//	�R���X�g���N�^�B
	~CCollisionDetection();		//	�f�X�g���N�^�B
};

