/******************************************************************************
	タイトル名 : ライトクラス
	ファイル名 : light.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/04/26
	更新日     :
******************************************************************************/
/******************************************************************************
	インクルードファイル
******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "light.h"

/******************************************************************************
	関数名 : CLight::CLight()
	説明   : コンストラクタ
******************************************************************************/
CLight::CLight()
{

}

/******************************************************************************
	関数名 : CLight::~CLight()
	説明   : デストラクタ
******************************************************************************/
CLight::~CLight()
{

}

/******************************************************************************
	関数名 : void CLight::Init(void)
	引数   : void
	戻り値 : なし
	説明   : ライトの設定
******************************************************************************/
void CLight::Init(void)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	D3DXVECTOR3 vecDir;		//作業用変数

	//	1つ目のライトの設定
	ZeroMemory(&m_Light[0], sizeof(D3DLIGHT9));		//0で初期化

	m_Light[0].Type = D3DLIGHT_DIRECTIONAL;						//平行光源
	m_Light[0].Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);		//光の色
	vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);					//光の向き

	D3DXVec3Normalize((D3DXVECTOR3 *)&m_Light[0].Direction, &vecDir);

	pDevice->SetLight(0, &m_Light[0]);				//0番目のライトに、配列の0番目の値を設定
	pDevice->LightEnable(0, TRUE);					//0番目のライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);	//ライティング自体のon,off設定


	//	2つ目のライトの設定
	ZeroMemory(&m_Light[1], sizeof(D3DLIGHT9));		//0で初期化

	m_Light[1].Type = D3DLIGHT_DIRECTIONAL;		//平行光源
	m_Light[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//光の色
	vecDir = D3DXVECTOR3(-0.2f, 0.8f, 0.5f);		//光の向き

	D3DXVec3Normalize((D3DXVECTOR3 *)&m_Light[1].Direction, &vecDir);

	pDevice->SetLight(1, &m_Light[1]);		//0番目のライトに、配列の0番目の値を設定
	pDevice->LightEnable(1, TRUE);				//0番目のライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);		//ライティング自体のon,off設定
}

/******************************************************************************
	関数名 : void CLight::Uninit(void)
	引数   : void
	戻り値 : なし
	説明   : 終了処理
******************************************************************************/
void CLight::Uninit(void)
{

}

/******************************************************************************
	関数名 : void CLight::Update(void)
	引数   : void
	戻り値 : なし
	説明   : 更新処理
******************************************************************************/
void CLight::Update(void)
{

}