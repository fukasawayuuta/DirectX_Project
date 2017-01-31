/******************************************************************************
	タイトル名 : Scoreクラス
	ファイル名 : score.h
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2017/01/12
	更新日     : 
******************************************************************************/
/******************************************************************************
	インクルードガード
******************************************************************************/
#pragma once

/******************************************************************************
	クラス宣言
******************************************************************************/
class CNumber;

class CScore : public CScene2D
{
public:
	CScore(int Priority = OBJECT_2D, OBJ_TYPE objType = OBJ_TYPE_UI, SCENE_TYPE sceneType = SCENE_TYPE_GAME);		//	コンストラクタ。
	~CScore();		//	デストラクタ。

	void Init(void);	//	初期化関数。
	void Uninit(void);	//	終了関数。
	void Update(void);	//	更新関数。
	void Draw(void);	//	描画関数。

	static void AddScore(int point) { m_Score += point; }	//	スコア加点関数。
	static void SubScore(int point) { m_Score -= point; }	//	スコア減点関数。
	static int GetScore(void) { return m_Score; }			//	スコア取得関数。
	static CScore *Create(void);
private:
	static int m_Score;	//	得点。
	CNumber *m_Number;	//	ナンバー。
	int m_DigitNumber;	//	桁数。
};

