/******************************************************************************
	タイトル名 : Scoreクラス
	ファイル名 : score.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2017/01/12
	更新日     : 
******************************************************************************/
/******************************************************************************
	インクルード宣言
******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "scene2D.h"
#include "texture.h"
#include "number.h"
#include "score.h"

/******************************************************************************
	マクロ定義
******************************************************************************/
const int ANIMATION_MAX_CNT = 10;	//	表示するファイルにあるすべてのアニメーション数
const int ANIMATION_MAX_CNT_X = 10;	//	表示するファイルにあるX座標のアニメーション数
const int ANIMATION_MAX_CNT_Y = 1;	//	表示するファイルにあるY座標のアニメーション数
const int NUMBER_OF_DIGITS = 3;		//	桁数
const float WIDTH = 50.0f;			//	ポリゴンの幅
const float HEIGHT = 50.0f;			//	ポリゴンの高さ
const int SCORE_MAX = 100000000;	//	スコアの最大値

//	静的メンバ変数/////////////////////////////////////////////////////////////
int CScore::m_Score = 0;


/******************************************************************************
	関数名 : CScore::CScore(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene2D(Priority, objType, sceneType)
	説明   : コンストラクタ
******************************************************************************/
CScore::CScore(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CScene2D(Priority, objType, sceneType)
{
	m_Score = 0;
	m_Number = NULL;
}

/******************************************************************************
	関数名 : CScore::~CScore()
	説明   : デストラクタ
******************************************************************************/
CScore::~CScore()
{
}

/******************************************************************************
	関数名 : void CScore::Init(void)
	引数   : void
	戻り値 : なし
	説明   : 初期化、座標設定
******************************************************************************/
void CScore::Init(void)
{
	m_Width = WIDTH;
	m_Height = HEIGHT;
	m_DigitNumber = NUMBER_OF_DIGITS;

	m_AnimationCountX_Axis = ANIMATION_MAX_CNT_X;
	m_AnimationCountY_Axis = ANIMATION_MAX_CNT_Y;
	m_AnimationTotalNumber = m_AnimationCountX_Axis * m_AnimationCountY_Axis;
	//	桁数だけ動的確保
	m_Number = new CNumber[m_DigitNumber];

	//	座標設定
	m_Pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.1f, 0.0f);

	//	桁ごとの初期化
	for (int i = 0; i < m_DigitNumber; i++) {
		m_Number[i].Init(m_Pos, m_Width, m_Height);
		//	座標変更
		m_Pos.x -= m_Width;
	}
}

/******************************************************************************
	関数名 : void CScore::Uninit(void)
	引数   : void
	戻り値 : なし
	説明   : 解放、終了処理
******************************************************************************/
void CScore::Uninit(void)
{
	//	桁数の分終了処理
	for (int i = 0; i < m_DigitNumber; i++) {
		m_Number[i].Uninit();
	}
	//	削除してNULLを代入
	SAFE_DELETE_ARRAY(m_Number);
}

/******************************************************************************
	関数名 : void CScore::Update(void)
	引数   : void
	戻り値 : なし
	説明   : 更新処理
******************************************************************************/
void CScore::Update(void)
{
	//	作業用変数。
	int workScore = m_Score;
	int nScore = 0;

	//	ナンバー更新。
	for (int i = 0; i < m_DigitNumber; i++) {
		nScore = workScore % 10;

		m_Number[i].Update(nScore, m_AnimationCountX_Axis, m_AnimationCountY_Axis);

		workScore = workScore / 10;
	}
}

/******************************************************************************
	関数名 : void CScore::Draw(void)
	引数   : void
	戻り値 : なし
	説明   : 描画処理
******************************************************************************/
void CScore::Draw(void)
{
	for (int i = 0; i < m_DigitNumber; i++) {
		m_Number[i].Draw();
	}
}

/******************************************************************************
	関数名 : CScore *CScore::Create(void)
	引数   : void
	戻り値 : obj
	説明   : 生成関数。
******************************************************************************/
CScore * CScore::Create(void)
{
	CScore *obj = new CScore;
	obj->Init();
	return obj;
}
