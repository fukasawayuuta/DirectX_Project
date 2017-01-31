/*******************************************************************************
* タイトル名：
* ファイル名：Vector3.h
* 作成者	：AT13B284 10 小笠原啓太
* 作成日	：
********************************************************************************
* 更新履歴	：
*
*******************************************************************************/
/*******************************************************************************
* インクルードガード
*******************************************************************************/
#ifndef _VECTOR_H_
#define _VECTOR_H_

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "main.h"
#include "renderer.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/

//*************************************
// ベクトルクラス
//*************************************
class Vector3 : public D3DXVECTOR3
{
public:
	//float x, y, z;	//	OpenGLの時はこれのコメントを外して 「public D3DXVECTOR3」を消す。

	Vector3() {}
	Vector3(float x, float y, float z);

	Vector3 operator += (const Vector3 Vec3);
	Vector3 operator -= (const Vector3 Vec3);

	Vector3 operator + (const Vector3 Vec3);
	Vector3 operator - (const Vector3 Vec3);

	Vector3 operator *= (float f);
	Vector3 operator /= (float f);

	Vector3 operator * (float f);
	Vector3 operator / (float f);

	Vector3 Normalize(void);

	operator float* ();
	operator const float* () const;

	float Length(void);
};

class Vector2 : public D3DXVECTOR2
{
public:
	//float x, y;

	Vector2() {}
	Vector2(float x, float y);

	Vector2 operator += (const Vector2 Vec2);
	Vector2 operator -= (const Vector2 Vec2);

	Vector2 operator + (const Vector2 Vec2);
	Vector2 operator - (const Vector2 Vec2);

	Vector2 operator *= (float f);
	Vector2 operator /= (float f);

	Vector2 operator * (float f);
	Vector2 operator / (float f);

	Vector2 Normalize(void);

	operator float* ();
	operator const float* () const;

	float Length(void);
};

#endif
