/******************************************************************************
	タイトル名 : Playerクラス
	ファイル名 : player.h
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/12/14
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
class CMeshField;
class CInput;
class CPlayerLife;
class CBoost;

class CPlayer : public CDynamicModel
{
public:
	CPlayer(int Priority = OBJECT_3D, OBJ_TYPE objType = OBJ_TYPE_PLAYER, SCENE_TYPE sceneType = SCENE_TYPE_GAME);		//	コンストラクタ。
	~CPlayer();		//	デストラクタ。

	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//	初期化関数。
	void Uninit(void);		//	終了関数。
	void Update(void);		//	更新関数。
	void Draw(void);		//	描画関数。

	int GetHP(void) { return m_Hp; }	//	体力取得関数。
	bool GetLockOn(void) { return m_LockOn; }
	bool GetAlive(void) { return m_Alive; }		//	生存状況取得関数。
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//	生成関数。

private:
	//	Playerのモーション数。
	typedef enum {
		MOTION_MDOE_NEUTRAL = 0,		//	待機モーション。
		MOTION_MODE_MOVE,				//	移動モーション。
		MOTION_MDOE_ACTION,				//	特殊(アクション)モーション。
		MOTION_MODE_JUMP,				//	ジャンプモーション。
		MOTION_MODE_LANDING,			//	着地モーション。
		MOTION_MODE_DIE,				//	死亡モーション。
		MOTION_MODE_MAX					//	モーションの最大数。
	};

	D3DXVECTOR3  m_movePos;		//	移動量。
	D3DXVECTOR3  m_moveRot;		//	回転量。
	D3DXVECTOR3  m_TargetPos;	//	対象の座標。
	CCollision  *m_pCollision;	//	あたり判定のクラス。
	CPlayerLife *m_Life;		//	ライフバークラス。
	CBoost      *m_BoostEffect;	//	ブーストクラス。

	int m_Hp;				//	体力の変数。
	int m_CoolingTimeBoost;	//	ブーストのクールタイム。
	bool m_Alive;			//	生きているかどうかのフラグ。
	bool m_Boost;			//	ブーストしているかどうか。
	bool m_Jump;			//	ジャンプしているかどうか。
	bool m_AirBoost;		//	エアブーストしているかどうか。
	bool m_Move;			//	動いているかどうか。
	bool m_Attack;			//	攻撃しているかどうか。
	bool m_LockOn;			//	ロックオンフラグ。
	float m_purposeRot;		//	目的の角度。
	float m_MoveSpeed;		//	移動量。
	float m_MoveInertia;	//	移動の慣性値。
	float m_RotInertia;		//	回転の慣性値。
	float m_CollRadius;		//	あたり判定の半径。

	void MoveUpdate(void);						//	移動処理。
	void MoveUpdateJoyStick(void);				//	JoyStickでの移動処理。
	void HitTest_With_Mesh(CMeshField *mesh);	//	メッシュフィールドとのあたり判定関数。
	void Angle_Modification(void);				//	角度を修正する関数。
	void Moving_Distance_Check(void);			//	移動量をチェックする関数。
	void Jump_Check(CInput *input);				//	ジャンプしたかチェックする関数。
	void Attack(CInput *input);							//	攻撃関数。
	void Boost(CInput *input);					//	ブースト処理。
	void CollisionCheck(void);					//	あたり判定の排斥処理。
	void EnemyLockOn(void);						//	敵をロックオンする関数。
};

