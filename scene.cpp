/******************************************************************************
	タイトル名 : 描画クラス
	ファイル名 : scene.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/04/19
	更新日     : 2016/06/21
******************************************************************************/
/******************************************************************************
	インクルードファイル
******************************************************************************/
#include "main.h"
#include "scene.h"
#include "renderer.h"
#include "manager.h"

/******************************************************************************
	グローバル変数宣言
******************************************************************************/
//	静的メンバ変数宣言/////////////////////////////////////////////////////////
CScene *CScene::m_Top[PRIORITY_MAX] = {NULL};		//	NULLで初期化
CScene *CScene::m_Cur[PRIORITY_MAX] = {NULL};		//	NULLで初期化

/******************************************************************************
	関数名 : CScene::CScene()
	説明   : コンストラクタ
			 リスト構造生成
			 アドレスの代入
******************************************************************************/
CScene::CScene(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType)
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

	m_Objtype = objType;		//	オブジェクトタイプを設定
	m_Priority = Priority;		//	優先度を設定
	m_SceneType = sceneType;	//	シーンを設定
}

/******************************************************************************
	関数名 : CScene::~CScene()
	説明   : デストラクタ
			 破棄されたときに呼ばれるが、中身がないので特に記述する事なし
******************************************************************************/
CScene::~CScene()
{
}

/******************************************************************************
	関数名 : void CScene::ReleaseAll(void)
	引数   : void
	戻り値 : なし
	説明   : 登録されているオブジェクトを一気に解放
******************************************************************************/
void CScene::ReleaseAll(void)
{
	for (int i = 0 ; i < PRIORITY_MAX; i++) {
		CScene *scene = m_Top[i];
		CScene *NextScene;

		//	NULL じゃない限り回す
		while (scene != NULL) {
			//	消す前に次のポインタ取得
			NextScene = scene->m_Next;

			scene->Uninit();		//	解放処理
			delete scene;			//	削除
			scene = NextScene;		//	次のポインタ代入
		}
		m_Top[i] = null;
	}
}
/******************************************************************************
	関数名 : void CScene::Release(void)
	引数   : void
	戻り値 : なし
	説明   : 登録されているオブジェクトを解放して
			 リスト構造をつなぎなおす
******************************************************************************/
void CScene::Release(int Priority)
{
	//	一つ前のポインタがなくて、次のポインタがある場合 (自分 -> 次 ~ )
	if (this->m_Prev == NULL && this->m_Next != NULL) {
		m_Top[Priority] = this->m_Next;				//	自分の次を先頭に
		this->m_Next->m_Prev = NULL;		//	今自分の次に入っている m_Prev には自分が入っているため NULL を入れる

	}//	次のポインタがあって、前のポインタがない場合 (~ 前 -> 自分)
	else if(this->m_Prev != NULL && this->m_Next == NULL) {
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
/******************************************************************************
	関数名 : void CScene::UpdateAll(void)
	引数   : void
	戻り値 : なし
	説明   : 登録されているオブジェクトを更新
			 削除フラグがtrueになっているものを削除する
******************************************************************************/
void CScene::UpdateAll(void)
{
	//	優先度が高い順に更新
	for (int i = 0; i < PRIORITY_MAX; i++) {
		CScene *scene = m_Top[i];
		CScene *NextScene;

		while (scene != null) {
			//	次のポインタ取得
			NextScene = scene->m_Next;
			//	アクティブだったら。
			if (scene->GetActive()) {
				scene->Update();		//	更新
			}
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
	関数名 : void CScene::DrawAll(void)
	引数   : void
	戻り値 : なし
	説明   : 登録されているオブジェクトを描画
******************************************************************************/
void CScene::DrawAll(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//	優先度が高い順に描画
	for (int i = 0; i < PRIORITY_MAX; i++) {
		CScene *scene = m_Top[i];
		CScene *NextScene;

		if (i >= OBJECT_BILLBOARD) {
			pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);	//	フォグ：OFF
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		//	ライティング : OFF
		}

		while (scene != NULL) {
			//	次のポインタ取得
			NextScene = scene->m_Next;

			//	アクティブだったら。
			if (scene->GetActive()) {
				scene->Draw();			//	描画
			}
			//}
			scene = NextScene;		//	次のポインタ代入
		}

		if (i >= OBJECT_BILLBOARD) {
			pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);		//	フォグ：ON
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);		//	ライティング : OFF
		}
	}
}