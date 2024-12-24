//=====================================================
//	�����_���[�֘ACPP(renderer.cpp)
//
//
//=====================================================
#include "object.h"
#include "renderer.h"
#include "manager.h"
#include "process.h"
#include "main.h"
//#include "timemanager.h"

LPD3DXFONT CProcess::m_pFont = nullptr;

//
//	�R���X�g���N�^
//
CRenderer::CRenderer() :
	m_bWireFrame(false),
	m_fFogStartPos(0.0f),
	m_fFogEndPos(0.0f),
	m_fFogDensity(0.0f)
{
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;
}

//
//	�f�X�g���N�^
//
CRenderer::~CRenderer()
{

}

//
//	�����ݒ�
//
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	//�v���[���e�[�V�����p�����[�^

	CProcess* pProcess = CManager::GetProcess();

	//Direct3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h�擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));			//�p�����[�^�̃[���N���A
	d3dpp.BackBufferWidth = CProcess::SCREEN_WIDTH;		//�Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = CProcess::SCREEN_HEIGHT;		//�@�@�@�V        (����)
	d3dpp.BackBufferFormat = d3ddm.Format;		//�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;					//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//�_�u���o�b�t�@�̐؂�ւ�
	d3dpp.EnableAutoDepthStencil = TRUE;		//�f�v�X�o�b�t�@�ƃX�e���W���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	//�f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;					//�E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//�C���^�[�o��

	//Direct3D�f�o�C�X�̐���(�`�揈���ƒ��_�������n�[�h�E�F�A�ōs��)
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		//Direct3D�f�o�C�X�̐���(�`�揈���̓n�[�h�E�F�A�A���_������CPU�ōs��)
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			//Direct3D�f�o�C�X�̐���(�`�揈���ƒ��_������CPU�ōs��)
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//�f�o�b�O�\���p�t�H���g�̐���
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0,
		FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &pProcess->m_pFont);

	//�����_�[�X�e�[�g
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ���̃u�����f�B���O��L���ɂ���
	m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	// ���̐F��ݒ肷��
	m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// ���̎�ނ�ݒ肷��
	m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);

	// ���̃p�����[�^�ݒ�
	if (1)
	{
		float start = 100.0f;
		float end = 7000.0f;

		m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&start));
		m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&end));
	}
	else
	{//	���s���e�����疧�x�����̕�
		float density = 0.8f;

		m_pD3DDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&density));
	}

	//
	//�e��I�u�W�F�N�g�̏���������
	//
	return S_OK;
}

//
//	�I������
//
void CRenderer::Uninit()
{
	//
	//�e��I�u�W�F�N�g�̏I������
	//

	//�|���S���̏I������

	//�RD�f�o�C�X�̔j��
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}
	//Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//
//	�X�V����
//
void CRenderer::Update()
{
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();
#ifdef _DEBUG
	if (pKeyboard->GetTrigger(DIK_F9) == true)
	{
		m_bWireFrame = !m_bWireFrame;
		WireFrame();
	}
#endif // DEBUG


	//�|���S���̍X�V����
	CObject::UpdateAll();

	//�J�����̍X�V����
	CManager::GetCamera()->Update();

	//
	//�X�V����
	//

}

//
//	�`�揈��
//
void CRenderer::Draw()
{
	CProcess* pProcess = CManager::GetProcess();
	CTimeManager* pTimeManager = CManager::GetTimeManager();

	//��ʃN���A(�o�b�N�o�b�t�@&z�o�b�t�@�̃N���A)
	m_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(100, 100, 100, 255), 1.0f, 0);
	//�`��J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		//�J�����̐ݒ�
		CManager::GetCamera()->SetCamera();

		//�|���S���̕`�揈��
		CObject::DrawAll();

		//
		//�e��I�u�W�F�N�g�̕`�揈��
		//

#ifdef _DEBUG
		//FPS�̕\��
		pProcess->DrawFPS();		//FPS�\��
		pTimeManager->DrawInfo();	//���ԊǗ��\��
#endif // _DEBUG


		//�`��I��
		m_pD3DDevice->EndScene();
	}
	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}

//
//	3D�f�o�C�X�̎擾
//
LPDIRECT3DDEVICE9 CRenderer::GetDevice()
{
	return m_pD3DDevice;
}

//
//	���C���[�t���[��
//
void CRenderer::WireFrame()
{
	if (m_bWireFrame)
	{
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else
	{
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}
