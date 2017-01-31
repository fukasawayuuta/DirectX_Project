/******************************************************************************
	タイトル名 : DirectX 雛形
	ファイル名 : main.h
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/04/18
	更新日     : 
******************************************************************************/
#ifndef _MAIN_H_
#define _MAIN_H_

/******************************************************************************
	インクルードファイル
******************************************************************************/
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "d3dx9.h"
#include "Vector.h"
#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h> 
#include <crtdbg.h>
#include <list>

/******************************************************************************
	ライブラリのリンク
******************************************************************************/
/******************************************************************************
	マクロ定義
******************************************************************************/
#define SCREEN_WIDTH ( 1280 )		//	画面の幅
#define SCREEN_HEIGHT ( 720 )		//	画面の高さ
#define null (NULL)
#define _CRT_SECURE_NO_WARNINGS
#define MATH_LENGTH_2D(width, height) sqrtf(width * width + height * height)		//	対角線の長さ計算
#define MATH_LENGTH_3D(width, height, depth) sqrtf(width * width + height * height + depth * depth)		//	対角線の長さ計算
#define MATH_ANGLE(width, height) atan2f(width, height);		//	角度計算
#define _CRT_SECURE_NO_WARNINGS

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p) = NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p) = NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p) = NULL; } }
#endif

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW 
#endif 
#endif

/******************************************************************************
	構造体宣言
******************************************************************************/

/******************************************************************************
	クラス宣言
******************************************************************************/
//	前方宣言///////////////////////////////////////////////////////////////////
class CManager;

/******************************************************************************
	プロトタイプ宣言
******************************************************************************/
CManager  *GetManager(void);
#endif