/******************************************************************************
	�^�C�g���� : �����_���N���X
	�t�@�C���� : renderer.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/04/18
	�X�V��     : 2016/04/19
				 2016/05/09
******************************************************************************/
/******************************************************************************
	�C���N���[�h�K�[�h
******************************************************************************/
#ifndef _RENDERER_H_
#define _RENDERER_H_

/******************************************************************************
	�C���N���[�h�t�@�C��
******************************************************************************/
#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION ( 0x0800 )	//	�x���Ώ��p
#include "dinput.h"						//	���͏����ɕK�v
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "xaudio2.h"					//	�T�E���h�̍Đ��ɕK�v
#include "Vector.h"

/******************************************************************************
	���C�u�����̃����N
******************************************************************************/
#pragma comment( lib , "d3d9.lib" )		//	�`�揈���ɕK�v
#pragma comment( lib , "d3dx9.lib" )	//	��L�̊g�����C�u����
#pragma comment( lib , "dxguid.lib" )	//	DirectX�R���|�[�l���g�g�p�ɕK�v(�ׂ�����ƂɕK�v)
#pragma comment( lib , "winmm.lib" )	//	�V�X�e�������擾�ɕK�v
#pragma comment( lib , "dinput8.lib" )	//	���͏����ɕK�v

/******************************************************************************
	�}�N����`
******************************************************************************/
//	3D�|���S���̒��_�t�H�[�}�b�g
#define FVF_VERTEX_3D ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

//	2D�|���S���̒��_�t�H�[�}�b�g
#define FVF_VERTEX_2D ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

//	�`�悷��|���S����
#define DRAW_PORYGON_NUM ( 2 )

/******************************************************************************
	�\���̐錾
******************************************************************************/
//	3D�p�̍\����
typedef struct {
	D3DXVECTOR3 pos;		//	���_���W
	D3DXVECTOR3 nor;		//	�@���̌���
	D3DCOLOR    col;		//	���_�J���[
	D3DXVECTOR2 tex;		//	�e�N�X�`�����W
}VERTEX_3D;

//	2D�p�̍\����
typedef struct {
	D3DXVECTOR3 pos;		//	���_���W
	float       rhw;		//	���W�ϊ��p�W��
	D3DCOLOR    col;		//	���_�J���[
	D3DXVECTOR2 tex;		//	�e�N�X�`�����W
}VERTEX_2D;

/******************************************************************************
	�N���X
******************************************************************************/
//	�O���錾///////////////////////////////////////////////////////////////////
class CRenderer
{
public:
	CRenderer();			//	�R���X�g���N�^
	~CRenderer();			//	�f�X�g���N�^

	HRESULT Init(HINSTANCE hInstance , HWND hWnd , BOOL bWindow);			//	�������֐�
	void Uninit(void);														//	�I���֐�
	void Update(void);														//	�X�V�֐�
	void Draw(void);														//	�`��֐�
	bool DrawSceneFlag(void);												//	�`����n�߂�֐�
	void ExchangeBuffer(void);												//	�`����I�����A�o�b�t�@�����ւ���֐�
	static void SetBlur(bool flag) { m_bBlurFlag = flag; }

	//	�f�o�C�X�̃A�h���X��n���֐�
	LPDIRECT3DDEVICE9 GetDevice(void){return m_pD3DDevice;}

private:
	LPDIRECT3D9       m_pD3D;			//	Direct3D�ւ̃|�C���^
	LPDIRECT3DDEVICE9 m_pD3DDevice;		//	Direct3D�f�o�C�X�ւ̃|�C���^

	bool m_bWireframe;					//	���C���[�t���[���̃t���O
	static bool m_bBlurFlag;					//	�u���[���g�����ǂ����̃t���O�B
	
	LPDIRECT3DTEXTURE9 m_BlurTexture1;
	LPDIRECT3DTEXTURE9 m_BlurTexture2;
	LPDIRECT3DSURFACE9 m_BlurSurface1;
	LPDIRECT3DSURFACE9 m_BlurSurface2;

	LPDIRECT3DSURFACE9 m_BackBufferSurface;
	LPDIRECT3DVERTEXBUFFER9 m_pBlurVertex;
};


#endif