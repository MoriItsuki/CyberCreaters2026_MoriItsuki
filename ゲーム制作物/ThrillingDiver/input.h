//====================================================================
//	�C���v�b�g�w�b�_(input.h)
//
//
//
//====================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//==========================================
//	�C���N���[�h
//==========================================
#include "main.h"

//==========================================
//	�}�N����`
//==========================================
#define MAX_KEY	(256)	//	�ő�L�[��
#define MAX_BUTTON	(32)	//	�ő�{�^����

//==========================================
//	���͊��N���X
//==========================================
class  CInput
{
public:
	CInput();
	~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit();
	virtual void Update() = 0;	//���͒[�����ƂɎ���
protected:
	//	�S���͏����ŋ��L
	static LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pDevice;
private:
};


//==========================================
//	�L�[�{�[�h����	
//==========================================
class CInputKeyboard :public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();
	bool GetPress(int key);		//�v���X��Ԏ擾
	bool GetTrigger(int key);	//�g���K�[��Ԏ擾
	bool GetRelease(int key);	//�����[�X��Ԏ擾
	bool GetRepeat(int key);	//���s�[�g��Ԏ擾

private:
	BYTE m_aKeyState[MAX_KEY];
	BYTE m_aKeyStateTrigger[MAX_KEY];
	BYTE m_aKeyStateRelease[MAX_KEY];
	BYTE m_aKeyStateRepeat[MAX_KEY];
	DWORD m_aKeyCurrentTime[MAX_KEY];		// �L�[�{�[�h�̌��݂̎���
	DWORD m_aKeyExecLastTime[MAX_KEY];		// �L�[�{�[�h�̍Ō�ɐ^��Ԃ�������
};

//==========================================
//	�p�b�h����	
//==========================================
class CInputGamePad :public CInput
{
public:
	CInputGamePad();
	~CInputGamePad();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();
	bool GetPress(int Button);		//�v���X��Ԏ擾
	bool GetTrigger(int Button);	//�g���K�[��Ԏ擾
	bool GetRelease(int Button);	//�����[�X��Ԏ擾
	bool GetRepeat(int Button);		//���s�[�g��Ԏ擾
	int GetTriggerValue();			//�g���K�[�̒l�擾
	float GetNorTriggerValue();		//���K���g���K�[�̒l�擾
	float GetNorStick_lx();			//���K�����X�e�B�b�Nx���擾
	float GetNorStick_ly();			//���K�����X�e�B�b�Ny���擾
	float GetNorStick_rx();			//���K���E�X�e�B�b�Nx���擾
	float GetNorStick_ry();			//���K���E�X�e�B�b�Ny���擾
private:
	DIJOYSTATE2 m_GamepadState;
	BYTE m_aButtonState[MAX_BUTTON];
	BYTE m_aButtonStateTrigger[MAX_BUTTON];
	BYTE m_aButtonStateRelease[MAX_BUTTON];
	BYTE m_aButtonStateRepeat[MAX_BUTTON];
	DWORD m_aButtonCurrentTime[MAX_BUTTON];		// �L�[�{�[�h�̌��݂̎���
	DWORD m_aButtonExecLastTime[MAX_BUTTON];		// �L�[�{�[�h�̍Ō�ɐ^��Ԃ�������
	float m_fNorTriggerValue;	//�g���K�[�̒l
	float m_fNorStickL_X;
	float m_fNorStickL_Y;
	float m_fNorStickR_X;
	float m_fNorStickR_Y;
};


#endif // !_INPUT_H_
