/******************************************************************************
	タイトル名：テクスチャクラス
	ファイル名：texture.h
	作成者    ：AT-13C-284 35 深澤佑太
	作成日     : 2016/11/25
	更新日     :
******************************************************************************/
/******************************************************************************
	インクルードガード
******************************************************************************/
#pragma once

/******************************************************************************
	クラス宣言
******************************************************************************/
class CTexture
{
public:
	CTexture();			//	コンストラクタ。
	~CTexture();		//	デストラクタ。

	//	sceneの列挙型。
	typedef enum {
		TITLE = 0,		//	タイトル。
		GAME,			//	ゲーム。
		RESULT,			//	リザルト。
		SCENE_MAX
	};

	//	タイトルで使うテクスチャ
	typedef enum {
		T_BACKGROUND = 0,	//	タイトルの背景画像。
		//T_PRESS_ENTER,		//	Press_Enter の画像。
		T_MAX				//	タイトルで使う画像の最大数。
	}TITLE_TEXTURE;

	//	ゲームで使うテクスチャ
	typedef enum {
		G_BACKGROUND = 0,	//	ゲームの背景画像。
		G_FLOOR,			//	床の画像。
		G_BULLET,			//	弾の画像。
		G_EXPLOSION,		//	爆発の画像。
		G_NUMBER,			//	0~9の数字の画像。
		G_SKYBOX,			//	SkyBoxの画像。
		G_SMOKE,			//	煙の画像。
		G_LIFE,				//	ライフに使う画像。
		G_LIVEBAR,			//	ライフバーに使う画像。
		G_DUST,				//	砂埃の画像。
		G_BOOST,			//	ブーストの画像。
		G_MAX				//	ゲームで使う画像の最大数。
	}GAME_TEXTURE;

	//	リザルトで使うテクスチャ
	typedef enum {
		R_BACKGROUND = 0,
		//R_PRESS_ENTER,
		R_MAX
	}RESULT_TEXTURE;

	//	テクスチャの構造体。
	typedef struct
	{
		char pass[64];					//	FilePassが入る変数
		LPDIRECT3DTEXTURE9 pTexture;	//	テクスチャのポインタが入る変数
	}TEXTURE;

	static CTexture *Create(int index);	//	作成関数。
	static void UnLoad(CTexture *texture, int index);		//	解放関数。
	void TextureInit(int index);		//	引数に応じた初期化をする関数。
	void TextureUninit(int index);		//	引数に応じた破棄処理をする関数。

	static LPDIRECT3DTEXTURE9 GetTexture(int index, int num);	//	自分が持っているテクスチャのインデクスを渡して、ファイルポインタを返してもらう関数

private:
	void Init_Use_In_Title(void);		//	タイトルで使うテクスチャの初期化関数。
	void Init_Use_In_Game(void);		//	ゲームで使うテクスチャの初期化関数。
	void Init_Use_In_Result(void);		//	リザルトで使うテクスチャの初期化関数。

	void Uninit_Use_In_Title(void);		//	タイトルで使うテクスチャの終了関数。
	void Uninit_Use_In_Game(void);		//	ゲームで使うテクスチャの終了関数。
	void Uninit_Use_In_Result(void);	//	リザルトで使うテクスチャの終了関数。

	void(CTexture::*Init[SCENE_MAX])();		//	初期化関数(関数ポインタ)。
	void(CTexture::*Uninit[SCENE_MAX])();	//	終了関数(関数ポインタ)。

	static TEXTURE m_Texture_Title[T_MAX];
	static TEXTURE m_Texture_Game[G_MAX];
	static TEXTURE m_Texture_Result[R_MAX];
};

