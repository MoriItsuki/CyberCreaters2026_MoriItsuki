//=====================================================
//	レンダラー関連CPP(renderer.cpp)
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
//	コンストラクタ
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
//	デストラクタ
//
CRenderer::~CRenderer()
{

}

//
//	初期設定
//
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	//プレゼンテーションパラメータ

	CProcess* pProcess = CManager::GetProcess();

	//Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモード取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));			//パラメータのゼロクリア
	d3dpp.BackBufferWidth = CProcess::SCREEN_WIDTH;		//ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = CProcess::SCREEN_HEIGHT;		//　　　〃        (高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;		//バックバッファの形式
	d3dpp.BackBufferCount = 1;					//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//ダブルバッファの切り替え
	d3dpp.EnableAutoDepthStencil = TRUE;		//デプスバッファとステンジルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	//デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;					//ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//インターバル

	//Direct3Dデバイスの生成(描画処理と頂点処理をハードウェアで行う)
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		//Direct3Dデバイスの生成(描画処理はハードウェア、頂点処理はCPUで行う)
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			//Direct3Dデバイスの生成(描画処理と頂点処理をCPUで行う)
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

	//デバッグ表示用フォントの生成
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0,
		FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &pProcess->m_pFont);

	//レンダーステート
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// 霧のブレンディングを有効にする
	m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	// 霧の色を設定する
	m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// 霧の種類を設定する
	m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);

	// 霧のパラメータ設定
	if (1)
	{
		float start = 100.0f;
		float end = 7000.0f;

		m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&start));
		m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&end));
	}
	else
	{//	平行投影だから密度処理の方
		float density = 0.8f;

		m_pD3DDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&density));
	}

	//
	//各種オブジェクトの初期化処理
	//
	return S_OK;
}

//
//	終了処理
//
void CRenderer::Uninit()
{
	//
	//各種オブジェクトの終了処理
	//

	//ポリゴンの終了処理

	//３Dデバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}
	//Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//
//	更新処理
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


	//ポリゴンの更新処理
	CObject::UpdateAll();

	//カメラの更新処理
	CManager::GetCamera()->Update();

	//
	//更新処理
	//

}

//
//	描画処理
//
void CRenderer::Draw()
{
	CProcess* pProcess = CManager::GetProcess();
	CTimeManager* pTimeManager = CManager::GetTimeManager();

	//画面クリア(バックバッファ&zバッファのクリア)
	m_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(100, 100, 100, 255), 1.0f, 0);
	//描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		//カメラの設定
		CManager::GetCamera()->SetCamera();

		//ポリゴンの描画処理
		CObject::DrawAll();

		//
		//各種オブジェクトの描画処理
		//

#ifdef _DEBUG
		//FPSの表示
		pProcess->DrawFPS();		//FPS表示
		pTimeManager->DrawInfo();	//時間管理表示
#endif // _DEBUG


		//描画終了
		m_pD3DDevice->EndScene();
	}
	//バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}

//
//	3Dデバイスの取得
//
LPDIRECT3DDEVICE9 CRenderer::GetDevice()
{
	return m_pD3DDevice;
}

//
//	ワイヤーフレーム
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
