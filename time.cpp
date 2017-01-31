/******************************************************************************
	タイトル名 : タイマークラス
	ファイル名 : time.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2017/01/12
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
#include "texture.h"
#include "number.h"
#include "time.h"

/******************************************************************************
	マクロ定義
******************************************************************************/
const int ANIMATION_TIME_MAX_CNT = 10;  //	表示するファイルにあるすべてのアニメーション数
const int ANIMATION_TIME_MAX_CNT_X = 10;//	表示するファイルにあるX座標のアニメーション数
const int ANIMATION_TIME_MAX_CNT_Y = 1; //	表示するファイルにあるY座標のアニメーション数
const int TIME_NUMBER_OF_DIGITS = 3;	//	桁数
const float TIME_WIDTH = 50.0f;			//	ポリゴンの幅
const float TIME_HEIGHT = 50.0f;		//	ポリゴンの高さ
const int TIME_MAX = 15;				//	制限時間時間
const int TIME_FPS = 60;				//	fpsカウンタ

//	静的メンバ変数/////////////////////////////////////////////////////////////
int CTime::m_Time = 0;

/******************************************************************************
	関数名 : CTime::CTime(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene2D(Priority, objType, sceneType)
	説明   : コンストラクタ
******************************************************************************/
CTime::CTime(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene2D(Priority, objType, sceneType)
{
	m_Time = 0;
	m_TimerCnt = 0;
	m_Number = NULL;
}

/******************************************************************************
	関数名 : CTime::~CTime()
	説明   : デストラクタ
******************************************************************************/
CTime::~CTime()
{
}

/******************************************************************************
	関数名 : void CTime::Init(void)
	引数   : void
	戻り値 : なし
	説明   : CNumberの初期化と、変数の初期化
******************************************************************************/
void CTime::Init(void)
{
	m_Width = TIME_WIDTH;
	m_Height = TIME_HEIGHT;
	m_DigitNumber = TIME_NUMBER_OF_DIGITS;

	m_AnimationCountX_Axis = ANIMATION_TIME_MAX_CNT_X;
	m_AnimationCountY_Axis = ANIMATION_TIME_MAX_CNT_Y;
	m_AnimationTotalNumber = m_AnimationCountX_Axis * m_AnimationCountY_Axis;

	//	桁数だけ動的確保
	m_Number = new CNumber[m_DigitNumber];

	//	座標設定
	m_Pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.1f, 0.0f);

	//	桁ごとの初期化
	for (int i = 0;i < m_DigitNumber; i++) {
		m_Number[i].Init(m_Pos, m_Width, m_Height);
		//	座標変更
		m_Pos.x -= m_Width;
	}
	//	タイム設定
	m_Time = TIME_MAX;
}

/******************************************************************************
	関数名 : void CTime::Uninit(void)
	引数   : void
	戻り値 : なし
	説明   : CNumberの終了処理
******************************************************************************/
void CTime::Uninit(void)
{
	//	桁数の分終了処理
	for (int i = 0; i < m_DigitNumber; i++) {
		m_Number[i].Uninit();
	}
	//	削除してNULLを代入
	SAFE_DELETE_ARRAY(m_Number);
}

/******************************************************************************
	関数名 : void CTime::Update(void)
	引数   : void
	戻り値 : なし
	説明   : 更新処理
******************************************************************************/
void CTime::Update(void)
{
	//	作業用変数
	int workTime = m_Time;
	int nTime;

	//	タイムの桁ごとの更新
	for (int i = 0; i < m_DigitNumber; i++) {
		//	一桁取得
		nTime = workTime % 10;
		//	桁ごとの更新
		m_Number[i].Update(nTime, m_AnimationCountX_Axis, m_AnimationCountY_Axis);
		//	次の桁準備
		workTime = workTime / 10;
	}

	//	タイムカウンタアップしてTIME_FPSをこえたら0を、超えてなければ今の値を返す
	m_TimerCnt = m_TimerCnt < TIME_FPS ? m_TimerCnt + 1 : 0;

	//	タイムカウンタが0以下だったら
	if (m_TimerCnt <= 0) {
		//	タイムマイナス
		--m_Time;

		//	タイムが0以下だったら
		if (m_Time <= 0) {
			m_Time = 0;
		}
	}
}

/******************************************************************************
	関数名 : void CTime::Draw(void)
	引数   : void
	戻り値 : なし
	説明   : 描画処理
******************************************************************************/
void CTime::Draw(void)
{
	for (int i = 0; i < m_DigitNumber; i++) {
		m_Number[i].Draw();
	}
}

/******************************************************************************
	関数名 : CTime *CTime::Create(void)
	引数   : void
	戻り値 : なし
	説明   : 作成関数
******************************************************************************/
CTime *CTime::Create(void)
{
	CTime *pScene = new CTime;
	pScene->Init();

	return pScene;
}