/******************************************************************************
	タイトル名 : CDustGeneraterクラス
	ファイル名 : dustGenerater.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2017/01/19
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
#include "sceneIndex.h"
#include "camera.h"
#include "billboard.h"
#include "desertDust.h"
#include "dustGenerate.h"
#include "MeshField.h"
#include "Mode.h"
#include "game.h"

/******************************************************************************
	関数名 : CDustGenerater::CDustGenerater()
	説明   : コンストラクタ
******************************************************************************/
CDustGenerater::CDustGenerater()
{
	m_DustNum = 0;
}

/******************************************************************************
	関数名 : CDustGenerater::~CDustGenerater()
	説明   : デストラクタ
******************************************************************************/
CDustGenerater::~CDustGenerater()
{
}

/******************************************************************************
	関数名 : void CDustGenerater::Init(int num)
	引数   : void
	戻り値 : なし
	説明   : 砂埃を生成。
******************************************************************************/
void CDustGenerater::Init(int num)
{
	// それぞれのポインタと値を取得
	CManager *pManager = GetManager();
	CGame *pGame = (CGame *)CManager::GetMode();
	CMeshField *pMeshField = pGame->GetMeshField();

	D3DXVECTOR3 setPos;
	float setSize;
	//	数を取得。
	m_DustNum = num;
	//	数分砂埃を生成。
	for (int i = 0; i < m_DustNum; i++) {
		setPos.x = rand() % (int)pMeshField->GetFieldWidth();
		setPos.y = 0.0f;
		setPos.z = rand() % (int)(pMeshField->GetFieldDepth() * 2.0f) - (pMeshField->GetFieldDepth());
		setSize = rand() % 100 + 35.0f;

		CDesertDust::Create(setPos, setSize, setSize, CTexture::G_DUST);
	}
}

/******************************************************************************
	関数名 : CDustGenerater * CDustGenerater::Create(int num)
	引数   : void
	戻り値 : なし
	説明   : 生成関数。
******************************************************************************/
CDustGenerater * CDustGenerater::Create(int num)
{
	CDustGenerater *obj = new CDustGenerater;
	obj->Init(num);
	return obj;
}
