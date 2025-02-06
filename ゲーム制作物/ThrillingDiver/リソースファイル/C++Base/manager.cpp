//
//	マネージャーCPP(manager.cpp)
//
//
//

//====================================================
//	インクルード
//====================================================
#include "manager.h"
#include "texture.h"
#include "block.h"

//====================================================
//	静的メンバ変数の初期化
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
//	定数
//====================================================
namespace
{
	constexpr float TIME_GAME = 500.0f;	//ゲームシーンの時間
}

//====================================================
//	コンストラクタ
//====================================================
CManager::CManager()
{

}

//====================================================
//	デストラクタ
//====================================================
CManager::~CManager()
{

}

//====================================================
//	初期設定
//====================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{

	//レンダラーの生成
	m_pRenderer = new CRenderer;
	m_pRenderer->Init(hWnd, bWindow);

	//プロセスの生成
	m_pProcess = new CProcess;
	m_pProcess->Init();

	//キーボードの生成
	m_pKeyboard = new CInputKeyboard;
	m_pKeyboard->Init(hInstance, hWnd);

	//ゲームパッドの生成
	m_pGamepad = new CInputGamePad;
	m_pGamepad->Init(hInstance, hWnd);

	//カメラ生成
	m_pCamera = new CCamera();
	m_pCamera->Init();

	//ライト生成
	m_pLight = new CLight();
	m_pLight->Init();

	//テクスチャ生成
	m_pTexture = new CTexture();
	m_pTexture->Init();

 	m_ShareData = std::make_shared<ShereData>();

	//最初のシーンの設定
	SetMode(CScene::MODE::MODE_TITLE);

	return S_OK;
}

//====================================================
//	終了処理
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

	CObject::ReleaseAll();	//全オブジェクトの解放
}

//====================================================
//	更新処理
//====================================================
void CManager::Update()
{
	m_pRenderer->Update();
	m_pProcess->Update();
	m_pKeyboard->Update();
	m_pGamepad->Update();
	m_pScene->Update();
	
	//	デルタタイムの取得
	float deltatime = m_pProcess->GetDeltaTime();

	if (m_pTimeManager)
	{
		m_pTimeManager->Update(deltatime);
	}
}

//====================================================
//	描画処理
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
//	レンダラー取得
//====================================================
CRenderer* CManager::GetRenderer()
{
	return m_pRenderer;
}

//====================================================
//	プロセス取得
//====================================================
CProcess* CManager::GetProcess()
{
	return m_pProcess;
}

//====================================================
//	キーボード取得
//====================================================
CInputKeyboard* CManager::GetKeyboard()
{
	return m_pKeyboard;
}

//====================================================
//	キーボード取得
//====================================================
CInputGamePad* CManager::GetGamepad()
{
	return m_pGamepad;
}

//====================================================
//	カメラ取得
//====================================================
CCamera* CManager::GetCamera()
{
	return m_pCamera;
}

//====================================================
//	ライト取得
//====================================================
CLight* CManager::GetLight()
{
	return m_pLight;
}

//====================================================
//	テクスチャ取得
//====================================================
CTexture* CManager::GetTexture()
{
	return m_pTexture;
}

//====================================================
//	時間管理取得
//====================================================
CTimeManager* CManager::GetTimeManager()
{
	return m_pTimeManager;
}

//====================================================
//	モード設定
//====================================================
void CManager::SetMode(CScene::MODE mode)
{
	//シーン終了処理
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	//時間管理の終了処理
	if (m_pTimeManager != nullptr)
	{
		m_pTimeManager->Uninit();
		delete m_pTimeManager;
		m_pTimeManager = nullptr;
	}

	//新しいシーン生成
	m_pScene = CScene::Create(mode);

	m_pScene->SetShareData(m_ShareData);

	//必要に応じて新たに初期化
	m_pTimeManager = new CTimeManager();

	//モードに応じてCTimeManagerを初期化
	if (mode == CScene::MODE_GAME)
	{
		//2分の時間制限
		m_pTimeManager->Init(TIME_GAME);
	}
	else
	{
		m_pTimeManager->Init();
	}
}