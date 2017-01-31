/******************************************************************************
	ファイル名 : collision.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2017/01/05
	更新日     : 
******************************************************************************/
/******************************************************************************
	インクルード宣言
******************************************************************************/
#include "main.h"
#include "collision.h"
#include "collisionList.h"
#include "scene.h"

/******************************************************************************
	静的メンバ変数
******************************************************************************/
CCollision *CCollision::m_Top[PRIORITY_MAX];		//	先頭のポインタの配列。
CCollision *CCollision::m_Cur[PRIORITY_MAX];		//	現在のポインタの配列。

/******************************************************************************
	関数名 : CCollision::CCollision(CScene *scene, int Priority)
	説明   : コンストラクタ
******************************************************************************/
CCollision::CCollision(CScene *scene, int Priority)
{
	//	先頭だった場合
	if (m_Top[Priority] == NULL) {
		m_Top[Priority] = this;			//	自分のポインタを設定
		m_Cur[Priority] = this;			//	自分のポインタを設定
		m_Prev = NULL;					//	自分の前のポインタにNULLを設定
		m_Next = NULL;					//	自分の次のポインタにNULLを設定
	}
	else {	//	先頭以外だった場合
		m_Cur[Priority]->m_Next = this;		//	次のアドレス設定
		m_Prev = m_Cur[Priority];			//	前のアドレス設定
		m_Cur[Priority] = this;				//	アドレス更新
		m_Next = NULL;						//	自分の次のアドレスはまだないのでNULL設定
	}

	m_pScene = scene;
	m_Priority = Priority;
}

/******************************************************************************
	関数名 : CCollision::~CCollision()
	説明   : コンストラクタ
******************************************************************************/
CCollision::~CCollision()
{
}

/******************************************************************************
	関数名 : void CCollision::Update(void)
	引数   : void
	戻り値 : なし
	説明   : 親の座標を受け取って、座標を更新する。
******************************************************************************/
void CCollision::Update(void)
{
	if (m_Delete == false) {
		if (m_pScene != NULL) {
			m_WorldPos = m_pScene->GetPos() + m_LoaclPos;
		}
		else {
			m_WorldPos = m_LoaclPos;
		}
	}
}
/******************************************************************************
	関数名 : void CCollision::UpdateAll(void)
	引数   : void
	戻り値 : なし
	説明   : あたり判定の更新。
******************************************************************************/
void CCollision::UpdateAll(void)
{
	//	優先度が高い順に更新
	for (int i = 0; i < PRIORITY_MAX; i++) {
		CCollision *scene = m_Top[i];
		CCollision *NextScene;

		while (scene != null) {
			//	次のポインタ取得
			NextScene = scene->m_Next;
			
			scene->Update();		//	更新
			
			scene = NextScene;		//	次のポインタ代入
		}

		scene = m_Top[i];		//	先頭アドレス代入

		while (scene != null) {
			//	次のポインタ取得
			NextScene = scene->m_Next;
			//	削除フラグがtrueだったら
			if (scene->m_Delete == true) {
				scene->Release(scene->m_Priority);		//	リストから削除
			}

			scene = NextScene;		//	次のポインタ代入
		}
	}
}

/******************************************************************************
	関数名 : void CCollision::DrawAll(void)
	引数   : void
	戻り値 : なし
	説明   : 描画処理。
******************************************************************************/
void CCollision::DrawAll(void)
{
	//	優先度が高い順に描画
	for (int i = 0; i < PRIORITY_MAX; i++) {
		CCollision *scene = m_Top[i];
		CCollision *NextScene;

		while (scene != NULL) {
			//	次のポインタ取得
			NextScene = scene->m_Next;
			scene->Draw();			//	描画
			scene = NextScene;		//	次のポインタ代入
		}
	}
}

/******************************************************************************
	関数名 : void CCollision::ReleaseAll(void)
	引数   : void
	戻り値 : なし
	説明   : 解放処理。
******************************************************************************/
void CCollision::ReleaseAll(void)
{
	for (int i = 0; i < PRIORITY_MAX; i++) {
		CCollision *scene = m_Top[i];
		CCollision *NextScene;

		//	NULL じゃない限り回す
		while (scene != NULL) {
			//	消す前に次のポインタ取得
			NextScene = scene->m_Next;
			scene->Release(scene->m_Priority);	//	リストから削除
			scene = NextScene;		//	次のポインタ代入
		}
		m_Top[i] = null;
	}
}

/******************************************************************************
	関数名 : void CCollision::ReleaseAll(void)
	引数   : void
	戻り値 : なし
	説明   : 一つだけ解放してリストを繋ぎ直す。
******************************************************************************/
void CCollision::Release(int Priority)
{
	//	一つ前のポインタがなくて、次のポインタがある場合 (自分 -> 次 ~ )
	if (this->m_Prev == NULL && this->m_Next != NULL) {
		m_Top[Priority] = this->m_Next;				//	自分の次を先頭に
		this->m_Next->m_Prev = NULL;		//	今自分の次に入っている m_Prev には自分が入っているため NULL を入れる

	}//	次のポインタがあって、前のポインタがない場合 (~ 前 -> 自分)
	else if (this->m_Prev != NULL && this->m_Next == NULL) {
		m_Cur[Priority] = this->m_Prev;				//	自分の前を最後に
		this->m_Prev->m_Next = NULL;		//	今今自分の前に入っている m_Next には自分が入っているため NULL を入れる

	}//	前にも次にもポインタがある場合 (~ -> 前 -> 自分 -> 次 -> ~)
	else if (this->m_Prev != NULL && this->m_Next != NULL) {
		this->m_Prev->m_Next = this->m_Next;		//	自分の前のポインタの次のポインタに自分の次のポインタを入れる
		this->m_Next->m_Prev = this->m_Prev;		//	自分の次のポインタの前のポインタに自分の前のポインタを入れる

	}//	自分だけの場合
	else if (this == m_Top[Priority] && this == m_Cur[Priority]) {
		m_Top[Priority] = NULL;			//	m_Top に NULL を入れる
		m_Cur[Priority] = NULL;			//	m_Cur に NULL を入れる
	}

	//	解放処理と削除
	this->Uninit();
	delete this;
}