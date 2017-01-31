#pragma once
class CResult_BG : public CScene2D
{
public:
	CResult_BG(int Priority = OBJECT_2D, OBJ_TYPE objType = OBJ_TYPE_UI, SCENE_TYPE sceneType = SCENE_TYPE_RESULT);
	~CResult_BG();

	static CResult_BG *Create(D3DXVECTOR3 pos, float width, float height, int textureType);		//	ê∂ê¨ä÷êîÅB
};

