/******************************************************************************
	タイトル名 : レンダラクラス
	ファイル名 : renderer.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/04/18
	更新日     : 2016/04/19
******************************************************************************/
/******************************************************************************
	インクルードファイル
******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"

/******************************************************************************
	静的メンバ変数
******************************************************************************/
bool CRenderer::m_bBlurFlag = false;

/******************************************************************************
	関数名 : CRenderer::CRenderer()
	説明   : コンストラクタ
			 生成時に  m_pD3D, m_pD3DDevice  にNULLを代入
******************************************************************************/
CRenderer::CRenderer()
{
	m_pD3D       = NULL;
	m_pD3DDevice = NULL;
}

/******************************************************************************
	関数名 : CRenderer::~CRenderer()
	説明   : デストラクタ
			 破棄されたときに呼ばれるが、中身がないので特に記述する事なし
******************************************************************************/
CRenderer::~CRenderer()
{

}

/******************************************************************************
	関数名 ; HRESULT CRenderer::Init( HINSTANCE hInstance , HWND hWnd , BOOL bWindow )
	引数   : hInstance, hWnd, bWindow
	戻り値 : S_OK, E_FAIL
	説明   : オブジェクトの作成し、画面幅や高さ、バックバッファの形式と数を指定し、
			 ウィンドウモードかフルスクリーンモードで出すかを指定し、
			 デバイスを作成する。

			 ポリゴンを描画するための初期化処理を入れてある。
******************************************************************************/
HRESULT CRenderer::Init(HINSTANCE hInstance , HWND hWnd , BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	VERTEX_2D *pVtx;
	m_pBlurVertex = NULL;

	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );//オブジェクトの生成

	if( m_pD3D == NULL )
		return E_FAIL;

	//現在のディスプレイモードを取得
	if( FAILED( m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT , &d3ddm ) ) )
		return E_FAIL;

	ZeroMemory( &d3dpp , sizeof( d3dpp ) );

	d3dpp.BackBufferWidth = SCREEN_WIDTH;			//画面の幅
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;			//画面の高さ
	d3dpp.BackBufferFormat = d3ddm.Format;			//バックバッファ形式
	d3dpp.BackBufferCount = 1;						//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;			//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;		//デプスバッファとして16ビットを使用
	d3dpp.Windowed = bWindow;						//ウィンドウモード/フルスクリーンモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//Direct3Dデバイスの生成
	if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT ,
									  D3DDEVTYPE_HAL ,
									  hWnd ,
								      D3DCREATE_MULTITHREADED,
									  &d3dpp ,
									  &m_pD3DDevice ) ) ) {
		if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT ,
										  D3DDEVTYPE_HAL ,
										  hWnd ,
										  D3DCREATE_SOFTWARE_VERTEXPROCESSING ,
										  &d3dpp ,
										  &m_pD3DDevice ) ) ) {
			if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT ,
											  D3DDEVTYPE_REF ,
											  hWnd ,
											  D3DCREATE_SOFTWARE_VERTEXPROCESSING ,
											  &d3dpp ,
											  &m_pD3DDevice ) ) ) {
				return E_FAIL;

			}
		}
	}

	//	レンダーステートの設定
	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_CCW );			//	カリングの設定
	m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , TRUE );			//	アルファブレンドの設定
	m_pD3DDevice->SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );		//	ソースブレンドの設定
	m_pD3DDevice->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );	//	インバースブレンドの設定

	//	サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	//	テクスチャの縮小時の補間設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	//	テクスチャの拡大時の補間設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	//	テクスチャ(U値) 繰り返しに設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	//	テクスチャ(V値) 繰り返しに設定

	//テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0 , D3DTSS_ALPHAOP , D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0 , D3DTSS_ALPHAARG1 , D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0 , D3DTSS_ALPHAARG2 , D3DTA_CURRENT);

	//	フォグ///////////////////////////////////////////////////////////////////////////////////////////
	D3DCAPS9 caps;
	ZeroMemory(&caps, sizeof(D3DCAPS9));    //初期化
	m_pD3DDevice->GetDeviceCaps(&caps);
    
	//ピクセルフォグが使えるかどうか確認
	if((caps.RasterCaps & D3DPRASTERCAPS_FOGRANGE) == 0) 
		return FALSE;

	//フォグの設定
	FLOAT StartPos   = 200;  //開始位置
	FLOAT EndPos     = 2000; //終了位置
	m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON
	m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_RGBA(0xf5, 0xde, 0xb3, 0xff)); //白色で不透明
	m_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);      //頂点モード
	m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);     //テーブルモード
	m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*) (&StartPos)); //開始位置
	m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*) (&EndPos));     //終了位置
	//	フォグ///////////////////////////////////////////////////////////////////////////////////////////

	//	ワイヤーフレームフラグ設定
	m_bWireframe = false;

	D3DXCreateTexture(m_pD3DDevice, SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_BlurTexture1);
	m_BlurTexture1->GetSurfaceLevel(0, &m_BlurSurface1);

	D3DXCreateTexture(m_pD3DDevice, SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_BlurTexture2);
	m_BlurTexture2->GetSurfaceLevel(0, &m_BlurSurface2);

	m_pD3DDevice->GetRenderTarget(0, &m_BackBufferSurface);

	//頂点バッファの生成
	if (FAILED(m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pBlurVertex, NULL))) {
		return E_FAIL;
	}

	//頂点情報へのポインタを取得
	m_pBlurVertex->Lock(0, 0, (void**)&pVtx, 0);

	//	座標設定
	pVtx[0].pos = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH + 1.0f, -1.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-1.0f, SCREEN_HEIGHT + 1.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH + 1.0f, SCREEN_HEIGHT + 1.0f, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//テクスチャ座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//	色設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 254);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 254);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 254);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 254);

	//	バッファ解放
	m_pBlurVertex->Unlock();

	return S_OK;
}

/******************************************************************************
	関数名 : void CRenderer::Uninit(void)
	引数   : void
	戻り値 : なし
	説明   : 生成したデバイスやオブジェクトの解放
			 ポリゴンの解放処理
******************************************************************************/
void CRenderer::Uninit(void)
{
	//	デバイスがNULLじゃなかったら
	if(m_pD3DDevice != NULL) {
		m_pD3DDevice->Release();	//	デバイスの開放
		m_pD3DDevice = NULL;
	}

	//	オブジェクトがNULLじゃなかったら
	if( m_pD3D != NULL ) {
		m_pD3D->Release();			//	オブジェクトの開放
		m_pD3D = NULL;
	}

	m_BlurTexture1->Release();
	m_BlurTexture1 = NULL;
	m_BlurTexture2->Release();
	m_BlurTexture2 = NULL;
	m_BlurSurface1->Release();
	m_BlurSurface1 = NULL;
	m_BlurSurface2->Release();
	m_BlurSurface2 = NULL;
	m_BackBufferSurface->Release();
	m_BackBufferSurface = NULL;
	m_pBlurVertex->Release();
	m_pBlurVertex = NULL;
}

/******************************************************************************
	関数名 : void CRenderer::Update(void)
	引数   : void
	戻り値 : なし
	説明   : なし
******************************************************************************/
void CRenderer::Update(void)
{
	// 入力処理へのポインタを取得
	CManager *pManager = GetManager();
	CInput *pInput = pManager->GetInput();

	//	描画方法の切り替え
	if (pInput->GetKeyboardTrigger(DIK_F8)) {
		m_bWireframe = m_bWireframe == true ? false : true;

		if (m_bWireframe == true) {
			//	ワイヤーフレームで描画
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}
		else {
			//	通常モードで描画
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
	}
}

/******************************************************************************
	関数名 : void CRenderer::Draw(void)
	引数   : void
	戻り値 : なし
	説明   : なし
******************************************************************************/
void CRenderer::Draw(void)
{

}

/******************************************************************************
	関数名 : bool CRenderer::DrawSceneFlag(void)
	引数   : void
	戻り値 : true, false
	説明   : ポリゴンの描画処理フラグ
******************************************************************************/
bool CRenderer::DrawSceneFlag(void)
{
	//	テクスチャに書き込み
	if (m_bBlurFlag) {
		m_pD3DDevice->SetRenderTarget(0, m_BlurSurface1);
	}
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR RGB(0, 0, 0, 0), 1.0f, 0);//D3DCOLOR RGB( R , G , B , A(透明度:0~255) )

	if (SUCCEEDED(m_pD3DDevice->BeginScene())) {
		//成功したら描画開始
		return true;
	}
	else {
		return false;
	}
}

/******************************************************************************
	関数名 : void CRenderer::ExchangeBuffer(void)
	引数   : void
	戻り値 : void
	説明   : 描画を終了して、バッファを入れ替える
******************************************************************************/
void CRenderer::ExchangeBuffer(void)
{
	m_pD3DDevice->EndScene();	//	描画終了

	if (m_bBlurFlag) {
		m_pD3DDevice->BeginScene();
		m_pD3DDevice->SetTexture(0, m_BlurTexture2);
		/* 全画面2Dポリゴン描画 */
		//頂点バッファをデータストリームにバインド
		m_pD3DDevice->SetStreamSource(0, m_pBlurVertex, 0, sizeof(VERTEX_2D));
		//頂点座標ふぉーまっと
		m_pD3DDevice->SetFVF(FVF_VERTEX_2D);
		//ポリゴン描画
		m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		m_pD3DDevice->EndScene();

		//	バックバッファに書き込み
		m_pD3DDevice->SetRenderTarget(0, m_BackBufferSurface);
		m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR RGB(0, 0, 0, 255), 1.0f, 0);//D3DCOLOR RGB( R , G , B , A(透明度:0~255) )
		m_pD3DDevice->BeginScene();
		m_pD3DDevice->SetTexture(0, m_BlurTexture1);

		/* 全画面2Dポリゴン描画 */
		//頂点バッファをデータストリームにバインド
		m_pD3DDevice->SetStreamSource(0, m_pBlurVertex, 0, sizeof(VERTEX_2D));
		//頂点座標ふぉーまっと
		m_pD3DDevice->SetFVF(FVF_VERTEX_2D);
		//ポリゴン描画
		m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		/* 全画面2Dポリゴン描画 */

		m_pD3DDevice->EndScene();	//	描画終了
		//	バックバッファに書き込み

		//	ポインタ入れ替え
		LPDIRECT3DTEXTURE9 texture;
		texture = m_BlurTexture1;
		m_BlurTexture1 = m_BlurTexture2;
		m_BlurTexture2 = texture;
		//	ポインタ入れ替え
		LPDIRECT3DSURFACE9 surface;
		surface = m_BlurSurface1;
		m_BlurSurface1 = m_BlurSurface2;
		m_BlurSurface2 = surface;
	}

	//バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}