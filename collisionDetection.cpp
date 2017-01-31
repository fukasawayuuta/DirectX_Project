/******************************************************************************
	タイトル名 : collisionDetectionクラス
	ファイル名 : collisionDetection.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2017/01/05
	更新日     : 
******************************************************************************/
/******************************************************************************
	インクルード宣言
******************************************************************************/
#include "main.h"
#include "collision.h"
#include "sphere.h"
#include "capsule.h"
#include "collisionDetection.h"
#include "collisionList.h"

/******************************************************************************
	静的メンバ変数宣言
******************************************************************************/
CCollisionDetection *CCollisionDetection::m_pInstance = NULL;

/******************************************************************************
	関数名 : void CCollisionDetection::CollisionDetectionManager(void)
	引数   : void
	戻り値 : なし
	説明   : あたり判定のする関数。
******************************************************************************/
void CCollisionDetection::CollisionDetectionManager(void)
{
	for (int i = 0; i < CCollision::PRIORITY_MAX; i++) {
		CCollision *scene = CCollision::GetListTopPointer(i);
		CCollision *NextScene;

		while (scene != NULL) {
			NextScene = scene->GetSceneNext();

			for (int i = 0; i < CCollision::PRIORITY_MAX; i++) {
				CCollision *_scene = CCollision::GetListTopPointer(i);
				CCollision *_NextScene;

				while (_scene != NULL) {
					_NextScene = _scene->GetSceneNext();
					if (scene != _scene) {
						scene->Dispatch(_scene);
					}
					_scene = _NextScene;
				}
			}
			scene = NextScene;
		}
	}
}

/******************************************************************************
	関数名 : void CCollisionDetection::Sphere_To_Sphere(CSphere * sphere1, CSphere * sphere2)
	引数   : sphere1, sphere2
	戻り値 : なし
	説明   : 球体と球体のあたり判定をする関数。
******************************************************************************/
void CCollisionDetection::Sphere_To_Sphere(CSphere * sphere1, CSphere * sphere2)
{
	D3DXVECTOR3 difference;
	D3DXVec3Subtract(&difference, &sphere1->GetWorldPos(), &sphere2->GetWorldPos());		//	お互いの座標を引いて差分を求める。
	//	求めた差分が2つの半径より短ければ衝突。
	if (D3DXVec3LengthSq(&difference) <= pow((sphere1->GetRadius() + sphere2->GetRadius()), 2)) {
		//	当たったフラグをセット。
		sphere1->SetHit(true);
		sphere2->SetHit(true);
		//	衝突した相手の情報をお互いにセット。
		sphere1->GetCollisionList()->AddList(sphere2);
		sphere2->GetCollisionList()->AddList(sphere1);
	}
}

/******************************************************************************
	関数名 : void CCollisionDetection::Capsule_To_Capsule(CCapsule * capsule1, CCapsule * capsule2)
	引数   : capsule1, capsule2
	戻り値 : なし
	説明   : カプセルとカプセルのあたり判定をする関数。
******************************************************************************/
void CCollisionDetection::Capsule_To_Capsule(CCapsule * capsule1, CCapsule * capsule2)
{
	D3DXVECTOR3 difference1;
	D3DXVECTOR3 difference2;

	NearPoint(capsule1, capsule2, difference1, difference2);		//	お互いの近似点をもとめる。
	D3DXVec3Subtract(&difference1, &difference1, &difference2);		//	お互いの近似点から座標をひいて差分を求める。
	//	求めた差分が2つの半径より短ければ衝突。
	if (D3DXVec3LengthSq(&difference1) <= pow((capsule1->GetSegment()->radius + capsule2->GetSegment()->radius), 2)) {
		//	当たったフラグをセット。
		capsule1->SetHit(true);
		capsule2->SetHit(true);
		//	衝突した相手の情報をお互いにセット。
		capsule1->GetCollisionList()->AddList(capsule2);
		capsule2->GetCollisionList()->AddList(capsule1);
	}
}

/******************************************************************************
	関数名 : void CCollisionDetection::Sphere_To_Capsule(CSphere * sphere, CCapsule * capsule)
	引数   : sphere, capsule
	戻り値 : なし
	説明   : カプセルと球体のあたり判定をする関数。
******************************************************************************/
void CCollisionDetection::Sphere_To_Capsule(CSphere * sphere, CCapsule * capsule)
{
	D3DXVECTOR3 difference;
	//	球体とカプセルの近似点の距離を引いて差分を求める。
	D3DXVec3Subtract(&difference, &sphere->GetWorldPos(), &NearPoint(sphere, capsule));

	//	求めた差分が2つの半径より短ければ衝突。
	if (D3DXVec3LengthSq(&difference) <= pow((sphere->GetRadius() + capsule->GetSegment()->radius), 2)) {
		//	当たったフラグをセット。
		sphere->SetHit(true);
		capsule->SetHit(true);
		//	衝突した相手の情報をお互いにセット。
		sphere->GetCollisionList()->AddList(capsule);
		capsule->GetCollisionList()->AddList(sphere);
	}
}

/******************************************************************************
	関数名 : D3DXVECTOR3 CCollisionDetection::NearPoint(CSphere * sphere, CCapsule * capsule)
	引数   : sphere, capsule
	戻り値 : start
	説明   : カプセルと球体の近似点をもとめて値を返す関数。
******************************************************************************/
D3DXVECTOR3 CCollisionDetection::NearPoint(CSphere * sphere, CCapsule * capsule)
{
	D3DXVECTOR3 start = capsule->GetWorldPos() + capsule->GetSegment()->start_pos;		//	ベクトルの始点。
	D3DXVECTOR3 vec1 = capsule->GetSegment()->end_pos - capsule->GetSegment()->start_pos;	//	始点と終点の方向ベクトル。
	D3DXVECTOR3 vec2 = sphere->GetWorldPos() - start;			//	弾とカプセルの始点の方向ベクトル。
	D3DXVECTOR3 nor1;		//	正規化した単位ベクトルが入る変数。
	float rate;

	D3DXVec3Normalize(&nor1, &vec1);		//	vec1を正規化した単位ベクトルをnor1に代入。

	rate = D3DXVec3Dot(&vec1, &vec2) / D3DXVec3LengthSq(&vec1);		//	割合を求める。
	rate = rate < 0.0f ? 0.0f : (rate > 1.0f ? 1.0f : rate);		//	上限下限の設定。

	return start += (vec1 * rate);		//	近似点の設定。
}

/******************************************************************************
	関数名 : void CCollisionDetection::NearPoint(CCapsule *capsule1, CCapsule *capsule2, D3DXVECTOR3 & pOut1, D3DXVECTOR3 & pOut2)
	引数   : capsule1, capsule2, pOut1, pOut2
	戻り値 : なし
	説明   : カプセル同士の近似点をもとめて値をそれぞれpOut1, pOut2に代入する関数。
******************************************************************************/
void CCollisionDetection::NearPoint(CCapsule *capsule1, CCapsule *capsule2, D3DXVECTOR3 & pOut1, D3DXVECTOR3 & pOut2)
{
	D3DXVECTOR3 start1 = capsule1->GetWorldPos() + capsule1->GetSegment()->start_pos;
	D3DXVECTOR3 start2 = capsule2->GetWorldPos() + capsule2->GetSegment()->start_pos;

	D3DXVECTOR3 vec1 = capsule1->GetSegment()->end_pos - capsule1->GetSegment()->start_pos;	//	始点と終点の方向ベクトル。
	D3DXVECTOR3 vec2 = start2 - start1;			//	弾とカプセルの始点の方向ベクトル。
	D3DXVECTOR3 nor1;		//	正規化した単位ベクトルが入る変数。
	float rate;

	D3DXVec3Normalize(&nor1, &vec1);		//	vec1を正規化した単位ベクトルをnor1に代入。

	rate = D3DXVec3Dot(&vec1, &vec2) / D3DXVec3LengthSq(&vec1);		//	割合を求める。
	rate = rate < 0.0f ? 0.0f : (rate > 1.0f ? 1.0f : rate);		//	上限下限の設定。

	pOut1 = start1 + (vec1 * rate);

	vec1 = capsule2->GetSegment()->end_pos - capsule2->GetSegment()->start_pos;	//	始点と終点の方向ベクトル。
	vec2 = pOut1 - start2;			//	弾とカプセルの始点の方向ベクトル。

	D3DXVec3Normalize(&nor1, &vec1);		//	vec1を正規化した単位ベクトルをnor1に代入。

	rate = D3DXVec3Dot(&vec1, &vec2) / D3DXVec3LengthSq(&vec1);		//	割合を求める。
	rate = rate < 0.0f ? 0.0f : (rate > 1.0f ? 1.0f : rate);		//	上限下限の設定。

	pOut2 = start2 + (vec1 * rate);
}

/******************************************************************************
	関数名 : CCollisionDetection::CCollisionDetection()
	説明   : コンストラクタ
******************************************************************************/
CCollisionDetection::CCollisionDetection()
{
}

/******************************************************************************
	関数名 : CCollisionDetection::~CCollisionDetection()
	説明   : デストラクタ
******************************************************************************/
CCollisionDetection::~CCollisionDetection()
{
}
