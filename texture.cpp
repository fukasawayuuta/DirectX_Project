/******************************************************************************
	タイトル名：テクスチャクラス
	ファイル名：texture.cpp
	作成者    ：AT-13C-284 35 深澤佑太
	作成日     : 2016/11/25
	更新日     :
******************************************************************************/
/******************************************************************************
	インクルードファイル
******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

/******************************************************************************
	静的メンバ変数
******************************************************************************/
//	タイトル画面で使うテクスチャの一覧
CTexture::TEXTURE CTexture::m_Texture_Title[T_MAX] = {
	{"data/texture/title_BG001.jpg"}		//	2D背景画像。
};

//	ゲーム画面で使うテクスチャの一覧。
CTexture::TEXTURE CTexture::m_Texture_Game[G_MAX] = {
	{"data/texture/game_BG001.jpg"},		//	2D背景画像。
	{"data/texture/ground002.jpg"},			//	地面の画像。
	{"data/texture/block.png"},				//	弾の画像
	{"data/texture/explosion002.png"},		//	爆発の画像。
	{"data/texture/number000.png"},			//	数字の画像。
	{"data/texture/skybox004.png"},			//	skyboxの画像。
	{"data/texture/smoke001.png"},			//	煙の画像。
	{"data/texture/player_hp.png"},			//	ライフの画像。
	{"data/texture/player_hp_var.png"},		//	ライフバーの画像。
	{"data/texture/Dust.png"},				//	砂埃の画像。
	{"data/texture/boost.png"}				//	ブーストの画像。
};

//	リザルト画面で使うテクスチャの一覧。
CTexture::TEXTURE CTexture::m_Texture_Result[R_MAX] = {
	{"data/texture/result_BG001.jpg"}		//	2D背景画像。
};

/******************************************************************************
	関数名 : CTexture::CTexture()
	説明   : 関数ポインタを設定。
******************************************************************************/
CTexture::CTexture()
{
	//	関数ポインタ設定。
	Init[TITLE] = &CTexture::Init_Use_In_Title;
	Init[GAME] = &CTexture::Init_Use_In_Game;
	Init[RESULT] = &CTexture::Init_Use_In_Result;
	//	関数ポインタ設定。
	Uninit[TITLE] = &CTexture::Uninit_Use_In_Title;
	Uninit[GAME] = &CTexture::Uninit_Use_In_Game;
	Uninit[RESULT] = &CTexture::Uninit_Use_In_Result;
}

/******************************************************************************
	関数名 : CTexture::~CTexture()
	説明   : デストラクタ。
******************************************************************************/
CTexture::~CTexture()
{
}

/******************************************************************************
	関数名 : void CTexture::Init_Use_In_Title(void)
	引数   : void
	戻り値 : なし
	説明   : タイトルで使うテクスチャの初期化。
******************************************************************************/
void CTexture::Init_Use_In_Title(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	//	初期化
	for (int i = 0; i < T_MAX; i++) {
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, m_Texture_Title[i].pass, &m_Texture_Title[i].pTexture);
	}
}

/******************************************************************************
	関数名 : void CTexture::Init_Use_In_Game(void)
	引数   : void
	戻り値 : なし
	説明   : ゲームで使うテクスチャの初期化。
******************************************************************************/
void CTexture::Init_Use_In_Game(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	//	初期化
	for (int i = 0; i < G_MAX; i++) {
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, m_Texture_Game[i].pass, &m_Texture_Game[i].pTexture);
	}
}

/******************************************************************************
	関数名 : void CTexture::Init_Use_In_Result(void)
	引数   : void
	戻り値 : なし
	説明   : リザルトで使うテクスチャの初期化。
******************************************************************************/
void CTexture::Init_Use_In_Result(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	//	初期化
	for (int i = 0; i < R_MAX; i++) {
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, m_Texture_Result[i].pass, &m_Texture_Result[i].pTexture);
	}
}

/******************************************************************************
	関数名 : void CTexture::Uninit_Use_In_Title(void)
	引数   : void
	戻り値 : なし
	説明   : テクスチャの解放処理。
******************************************************************************/
void CTexture::Uninit_Use_In_Title(void)
{
	//	テクスチャの数だけ回す
	for (int i = 0; i < T_MAX; i++) {
		SAFE_RELEASE(m_Texture_Title[i].pTexture);
	}
}

/******************************************************************************
	関数名 : void CTexture::Uninit_Use_In_Game(void)
	引数   : void
	戻り値 : なし
	説明   : テクスチャの解放処理。
******************************************************************************/
void CTexture::Uninit_Use_In_Game(void)
{
	//	テクスチャの数だけ回す
	for (int i = 0; i < G_MAX; i++) {
		SAFE_RELEASE(m_Texture_Game[i].pTexture);
	}
}

/******************************************************************************
	関数名 : void CTexture::Uninit_Use_In_Result(void)
	引数   : void
	戻り値 : なし
	説明   : テクスチャの解放処理。
******************************************************************************/
void CTexture::Uninit_Use_In_Result(void)
{
	//	テクスチャの数だけ回す
	for (int i = 0; i < R_MAX; i++) {
		SAFE_RELEASE(m_Texture_Result[i].pTexture);
	}
}

/******************************************************************************
	関数名 : CTexture * CTexture::Create(int index)
	引数   : index
	戻り値 : なし
	説明   : 生成関数。
******************************************************************************/
CTexture * CTexture::Create(int index)
{
	CTexture *obj = new CTexture;
	obj->TextureInit(index);

	return obj;
}

/******************************************************************************
	関数名 : void CTexture::UnLoad(CTexture * texture, int index)
	引数   : texture
	戻り値 : なし
	説明   : 解放関数。
******************************************************************************/
void CTexture::UnLoad(CTexture * texture, int index)
{
	texture->TextureUninit(index);
	SAFE_DELETE(texture);
}

/******************************************************************************
	関数名 : void CTexture::TextureInit(int index)
	引数   : index
	戻り値 : なし
	説明   : 引数のポインタの中の初期化処理を行う。
******************************************************************************/
void CTexture::TextureInit(int index)
{
	if (index < SCENE_MAX)
		(this->*Init[index])();
}

/******************************************************************************
	関数名 : void CTexture::TextureUninit(int index)
	引数   : index
	戻り値 : なし
	説明   : 引数のポインタの中の解放処理を行う。
******************************************************************************/
void CTexture::TextureUninit(int index)
{
	if (index < SCENE_MAX)
		(this->*Uninit[index])();
}

/******************************************************************************
	関数名 : LPDIRECT3DTEXTURE9 CTexture::GetTexture(int index, int num)
	引数   : index, num
	戻り値 : なし
	説明   : 引数のindexのnum番目のテクスチャのポインタを返す。
******************************************************************************/
LPDIRECT3DTEXTURE9 CTexture::GetTexture(int index, int num)
{
	if ((index - 1) < SCENE_MAX) {
		switch (index - 1) {
		case 0:
			if (num < T_MAX)
				return m_Texture_Title[num].pTexture;
			else 
				return NULL;
			break;
		case 1:
			if (num < G_MAX)
				return m_Texture_Game[num].pTexture;
			else 
				return NULL;
			break;
		case 2:
			if (num < R_MAX)
				return m_Texture_Result[num].pTexture;
			else
				return NULL;
			break;
		default:
			return NULL;
			break;
		}
	}
	else {
		return NULL;
	}
	return NULL;
}
