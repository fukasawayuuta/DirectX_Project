/******************************************************************************
	タイトル名 : EnemyManagerクラス
	ファイル名 : EnemyManager.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2017/1/26
	更新日     :
******************************************************************************/
/******************************************************************************
	インクルードファイル
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
	静的メンバ変数
******************************************************************************/
int CEnemyManager::m_ActiveEnemyCnt = 0;

/******************************************************************************
	関数名 : CEnemyManager::CEnemyManager()
	説明   : コンストラクタ
******************************************************************************/
CEnemyManager::CEnemyManager()
{
}

/******************************************************************************
	関数名 : CEnemyManager::~CEnemyManager()
	説明   : デストラクタ
******************************************************************************/
CEnemyManager::~CEnemyManager()
{
}

/******************************************************************************
	関数名 : void CEnemyManager::Update(void)
	引数   : void
	戻り値 : なし
	説明   : 敵の数取得。
******************************************************************************/
void CEnemyManager::Update(void)
{
	CScene *pScene = CScene::GetListTopPointer(CScene::OBJECT_ENEMY);
	CScene *pSceneNext;

	int enemyNum = 0;

	//	nullじゃなければ回す。
	while (pScene != null) {
		//	次のポインタ取得。
		pSceneNext = pScene->GetSceneNext();
		//	タイプがOBJ_TYPE_ENEMYだったら入る。
		if (pScene->GetObjectType() == CScene::OBJ_TYPE_ENEMY) {
			++enemyNum;
		}
		//	次のポインタ設定。
		pScene = pSceneNext;
	}

	m_ActiveEnemyCnt = enemyNum;
}

/******************************************************************************
	関数名 : void CEnemyManager::Load(const char * fileName)
	引数   : fileName
	戻り値 : なし
	説明   : テキストにある敵の情報を読み込んで生成する処理。
******************************************************************************/
void CEnemyManager::Load(const char * fileName)
{
	FILE *fp;
	char  str[32];
	int   Enemy_Num = 0;
	D3DXVECTOR3 pos, rot;
	int ModelType;
	int EnemyType;

	//	エネミーのファイルを開く。
	fp = fopen(fileName, "r");
	//	ファイルがあれば開く。
	if (fp != NULL) {
		//	エネミーの数取得。
		fscanf(fp, "%s", &str);
		fscanf(fp, "%s", &str);
		fscanf(fp, "%d", &Enemy_Num);

		//	取得したエネミーの数だけ回す。
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

	//	ファイルを閉じる
	fclose(fp);
}
