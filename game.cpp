/******************************************************************************
	タイトル名 : ゲームクラス
	ファイル名 : game.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/07/14
	更新日     :
******************************************************************************/
/******************************************************************************
	インクルードファイル
******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneX.h"
#include "sceneIndex.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "Mode.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "Fade.h"
#include "texture.h"
#include "game_BG.h"
#include "dynamicModel.h"
#include "motionManager.h"
#include "player.h"
#include "MeshField.h"
#include "Enemy.h"
#include "collision.h"
#include "sphere.h"
#include "collisionDetection.h"
#include "time.h"
#include "skyBox.h"
#include "dustGenerate.h"
#include "EnemyManager.h"
#include "fixedBattery.h"
#include "score.h"

/******************************************************************************
	マクロ定義
******************************************************************************/
/******************************************************************************
	構造体宣言
******************************************************************************/
//	静的メンバ変数宣言/////////////////////////////////////////////////////////
bool CGame::m_PauseFlag = false;

/******************************************************************************
	関数名 : CGame::CGame()
	説明   : コンストラクタ
******************************************************************************/
CGame::CGame()
{
	m_pCamera = NULL;
	m_Light = NULL;

	m_bMultiThreed = false;		//	マルチスレッド用フラグ。
}

/******************************************************************************
	関数名 : CGame::~CGame()
	説明   : デストラクタ
******************************************************************************/
CGame::~CGame()
{
}

/******************************************************************************
	関数名 : void CGame::Init(void)
	引数   : void
	戻り値 : なし
	説明   : メッシュフィールドの初期化、プレイヤーの初期化、メッシュドームの初期化、
			 フィールドオブジェクトの初期化、スコアの初期化、タイムの初期化、
			 レティクルの初期化、モデルマネージャの初期化、敵の初期化、カメラの初期化、
			 ライトの初期化。
******************************************************************************/
void CGame::Init(void)
{
	//	クラスの実体化
	m_pCamera = new CCamera;		//	カメラの作成。
	m_Light = new CLight;			//	ライトの作成。

	m_pCamera->Init();		//	カメラの初期化。
	m_Light->Init();		//	ライトの初期化。

	CMotionManager::Load();
	m_pGameTexture = CTexture::Create(GAME);
	m_Player = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_MeshField = CMeshField::Create("data/field_data/MeshFiled_Postision.txt", CTexture::G_FLOOR);
	//m_Enemy = CEnemy::Create(D3DXVECTOR3(300.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//CFixedBattery::Create(D3DXVECTOR3(300.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
	CSkyBox::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2500.0f, 2500.0f, 2500.0f, CTexture::G_SKYBOX);
	m_DustGenerater = CDustGenerater::Create(300);
	CEnemyManager::Load("EnemyInfo.txt");
	CScore::Create();
	//CGame_BG::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CTexture::G_BACKGROUND);
	
	m_PauseFlag = false;
}

/******************************************************************************
	関数名 : void CGame::Uninit(void)
	引数   : void
	戻り値 : なし
	説明   : シーンの解放、カメラの解放、ライトの解放、モデルマネージャの解放。
******************************************************************************/
void CGame::Uninit(void)
{
	CScene::ReleaseAll();

	//	カメラの終了処理
	m_pCamera->Uninit();
	SAFE_DELETE(m_pCamera);
	
	//	ライトの終了処理
	m_Light->Uninit();
	SAFE_DELETE(m_Light);

	SAFE_DELETE(m_DustGenerater);

	CTexture::UnLoad(m_pGameTexture, GAME);	//	テクスチャの解放。
	CMotionManager::Unload();				//	モーションマネージャの解放。
	CCollision::ReleaseAll();				//	あたり判定の解放。
	CCollisionDetection::Release();			//	あたり判定管理クラスの解放。
}

/******************************************************************************
	関数名 : void CGame::Update(void)
	引数   : void
	戻り値 : なし
	説明   : シーンの更新、カメラの更新、入力処理系統の更新。
******************************************************************************/
void CGame::Update(void)
{
	CManager *pManager = GetManager();
	CInput   *pInput = pManager->GetInput();

	if (pInput->GetKeyboardTrigger(DIK_P) == true) {
		if (CFade::GetFadeMode() == FADE_NONE) {
			m_PauseFlag = m_PauseFlag ? false : true;
		}
		if (m_PauseFlag == true) {
			
		}
	}

	if (m_PauseFlag == false) {
		//	カメラの更新
		m_pCamera->Update();

		CScene::UpdateAll();
		CCollision::UpdateAll();
		CCollisionDetection::GetInstance()->CollisionDetectionManager();
	}
	else {
		
	}

	if (pInput->GetKeyboardTrigger(DIK_RETURN) == true) {
		if (CFade::GetFadeMode() == FADE_NONE) {
			CFade::Start(FADE_OUT, new CResult);
		}
	}
}

/******************************************************************************
	関数名 : void CGame::Draw(void)
	引数   : void
	戻り値 : なし
	説明   : シーンの描画。
******************************************************************************/
void CGame::Draw(void)
{
	m_pCamera->SetCamera();

	CScene::DrawAll();
	if (m_PauseFlag == true) {
	}
}