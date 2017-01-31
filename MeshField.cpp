/******************************************************************************
	タイトル名 : MeshFieldクラス
	ファイル名 : MeshField.cpp
	作成者     : AT-13C-284 36 深澤佑太
	作成日     : 2016/12/15
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
#include "sceneIndex.h"
#include "MeshField.h"

/******************************************************************************
	関数名 : CMeshField::CMeshField(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CSceneIndex(Priority, objType, sceneType)
	説明   : コンストラクタ。
******************************************************************************/
CMeshField::CMeshField(int Priority, OBJ_TYPE objType, SCENE_TYPE sceneType) : CSceneIndex(Priority, objType, sceneType)
{
	m_SelectXIndex = 0;
	m_SelectZIndex = 0;
	m_SelectPolygonIndex = 0;
	m_SelectPolygonIndexX = 0;
	m_SelectPolygonIndexZ = 0;
	m_SelectBlockNum = 0;
}

/******************************************************************************
	関数名 : CMeshField::~CMeshField()
	説明   : デストラクタ。
******************************************************************************/
CMeshField::~CMeshField()
{
}

/******************************************************************************
	関数名 : void CMeshField::Init(const char *filename, int textureType)
	引数   : filename, textureType
	戻り値 : なし
	説明   : 使うファイルを読み込んで、ハイトマップを読み込む。
******************************************************************************/
void CMeshField::Init(const char *filename, int textureType)
{
	CManager *pManager = GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	VERTEX_3D *pVtx;
	WORD *pIdx;

	m_Field_Block_Num_Xline = 128;		//	X軸上のブロック数。
	m_Field_Block_Num_Zline = 128;		//	Z軸上のブロック数。
	m_Xline_Block_Between_Interval = 50.0f;		//	X軸上の頂点間の間隔。
	m_Zline_Block_Between_Interval = 50.0f;		//	Z軸上の頂点間の間隔。
	m_Field_Vertex_Num_Xline = m_Field_Block_Num_Xline + 1;		//	X軸上の頂点数。	
	m_Field_Vertex_Num_Zline = m_Field_Block_Num_Zline + 1;		//	Z軸上の頂点数。
	m_Width_Total_Field = (m_Field_Block_Num_Xline / 2) * m_Xline_Block_Between_Interval;		//	メッシュフィールドの全体の横幅。
	m_Depth_Total_Field = (m_Field_Block_Num_Zline / 2) * m_Zline_Block_Between_Interval;		//	メッシュフィールドの全体の奥行。
	m_VertexNum = m_Field_Vertex_Num_Xline * m_Field_Vertex_Num_Zline;		//	全体の頂点数。
	m_PolygonNum = ((m_Field_Block_Num_Xline * m_Field_Block_Num_Zline) * 2) + ((m_Field_Block_Num_Zline - 1) * 4);		//	全体のポリゴン数。
	m_Index_Total_Field = m_PolygonNum + 2;		//	全体のインデックス数。
	m_TextureType = textureType;		//	テクスチャのタイプ

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_VertexNum, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffPolygon, NULL))) {
		return;
	}

	//インデックスバッファの確保
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) *  m_Index_Total_Field, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL))) {
		return;
	}

	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 PosMeshField;

	//座標(0 , 0 , 0)を中心に描画
	for (int z = 0; z < m_Field_Vertex_Num_Zline; z++) {
		for (int x = 0; x < m_Field_Vertex_Num_Xline; x++) {
			PosMeshField = D3DXVECTOR3(x * m_Xline_Block_Between_Interval - m_Xline_Block_Between_Interval * (m_Field_Block_Num_Xline * 0.5f),
									   0.0f,
									   -z * m_Zline_Block_Between_Interval + m_Zline_Block_Between_Interval * (m_Field_Block_Num_Zline * 0.5f));

			pVtx[x + (m_Field_Vertex_Num_Xline * z)].pos = PosMeshField;
			pVtx[x + (m_Field_Vertex_Num_Xline * z)].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[x + (m_Field_Vertex_Num_Xline * z)].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[x + (m_Field_Vertex_Num_Xline * z)].tex = D3DXVECTOR2((float)x, (float)z);
		}
	}

	m_pVtxBuffPolygon->Unlock();

	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	int ZIdxCnt = 0;
	int IdxNum = 0;
	int XLineMax = (m_Field_Vertex_Num_Xline * 2) - 1;		//	右端の縮退ポリゴンのあるインデックスを求める
	int XLineMin = XLineMax + 2;						//	左端の縮退ポリゴンのあるインデックスを求める

	for (int IdxCnt = 0; IdxCnt < m_Index_Total_Field; IdxCnt++, IdxNum++) {
		pIdx[IdxCnt] = IdxNum % m_Field_Vertex_Num_Xline + ((ZIdxCnt + 1) * m_Field_Vertex_Num_Xline);

		//	もし端の縮退ポリゴンのあるインデックスだったら
		if (IdxCnt == XLineMin) {
			XLineMin = XLineMax + 2;		//	次の左端の縮退ポリゴンのあるインデックスを求める
			pIdx[IdxCnt + 1] = pIdx[IdxCnt];
			IdxCnt++;
		}

		IdxCnt++;

		pIdx[IdxCnt] = IdxNum % m_Field_Vertex_Num_Xline + ZIdxCnt * m_Field_Vertex_Num_Xline;

		//	もし右端の縮退ポリゴンのあるインデックスかつインデックスが最大じゃなかったら
		if (IdxCnt == XLineMax && XLineMax != m_Index_Total_Field - 1) {
			XLineMax += (m_Field_Block_Num_Xline + 2) * 2;		//	次の右端の縮退ポリゴンのあるインデックスを求める

			++ZIdxCnt;
			++IdxCnt;

			pIdx[IdxCnt] = pIdx[IdxCnt - 1];
		}
	}

	//	ファイルを読み込めたら。
	if (LoadField(filename) != NULL) {
		//	法線情報更新。
		UpdateMeshNormal();
	}

	m_pIdxBuff->Unlock();
}

/******************************************************************************
	関数名 : void CMeshField::Uninit(void)
	引数   : void
	戻り値 : なし
	説明   : 解放処理。
******************************************************************************/
void CMeshField::Uninit(void)
{
	CSceneIndex::Uninit();
}

/******************************************************************************
	関数名 : void CMeshField::Update(void)
	引数   : void
	戻り値 : なし
	説明   : 更新処理。
******************************************************************************/
void CMeshField::Update(void)
{
}

/******************************************************************************
	関数名 : void CMeshField::Draw(void)
	引数   : void
	戻り値 : なし
	説明   : 描画処理。
******************************************************************************/
void CMeshField::Draw(void)
{
	CSceneIndex::Draw();
}

/******************************************************************************
	関数名 : D3DXVECTOR3 CMeshField::GetFieldNormal(D3DXVECTOR3 pos)
	引数   : pos
	戻り値 : なし
	説明   : 対象がいる座標から法線ベクトルを計算し、結果を返す。
******************************************************************************/
D3DXVECTOR3 CMeshField::GetFieldNormal(D3DXVECTOR3 pos)
{
	m_SelectPolygonIndexX = (pos.x / m_Xline_Block_Between_Interval) + (m_Field_Block_Num_Xline * 0.5f);		//	プレイヤーが乗っているX軸にあるブ頂点のインデックスを調べる
	m_SelectPolygonIndexZ = (m_Field_Block_Num_Zline * 0.5f) - (pos.z / m_Zline_Block_Between_Interval);		//	プレイヤーが乗っているZ軸にあるブ頂点のインデックスを調べる
	m_SelectPolygonIndex = m_SelectPolygonIndexX + (m_Field_Vertex_Num_Xline * m_SelectPolygonIndexZ);			//	求めたXとZとのインデックスを使って、何番目の頂点のインデックスか調べる
	m_SelectBlockNum = ((m_Field_Block_Num_Xline * 2) * m_SelectPolygonIndexZ) + (m_SelectPolygonIndexX * 2);	//	求めたXとZとのインデックスを使って、何個目のポリゴンに乗っているのか調べる
	VERTEX_3D *pVtx;

	//	作業用変数
	D3DXVECTOR3 vec1, vec2;
	D3DXVECTOR3 nor1, nor2, nor3;
	float fNor1;
	D3DXVECTOR3 Normal;

	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
	m_pVtxBuffPolygon->Unlock();

	//	最初のベクトルを求める
	vec1 = pVtx[m_SelectPolygonIndex].pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline + 1].pos;
	vec2 = pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline + 1].pos;

	//	二次元の外積
	fNor1 = vec2.x * vec1.z - vec2.z * vec1.x;
	//	ノーマライズ
	D3DXVec3Cross(&nor1, &vec1, &vec2);
	D3DXVec3Normalize(&nor1, &nor1);

	//	四角形ポリゴンのどっちのポリゴンに乗っているかの判定
	if (fNor1 > 0) {
		//	インデクスを加算
		m_SelectPolygonIndex += 1;

		vec1 = pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos - pVtx[m_SelectPolygonIndex].pos;
		vec2 = pVtx[m_SelectPolygonIndex - 1].pos - pVtx[m_SelectPolygonIndex].pos;

		D3DXVec3Cross(&nor1, &vec1, &vec2);
		D3DXVec3Normalize(&nor1, &nor1);
	}
	else {
		vec1 = pVtx[m_SelectPolygonIndex].pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos;
		vec2 = pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline + 1].pos - pVtx[m_SelectPolygonIndex].pos;

		D3DXVec3Cross(&nor1, &vec1, &vec2);
		D3DXVec3Normalize(&nor1, &nor1);
	}

	Normal = nor1;
	return Normal;
}

/******************************************************************************
	関数名 : CMeshField * CMeshField::Create(const char * filename, int textureType)
	引数   : filename, textureType
	戻り値 : obj
	説明   : 生成処理。
******************************************************************************/
CMeshField * CMeshField::Create(const char * filename, int textureType)
{
	CMeshField *obj = new CMeshField;
	obj->Init(filename, textureType);
	return obj;
}

/******************************************************************************
	関数名 : float CMeshField::Collision(D3DXVECTOR3 pos)
	引数   : pos
	戻り値 : pos.y
	説明   : ポリゴンに乗っているときのY座標を求めて戻り値で返す関数。
******************************************************************************/
float CMeshField::Collision(D3DXVECTOR3 pos)
{
	m_SelectPolygonIndexX = (pos.x / m_Xline_Block_Between_Interval) + (m_Field_Block_Num_Xline * 0.5f);		//	プレイヤーが乗っているX軸にあるブ頂点のインデックスを調べる
	m_SelectPolygonIndexZ = (m_Field_Block_Num_Zline * 0.5f) - (pos.z / m_Zline_Block_Between_Interval);		//	プレイヤーが乗っているZ軸にあるブ頂点のインデックスを調べる
	m_SelectPolygonIndex = m_SelectPolygonIndexX + (m_Field_Vertex_Num_Xline * m_SelectPolygonIndexZ);			//	求めたXとZとのインデックスを使って、何番目の頂点のインデックスか調べる
	m_SelectBlockNum = ((m_Field_Block_Num_Xline * 2) * m_SelectPolygonIndexZ) + (m_SelectPolygonIndexX * 2);	//	求めたXとZとのインデックスを使って、何個目のポリゴンに乗っているのか調べる

	VERTEX_3D *pVtx;

	//	作業用変数
	D3DXVECTOR3 vec1, vec2;
	D3DXVECTOR3 nor1, nor2, nor3;
	float fNor1;

	D3DXVECTOR3 mathPos;

	//	範囲外ならNULLを返す。
	if (m_SelectPolygonIndex > m_Index_Total_Field || m_SelectPolygonIndex < 0) {
		return NULL;
	}

	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
	//	最初のベクトルを求める
	vec1 = pVtx[m_SelectPolygonIndex].pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline + 1].pos;
	vec2 = pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline + 1].pos;

	//	二次元の外積
	fNor1 = vec2.x * vec1.z - vec2.z * vec1.x;
	//	ノーマライズ
	D3DXVec3Cross(&nor1, &vec1, &vec2);
	D3DXVec3Normalize(&nor1, &nor1);

	//	四角形ポリゴンのどっちのポリゴンに乗っているかの判定
	if (fNor1 > 0) {
		//	インデクスを加算
		m_SelectPolygonIndex += 1;

		//	ベクトル求めて外積計算
		vec1 = pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos - pVtx[m_SelectPolygonIndex].pos;
		vec2 = pos - pVtx[m_SelectPolygonIndex].pos;
		//	ノーマライズ
		D3DXVec3Cross(&nor1, &vec1, &vec2);
		D3DXVec3Normalize(&nor1, &nor1);

		//	ベクトル求めて外積計算
		vec1 = pVtx[m_SelectPolygonIndex - 1].pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos;
		vec2 = pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos;
		//	ノーマライズ
		D3DXVec3Cross(&nor2, &vec1, &vec2);
		D3DXVec3Normalize(&nor2, &nor2);

		//	ベクトル求めて外積計算
		vec1 = pVtx[m_SelectPolygonIndex].pos - pVtx[m_SelectPolygonIndex - 1].pos;
		vec2 = pos - pVtx[m_SelectPolygonIndex - 1].pos;
		//	ノーマライズ
		D3DXVec3Cross(&nor3, &vec1, &vec2);
		D3DXVec3Normalize(&nor3, &nor3);

		//	三角形の中に入っているかどうか
		if (nor1 > 0 && nor2 > 0 && nor3 > 0) {		//	入ってる
			//	ベクトル求める
			vec1 = pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos - pVtx[m_SelectPolygonIndex].pos;
			vec2 = pVtx[m_SelectPolygonIndex - 1].pos - pVtx[m_SelectPolygonIndex].pos;

			//	のーまらいず
			D3DXVec3Cross(&nor1, &vec1, &vec2);
			D3DXVec3Normalize(&nor1, &nor1);

			//	高さを求める
			mathPos.y = pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos.y - ((nor1.x * (pos.x - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos.x) +
				nor1.z * (pos.z - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos.z)) / nor1.y);

			pos.y = mathPos.y;
		}
		else {		//	入ってない
			//	プレイヤーのY座標に直接代入
			pos.y = 0.0f;
		}
	}
	else {
		//	ベクトルを求める
		vec1 = pVtx[m_SelectPolygonIndex].pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos;
		vec2 = pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos;
		//	外積求めてノーマライズ
		D3DXVec3Cross(&nor1, &vec1, &vec2);
		D3DXVec3Normalize(&nor1, &nor1);

		//	ベクトルを求める
		vec1 = pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline + 1].pos - pVtx[m_SelectPolygonIndex].pos;
		vec2 = pos - pVtx[m_SelectPolygonIndex].pos;
		//	外積求めてノーマライズ
		D3DXVec3Cross(&nor2, &vec1, &vec2);
		D3DXVec3Normalize(&nor2, &nor2);

		//	ベクトルを求める
		vec1 = pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline + 1].pos;
		vec2 = pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos;
		//	外積求めてノーマライズ
		D3DXVec3Cross(&nor3, &vec1, &vec2);
		D3DXVec3Normalize(&nor3, &nor3);

		//	三角形の中に入っているかどうか
		if (nor1 > 0 && nor2 > 0 && nor3 > 0) {		//	入っている
			//	ベクトル求める
			vec1 = pVtx[m_SelectPolygonIndex].pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos;
			vec2 = pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline + 1].pos - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos;

			//	のーまらいず
			D3DXVec3Cross(&nor1, &vec1, &vec2);
			D3DXVec3Normalize(&nor1, &nor1);

			//	高さを求める
			mathPos.y = pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos.y - ((nor1.x * (pos.x - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos.x) +
				nor1.z * (pos.z - pVtx[m_SelectPolygonIndex + m_Field_Vertex_Num_Xline].pos.z)) / nor1.y);

			pos.y = mathPos.y;
		}
		else {		//	入ってない
			//	プレイヤーのY座標に直接代入
			pos.y = 0.0f;
		}
	}

	m_pVtxBuffPolygon->Unlock();

	return pos.y;
}

/******************************************************************************
	関数名 : bool CMeshField::LoadField(const char *filename)
	引数   : filename
	戻り値 : true, false
	説明   : ファイルが読み込めたらハイトマップを読み込んでtrueを返して、
			 無ければ何もせずfalseを返す。
******************************************************************************/
bool CMeshField::LoadField(const char *filename)
{
	VERTEX_3D *pVtx;
	FILE *fp;
	char str[32];

	//	ファイル書き込み
	fp = fopen(filename, "r");

	if (fp != NULL) {		//	ファイルが存在する。
		m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
		//	各頂点のY座標の頂点読み込み
		for (int i = 0; i < m_VertexNum; i++) {
			fscanf(fp, "%s", str);
			fscanf(fp, "%f", &pVtx[i].pos.y);
		}
		m_pVtxBuffPolygon->Unlock();
		fclose(fp);
		//	ファイルを閉じる
		return true;
	}
	else {		//	ファイルが存在しない。
		fclose(fp);
		//	ファイルを閉じる
		return false;
	}
}

/******************************************************************************
	関数名 : void CMeshField::UpdateMeshNormal(void)
	引数   : void
	戻り値 : なし
	説明   : メッシュフィールドの法線を更新する関数。
******************************************************************************/
void CMeshField::UpdateMeshNormal(void)
{
	VERTEX_3D *pVtx;
	//	作業用変数
	D3DXVECTOR3 vec1, vec2, vec3, vec4, vec5, vec6;			//	ベクトルが入る変数	
	D3DXVECTOR3 nor1, nor2, nor3, nor4, nor5, nor6;			//	外積の計算結果が入る変数
	int Idx;		//	頂点番号が入る変数

	m_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
	for (int i = 0; i < m_Field_Vertex_Num_Zline; i++) {
		for (int j = 0; j < m_Field_Vertex_Num_Xline; j++) {
			//	今の頂点番号が入る
			Idx = j + (m_Field_Vertex_Num_Xline * i);

			pVtx[Idx].col = D3DXCOLOR(255, 255, 255, 255);

			//	内側の頂点の法線処理
			if ((i != 0 && i != m_Field_Vertex_Num_Zline - 1) && (j != 0 && j != m_Field_Vertex_Num_Xline - 1)) {

				//	それぞれのベクトルの長さを求める
				vec1 = pVtx[(Idx - m_Field_Vertex_Num_Xline) - 1].pos - pVtx[Idx].pos;		//	左上
				vec2 = pVtx[Idx - m_Field_Vertex_Num_Xline].pos - pVtx[Idx].pos;			//	上
				vec3 = pVtx[Idx + 1].pos - pVtx[Idx].pos;									//	右
				vec4 = pVtx[(Idx + m_Field_Vertex_Num_Xline) + 1].pos - pVtx[Idx].pos;		//	右下
				vec5 = pVtx[(Idx)+m_Field_Vertex_Num_Xline].pos - pVtx[Idx].pos;			//	下
				vec6 = pVtx[(Idx)-1].pos - pVtx[Idx].pos;									//	左

				//	それぞれのベクトルの外積の計算結果をそれぞれの変数に代入
				D3DXVec3Cross(&nor1, &vec1, &vec2);
				D3DXVec3Normalize(&nor1, &nor1);

				D3DXVec3Cross(&nor2, &vec2, &vec3);
				D3DXVec3Normalize(&nor2, &nor2);

				D3DXVec3Cross(&nor3, &vec3, &vec4);
				D3DXVec3Normalize(&nor3, &nor3);

				D3DXVec3Cross(&nor4, &vec4, &vec5);
				D3DXVec3Normalize(&nor4, &nor4);

				D3DXVec3Cross(&nor5, &vec5, &vec6);
				D3DXVec3Normalize(&nor5, &nor5);

				D3DXVec3Cross(&nor6, &vec6, &vec1);
				D3DXVec3Normalize(&nor6, &nor6);

				//	今の頂点に求めた法線ベクトルの平均を設定
				D3DXVec3Normalize(&pVtx[Idx].nor, &(nor1 + nor2 + nor3 + nor4 + nor5 + nor6));
			}
			else if (i == 0 && j == 0) {	//	左上
				vec1 = pVtx[Idx + 1].pos - pVtx[Idx].pos;
				vec2 = pVtx[Idx + m_Field_Vertex_Num_Xline + 1].pos - pVtx[Idx].pos;
				vec3 = pVtx[Idx + m_Field_Vertex_Num_Xline].pos - pVtx[Idx].pos;

				//	それぞれのベクトルの外積の計算結果をそれぞれの変数に代入
				D3DXVec3Cross(&nor1, &vec1, &vec2);
				D3DXVec3Normalize(&nor1, &nor1);

				D3DXVec3Cross(&nor2, &vec2, &vec3);
				D3DXVec3Normalize(&nor2, &nor2);

				//	今の頂点に求めた法線ベクトルの平均を設定
				D3DXVec3Normalize(&pVtx[Idx].nor, &(nor1 + nor2));
			}
			else if (i == 0 && j == m_Field_Vertex_Num_Xline - 1) {		//	右上
				vec1 = pVtx[Idx + m_Field_Vertex_Num_Xline].pos - pVtx[Idx].pos;
				vec2 = pVtx[Idx - 1].pos - pVtx[Idx].pos;

				//	それぞれのベクトルの外積の計算結果をそれぞれの変数に代入
				D3DXVec3Cross(&nor1, &vec1, &vec2);
				D3DXVec3Normalize(&nor1, &nor1);

				//	それぞれのベクトルの外積の計算結果をそれぞれの変数に代入
				D3DXVec3Normalize(&pVtx[Idx].nor, &(nor1));
			}
			else if (i == m_Field_Vertex_Num_Zline - 1 && j == 0) {		//	左下
				vec1 = pVtx[Idx - m_Field_Vertex_Num_Xline].pos - pVtx[Idx].pos;
				vec2 = pVtx[Idx + 1].pos - pVtx[Idx].pos;

				//	それぞれのベクトルの外積の計算結果をそれぞれの変数に代入
				D3DXVec3Cross(&nor1, &vec1, &vec2);
				D3DXVec3Normalize(&nor1, &nor1);

				//	それぞれのベクトルの外積の計算結果をそれぞれの変数に代入
				D3DXVec3Normalize(&pVtx[Idx].nor, &(nor1));
			}
			else if (i == m_Field_Vertex_Num_Zline - 1 && j == m_Field_Vertex_Num_Xline - 1) {		//	右下
				vec1 = pVtx[Idx - 1].pos - pVtx[Idx].pos;
				vec2 = pVtx[(Idx - (m_Field_Vertex_Num_Xline)) - 1].pos - pVtx[Idx].pos;
				vec3 = pVtx[Idx - m_Field_Vertex_Num_Xline].pos - pVtx[Idx].pos;

				//	それぞれのベクトルの外積の計算結果をそれぞれの変数に代入
				D3DXVec3Cross(&nor1, &vec1, &vec2);
				D3DXVec3Normalize(&nor1, &nor1);

				D3DXVec3Cross(&nor2, &vec2, &vec3);
				D3DXVec3Normalize(&nor2, &nor2);

				//	それぞれのベクトルの外積の計算結果をそれぞれの変数に代入
				D3DXVec3Normalize(&pVtx[Idx].nor, &(nor1 + nor2));
			}
			else if ((i != 0 && i != m_Field_Vertex_Num_Zline - 1) && j == 0) {		//	左端
				vec1 = pVtx[Idx - m_Field_Vertex_Num_Xline].pos - pVtx[Idx].pos;
				vec2 = pVtx[Idx + 1].pos - pVtx[Idx].pos;
				vec3 = pVtx[Idx + (m_Field_Vertex_Num_Xline + 1)].pos - pVtx[Idx].pos;
				vec4 = pVtx[Idx + m_Field_Vertex_Num_Xline].pos - pVtx[Idx].pos;

				//	それぞれのベクトルの外積の計算結果をそれぞれの変数に代入
				D3DXVec3Cross(&nor1, &vec1, &vec2);
				D3DXVec3Normalize(&nor1, &nor1);

				D3DXVec3Cross(&nor2, &vec2, &vec3);
				D3DXVec3Normalize(&nor2, &nor2);

				D3DXVec3Cross(&nor3, &vec3, &vec4);
				D3DXVec3Normalize(&nor3, &nor3);

				//	それぞれのベクトルの外積の計算結果をそれぞれの変数に代入
				D3DXVec3Normalize(&pVtx[Idx].nor, &(nor1 + nor2 + nor3));
			}
			else if ((i != 0 && i != m_Field_Vertex_Num_Zline - 1) && j == m_Field_Vertex_Num_Xline - 1) {		//	右端
				vec1 = pVtx[Idx + m_Field_Vertex_Num_Xline].pos - pVtx[j + (m_Field_Vertex_Num_Xline * i)].pos;	//	下
				vec2 = pVtx[Idx - 1].pos - pVtx[Idx].pos;	//	左
				vec3 = pVtx[(Idx - m_Field_Vertex_Num_Xline) - 1].pos - pVtx[Idx].pos;//	左上
				vec4 = pVtx[Idx - m_Field_Vertex_Num_Xline].pos - pVtx[Idx].pos;//	上

																			//	それぞれのベクトルの外積の計算結果をそれぞれの変数に代入
				D3DXVec3Cross(&nor1, &vec1, &vec2);
				D3DXVec3Normalize(&nor1, &nor1);

				D3DXVec3Cross(&nor2, &vec2, &vec3);
				D3DXVec3Normalize(&nor2, &nor2);

				D3DXVec3Cross(&nor3, &vec3, &vec4);
				D3DXVec3Normalize(&nor3, &nor3);

				//	それぞれのベクトルの外積の計算結果をそれぞれの変数に代入
				D3DXVec3Normalize(&pVtx[Idx].nor, &(nor1 + nor2 + nor3));
			}
			else if (i == 0 && (j != 0 && j != m_Field_Vertex_Num_Xline - 1)) {		//	上端
				vec1 = pVtx[Idx + 1].pos - pVtx[Idx].pos;
				vec2 = pVtx[Idx + (m_Field_Vertex_Num_Xline + 1)].pos - pVtx[Idx].pos;
				vec3 = pVtx[Idx + m_Field_Vertex_Num_Xline].pos - pVtx[Idx].pos;
				vec4 = pVtx[Idx - 1].pos - pVtx[Idx].pos;

				//	それぞれのベクトルの外積の計算結果をそれぞれの変数に代入
				D3DXVec3Cross(&nor1, &vec1, &vec2);
				D3DXVec3Normalize(&nor1, &nor1);

				D3DXVec3Cross(&nor2, &vec2, &vec3);
				D3DXVec3Normalize(&nor2, &nor2);

				D3DXVec3Cross(&nor3, &vec3, &vec4);
				D3DXVec3Normalize(&nor3, &nor3);

				//	それぞれのベクトルの外積の計算結果をそれぞれの変数に代入
				D3DXVec3Normalize(&pVtx[Idx].nor, &(nor1 + nor2 + nor3));

			}
			else if (i == m_Field_Vertex_Num_Zline - 1 && (j != 0 && j != m_Field_Vertex_Num_Xline - 1)) {		//	下端
				vec1 = pVtx[Idx - 1].pos - pVtx[Idx].pos;
				vec2 = pVtx[(Idx - m_Field_Vertex_Num_Xline) - 1].pos - pVtx[Idx].pos;
				vec3 = pVtx[(Idx - m_Field_Vertex_Num_Xline)].pos - pVtx[Idx].pos;
				vec4 = pVtx[Idx + 1].pos - pVtx[Idx].pos;

				//	それぞれのベクトルの外積の計算結果をそれぞれの変数に代入
				D3DXVec3Cross(&nor1, &vec1, &vec2);
				D3DXVec3Normalize(&nor1, &nor1);

				D3DXVec3Cross(&nor2, &vec2, &vec3);
				D3DXVec3Normalize(&nor2, &nor2);

				D3DXVec3Cross(&nor3, &vec3, &vec4);
				D3DXVec3Normalize(&nor3, &nor3);

				//	それぞれのベクトルの外積の計算結果をそれぞれの変数に代入
				D3DXVec3Normalize(&pVtx[Idx].nor, &(nor1 + nor2 + nor3));
			}
		}
	}
	m_pVtxBuffPolygon->Unlock();
}
