/******************************************************************************
	�^�C�g���� : EnemyManager�N���X
	�t�@�C���� : EnemyManager.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2017/1/26
	�X�V��     :
******************************************************************************/
/******************************************************************************
	�C���N���[�h�t�@�C��
******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "texture.h"
#include "scene3D.h"
#include "sceneX.h"
#include "billboard.h"
#include "bullet.h"
#include "model.h"
#include "motionManager.h"
#include "dynamicModel.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "fixedBattery.h"

/******************************************************************************
	�ÓI�����o�ϐ�
******************************************************************************/
int CEnemyManager::m_ActiveEnemyCnt = 0;

/******************************************************************************
	�֐��� : CEnemyManager::CEnemyManager()
	����   : �R���X�g���N�^
******************************************************************************/
CEnemyManager::CEnemyManager()
{
}

/******************************************************************************
	�֐��� : CEnemyManager::~CEnemyManager()
	����   : �f�X�g���N�^
******************************************************************************/
CEnemyManager::~CEnemyManager()
{
}

/******************************************************************************
	�֐��� : void CEnemyManager::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �G�̐��擾�B
******************************************************************************/
void CEnemyManager::Update(void)
{
	CScene *pScene = CScene::GetListTopPointer(CScene::OBJECT_ENEMY);
	CScene *pSceneNext;

	int enemyNum = 0;

	//	null����Ȃ���Ή񂷁B
	while (pScene != null) {
		//	���̃|�C���^�擾�B
		pSceneNext = pScene->GetSceneNext();
		//	�^�C�v��OBJ_TYPE_ENEMY�����������B
		if (pScene->GetObjectType() == CScene::OBJ_TYPE_ENEMY) {
			++enemyNum;
		}
		//	���̃|�C���^�ݒ�B
		pScene = pSceneNext;
	}

	m_ActiveEnemyCnt = enemyNum;
}

/******************************************************************************
	�֐��� : void CEnemyManager::Load(const char * fileName)
	����   : fileName
	�߂�l : �Ȃ�
	����   : �e�L�X�g�ɂ���G�̏���ǂݍ���Ő������鏈���B
******************************************************************************/
void CEnemyManager::Load(const char * fileName)
{
	FILE *fp;
	char  str[32];
	int   Enemy_Num = 0;
	D3DXVECTOR3 pos, rot;
	int ModelType;
	int EnemyType;

	//	�G�l�~�[�̃t�@�C�����J���B
	fp = fopen(fileName, "r");
	//	�t�@�C��������ΊJ���B
	if (fp != NULL) {
		//	�G�l�~�[�̐��擾�B
		fscanf(fp, "%s", &str);
		fscanf(fp, "%s", &str);
		fscanf(fp, "%d", &Enemy_Num);

		//	�擾�����G�l�~�[�̐������񂷁B
		for (int i = 0; i < Enemy_Num; i++) {
			fscanf(fp, "%s", str);
			fscanf(fp, "%f, %f, %f, %f, %f, %f,%d, %d\n", &pos.x, &pos.y, &pos.z, &rot.x, &rot.y, &rot.z, &ModelType, &EnemyType);

			switch (ModelType) {
			case DYNAMICMODEL_TYPE_ENEMY_00:
				CFixedBattery::Create(pos, rot);
				break;
			case DYNAMICMODEL_TYPE_PLAYER_00:
				CEnemy::Create(pos, rot, DYNAMICMODEL_TYPE_PLAYER_00);
				break;
			}
			++m_ActiveEnemyCnt;
		}
	}

	//	�t�@�C�������
	fclose(fp);
}
