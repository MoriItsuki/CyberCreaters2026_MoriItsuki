//=======================================================================================
//	インプットCPP(input.cpp)
//
//
//
//=======================================================================================

//====================================================================
//	マクロ定義
//====================================================================
#define INTERVAL_REPEAT	(120)	//リピートの間隔

//====================================================================
//	インクルード
//====================================================================
#include "input.h"

//====================================================================
// CInput実装
//====================================================================

//==========================================
//	静的メンバ関数初期化
//==========================================
LPDIRECTINPUT8 CInput::m_pInput = nullptr;

//===============================================
//	コンストラクタ
//===============================================
CInput::CInput() :m_pDevice(nullptr)
{

}

//===============================================
//	デストラクタ
//===============================================
CInput::~CInput()
{

}

//===============================================
//	初期化設定
//===============================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == nullptr)
	{
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

//===============================================
//	破棄(入力デバイスとDirectInputの破棄)
//===============================================
void CInput::Uninit()
{
	//入力デバイスの破棄
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();	//デバイスへのアクセス権を放棄
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
	//DirectInputの破棄
	if (m_pInput != nullptr)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}
}

//====================================================================
// CInputKeyboard実装
//====================================================================

//===============================================
//	キーボードコンストラクタ
//===============================================
CInputKeyboard::CInputKeyboard()
{
	for (int nCnt = 0; nCnt < MAX_KEY; nCnt++)
	{
		m_aKeyState[nCnt] = 0;
		m_aKeyStateTrigger[nCnt] = 0;
		m_aKeyStateRelease[nCnt] = 0;
		m_aKeyStateRepeat[nCnt] = 0;
		m_aKeyCurrentTime[nCnt] = 0;
		m_aKeyExecLastTime[nCnt] = 0;
	}
}

//===============================================
//	キーボードデストラクタ
//===============================================
CInputKeyboard::~CInputKeyboard()
{

}

//===============================================
//	キーボード初期設定
//===============================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);
	//入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	//データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	//協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	//キーボードへのアクセス権を獲得
	m_pDevice->Acquire();
	return S_OK;
}

//===============================================
//	キーボードの破棄
//===============================================
void CInputKeyboard::Uninit()
{
	CInput::Uninit();
}

//===============================================
// キーボード入力更新
//===============================================
void CInputKeyboard::Update()
{
	BYTE aKeyState[MAX_KEY];	//キーボードの入力情報
	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntkey = 0; nCntkey < MAX_KEY; nCntkey++)
		{
			//キーボードのトリガー情報を保存
			m_aKeyStateTrigger[nCntkey] = (m_aKeyState[nCntkey] ^ aKeyState[nCntkey]) & aKeyState[nCntkey];

			//キーボードのリリース情報を保存
			m_aKeyStateRelease[nCntkey] = (m_aKeyState[nCntkey] ^ aKeyState[nCntkey]) & ~aKeyState[nCntkey];

			//現在の時間を取得する
			m_aKeyCurrentTime[nCntkey] = timeGetTime();

			if (aKeyState[nCntkey] && ((m_aKeyCurrentTime[nCntkey] - m_aKeyExecLastTime[nCntkey]) > INTERVAL_REPEAT))
			{
				//最後に真を返した時間を保存
				m_aKeyStateRepeat[nCntkey] = aKeyState[nCntkey];
			}
			else
			{
				//キーボードのリピート情報を保存
				m_aKeyStateRepeat[nCntkey] = aKeyState[nCntkey];
			}
			//キーボードのプレス情報を保存
			m_aKeyState[nCntkey] = aKeyState[nCntkey];
		}
	}
	else
	{
		m_pDevice->Acquire();	//キーボードへのアクセス権を獲得
	}
}

//===============================================
//	プレス状態取得
//===============================================
bool CInputKeyboard::GetPress(int key)
{
	return (m_aKeyState[key] & 0x80) != 0;
}

//===============================================
//	トリガー状態取得
//===============================================
bool CInputKeyboard::GetTrigger(int key)	//トリガー状態取得
{
	return (m_aKeyStateTrigger[key] & 0x80) != 0;
}

//===============================================
//	リリース状態取得
//===============================================
bool CInputKeyboard::GetRelease(int key)	//リリース状態取得
{
	return (m_aKeyStateRelease[key] & 0x80) != 0;
}
//===============================================
//	リピート状態取得
//===============================================
bool CInputKeyboard::GetRepeat(int key)	//リピート状態取得
{
	return (m_aKeyStateRepeat[key] & 0x80) != 0;
}

//====================================================================
// CInputGamePad実装
//====================================================================

//===============================================
//	ゲームパッドコンストラクタ
//===============================================
CInputGamePad::CInputGamePad()
{
	ZeroMemory(&m_GamepadState, sizeof(DIJOYSTATE));
}

//===============================================
//	ゲームパッドストラクタ
//===============================================
CInputGamePad::~CInputGamePad()
{

}

//===============================================
//	ゲームパッド初期設定
//===============================================
HRESULT CInputGamePad::Init(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputの初期化
	CInput::Init(hInstance, hWnd);

	//入力デバイス(ゲームパッド)の生成
	HRESULT hr = m_pInput->CreateDevice(GUID_Joystick, &m_pDevice, NULL);
	if (FAILED(hr))
	{
		m_pDevice = nullptr;
		return E_FAIL;
	}
	//データフォーマットを設定
	hr = m_pDevice->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	//協調モードを設定
	hr = m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	//ゲームパッドの仕様準備ができたらアクセスを開始
	hr = m_pDevice->Acquire();
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	//ゲームパッドへのアクセス権を獲得
	return S_OK;
}

//===============================================
//	ゲームパッドの破棄
//===============================================
void CInputGamePad::Uninit()
{
	CInput::Uninit();
}

//===============================================
// ゲームパッド入力更新
//===============================================
void CInputGamePad::Update()
{
	//ゲームパッドデバイスが作成されていない場合はスキップする
	if (m_pDevice == nullptr)
	{
		return;	//デバイスがないからスキップ
	}

	//入力デバイスからデータを取得
	HRESULT hr = m_pDevice->GetDeviceState(sizeof(DIJOYSTATE2), &m_GamepadState);
	//デバイスの取得に失敗した場合
	if (FAILED(hr))
	{
		//デバイスが取得されていない、接続が切れたときに出す
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
		{
			//デバイスを再取得をしようとする
			m_pDevice->Acquire();	//ゲームパッドへのアクセス権を獲得
		}

		return;	//ゲームパッドが接続されてないから更新処理をスキップ
	}

	//=================================
	//	スティックの二次元軸を取得
	//=================================
	//左スティック
	LONG StickL_X = m_GamepadState.lX;	//x軸
	LONG StickL_Y = m_GamepadState.lY;	//y軸
	//右スティック
	LONG StickR_X = m_GamepadState.lRx;	//x軸
	LONG StickR_Y = m_GamepadState.lRy;	//y軸

	//=================================
	//	トリガーのZ軸を取得
	//=================================
	//LT,RTの値を取得
	LONG triggerValue = m_GamepadState.lZ;

	//=================================
	//	デッドゾーン処理
	//=================================
	//デッドゾーンの値を設定
	const LONG DEADZONE = 8000;

	//それぞれの軸がデッドゾーン値未満なら '0' に設定
	if (abs(StickL_X) < DEADZONE) StickL_X = 0;
	if (abs(StickL_Y) < DEADZONE) StickL_Y = 0;
	if (abs(StickR_X) < DEADZONE) StickR_X = 0;
	if (abs(StickR_Y) < DEADZONE) StickR_Y = 0;

	//スティックの値を正規化
	m_fNorStickL_X = static_cast<float>(StickL_X) / 32767.0f;
	m_fNorStickL_Y = static_cast<float>(StickL_Y) / 32767.0f;
	m_fNorStickR_X = static_cast<float>(StickR_X) / 32767.0f;
	m_fNorStickR_Y = static_cast<float>(StickR_Y) / 32767.0f;

	// トリガーの-32768 ～ +32767 の範囲を -1.0 ～ +1.0 の範囲に正規化する
	m_fNorTriggerValue = static_cast<float>(triggerValue) / 32767.0f;

	//トリガーの値の+-で左右判別
	if (triggerValue > 32767)
	{
		//右トリガーが押されている
	}
	else if (triggerValue < 32767)
	{
		//左トリガーが押されている
	}
	else
	{
		//トリガーが押されていない
	}

	//デバイスの取得に成功していたら入力情報を更新
	for (int nCntButton = 0; nCntButton < MAX_BUTTON; nCntButton++)
	{
		//キーボードのトリガー情報を保存
		m_aButtonStateTrigger[nCntButton] = (m_aButtonState[nCntButton] ^ m_GamepadState.rgbButtons[nCntButton]) & m_GamepadState.rgbButtons[nCntButton];

		//キーボードのリリース情報を保存
		m_aButtonStateRelease[nCntButton] = (m_aButtonState[nCntButton] ^ m_GamepadState.rgbButtons[nCntButton]) & ~m_GamepadState.rgbButtons[nCntButton];

		//	現在のボタンの情報を保存
		m_aButtonState[nCntButton] = m_GamepadState.rgbButtons[nCntButton];
	}
}

//===============================================
//	プレス状態取得
//===============================================
bool CInputGamePad::GetPress(int Button)
{
	return (m_aButtonState[Button] & 0x80) != 0;
}

//===============================================
//	トリガー状態取得
//===============================================
bool CInputGamePad::GetTrigger(int Button)	//トリガー状態取得
{
	return (m_aButtonStateTrigger[Button] & 0x80) != 0;
}

//===============================================
//	リリース状態取得
//===============================================
bool CInputGamePad::GetRelease(int Button)	//リリース状態取得
{
	return (m_aButtonStateRelease[Button] & 0x80) != 0;
}
//===============================================
//	リピート状態取得
//===============================================
bool CInputGamePad::GetRepeat(int Button)	//リピート状態取得
{
	return (m_aButtonStateRepeat[Button] & 0x80) != 0;
}

//===============================================
//	トリガーの値取得
//===============================================
int CInputGamePad::GetTriggerValue()
{
	return m_GamepadState.lZ;
}

//===============================================
//	正規化されたトリガーの値取得
//===============================================
float CInputGamePad::GetNorTriggerValue()
{
	return m_fNorTriggerValue;
}

//===============================================
//	正規化された左スティックx軸の値取得
//===============================================
float CInputGamePad::GetNorStick_lx()
{
	return m_fNorStickL_X;
}

//===============================================
//	正規化された左スティックy軸の値取得
//===============================================
float CInputGamePad::GetNorStick_ly()
{
	return m_fNorStickL_Y;
}

//===============================================
//	正規化された右スティックx軸の値取得
//===============================================
float CInputGamePad::GetNorStick_rx()
{
	return m_fNorStickR_X;
}

//===============================================
//	正規化された右スティックy軸の値取得
//===============================================
float CInputGamePad::GetNorStick_ry()
{
	return m_fNorStickR_Y;
}