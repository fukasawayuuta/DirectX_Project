/******************************************************************************
	�^�C�g���� : collisionDetection�N���X
	�t�@�C���� : collisionDetection.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2017/01/05
	�X�V��     : 
******************************************************************************/
/******************************************************************************
	�C���N���[�h�錾
******************************************************************************/
#include "main.h"
#include "collision.h"
#include "sphere.h"
#include "capsule.h"
#include "collisionDetection.h"
#include "collisionList.h"

/******************************************************************************
	�ÓI�����o�ϐ��錾
******************************************************************************/
CCollisionDetection *CCollisionDetection::m_pInstance = NULL;

/******************************************************************************
	�֐��� : void CCollisionDetection::CollisionDetectionManager(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �����蔻��̂���֐��B
******************************************************************************/
void CCollisionDetection::CollisionDetectionManager(void)
{
	for (int i = 0; i < CCollision::PRIORITY_MAX; i++) {
		CCollision *scene = CCollision::GetListTopPointer(i);
		CCollision *NextScene;

		while (scene != NULL) {
			NextScene = scene->GetSceneNext();

			for (int i = 0; i < CCollision::PRIORITY_MAX; i++) {
				CCollision *_scene = CCollision::GetListTopPointer(i);
				CCollision *_NextScene;

				while (_scene != NULL) {
					_NextScene = _scene->GetSceneNext();
					if (scene != _scene) {
						scene->Dispatch(_scene);
					}
					_scene = _NextScene;
				}
			}
			scene = NextScene;
		}
	}
}

/******************************************************************************
	�֐��� : void CCollisionDetection::Sphere_To_Sphere(CSphere * sphere1, CSphere * sphere2)
	����   : sphere1, sphere2
	�߂�l : �Ȃ�
	����   : ���̂Ƌ��̂̂����蔻�������֐��B
******************************************************************************/
void CCollisionDetection::Sphere_To_Sphere(CSphere * sphere1, CSphere * sphere2)
{
	D3DXVECTOR3 difference;
	D3DXVec3Subtract(&difference, &sphere1->GetWorldPos(), &sphere2->GetWorldPos());		//	���݂��̍��W�������č��������߂�B
	//	���߂�������2�̔��a���Z����ΏՓˁB
	if (D3DXVec3LengthSq(&difference) <= pow((sphere1->GetRadius() + sphere2->GetRadius()), 2)) {
		//	���������t���O���Z�b�g�B
		sphere1->SetHit(true);
		sphere2->SetHit(true);
		//	�Փ˂�������̏������݂��ɃZ�b�g�B
		sphere1->GetCollisionList()->AddList(sphere2);
		sphere2->GetCollisionList()->AddList(sphere1);
	}
}

/******************************************************************************
	�֐��� : void CCollisionDetection::Capsule_To_Capsule(CCapsule * capsule1, CCapsule * capsule2)
	����   : capsule1, capsule2
	�߂�l : �Ȃ�
	����   : �J�v�Z���ƃJ�v�Z���̂����蔻�������֐��B
******************************************************************************/
void CCollisionDetection::Capsule_To_Capsule(CCapsule * capsule1, CCapsule * capsule2)
{
	D3DXVECTOR3 difference1;
	D3DXVECTOR3 difference2;

	NearPoint(capsule1, capsule2, difference1, difference2);		//	���݂��̋ߎ��_�����Ƃ߂�B
	D3DXVec3Subtract(&difference1, &difference1, &difference2);		//	���݂��̋ߎ��_������W���Ђ��č��������߂�B
	//	���߂�������2�̔��a���Z����ΏՓˁB
	if (D3DXVec3LengthSq(&difference1) <= pow((capsule1->GetSegment()->radius + capsule2->GetSegment()->radius), 2)) {
		//	���������t���O���Z�b�g�B
		capsule1->SetHit(true);
		capsule2->SetHit(true);
		//	�Փ˂�������̏������݂��ɃZ�b�g�B
		capsule1->GetCollisionList()->AddList(capsule2);
		capsule2->GetCollisionList()->AddList(capsule1);
	}
}

/******************************************************************************
	�֐��� : void CCollisionDetection::Sphere_To_Capsule(CSphere * sphere, CCapsule * capsule)
	����   : sphere, capsule
	�߂�l : �Ȃ�
	����   : �J�v�Z���Ƌ��̂̂����蔻�������֐��B
******************************************************************************/
void CCollisionDetection::Sphere_To_Capsule(CSphere * sphere, CCapsule * capsule)
{
	D3DXVECTOR3 difference;
	//	���̂ƃJ�v�Z���̋ߎ��_�̋����������č��������߂�B
	D3DXVec3Subtract(&difference, &sphere->GetWorldPos(), &NearPoint(sphere, capsule));

	//	���߂�������2�̔��a���Z����ΏՓˁB
	if (D3DXVec3LengthSq(&difference) <= pow((sphere->GetRadius() + capsule->GetSegment()->radius), 2)) {
		//	���������t���O���Z�b�g�B
		sphere->SetHit(true);
		capsule->SetHit(true);
		//	�Փ˂�������̏������݂��ɃZ�b�g�B
		sphere->GetCollisionList()->AddList(capsule);
		capsule->GetCollisionList()->AddList(sphere);
	}
}

/******************************************************************************
	�֐��� : D3DXVECTOR3 CCollisionDetection::NearPoint(CSphere * sphere, CCapsule * capsule)
	����   : sphere, capsule
	�߂�l : start
	����   : �J�v�Z���Ƌ��̂̋ߎ��_�����Ƃ߂Ēl��Ԃ��֐��B
******************************************************************************/
D3DXVECTOR3 CCollisionDetection::NearPoint(CSphere * sphere, CCapsule * capsule)
{
	D3DXVECTOR3 start = capsule->GetWorldPos() + capsule->GetSegment()->start_pos;		//	�x�N�g���̎n�_�B
	D3DXVECTOR3 vec1 = capsule->GetSegment()->end_pos - capsule->GetSegment()->start_pos;	//	�n�_�ƏI�_�̕����x�N�g���B
	D3DXVECTOR3 vec2 = sphere->GetWorldPos() - start;			//	�e�ƃJ�v�Z���̎n�_�̕����x�N�g���B
	D3DXVECTOR3 nor1;		//	���K�������P�ʃx�N�g��������ϐ��B
	float rate;

	D3DXVec3Normalize(&nor1, &vec1);		//	vec1�𐳋K�������P�ʃx�N�g����nor1�ɑ���B

	rate = D3DXVec3Dot(&vec1, &vec2) / D3DXVec3LengthSq(&vec1);		//	���������߂�B
	rate = rate < 0.0f ? 0.0f : (rate > 1.0f ? 1.0f : rate);		//	��������̐ݒ�B

	return start += (vec1 * rate);		//	�ߎ��_�̐ݒ�B
}

/******************************************************************************
	�֐��� : void CCollisionDetection::NearPoint(CCapsule *capsule1, CCapsule *capsule2, D3DXVECTOR3 & pOut1, D3DXVECTOR3 & pOut2)
	����   : capsule1, capsule2, pOut1, pOut2
	�߂�l : �Ȃ�
	����   : �J�v�Z�����m�̋ߎ��_�����Ƃ߂Ēl�����ꂼ��pOut1, pOut2�ɑ������֐��B
******************************************************************************/
void CCollisionDetection::NearPoint(CCapsule *capsule1, CCapsule *capsule2, D3DXVECTOR3 & pOut1, D3DXVECTOR3 & pOut2)
{
	D3DXVECTOR3 start1 = capsule1->GetWorldPos() + capsule1->GetSegment()->start_pos;
	D3DXVECTOR3 start2 = capsule2->GetWorldPos() + capsule2->GetSegment()->start_pos;

	D3DXVECTOR3 vec1 = capsule1->GetSegment()->end_pos - capsule1->GetSegment()->start_pos;	//	�n�_�ƏI�_�̕����x�N�g���B
	D3DXVECTOR3 vec2 = start2 - start1;			//	�e�ƃJ�v�Z���̎n�_�̕����x�N�g���B
	D3DXVECTOR3 nor1;		//	���K�������P�ʃx�N�g��������ϐ��B
	float rate;

	D3DXVec3Normalize(&nor1, &vec1);		//	vec1�𐳋K�������P�ʃx�N�g����nor1�ɑ���B

	rate = D3DXVec3Dot(&vec1, &vec2) / D3DXVec3LengthSq(&vec1);		//	���������߂�B
	rate = rate < 0.0f ? 0.0f : (rate > 1.0f ? 1.0f : rate);		//	��������̐ݒ�B

	pOut1 = start1 + (vec1 * rate);

	vec1 = capsule2->GetSegment()->end_pos - capsule2->GetSegment()->start_pos;	//	�n�_�ƏI�_�̕����x�N�g���B
	vec2 = pOut1 - start2;			//	�e�ƃJ�v�Z���̎n�_�̕����x�N�g���B

	D3DXVec3Normalize(&nor1, &vec1);		//	vec1�𐳋K�������P�ʃx�N�g����nor1�ɑ���B

	rate = D3DXVec3Dot(&vec1, &vec2) / D3DXVec3LengthSq(&vec1);		//	���������߂�B
	rate = rate < 0.0f ? 0.0f : (rate > 1.0f ? 1.0f : rate);		//	��������̐ݒ�B

	pOut2 = start2 + (vec1 * rate);
}

/******************************************************************************
	�֐��� : CCollisionDetection::CCollisionDetection()
	����   : �R���X�g���N�^
******************************************************************************/
CCollisionDetection::CCollisionDetection()
{
}

/******************************************************************************
	�֐��� : CCollisionDetection::~CCollisionDetection()
	����   : �f�X�g���N�^
******************************************************************************/
CCollisionDetection::~CCollisionDetection()
{
}
