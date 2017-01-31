/******************************************************************************
	タイトル名 : Bulletクラス
	ファイル名 : bullet.h
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/12/15
	更新日     : 
******************************************************************************/
/******************************************************************************
	インクルードガード
******************************************************************************/
#pragma once

/******************************************************************************
	クラス宣言
******************************************************************************/
class CCollision;
class CMeshField;

class CBullet : CBillboard
{
public:
	CBullet(int Priority = OBJECT_BILLBOARD, OBJ_TYPE objType = OBJ_TYPE_BULLET, SCENE_TYPE sceneType = SCENE_TYPE_GAME);		//	コンストラクタ。
	~CBullet();		//	デストラクタ。

	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float height, int textureType, CScene::OBJ_TYPE objType);		//	初期化関数。
	void Uninit(void);		//	終了処理。
	void Update(void);		//	更新関数。

	void RendererBegin(void);		//	描画前設定関数。
	void RendererEnd(void);			//	描画後設定関数。
	void CheckOutsideRange(CMeshField *mesh);		//	範囲外チェック。

	OBJ_TYPE GetTypeUsingSelf(void) { return m_TypeUsingSelf; }		//	自分を使った対象のタイプを取得する関数。
	static void Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float width, float height, int textureType, CScene::OBJ_TYPE objType);

private:
	CCollision *m_pCollision;		//	あたり判定のクラス。

	OBJ_TYPE m_TypeUsingSelf;		//	自分を使った対象のタイプ。
	D3DXVECTOR3 m_GoAngle;			//	自分の進行方向の角度。
	D3DXVECTOR3 m_Movement;

	float m_CollRadius;	//	あたり判定の半径。
	float m_MovingDistance;	//	移動量。
};

