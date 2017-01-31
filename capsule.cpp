/******************************************************************************
	�^�C�g���� : CCapsule�N���X
	�t�@�C���� : capsule.cpp
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
#include "capsule.h"
#include "collisionDetection.h"
#include "collisionList.h"


/******************************************************************************
	�֐��� : CCapsule::CCapsule(CScene *scene, int Priority) : CCollision(scene, Priority)
	����   : �R���X�g���N�^
******************************************************************************/
CCapsule::CCapsule(CScene *scene, int Priority) : CCollision(scene, Priority)
{
	m_Hit = false;
	m_Delete = false;
	m_Segment.start_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Segment.end_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Segment.radius = 0.0f;
	m_HitPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

/******************************************************************************
	�֐��� : CSphere::~CSphere()
	����   : �f�X�g���N�^
******************************************************************************/
CCapsule::~CCapsule()
{
}

/******************************************************************************
	�֐��� : void CCapsule::Action(CCapsule * other)
	����   : other
	�߂�l : �Ȃ�
	����   : other�Ǝ����ƂŃJ�v�Z�����m�̂����蔻�������B
******************************************************************************/
void CCapsule::Action(CCapsule * other)
{
	CCollisionDetection::GetInstance()->Capsule_To_Capsule(other, this);
}

/******************************************************************************
	�֐��� : void CCapsule::Action(CSphere * other)
	����   : other
	�߂�l : �Ȃ�
	����   : other�Ǝ����Ƃŋ��̂ƃJ�v�Z���̂����蔻�������B
******************************************************************************/
void CCapsule::Action(CSphere * other)
{
	CCollisionDetection::GetInstance()->Sphere_To_Capsule(other, this);
}

/******************************************************************************
	�֐��� : void CSphere::Init(D3DXVECTOR3 pos, float radius)
	����   : pos, radius
	�߂�l : �Ȃ�
	����   : ���W�Ɣ��a�Ǝn�_�ƏI�_�̐ݒ�B
******************************************************************************/
void CCapsule::Init(D3DXVECTOR3 pos, float radius, D3DXVECTOR3 start_pos, D3DXVECTOR3 end_pos)
{
	m_Segment.start_pos = start_pos;
	m_Segment.end_pos = end_pos;
	m_Segment.radius = radius;
	m_LoaclPos = pos;

	m_CollisionList = new CCollisionList;
}

/******************************************************************************
	�֐��� : void CSphere::Uninit(void)
	����   : void
	�߂�l : �Ȃ�
	����   : ���X�g�̉���B
******************************************************************************/
void CCapsule::Uninit(void)
{
	m_CollisionList->Uninit();
	SAFE_DELETE(m_CollisionList);
}

/******************************************************************************
	�֐��� : void CCapsule::Draw(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �`��֐��B
******************************************************************************/
void CCapsule::Draw(void)
{
}

/******************************************************************************
	�֐��� : CCapsule * CCapsule::Create(D3DXVECTOR3 pos, float radius, D3DXVECTOR3 start_pos, D3DXVECTOR3 end_pos, CScene *scene)
	����   : pos, radius, start_pos, end_pos, scene
	�߂�l : �Ȃ�
	����   : �����֐��B
******************************************************************************/
CCapsule * CCapsule::Create(D3DXVECTOR3 pos, float radius, D3DXVECTOR3 start_pos, D3DXVECTOR3 end_pos, CScene *scene)
{
	CCapsule *obj = new CCapsule(scene);
	obj->Init(pos, radius, start_pos, end_pos);
	return obj;
}
