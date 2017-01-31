/******************************************************************************
	�^�C�g���� : MeshField�N���X
	�t�@�C���� : MeshField.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/12/15
	�X�V��     : 
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#pragma once

/******************************************************************************
	�N���X�錾
******************************************************************************/
class CMeshField : public CSceneIndex
{
public:
	CMeshField(int Priority = BACKGROUND, OBJ_TYPE objType = OBJ_TYPE_FIELD, SCENE_TYPE sceneType = SCENE_TYPE_GAME);		//	�R���X�g���N�^�B
	~CMeshField();		//	�f�X�g���N�^�B

	void Init(const char *filename, int textureType);		//	�������֐��B
	void Uninit(void);	//	�I���֐��B
	void Update(void);	//	�X�V�֐��B
	void Draw(void);	//	�`��֐��B

	float GetFieldWidth(void) { return m_Width_Total_Field; }		//	�t�B�[���h�S�̂̉����擾�B
	float GetFieldDepth(void) { return m_Depth_Total_Field; }		//	�t�B�[���h�S�̂̉��s�擾�B
	D3DXVECTOR3 GetFieldNormal(D3DXVECTOR3 pos);		//	��������W�̃t�B�[���h�̖@�����擾�B

	static CMeshField *Create(const char *filename, int textureType);		//	�����֐��B
	float Collision(D3DXVECTOR3 pos);		//	�����蔻�������֐��B
private:
	int m_SelectXIndex;				//	X����̃��b�V���̃C���f�b�N�X�̔ԍ��B
	int m_SelectZIndex;				//	Z����̃��b�V���̃C���f�b�N�X�̔ԍ��B
	int m_SelectPolygonIndex;		//	�S�̂̃��b�V���̃C���f�b�N�X�̔ԍ��B
	int m_SelectPolygonIndexX;		//	X����̃��b�V���̃|���S���̔ԍ��B
	int m_SelectPolygonIndexZ;		//	Z����̃��b�V���̃|���S���̔ԍ��B
	int m_SelectBlockNum;			//	�S�̂̃��b�V���̃|���S���̔ԍ��B

	int m_Field_Block_Num_Xline;		//	X���ɂ���u���b�N���B
	int m_Field_Block_Num_Zline;		//	Z���ɂ���u���b�N���B
	float m_Xline_Block_Between_Interval;		//	X���Ԃ̒��_�Ԃ̊Ԋu�B
	float m_Zline_Block_Between_Interval;		//	Z���Ԃ̒��_�Ԃ̊Ԋu�B
	int m_Field_Vertex_Num_Xline;		//	X����ɂ��钸�_���B
	int m_Field_Vertex_Num_Zline;		//	Z����ɂ��钸�_���B
	float m_Width_Total_Field;		//	���b�V���t�B�[���h�S�̂̉����B
	float m_Depth_Total_Field;		//	���b�V���t�B�[���h�S�̂̉��s�B
	int m_Index_Total_Field;		//	���b�V���t�B�[���h�Ɏg���Ă���C���f�b�N�X�̑����B

	bool LoadField(const char *filename);		//	�t�@�C����ǂݍ��ފ֐��B
	void UpdateMeshNormal(void);		//	�@���X�V�֐�
};

