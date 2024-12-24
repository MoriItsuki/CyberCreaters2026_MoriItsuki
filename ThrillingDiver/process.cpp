//
//	プロセス
//
//
//
#include "process.h"
#include "manager.h"
#include "camera.h"
#include "stdio.h"

#include "player.h"

//グローバル変数
int CProcess::m_nCntFPS = 0;

//
//	コンストラクタ
//
CProcess::CProcess() :
	m_DeltaTime(0.0f),
	m_LastTime({ 0 })
{

}

//
//	デストラクタ
//
CProcess::~CProcess()
{

}

//
//	初期設定
//
HRESULT CProcess::Init()
{
	//パフォーマンスカウンタの初期化
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&m_LastTime);	//最初のタイムスタンプ取得

	return S_OK;
}

//
//	終了設定
//
void CProcess::Uninit()
{
	//
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
}

//
//	更新処理
//
void CProcess::Update()
{
	//現在のタイムスタンプを取得
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	//前回と現在のフレーム時間の差を計算
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	m_DeltaTime = static_cast<float>(currentTime.QuadPart - m_LastTime.QuadPart) / frequency.QuadPart;

	//次回フレーム用の現在時間を保存
	m_LastTime = currentTime;
}

//
//	デルタタイムを取得
//
float CProcess::GetDeltaTime()const
{
	return m_DeltaTime;
}

//
//	FPS表示
//
void CProcess::DrawFPS()
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];
	CInputGamePad* pGamepad = CManager::GetGamepad();

	snprintf(&aStr[0],
		256,
		"FPS:%d\nデルタタイム:%f\nトリガー値:%f\nカメラの角度:%f\n<スティックの値>\n	LX:%f\n	LY:%f\n	RX:%f\n	RY:%f",
		m_nCntFPS,
		m_DeltaTime,
		pGamepad->GetNorTriggerValue(),
		CManager::GetCamera()->GetRot().y,
		pGamepad->GetNorStick_lx(),
		pGamepad->GetNorStick_ly(),
		pGamepad->GetNorStick_rx(),
		pGamepad->GetNorStick_ry());

	m_pFont->DrawText(nullptr, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0, 0, 0, 255));
}