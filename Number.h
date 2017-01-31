#pragma once
class CNumber
{
public:
	CNumber();		//	コンストラクタ。
	~CNumber();		//	デストラクタ。

	void Init(D3DXVECTOR3 pos, float width, float height);		//	初期化関数。
	void Uninit(void);											//	終了関数。
	void Update(int num, int anim_X_Max, int anim_Y_Max);		//	更新関数。
	void Draw(void);											//	描画関数。

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffPolygon;	//	頂点バッファ。
	D3DXCOLOR m_Color;					//	色情報。
	D3DXVECTOR3 m_pos;					//	座標。
	float m_width;						//	幅。
	float m_height;						//	高さ。
	int m_TextureType;					//	テクスチャのタイプ。
};

