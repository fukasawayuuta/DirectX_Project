/******************************************************************************
	�^�C�g���� : �����_���N���X
	�t�@�C���� : renderer.cpp
	�쐬��     : AT-13C-284 36 �[�V�C��
	�쐬��     : 2016/04/18
	�X�V��     : 2016/04/19
******************************************************************************/
/******************************************************************************
	�C���N���[�h�t�@�C��
******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"

/******************************************************************************
	�ÓI�����o�ϐ�
******************************************************************************/
bool CRenderer::m_bBlurFlag = false;

/******************************************************************************
	�֐��� : CRenderer::CRenderer()
	����   : �R���X�g���N�^
			 ��������  m_pD3D, m_pD3DDevice  ��NULL����
******************************************************************************/
CRenderer::CRenderer()
{
	m_pD3D       = NULL;
	m_pD3DDevice = NULL;
}

/******************************************************************************
	�֐��� : CRenderer::~CRenderer()
	����   : �f�X�g���N�^
			 �j�����ꂽ�Ƃ��ɌĂ΂�邪�A���g���Ȃ��̂œ��ɋL�q���鎖�Ȃ�
******************************************************************************/
CRenderer::~CRenderer()
{

}

/******************************************************************************
	�֐��� ; HRESULT CRenderer::Init( HINSTANCE hInstance , HWND hWnd , BOOL bWindow )
	����   : hInstance, hWnd, bWindow
	�߂�l : S_OK, E_FAIL
	����   : �I�u�W�F�N�g�̍쐬���A��ʕ��⍂���A�o�b�N�o�b�t�@�̌`���Ɛ����w�肵�A
			 �E�B���h�E���[�h���t���X�N���[�����[�h�ŏo�������w�肵�A
			 �f�o�C�X���쐬����B

			 �|���S����`�悷�邽�߂̏��������������Ă���B
******************************************************************************/
HRESULT CRenderer::Init(HINSTANCE hInstance , HWND hWnd , BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	VERTEX_2D *pVtx;
	m_pBlurVertex = NULL;

	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );//�I�u�W�F�N�g�̐���

	if( m_pD3D == NULL )
		return E_FAIL;

	//���݂̃f�B�X�v���C���[�h���擾
	if( FAILED( m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT , &d3ddm ) ) )
		return E_FAIL;

	ZeroMemory( &d3dpp , sizeof( d3dpp ) );

	d3dpp.BackBufferWidth = SCREEN_WIDTH;			//��ʂ̕�
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;			//��ʂ̍���
	d3dpp.BackBufferFormat = d3ddm.Format;			//�o�b�N�o�b�t�@�`��
	d3dpp.BackBufferCount = 1;						//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;			//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;		//�f�v�X�o�b�t�@�Ƃ���16�r�b�g���g�p
	d3dpp.Windowed = bWindow;						//�E�B���h�E���[�h/�t���X�N���[�����[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//Direct3D�f�o�C�X�̐���
	if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT ,
									  D3DDEVTYPE_HAL ,
									  hWnd ,
								      D3DCREATE_MULTITHREADED,
									  &d3dpp ,
									  &m_pD3DDevice ) ) ) {
		if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT ,
										  D3DDEVTYPE_HAL ,
										  hWnd ,
										  D3DCREATE_SOFTWARE_VERTEXPROCESSING ,
										  &d3dpp ,
										  &m_pD3DDevice ) ) ) {
			if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT ,
											  D3DDEVTYPE_REF ,
											  hWnd ,
											  D3DCREATE_SOFTWARE_VERTEXPROCESSING ,
											  &d3dpp ,
											  &m_pD3DDevice ) ) ) {
				return E_FAIL;

			}
		}
	}

	//	�����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_CCW );			//	�J�����O�̐ݒ�
	m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , TRUE );			//	�A���t�@�u�����h�̐ݒ�
	m_pD3DDevice->SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );		//	�\�[�X�u�����h�̐ݒ�
	m_pD3DDevice->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );	//	�C���o�[�X�u�����h�̐ݒ�

	//	�T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	//	�e�N�X�`���̏k�����̕�Ԑݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	//	�e�N�X�`���̊g�厞�̕�Ԑݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	//	�e�N�X�`��(U�l) �J��Ԃ��ɐݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	//	�e�N�X�`��(V�l) �J��Ԃ��ɐݒ�

	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0 , D3DTSS_ALPHAOP , D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0 , D3DTSS_ALPHAARG1 , D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0 , D3DTSS_ALPHAARG2 , D3DTA_CURRENT);

	//	�t�H�O///////////////////////////////////////////////////////////////////////////////////////////
	D3DCAPS9 caps;
	ZeroMemory(&caps, sizeof(D3DCAPS9));    //������
	m_pD3DDevice->GetDeviceCaps(&caps);
    
	//�s�N�Z���t�H�O���g���邩�ǂ����m�F
	if((caps.RasterCaps & D3DPRASTERCAPS_FOGRANGE) == 0) 
		return FALSE;

	//�t�H�O�̐ݒ�
	FLOAT StartPos   = 200;  //�J�n�ʒu
	FLOAT EndPos     = 2000; //�I���ʒu
	m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
	m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_RGBA(0xf5, 0xde, 0xb3, 0xff)); //���F�ŕs����
	m_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);      //���_���[�h
	m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);     //�e�[�u�����[�h
	m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*) (&StartPos)); //�J�n�ʒu
	m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*) (&EndPos));     //�I���ʒu
	//	�t�H�O///////////////////////////////////////////////////////////////////////////////////////////

	//	���C���[�t���[���t���O�ݒ�
	m_bWireframe = false;

	D3DXCreateTexture(m_pD3DDevice, SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_BlurTexture1);
	m_BlurTexture1->GetSurfaceLevel(0, &m_BlurSurface1);

	D3DXCreateTexture(m_pD3DDevice, SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_BlurTexture2);
	m_BlurTexture2->GetSurfaceLevel(0, &m_BlurSurface2);

	m_pD3DDevice->GetRenderTarget(0, &m_BackBufferSurface);

	//���_�o�b�t�@�̐���
	if (FAILED(m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pBlurVertex, NULL))) {
		return E_FAIL;
	}

	//���_���ւ̃|�C���^���擾
	m_pBlurVertex->Lock(0, 0, (void**)&pVtx, 0);

	//	���W�ݒ�
	pVtx[0].pos = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH + 1.0f, -1.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-1.0f, SCREEN_HEIGHT + 1.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH + 1.0f, SCREEN_HEIGHT + 1.0f, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//�e�N�X�`�����W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//	�F�ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 254);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 254);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 254);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 254);

	//	�o�b�t�@���
	m_pBlurVertex->Unlock();

	return S_OK;
}

/******************************************************************************
	�֐��� : void CRenderer::Uninit(void)
	����   : void
	�߂�l : �Ȃ�
	����   : ���������f�o�C�X��I�u�W�F�N�g�̉��
			 �|���S���̉������
******************************************************************************/
void CRenderer::Uninit(void)
{
	//	�f�o�C�X��NULL����Ȃ�������
	if(m_pD3DDevice != NULL) {
		m_pD3DDevice->Release();	//	�f�o�C�X�̊J��
		m_pD3DDevice = NULL;
	}

	//	�I�u�W�F�N�g��NULL����Ȃ�������
	if( m_pD3D != NULL ) {
		m_pD3D->Release();			//	�I�u�W�F�N�g�̊J��
		m_pD3D = NULL;
	}

	m_BlurTexture1->Release();
	m_BlurTexture1 = NULL;
	m_BlurTexture2->Release();
	m_BlurTexture2 = NULL;
	m_BlurSurface1->Release();
	m_BlurSurface1 = NULL;
	m_BlurSurface2->Release();
	m_BlurSurface2 = NULL;
	m_BackBufferSurface->Release();
	m_BackBufferSurface = NULL;
	m_pBlurVertex->Release();
	m_pBlurVertex = NULL;
}

/******************************************************************************
	�֐��� : void CRenderer::Update(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �Ȃ�
******************************************************************************/
void CRenderer::Update(void)
{
	// ���͏����ւ̃|�C���^���擾
	CManager *pManager = GetManager();
	CInput *pInput = pManager->GetInput();

	//	�`����@�̐؂�ւ�
	if (pInput->GetKeyboardTrigger(DIK_F8)) {
		m_bWireframe = m_bWireframe == true ? false : true;

		if (m_bWireframe == true) {
			//	���C���[�t���[���ŕ`��
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}
		else {
			//	�ʏ탂�[�h�ŕ`��
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
	}
}

/******************************************************************************
	�֐��� : void CRenderer::Draw(void)
	����   : void
	�߂�l : �Ȃ�
	����   : �Ȃ�
******************************************************************************/
void CRenderer::Draw(void)
{

}

/******************************************************************************
	�֐��� : bool CRenderer::DrawSceneFlag(void)
	����   : void
	�߂�l : true, false
	����   : �|���S���̕`�揈���t���O
******************************************************************************/
bool CRenderer::DrawSceneFlag(void)
{
	//	�e�N�X�`���ɏ�������
	if (m_bBlurFlag) {
		m_pD3DDevice->SetRenderTarget(0, m_BlurSurface1);
	}
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR RGB(0, 0, 0, 0), 1.0f, 0);//D3DCOLOR RGB( R , G , B , A(�����x:0~255) )

	if (SUCCEEDED(m_pD3DDevice->BeginScene())) {
		//����������`��J�n
		return true;
	}
	else {
		return false;
	}
}

/******************************************************************************
	�֐��� : void CRenderer::ExchangeBuffer(void)
	����   : void
	�߂�l : void
	����   : �`����I�����āA�o�b�t�@�����ւ���
******************************************************************************/
void CRenderer::ExchangeBuffer(void)
{
	m_pD3DDevice->EndScene();	//	�`��I��

	if (m_bBlurFlag) {
		m_pD3DDevice->BeginScene();
		m_pD3DDevice->SetTexture(0, m_BlurTexture2);
		/* �S���2D�|���S���`�� */
		//���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
		m_pD3DDevice->SetStreamSource(0, m_pBlurVertex, 0, sizeof(VERTEX_2D));
		//���_���W�ӂ��[�܂���
		m_pD3DDevice->SetFVF(FVF_VERTEX_2D);
		//�|���S���`��
		m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		m_pD3DDevice->EndScene();

		//	�o�b�N�o�b�t�@�ɏ�������
		m_pD3DDevice->SetRenderTarget(0, m_BackBufferSurface);
		m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR RGB(0, 0, 0, 255), 1.0f, 0);//D3DCOLOR RGB( R , G , B , A(�����x:0~255) )
		m_pD3DDevice->BeginScene();
		m_pD3DDevice->SetTexture(0, m_BlurTexture1);

		/* �S���2D�|���S���`�� */
		//���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
		m_pD3DDevice->SetStreamSource(0, m_pBlurVertex, 0, sizeof(VERTEX_2D));
		//���_���W�ӂ��[�܂���
		m_pD3DDevice->SetFVF(FVF_VERTEX_2D);
		//�|���S���`��
		m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		/* �S���2D�|���S���`�� */

		m_pD3DDevice->EndScene();	//	�`��I��
		//	�o�b�N�o�b�t�@�ɏ�������

		//	�|�C���^����ւ�
		LPDIRECT3DTEXTURE9 texture;
		texture = m_BlurTexture1;
		m_BlurTexture1 = m_BlurTexture2;
		m_BlurTexture2 = texture;
		//	�|�C���^����ւ�
		LPDIRECT3DSURFACE9 surface;
		surface = m_BlurSurface1;
		m_BlurSurface1 = m_BlurSurface2;
		m_BlurSurface2 = surface;
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}