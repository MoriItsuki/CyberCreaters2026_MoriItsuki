//
//	�}�l�[�W���[CPP(manager.cpp)
//
//
//

//====================================================
//	�C���N���[�h
//====================================================
#include "manager.h"
#include "texture.h"
#include "block.h"

//====================================================
//	�ÓI�����o�ϐ��̏�����
//====================================================
CRenderer* CManager::m_pRenderer = nullptr;
CProcess* CManager::m_pProcess = nullptr;
CInputKeyboard* CManager::m_pKeyboard = nullptr;
CInputGamePad* CManager::m_pGamepad = nullptr;
CCamera* CManager::m_pCamera = nullptr;
CLight* CManager::m_pLight = nullptr;
CTexture* CManager::m_pTexture = nullptr;
CScene* CManager::m_pScene = nullptr;
CTimeManager* CManager::m_pTimeManager = nullptr;

std::shared_ptr<ShereData> CManager::m_ShareData = nullptr;

//====================================================
//	�萔
//====================================================
namespace
{
	constexpr float TIME_GAME = 500.0f;	//�Q�[���V�[���̎���
}

//====================================================
//	�R���X�g���N�^
//====================================================
CManager::CManager()
{

}

//====================================================
//	�f�X�g���N�^
//====================================================
CManager::~CManager()
{

}

//====================================================
//	�����ݒ�
//====================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{

	//�����_���[�̐���
	m_pRenderer = new CRenderer;
	m_pRenderer->Init(hWnd, bWindow);

	//�v���Z�X�̐���
	m_pProcess = new CProcess;
	m_pProcess->Init();

	//�L�[�{�[�h�̐���
	m_pKeyboard = new CInputKeyboard;
	m_pKeyboard->Init(hInstance, hWnd);

	//�Q�[���p�b�h�̐���
	m_pGamepad = new CInputGamePad;
	m_pGamepad->Init(hInstance, hWnd);

	//�J��������
	m_pCamera = new CCamera();
	m_pCamera->Init();

	//���C�g����
	m_pLight = new CLight();
	m_pLight->Init();

	//�e�N�X�`������
	m_pTexture = new CTexture();
	m_pTexture->Init();

 	m_ShareData = std::make_shared<ShereData>();

	//�ŏ��̃V�[���̐ݒ�
	SetMode(CScene::MODE::MODE_TITLE);

	return S_OK;
}

//====================================================
//	�I������
//====================================================
void CManager::Uninit()
{
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}
	if (m_pProcess != nullptr)
	{
		m_pProcess->Uninit();
		delete m_pProcess;
		m_pProcess = nullptr;
	}
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Uninit();
		delete m_pKeyboard;
		m_pKeyboard = nullptr;
	}
	if (m_pGamepad != nullptr)
	{
		m_pGamepad->Uninit();
		delete m_pGamepad;
		m_pGamepad = nullptr;
	}
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pCamera = nullptr;
	}
	if (m_pTexture != nullptr)
	{
		m_pTexture->Unload();
		delete m_pTexture;
		m_pTexture = nullptr;
	}
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}
	if (m_pTimeManager != nullptr)
	{
		m_pTimeManager->Uninit();
		delete m_pTimeManager;
		m_pTimeManager = nullptr;
	}

	CObject::ReleaseAll();	//�S�I�u�W�F�N�g�̉��
}

//====================================================
//	�X�V����
//====================================================
void CManager::Update()
{
	m_pRenderer->Update();
	m_pProcess->Update();
	m_pKeyboard->Update();
	m_pGamepad->Update();
	m_pScene->Update();
	
	//	�f���^�^�C���̎擾
	float deltatime = m_pProcess->GetDeltaTime();

	if (m_pTimeManager)
	{
		m_pTimeManager->Update(deltatime);
	}
}

//====================================================
//	�`�揈��
//====================================================
void CManager::Draw()
{
	m_pRenderer->Draw();
	m_pScene->Draw();
	if (m_pScene)
	{
		m_pTimeManager->Draw();
	}
}

//====================================================
//	�����_���[�擾
//====================================================
CRenderer* CManager::GetRenderer()
{
	return m_pRenderer;
}

//====================================================
//	�v���Z�X�擾
//====================================================
CProcess* CManager::GetProcess()
{
	return m_pProcess;
}

//====================================================
//	�L�[�{�[�h�擾
//====================================================
CInputKeyboard* CManager::GetKeyboard()
{
	return m_pKeyboard;
}

//====================================================
//	�L�[�{�[�h�擾
//====================================================
CInputGamePad* CManager::GetGamepad()
{
	return m_pGamepad;
}

//====================================================
//	�J�����擾
//====================================================
CCamera* CManager::GetCamera()
{
	return m_pCamera;
}

//====================================================
//	���C�g�擾
//====================================================
CLight* CManager::GetLight()
{
	return m_pLight;
}

//====================================================
//	�e�N�X�`���擾
//====================================================
CTexture* CManager::GetTexture()
{
	return m_pTexture;
}

//====================================================
//	���ԊǗ��擾
//====================================================
CTimeManager* CManager::GetTimeManager()
{
	return m_pTimeManager;
}

//====================================================
//	���[�h�ݒ�
//====================================================
void CManager::SetMode(CScene::MODE mode)
{
	//�V�[���I������
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	//���ԊǗ��̏I������
	if (m_pTimeManager != nullptr)
	{
		m_pTimeManager->Uninit();
		delete m_pTimeManager;
		m_pTimeManager = nullptr;
	}

	//�V�����V�[������
	m_pScene = CScene::Create(mode);

	m_pScene->SetShareData(m_ShareData);

	//�K�v�ɉ����ĐV���ɏ�����
	m_pTimeManager = new CTimeManager();

	//���[�h�ɉ�����CTimeManager��������
	if (mode == CScene::MODE_GAME)
	{
		//2���̎��Ԑ���
		m_pTimeManager->Init(TIME_GAME);
	}
	else
	{
		m_pTimeManager->Init();
	}
}