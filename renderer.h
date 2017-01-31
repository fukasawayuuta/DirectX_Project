/******************************************************************************
	タイトル名 : レンダラクラス
	ファイル名 : renderer.h
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/04/18
	更新日     : 2016/04/19
				 2016/05/09
******************************************************************************/
/******************************************************************************
	インクルードガード
******************************************************************************/
#ifndef _RENDERER_H_
#define _RENDERER_H_

/******************************************************************************
	インクルードファイル
******************************************************************************/
#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION ( 0x0800 )	//	警告対処用
#include "dinput.h"						//	入力処理に必要
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "xaudio2.h"					//	サウンドの再生に必要
#include "Vector.h"

/******************************************************************************
	ライブラリのリンク
******************************************************************************/
#pragma comment( lib , "d3d9.lib" )		//	描画処理に必要
#pragma comment( lib , "d3dx9.lib" )	//	上記の拡張ライブラリ
#pragma comment( lib , "dxguid.lib" )	//	DirectXコンポーネント使用に必要(細かい作業に必要)
#pragma comment( lib , "winmm.lib" )	//	システム時刻取得に必要
#pragma comment( lib , "dinput8.lib" )	//	入力処理に必要

/******************************************************************************
	マクロ定義
******************************************************************************/
//	3Dポリゴンの頂点フォーマット
#define FVF_VERTEX_3D ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

//	2Dポリゴンの頂点フォーマット
#define FVF_VERTEX_2D ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

//	描画するポリゴン数
#define DRAW_PORYGON_NUM ( 2 )

/******************************************************************************
	構造体宣言
******************************************************************************/
//	3D用の構造体
typedef struct {
	D3DXVECTOR3 pos;		//	頂点座標
	D3DXVECTOR3 nor;		//	法線の向き
	D3DCOLOR    col;		//	頂点カラー
	D3DXVECTOR2 tex;		//	テクスチャ座標
}VERTEX_3D;

//	2D用の構造体
typedef struct {
	D3DXVECTOR3 pos;		//	頂点座標
	float       rhw;		//	座標変換用係数
	D3DCOLOR    col;		//	頂点カラー
	D3DXVECTOR2 tex;		//	テクスチャ座標
}VERTEX_2D;

/******************************************************************************
	クラス
******************************************************************************/
//	前方宣言///////////////////////////////////////////////////////////////////
class CRenderer
{
public:
	CRenderer();			//	コンストラクタ
	~CRenderer();			//	デストラクタ

	HRESULT Init(HINSTANCE hInstance , HWND hWnd , BOOL bWindow);			//	初期化関数
	void Uninit(void);														//	終了関数
	void Update(void);														//	更新関数
	void Draw(void);														//	描画関数
	bool DrawSceneFlag(void);												//	描画を始める関数
	void ExchangeBuffer(void);												//	描画を終了し、バッファを入れ替える関数
	static void SetBlur(bool flag) { m_bBlurFlag = flag; }

	//	デバイスのアドレスを渡す関数
	LPDIRECT3DDEVICE9 GetDevice(void){return m_pD3DDevice;}

private:
	LPDIRECT3D9       m_pD3D;			//	Direct3Dへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;		//	Direct3Dデバイスへのポインタ

	bool m_bWireframe;					//	ワイヤーフレームのフラグ
	static bool m_bBlurFlag;					//	ブラーを使うかどうかのフラグ。
	
	LPDIRECT3DTEXTURE9 m_BlurTexture1;
	LPDIRECT3DTEXTURE9 m_BlurTexture2;
	LPDIRECT3DSURFACE9 m_BlurSurface1;
	LPDIRECT3DSURFACE9 m_BlurSurface2;

	LPDIRECT3DSURFACE9 m_BackBufferSurface;
	LPDIRECT3DVERTEXBUFFER9 m_pBlurVertex;
};


#endif