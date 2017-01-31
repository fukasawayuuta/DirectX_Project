/******************************************************************************
	タイトル名 : DirectX 雛形
	ファイル名 : main.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/04/18
	更新日     : 
******************************************************************************/
/******************************************************************************
	scanfのwarning防止
******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS

/******************************************************************************
	インクルードファイル
******************************************************************************/
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "main.h"

/******************************************************************************
	マクロ定義
******************************************************************************/
#define PROJECTNAME ( "TEST" )
#define ID_BUTTON000 ( 101 )/* ボタンのID */
#define POSPOLYGONCNT ( 3 )

/******************************************************************************
	構造体定義
******************************************************************************/
/******************************************************************************
	プロトタイプ宣言
******************************************************************************/
LRESULT CALLBACK WndProc( HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam );
HRESULT Init( HINSTANCE hInstance , HWND hWnd , BOOL bWindow );
void Uninit( void );
void Update( void );
void Draw( void );

/******************************************************************************
	グローバル変数
******************************************************************************/
CManager *g_pManager;

/******************************************************************************
	関数名：int WINAPI WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nCmdShow )
	引数  ：HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nCmdShow
	戻り値：正常終了：int型の0
	説明  :DirectXの使用に必要な雛形作成
******************************************************************************/
int WINAPI WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nCmdShow )
{
	//	マネージャークラスの生成
	g_pManager = new CManager;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	WNDCLASSEX wcex = {
		sizeof( WNDCLASSEX ) ,           //メモリサイズを指定
		CS_CLASSDC ,                     //ウィンドウのスタイルを設定
		WndProc,                         //ウィンドウプロシージャのアドレス( 関数名 )を指定
		0 ,                              //使用しない
		0 ,                              //使用しない
		hInstance ,                      //WinMainのインスタンスハンドルを指定
		NULL ,                           //使用するアイコンの指定
		LoadCursor( NULL , IDC_ARROW ) , //マウスカーソルを指定( ウィンドウ内 )
		( HBRUSH )( COLOR_WINDOW + 1 ) , //クライアント領域の背景色を設定
		NULL ,                           //メニューの設定
		PROJECTNAME ,                    //ウィンドウクラスの名前( ニックネーム的な )
		NULL,                            //小さいアイコンが設定された場合の情報
	};

	RECT rect = { 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT };
	AdjustWindowRect( &rect , WS_OVERLAPPEDWINDOW , false );

	HWND hWnd;//ウィンドウのハンドル
	MSG msg;  //メッセージ

	RegisterClassEx( &wcex );//ウィンドウクラスの登録
	hWnd = CreateWindowEx( 0 ,
						   PROJECTNAME ,			//ウィンドウクラスの名前
						   "TEST" ,					//ウィンドウの名前
						   WS_OVERLAPPEDWINDOW ,	//ウィンドウのスタイル
						   CW_USEDEFAULT ,			//左上からのX座標
						   CW_USEDEFAULT ,			//左上からのY座標
						   rect.right - rect.left ,	//幅  ( 整数で入力 )
						   rect.bottom - rect.top ,	//高さ( 整数で入力 )
						   NULL ,					//親ウィンドウのハンドル
						   NULL ,					//メニューハンドル又は、子ウィンドウID
						   hInstance ,				//
						   NULL );					//ウィンドウの作成データ

	if( FAILED( Init( hInstance , hWnd , TRUE ) ) )//初期化処理
	{
		return -1;
	}

	/* ウィンドウの表示 */
	ShowWindow( hWnd , nCmdShow );//非クライアント領域
	UpdateWindow( hWnd );         //クライアント領域

	/* メッセージループ */
	while( 1 )
	{
		if( PeekMessage( &msg , NULL , 0 , 0 , PM_REMOVE ) != 0 )//Windowsが処理しているとき
		{
			if( msg.message == WM_QUIT )
			{
				break;
			}
			else
			{
				/* メッセージの翻訳を送出 */
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		else//DirectXが処理
		{
			Update();	//更新処理
			Draw();		//描画処理
		}
	}

	UnregisterClass( PROJECTNAME , wcex.hInstance );//ウィンドウクラスの登録解除

	Uninit();//終了処理
	return ( int )msg.wParam;

}
LRESULT CALLBACK WndProc( HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam )
{
	int nID;
	RECT rect;
	static HWND hWndButton000;

	switch( uMsg )
	{
	case WM_DESTROY://ウィンドウを破棄

		PostQuitMessage( 0 );//" WM_QUIT "メッセージを返す
		break;

	case WM_KEYDOWN://キーが押された

		switch( wParam )
		{
		case VK_ESCAPE://仮想キー

			nID = MessageBox( hWnd , " 終了しますか？ " , "  " , MB_YESNO + MB_ICONQUESTION );

			if( nID == IDYES )
			{
				DestroyWindow( hWnd );//ウィンドウを破棄
			}
			break;

		case VK_F11:
			static bool fullScreen = true;

			if (fullScreen == true) {
				rect.left = 0;
				rect.top = 0;
				rect.right = 1920;
				rect.bottom = 1080;

				SetWindowPos(hWnd, HWND_NOTOPMOST, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
				SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);

				fullScreen = false;
			}
			else {
				rect.left = 1920 / 5;
				rect.top = 1080 / 6;
				rect.right = rect.left + SCREEN_WIDTH;
				rect.bottom = rect.top + SCREEN_HEIGHT;

				SetWindowPos(hWnd, HWND_NOTOPMOST, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
				SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);

				fullScreen = true;
			}
			break;
		}
		break;

	case WM_CREATE:
		break;

	case WM_TIMER:
		break;

	case WM_PAINT:
		break;

	case WM_COMMAND:

		if( LOWORD( wParam ) == ID_BUTTON000 )//[終了]おされたら
		{
			nID = MessageBox( hWnd , "  終了してよろしいですか？" , " " , MB_YESNO + MB_ICONQUESTION );

			if( nID == IDYES )
			{
				DestroyWindow( hWnd );//ウィンドウを破棄
			}
			break;
		}
		break;

	case WM_LBUTTONDOWN :

		SetFocus( hWnd );
		break;
	default :

		break;
	}

	return DefWindowProc( hWnd , uMsg , wParam , lParam );
}

/******************************************************************************
	関数名 : HRESULT Init(HINSTANCE hInstance , HWND hWnd , BOOL bWindow)
	引数   : hInstance, hWnd, bWindow
	戻り値 : E_FAIL, S_OK
	説明   : Rendererの初期化処理
******************************************************************************/
HRESULT Init(HINSTANCE hInstance , HWND hWnd , BOOL bWindow)
{
	if( FAILED(g_pManager->Init( hInstance , hWnd , TRUE ) ) )//初期化処理
	{
		//	失敗
		return E_FAIL;
	}
	
	//	問題なく終了
	return S_OK;
}

/******************************************************************************
	関数名 : void Uninit(void)
	引数   : void
	戻り値 : なし
	説明   : Rendererの終了処理した後に、Rendererを解放する。
******************************************************************************/
void Uninit(void)
{
	//	Rendererの終了処理
	g_pManager->Uninit();

	//	Rendererの解放
	delete g_pManager;

	//	NULLを代入
	g_pManager = NULL;
}

/******************************************************************************
	関数名 : void Update(void)
	引数   : void
	戻り値 : なし
	説明   : Rendererの更新処理
******************************************************************************/
void Update(void)
{
	//更新処理
	g_pManager->Update();
}

/******************************************************************************
	関数名 : void Draw(void)
	引数   : void
	戻り値 : なし
	説明   : Rendererの描画処理
******************************************************************************/
void Draw(void)
{
	//描画処理
	g_pManager->Draw();
}

/******************************************************************************
	関数名 : CManager *GetManager(void)
	引数   : void
	戻り値 : なし
	説明   : Managerのアドレスを取得する関数
******************************************************************************/
CManager *GetManager(void)
{
	return g_pManager;
}