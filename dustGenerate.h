/******************************************************************************
	タイトル名 : CDustGeneraterクラス
	ファイル名 : dustGenerater.h
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2017/01/19
	更新日     : 
******************************************************************************/
/******************************************************************************
	インクルードガード
******************************************************************************/
#pragma once

/******************************************************************************
	クラス
******************************************************************************/
class CDesertDust;
class CDustGenerater
{
public:
	CDustGenerater();		//	コンストラクタ。
	~CDustGenerater();		//	デストラクタ。

	void Init(int num);		//	初期化関数。
	static CDustGenerater *Create(int num);		//	生成関数。
private:
	int m_DustNum;		//	砂埃の数。
};

