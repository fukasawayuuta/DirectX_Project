/******************************************************************************
	�^�C�g���� : CSphere�N���X
	�t�@�C���� : sphere.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2017/01/05
	�X�V��     :
******************************************************************************/
/******************************************************************************
	�C���N���[�h�t�@�C��
******************************************************************************/
#include "main.h"
#include "scene.h"
#include "collision.h"
#include "sphere.h"
#include "collisionDetection.h"
#include "collisionList.h"

/******************************************************************************
	�֐��� : CSphere::CSphere(CScene *scene, int Priority) : CCollision(scene, Priority)
	����   : �R���X�g���N�^
******************************************************************************/
CSphere::CSphere(CScene *scene, int Priority) : CCollision(scene, Priority)
{
	m_Delete = false;
	m_Hit = false;
	m_Radius = 0.0f;
}

/******************************************************************************
	�֐��� : CSphere::~CSphere()
	����   : �f�X�g���N�^
******************************************************************************/
CSphere::~CSphere()
{
}

/******************************************************************************
	�֐��� : void CSphere::Init(D3DXVECTOR3 pos, float radius)
	����   : pos, radius
	�߂�l : �Ȃ�
	����   : ���W�Ɣ��a�̐ݒ�B
******************************************************************************/
void CSphere::Init(D3DXVECTOR3 pos, float radius)
{
	m_LoaclPos = pos;
	m_Radius = radius;
	m_CollisionList = new CCollisionList;
}

/******************************************************************************
	�֐��� : void CSphere::Uninit(void)
	����   : void
	�߂�l : �Ȃ�
	����   : ���X�g�̉���B
******************************************************************************/
void CSphere::Uninit(void)
{
	m_CollisionList->Uninit();
	SAFE_DELETE(m_CollisionList);
}

/******************************************************************************
	�֐��� : void CSphere::Draw(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �`��֐��B
******************************************************************************/
void CSphere::Draw(void)
{
}

/******************************************************************************
	�֐��� : void CSphere::Action(CCapsule * other)
	����   : other
	�߂�l : �Ȃ�
	����   : other�Ǝ����Ƃŋ��̂ƃJ�v�Z���̂����蔻�������B
******************************************************************************/
void CSphere::Action(CCapsule * other)
{
	CCollisionDetection::GetInstance()->Sphere_To_Capsule(this, other);
}

/******************************************************************************
	�֐��� : void CSphere::Action(CSphere * other)
	����   : other
	�߂�l : �Ȃ�
	����   : other�Ǝ����Ƃŋ��̓��m�̂����蔻�������B
******************************************************************************/
void CSphere::Action(CSphere * other)
{
	CCollisionDetection::GetInstance()->Sphere_To_Sphere(this, other);
}

/******************************************************************************
	�֐��� : CSphere * CSphere::Create(D3DXVECTOR3 pos, float radius, CScene *scene)
	����   : pos, radius, scene
	�߂�l : �Ȃ�
	����   : �����֐��B
******************************************************************************/
CSphere * CSphere::Create(D3DXVECTOR3 pos, float radius, CScene *scene)
{
	CSphere *obj = new CSphere(scene);
	obj->Init(pos, radius);
	return obj;
}
