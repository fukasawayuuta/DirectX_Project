/******************************************************************************
	タイトル名 : Enemyクラス
	ファイル名 : Enemy.h
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/12/17
	更新日     :
******************************************************************************/
/******************************************************************************
	インクルードガード
******************************************************************************/
#pragma once

/******************************************************************************
	クラス
******************************************************************************/
class CCollision;

class CEnemy : public CDynamicModel
{
public:
	CEnemy(int Priority = NONE, OBJ_TYPE objType = OBJ_TYPE_NONE, SCENE_TYPE sceneType = SCENE_TYPE_NONE);		//	コンストラクタ。
	~CEnemy();		//	デストラクタ。

	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, DYNAMICMODEL_TYPE modelType);		//	初期化関数。
	void Uninit(void);		//	終了関数。
	void Update(void);		//	更新関数。
	void Draw(void);		//	描画関数。

	void AddHp(int power) { m_Hp += power; }		//	Hp加算関数。
	void SubHp(int power) { m_Hp -= power; }		//	Hp減算関数。

	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, DYNAMICMODEL_TYPE modelType);		//	生成関数。

protected:
	CCollision *m_pCollision;		//	あたり判定クラス。
	int m_Hp;						//	体力。
	float m_Radius;					//	半径。
};

