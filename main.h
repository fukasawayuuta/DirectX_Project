/******************************************************************************
	�^�C�g���� : DirectX ���`
	�t�@�C���� : main.h
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/04/18
	�X�V��     : 
******************************************************************************/
#ifndef _MAIN_H_
#define _MAIN_H_

/******************************************************************************
	�C���N���[�h�t�@�C��
******************************************************************************/
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "d3dx9.h"
#include "Vector.h"
#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h> 
#include <crtdbg.h>
#include <list>

/******************************************************************************
	���C�u�����̃����N
******************************************************************************/
/******************************************************************************
	�}�N����`
******************************************************************************/
#define SCREEN_WIDTH ( 1280 )		//	��ʂ̕�
#define SCREEN_HEIGHT ( 720 )		//	��ʂ̍���
#define null (NULL)
#define _CRT_SECURE_NO_WARNINGS
#define MATH_LENGTH_2D(width, height) sqrtf(width * width + height * height)		//	�Ίp���̒����v�Z
#define MATH_LENGTH_3D(width, height, depth) sqrtf(width * width + height * height + depth * depth)		//	�Ίp���̒����v�Z
#define MATH_ANGLE(width, height) atan2f(width, height);		//	�p�x�v�Z
#define _CRT_SECURE_NO_WARNINGS

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p) = NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p) = NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p) = NULL; } }
#endif

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW 
#endif 
#endif

/******************************************************************************
	�\���̐錾
******************************************************************************/

/******************************************************************************
	�N���X�錾
******************************************************************************/
//	�O���錾///////////////////////////////////////////////////////////////////
class CManager;

/******************************************************************************
	�v���g�^�C�v�錾
******************************************************************************/
CManager  *GetManager(void);
#endif