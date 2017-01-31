/******************************************************************************
	タイトル名 : CFixedBatteryクラス
	ファイル名 : fixedBattery.h
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/12/17
	更新日     :
******************************************************************************/
/******************************************************************************
	インクルードガード
******************************************************************************/
#pragma once

/******************************************************************************
	クラス宣言
******************************************************************************/
class CPlayer;

class CFixedBattery : public CEnemy
{
public:
	CFixedBattery(int Priority = OBJECT_ENEMY, OBJ_TYPE objType = OBJ_TYPE_ENEMY, SCENE_TYPE sceneType = SCENE_TYPE_GAME);		//	コンストラクタ。
	~CFixedBattery();		//	デストラクタ。

	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Update(void);

	static void Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void ViewDecision(CPlayer *player);

private:
	//	モーションの列挙型
	typedef enum {
		MOTION_MDOE_NEUTRAL = 0,	//	待機モーション。
		MOTION_MODE_SEARCH,			//	索敵モーション。
		MOTION_MODE_MAX				//	モーション最大数。
	};

	//	モデルの列挙型
	typedef enum {
		FOOT = 0,		//	足のモデル。
		BODY,			//	体のモデル。
		BARREL,			//	銃身のモデル。
		MAX				//	モデルの最大数。
	};

	int m_ShotInterval;		//	弾を打つ間隔。
	bool m_Discovery;		//	見つけたかどうか。
	float m_ViewDistance;	//	視界距離。
	float m_ViewAngle;		//	視界角度。
};

