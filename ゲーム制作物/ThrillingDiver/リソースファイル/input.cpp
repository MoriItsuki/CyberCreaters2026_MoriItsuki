//=======================================================================================
//	�C���v�b�gCPP(input.cpp)
//
//
//
//=======================================================================================

//====================================================================
//	�}�N����`
//====================================================================
#define INTERVAL_REPEAT	(120)	//���s�[�g�̊Ԋu

//====================================================================
//	�C���N���[�h
//====================================================================
#include "input.h"

//====================================================================
// CInput����
//====================================================================

//==========================================
//	�ÓI�����o�֐�������
//==========================================
LPDIRECTINPUT8 CInput::m_pInput = nullptr;

//===============================================
//	�R���X�g���N�^
//===============================================
CInput::CInput() :m_pDevice(nullptr)
{

}

//===============================================
//	�f�X�g���N�^
//===============================================
CInput::~CInput()
{

}

//===============================================
//	�������ݒ�
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
//	�j��(���̓f�o�C�X��DirectInput�̔j��)
//===============================================
void CInput::Uninit()
{
	//���̓f�o�C�X�̔j��
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();	//�f�o�C�X�ւ̃A�N�Z�X�������
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
	//DirectInput�̔j��
	if (m_pInput != nullptr)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}
}

//====================================================================
// CInputKeyboard����
//====================================================================

//===============================================
//	�L�[�{�[�h�R���X�g���N�^
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
//	�L�[�{�[�h�f�X�g���N�^
//===============================================
CInputKeyboard::~CInputKeyboard()
{

}

//===============================================
//	�L�[�{�[�h�����ݒ�
//===============================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);
	//���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	//�������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	m_pDevice->Acquire();
	return S_OK;
}

//===============================================
//	�L�[�{�[�h�̔j��
//===============================================
void CInputKeyboard::Uninit()
{
	CInput::Uninit();
}

//===============================================
// �L�[�{�[�h���͍X�V
//===============================================
void CInputKeyboard::Update()
{
	BYTE aKeyState[MAX_KEY];	//�L�[�{�[�h�̓��͏��
	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntkey = 0; nCntkey < MAX_KEY; nCntkey++)
		{
			//�L�[�{�[�h�̃g���K�[����ۑ�
			m_aKeyStateTrigger[nCntkey] = (m_aKeyState[nCntkey] ^ aKeyState[nCntkey]) & aKeyState[nCntkey];

			//�L�[�{�[�h�̃����[�X����ۑ�
			m_aKeyStateRelease[nCntkey] = (m_aKeyState[nCntkey] ^ aKeyState[nCntkey]) & ~aKeyState[nCntkey];

			//���݂̎��Ԃ��擾����
			m_aKeyCurrentTime[nCntkey] = timeGetTime();

			if (aKeyState[nCntkey] && ((m_aKeyCurrentTime[nCntkey] - m_aKeyExecLastTime[nCntkey]) > INTERVAL_REPEAT))
			{
				//�Ō�ɐ^��Ԃ������Ԃ�ۑ�
				m_aKeyStateRepeat[nCntkey] = aKeyState[nCntkey];
			}
			else
			{
				//�L�[�{�[�h�̃��s�[�g����ۑ�
				m_aKeyStateRepeat[nCntkey] = aKeyState[nCntkey];
			}
			//�L�[�{�[�h�̃v���X����ۑ�
			m_aKeyState[nCntkey] = aKeyState[nCntkey];
		}
	}
	else
	{
		m_pDevice->Acquire();	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}

//===============================================
//	�v���X��Ԏ擾
//===============================================
bool CInputKeyboard::GetPress(int key)
{
	return (m_aKeyState[key] & 0x80) != 0;
}

//===============================================
//	�g���K�[��Ԏ擾
//===============================================
bool CInputKeyboard::GetTrigger(int key)	//�g���K�[��Ԏ擾
{
	return (m_aKeyStateTrigger[key] & 0x80) != 0;
}

//===============================================
//	�����[�X��Ԏ擾
//===============================================
bool CInputKeyboard::GetRelease(int key)	//�����[�X��Ԏ擾
{
	return (m_aKeyStateRelease[key] & 0x80) != 0;
}
//===============================================
//	���s�[�g��Ԏ擾
//===============================================
bool CInputKeyboard::GetRepeat(int key)	//���s�[�g��Ԏ擾
{
	return (m_aKeyStateRepeat[key] & 0x80) != 0;
}

//====================================================================
// CInputGamePad����
//====================================================================

//===============================================
//	�Q�[���p�b�h�R���X�g���N�^
//===============================================
CInputGamePad::CInputGamePad()
{
	ZeroMemory(&m_GamepadState, sizeof(DIJOYSTATE));
}

//===============================================
//	�Q�[���p�b�h�X�g���N�^
//===============================================
CInputGamePad::~CInputGamePad()
{

}

//===============================================
//	�Q�[���p�b�h�����ݒ�
//===============================================
HRESULT CInputGamePad::Init(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�̏�����
	CInput::Init(hInstance, hWnd);

	//���̓f�o�C�X(�Q�[���p�b�h)�̐���
	HRESULT hr = m_pInput->CreateDevice(GUID_Joystick, &m_pDevice, NULL);
	if (FAILED(hr))
	{
		m_pDevice = nullptr;
		return E_FAIL;
	}
	//�f�[�^�t�H�[�}�b�g��ݒ�
	hr = m_pDevice->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	//�������[�h��ݒ�
	hr = m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	//�Q�[���p�b�h�̎d�l�������ł�����A�N�Z�X���J�n
	hr = m_pDevice->Acquire();
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	//�Q�[���p�b�h�ւ̃A�N�Z�X�����l��
	return S_OK;
}

//===============================================
//	�Q�[���p�b�h�̔j��
//===============================================
void CInputGamePad::Uninit()
{
	CInput::Uninit();
}

//===============================================
// �Q�[���p�b�h���͍X�V
//===============================================
void CInputGamePad::Update()
{
	//�Q�[���p�b�h�f�o�C�X���쐬����Ă��Ȃ��ꍇ�̓X�L�b�v����
	if (m_pDevice == nullptr)
	{
		return;	//�f�o�C�X���Ȃ�����X�L�b�v
	}

	//���̓f�o�C�X����f�[�^���擾
	HRESULT hr = m_pDevice->GetDeviceState(sizeof(DIJOYSTATE2), &m_GamepadState);
	//�f�o�C�X�̎擾�Ɏ��s�����ꍇ
	if (FAILED(hr))
	{
		//�f�o�C�X���擾����Ă��Ȃ��A�ڑ����؂ꂽ�Ƃ��ɏo��
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
		{
			//�f�o�C�X���Ď擾�����悤�Ƃ���
			m_pDevice->Acquire();	//�Q�[���p�b�h�ւ̃A�N�Z�X�����l��
		}

		return;	//�Q�[���p�b�h���ڑ�����ĂȂ�����X�V�������X�L�b�v
	}

	//=================================
	//	�X�e�B�b�N�̓񎟌������擾
	//=================================
	//���X�e�B�b�N
	LONG StickL_X = m_GamepadState.lX;	//x��
	LONG StickL_Y = m_GamepadState.lY;	//y��
	//�E�X�e�B�b�N
	LONG StickR_X = m_GamepadState.lRx;	//x��
	LONG StickR_Y = m_GamepadState.lRy;	//y��

	//=================================
	//	�g���K�[��Z�����擾
	//=================================
	//LT,RT�̒l���擾
	LONG triggerValue = m_GamepadState.lZ;

	//=================================
	//	�f�b�h�]�[������
	//=================================
	//�f�b�h�]�[���̒l��ݒ�
	const LONG DEADZONE = 8000;

	//���ꂼ��̎����f�b�h�]�[���l�����Ȃ� '0' �ɐݒ�
	if (abs(StickL_X) < DEADZONE) StickL_X = 0;
	if (abs(StickL_Y) < DEADZONE) StickL_Y = 0;
	if (abs(StickR_X) < DEADZONE) StickR_X = 0;
	if (abs(StickR_Y) < DEADZONE) StickR_Y = 0;

	//�X�e�B�b�N�̒l�𐳋K��
	m_fNorStickL_X = static_cast<float>(StickL_X) / 32767.0f;
	m_fNorStickL_Y = static_cast<float>(StickL_Y) / 32767.0f;
	m_fNorStickR_X = static_cast<float>(StickR_X) / 32767.0f;
	m_fNorStickR_Y = static_cast<float>(StickR_Y) / 32767.0f;

	// �g���K�[��-32768 �` +32767 �͈̔͂� -1.0 �` +1.0 �͈̔͂ɐ��K������
	m_fNorTriggerValue = static_cast<float>(triggerValue) / 32767.0f;

	//�g���K�[�̒l��+-�ō��E����
	if (triggerValue > 32767)
	{
		//�E�g���K�[��������Ă���
	}
	else if (triggerValue < 32767)
	{
		//���g���K�[��������Ă���
	}
	else
	{
		//�g���K�[��������Ă��Ȃ�
	}

	//�f�o�C�X�̎擾�ɐ������Ă�������͏����X�V
	for (int nCntButton = 0; nCntButton < MAX_BUTTON; nCntButton++)
	{
		//�L�[�{�[�h�̃g���K�[����ۑ�
		m_aButtonStateTrigger[nCntButton] = (m_aButtonState[nCntButton] ^ m_GamepadState.rgbButtons[nCntButton]) & m_GamepadState.rgbButtons[nCntButton];

		//�L�[�{�[�h�̃����[�X����ۑ�
		m_aButtonStateRelease[nCntButton] = (m_aButtonState[nCntButton] ^ m_GamepadState.rgbButtons[nCntButton]) & ~m_GamepadState.rgbButtons[nCntButton];

		//	���݂̃{�^���̏���ۑ�
		m_aButtonState[nCntButton] = m_GamepadState.rgbButtons[nCntButton];
	}
}

//===============================================
//	�v���X��Ԏ擾
//===============================================
bool CInputGamePad::GetPress(int Button)
{
	return (m_aButtonState[Button] & 0x80) != 0;
}

//===============================================
//	�g���K�[��Ԏ擾
//===============================================
bool CInputGamePad::GetTrigger(int Button)	//�g���K�[��Ԏ擾
{
	return (m_aButtonStateTrigger[Button] & 0x80) != 0;
}

//===============================================
//	�����[�X��Ԏ擾
//===============================================
bool CInputGamePad::GetRelease(int Button)	//�����[�X��Ԏ擾
{
	return (m_aButtonStateRelease[Button] & 0x80) != 0;
}
//===============================================
//	���s�[�g��Ԏ擾
//===============================================
bool CInputGamePad::GetRepeat(int Button)	//���s�[�g��Ԏ擾
{
	return (m_aButtonStateRepeat[Button] & 0x80) != 0;
}

//===============================================
//	�g���K�[�̒l�擾
//===============================================
int CInputGamePad::GetTriggerValue()
{
	return m_GamepadState.lZ;
}

//===============================================
//	���K�����ꂽ�g���K�[�̒l�擾
//===============================================
float CInputGamePad::GetNorTriggerValue()
{
	return m_fNorTriggerValue;
}

//===============================================
//	���K�����ꂽ���X�e�B�b�Nx���̒l�擾
//===============================================
float CInputGamePad::GetNorStick_lx()
{
	return m_fNorStickL_X;
}

//===============================================
//	���K�����ꂽ���X�e�B�b�Ny���̒l�擾
//===============================================
float CInputGamePad::GetNorStick_ly()
{
	return m_fNorStickL_Y;
}

//===============================================
//	���K�����ꂽ�E�X�e�B�b�Nx���̒l�擾
//===============================================
float CInputGamePad::GetNorStick_rx()
{
	return m_fNorStickR_X;
}

//===============================================
//	���K�����ꂽ�E�X�e�B�b�Ny���̒l�擾
//===============================================
float CInputGamePad::GetNorStick_ry()
{
	return m_fNorStickR_Y;
}