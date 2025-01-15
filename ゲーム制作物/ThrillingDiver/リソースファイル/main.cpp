//=====================================================
//
//メインプログラム
//Author:Mori Itsuki
//
//=====================================================

#include "main.h"
#include "manager.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//====================================================
//プロトタイプ宣言
//====================================================
LRESULT CALLBACK WindowProc(HWND hWind, UINT uMsg, WPARAM wParam, LPARAM lParam);

//====================================================
//グローバル変数宣言
//====================================================
CManager* g_pManager = nullptr;

//====================================================
//メイン関数
//====================================================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,						//ウィンドウのスタイル
		WindowProc,						//ウィンドウプロシージャ
		0,								//0にする(通常はしない)
		0,								//0にする(     〃     )
		hInstance,						//インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),	//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),		//マウスカーソル
		(HBRUSH)(CTLCOLOR_MAX),			//クライアント領域の背景色
		NULL,							//メニューバー
		CLASS_NAME,						//ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)	//ファイルのアイコン
	};

	HWND hWnd;	//ウィンドウハンドル(識別子)
	MSG msg;	//メッセージを格納する変数
	RECT rect = { 0,0,CProcess::SCREEN_WIDTH,CProcess::SCREEN_HEIGHT };//画面サイズの構造体
	DWORD dwCurrentTime;	//現在時刻
	DWORD dwExecLastTime;	//最後に処理した時刻
	DWORD dwFrameCount;		//フレームカウント
	DWORD dwFPSLastTime;	//最後にFPSを計測した時刻

	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウィンドウを生成
	hWnd = CreateWindowEx(0,	//拡張ウィンドウスタイル
		CLASS_NAME,
		WINDOW_NAME,			//ウィンドウの名前
		WS_OVERLAPPEDWINDOW,	//ウィンドウスタイル
		CW_USEDEFAULT,			//ウィンドウの左上X座標
		CW_USEDEFAULT,			//ウィンドウの左上Y座標
		(rect.right - rect.left),			//ウィンドウの幅
		(rect.bottom - rect.top),			//ウィンドウの高さ
		NULL,					//親ウィンドウのハンドル
		NULL,					//メニューハンドルor子ウィンドウID
		hInstance,				//インスタンスハンドル
		NULL);					//ウィンドウ作成データ


	g_pManager = new CManager;	//マネージャー生成

	//初期化処理
	g_pManager->Init(hInstance, hWnd, TRUE);

	//分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;	//初期化
	dwExecLastTime = timeGetTime();	//現在時刻を取得、保存
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);	//ウィンドウ表示状態を設定
	UpdateWindow(hWnd);			//クライアント領域を更新

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)//0を返す
		{//windowsの処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセを受け取ったらメッセループを抜ける
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);	//仮想キーメッセージを文字メッセージに変換
				DispatchMessage(&msg);	//ウィンドウプロシージャへメッセージを送出
			}
		}
		else
		{//DirectXの処理
			if ((dwCurrentTime - dwFPSLastTime) >= CProcess::OBSTIME)
			{//0.5秒計測
				//FPSを計測		0.5秒ごとに計測したFPSを表示する
				g_pManager->GetProcess()->m_nCntFPS = (dwFrameCount * CProcess::MEASURE_MS) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;	//FPSを保存した時刻を保存
				dwFrameCount = 0;	//フレームカウントをクリア
			}

			dwCurrentTime = timeGetTime();	//現在時刻を取得

			//60Fは16.666…ミリ秒なので
			if ((dwCurrentTime - dwExecLastTime) >= (CProcess::MEASURE_MS / CProcess::FPS_BASE))
			{
				dwExecLastTime = dwCurrentTime;	//処理開始の時刻(現在時刻)を保存

				//更新処理
				g_pManager->Update();

				//描画処理
				g_pManager->Draw();
				dwFrameCount++;	//フレームカウントを加算
			}
		}
	}

	//終了処理
	g_pManager->Uninit();
	delete g_pManager;
	g_pManager = nullptr;

	// メモリリーク検出
	_CrtDumpMemoryLeaks();

	//ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//====================================================
//ウィンドウプロシージャ
//====================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:
		//WM_QUITメッセ
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(NULL, "閉じますか？", "終了", MB_YESNO);
			if (nID == IDYES)
			{
				//ウィンドウを破棄
				DestroyWindow(hWnd);
				break;
			}
			else if (nID == IDNO)
			{
				MessageBox(hWnd, "終了をキャンセルしました", "キャンセル", MB_OK);
			}
		}
		break;
	case WM_CLOSE:
		nID = MessageBox(NULL, "閉じますか？", "終了", MB_YESNO);
		if (nID == IDYES)
		{
			//ウィンドウを破棄
			DestroyWindow(hWnd);
			break;
		}
		else if (nID == IDNO)
		{
			MessageBox(hWnd, "終了をキャンセルしました", "キャンセル", MB_OK);
			return 0;
		}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
