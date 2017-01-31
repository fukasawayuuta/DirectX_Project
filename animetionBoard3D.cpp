/******************************************************************************
	タイトル名 : CAnimetionBoard3Dクラス
	ファイル名 : animetionBoard3D.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/12/17
	更新日     : 
******************************************************************************/
/******************************************************************************
	インクルードファイル
******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "texture.h"
#include "scene3D.h"
#include "camera.h"
#include "billboard.h"
#include "animetionBoard3D.h"


/******************************************************************************
	関数名 : CAnimetionBoard3D::CAnimetionBoard3D(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CBillboard(Priority, objType, sceneType)
	説明   : コンストラクタ
			 変数の初期化。
******************************************************************************/
CAnimetionBoard3D::CAnimetionBoard3D(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CBillboard(Priority, objType, sceneType)
{
	m_UpdateUVCnt = 0;
	m_PatternAnim = 0;
	m_AnimMaxX = 0;
	m_AnimMaxY = 0;
	m_AnimTotalMax = 0;
}

/******************************************************************************
	関数名 : CAnimetionBoard3D::~CAnimetionBoard3D()
	説明   : デストラクタ
			 クラスが破棄されたときに呼ばれるが、中身がないので特に記述する事なし。
******************************************************************************/
CAnimetionBoard3D::~CAnimetionBoard3D()
{
}

/******************************************************************************
	関数名 :void CAnimetionBoard3D::TextureUV_Update(void)
	引数   : void
	戻り値 : なし
	説明   : アニメーションの処理。
******************************************************************************/
void CAnimetionBoard3D::TextureUV_Update(void)
{
	VERTEX_3D *pVtx;

	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//	テクスチャ座標更新
	pVtx[0].tex.x = (m_PatternAnim % m_AnimMaxX) * (1.0f / m_AnimMaxX);
	pVtx[0].tex.y = ((m_PatternAnim - (m_PatternAnim % m_AnimMaxX)) / m_AnimMaxX) * ((float)1 / m_AnimMaxY);

	pVtx[1].tex.x = (m_PatternAnim % m_AnimMaxX) * (1.0f / m_AnimMaxX) + (1.0f / m_AnimMaxX);
	pVtx[1].tex.y = ((m_PatternAnim - (m_PatternAnim % m_AnimMaxX)) / m_AnimMaxX) * ((float)1 / m_AnimMaxY);

	pVtx[2].tex.x = (m_PatternAnim % m_AnimMaxX) * (1.0f / m_AnimMaxX);
	pVtx[2].tex.y = (((m_PatternAnim - (m_PatternAnim % m_AnimMaxX)) / m_AnimMaxX) * ((float)1 / m_AnimMaxY)) + ((float)1 / m_AnimMaxY);

	pVtx[3].tex.x = (m_PatternAnim % m_AnimMaxX) * (1.0f / m_AnimMaxX) + (1.0f / m_AnimMaxX);
	pVtx[3].tex.y = (((m_PatternAnim - (m_PatternAnim % m_AnimMaxX)) / m_AnimMaxX) * ((float)1 / m_AnimMaxY)) + ((float)1 / m_AnimMaxY);

	m_pVtxBuffPolygon->Unlock();
}
