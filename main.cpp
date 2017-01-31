/******************************************************************************
	�^�C�g���� : DirectX ���`
	�t�@�C���� : main.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/04/18
	�X�V��     : 
******************************************************************************/
/******************************************************************************
	scanf��warning�h�~
******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS

/******************************************************************************
	�C���N���[�h�t�@�C��
******************************************************************************/
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "main.h"

/******************************************************************************
	�}�N����`
******************************************************************************/
#define PROJECTNAME ( "TEST" )
#define ID_BUTTON000 ( 101 )/* �{�^����ID */
#define POSPOLYGONCNT ( 3 )

/******************************************************************************
	�\���̒�`
******************************************************************************/
/******************************************************************************
	�v���g�^�C�v�錾
******************************************************************************/
LRESULT CALLBACK WndProc( HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam );
HRESULT Init( HINSTANCE hInstance , HWND hWnd , BOOL bWindow );
void Uninit( void );
void Update( void );
void Draw( void );

/******************************************************************************
	�O���[�o���ϐ�
******************************************************************************/
CManager *g_pManager;

/******************************************************************************
	�֐����Fint WINAPI WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nCmdShow )
	����  �FHINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nCmdShow
	�߂�l�F����I���Fint�^��0
	����  :DirectX�̎g�p�ɕK�v�Ȑ��`�쐬
******************************************************************************/
int WINAPI WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nCmdShow )
{
	//	�}�l�[�W���[�N���X�̐���
	g_pManager = new CManager;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	WNDCLASSEX wcex = {
		sizeof( WNDCLASSEX ) ,           //�������T�C�Y���w��
		CS_CLASSDC ,                     //�E�B���h�E�̃X�^�C����ݒ�
		WndProc,                         //�E�B���h�E�v���V�[�W���̃A�h���X( �֐��� )���w��
		0 ,                              //�g�p���Ȃ�
		0 ,                              //�g�p���Ȃ�
		hInstance ,                      //WinMain�̃C���X�^���X�n���h�����w��
		NULL ,                           //�g�p����A�C�R���̎w��
		LoadCursor( NULL , IDC_ARROW ) , //�}�E�X�J�[�\�����w��( �E�B���h�E�� )
		( HBRUSH )( COLOR_WINDOW + 1 ) , //�N���C�A���g�̈�̔w�i�F��ݒ�
		NULL ,                           //���j���[�̐ݒ�
		PROJECTNAME ,                    //�E�B���h�E�N���X�̖��O( �j�b�N�l�[���I�� )
		NULL,                            //�������A�C�R�����ݒ肳�ꂽ�ꍇ�̏��
	};

	RECT rect = { 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT };
	AdjustWindowRect( &rect , WS_OVERLAPPEDWINDOW , false );

	HWND hWnd;//�E�B���h�E�̃n���h��
	MSG msg;  //���b�Z�[�W

	RegisterClassEx( &wcex );//�E�B���h�E�N���X�̓o�^
	hWnd = CreateWindowEx( 0 ,
						   PROJECTNAME ,			//�E�B���h�E�N���X�̖��O
						   "TEST" ,					//�E�B���h�E�̖��O
						   WS_OVERLAPPEDWINDOW ,	//�E�B���h�E�̃X�^�C��
						   CW_USEDEFAULT ,			//���ォ���X���W
						   CW_USEDEFAULT ,			//���ォ���Y���W
						   rect.right - rect.left ,	//��  ( �����œ��� )
						   rect.bottom - rect.top ,	//����( �����œ��� )
						   NULL ,					//�e�E�B���h�E�̃n���h��
						   NULL ,					//���j���[�n���h�����́A�q�E�B���h�EID
						   hInstance ,				//
						   NULL );					//�E�B���h�E�̍쐬�f�[�^

	if( FAILED( Init( hInstance , hWnd , TRUE ) ) )//����������
	{
		return -1;
	}

	/* �E�B���h�E�̕\�� */
	ShowWindow( hWnd , nCmdShow );//��N���C�A���g�̈�
	UpdateWindow( hWnd );         //�N���C�A���g�̈�

	/* ���b�Z�[�W���[�v */
	while( 1 )
	{
		if( PeekMessage( &msg , NULL , 0 , 0 , PM_REMOVE ) != 0 )//Windows���������Ă���Ƃ�
		{
			if( msg.message == WM_QUIT )
			{
				break;
			}
			else
			{
				/* ���b�Z�[�W�̖|��𑗏o */
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		else//DirectX������
		{
			Update();	//�X�V����
			Draw();		//�`�揈��
		}
	}

	UnregisterClass( PROJECTNAME , wcex.hInstance );//�E�B���h�E�N���X�̓o�^����

	Uninit();//�I������
	return ( int )msg.wParam;

}
LRESULT CALLBACK WndProc( HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam )
{
	int nID;
	RECT rect;
	static HWND hWndButton000;

	switch( uMsg )
	{
	case WM_DESTROY://�E�B���h�E��j��

		PostQuitMessage( 0 );//" WM_QUIT "���b�Z�[�W��Ԃ�
		break;

	case WM_KEYDOWN://�L�[�������ꂽ

		switch( wParam )
		{
		case VK_ESCAPE://���z�L�[

			nID = MessageBox( hWnd , " �I�����܂����H " , "  " , MB_YESNO + MB_ICONQUESTION );

			if( nID == IDYES )
			{
				DestroyWindow( hWnd );//�E�B���h�E��j��
			}
			break;

		case VK_F11:
			static bool fullScreen = true;

			if (fullScreen == true) {
				rect.left = 0;
				rect.top = 0;
				rect.right = 1920;
				rect.bottom = 1080;

				SetWindowPos(hWnd, HWND_NOTOPMOST, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
				SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);

				fullScreen = false;
			}
			else {
				rect.left = 1920 / 5;
				rect.top = 1080 / 6;
				rect.right = rect.left + SCREEN_WIDTH;
				rect.bottom = rect.top + SCREEN_HEIGHT;

				SetWindowPos(hWnd, HWND_NOTOPMOST, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
				SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);

				fullScreen = true;
			}
			break;
		}
		break;

	case WM_CREATE:
		break;

	case WM_TIMER:
		break;

	case WM_PAINT:
		break;

	case WM_COMMAND:

		if( LOWORD( wParam ) == ID_BUTTON000 )//[�I��]�����ꂽ��
		{
			nID = MessageBox( hWnd , "  �I�����Ă�낵���ł����H" , " " , MB_YESNO + MB_ICONQUESTION );

			if( nID == IDYES )
			{
				DestroyWindow( hWnd );//�E�B���h�E��j��
			}
			break;
		}
		break;

	case WM_LBUTTONDOWN :

		SetFocus( hWnd );
		break;
	default :

		break;
	}

	return DefWindowProc( hWnd , uMsg , wParam , lParam );
}

/******************************************************************************
	�֐��� : HRESULT Init(HINSTANCE hInstance , HWND hWnd , BOOL bWindow)
	����   : hInstance, hWnd, bWindow
	�߂�l : E_FAIL, S_OK
	����   : Renderer�̏���������
******************************************************************************/
HRESULT Init(HINSTANCE hInstance , HWND hWnd , BOOL bWindow)
{
	if( FAILED(g_pManager->Init( hInstance , hWnd , TRUE ) ) )//����������
	{
		//	���s
		return E_FAIL;
	}
	
	//	���Ȃ��I��
	return S_OK;
}

/******************************************************************************
	�֐��� : void Uninit(void)
	����   : void
	�߂�l : �Ȃ�
	����   : Renderer�̏I������������ɁARenderer���������B
******************************************************************************/
void Uninit(void)
{
	//	Renderer�̏I������
	g_pManager->Uninit();

	//	Renderer�̉��
	delete g_pManager;

	//	NULL����
	g_pManager = NULL;
}

/******************************************************************************
	�֐��� : void Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : Renderer�̍X�V����
******************************************************************************/
void Update(void)
{
	//�X�V����
	g_pManager->Update();
}

/******************************************************************************
	�֐��� : void Draw(void)
	����   : void
	�߂�l : �Ȃ�
	����   : Renderer�̕`�揈��
******************************************************************************/
void Draw(void)
{
	//�`�揈��
	g_pManager->Draw();
}

/******************************************************************************
	�֐��� : CManager *GetManager(void)
	����   : void
	�߂�l : �Ȃ�
	����   : Manager�̃A�h���X���擾����֐�
******************************************************************************/
CManager *GetManager(void)
{
	return g_pManager;
}