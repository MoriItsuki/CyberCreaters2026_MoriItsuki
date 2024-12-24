//=====================================================
//
//���C���v���O����
//Author:Mori Itsuki
//
//=====================================================

#include "main.h"
#include "manager.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//====================================================
//�v���g�^�C�v�錾
//====================================================
LRESULT CALLBACK WindowProc(HWND hWind, UINT uMsg, WPARAM wParam, LPARAM lParam);

//====================================================
//�O���[�o���ϐ��錾
//====================================================
CManager* g_pManager = nullptr;

//====================================================
//���C���֐�
//====================================================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// ���������[�N���o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				//WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,						//�E�B���h�E�̃X�^�C��
		WindowProc,						//�E�B���h�E�v���V�[�W��
		0,								//0�ɂ���(�ʏ�͂��Ȃ�)
		0,								//0�ɂ���(     �V     )
		hInstance,						//�C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),	//�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),		//�}�E�X�J�[�\��
		(HBRUSH)(CTLCOLOR_MAX),			//�N���C�A���g�̈�̔w�i�F
		NULL,							//���j���[�o�[
		CLASS_NAME,						//�E�B���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION)	//�t�@�C���̃A�C�R��
	};

	HWND hWnd;	//�E�B���h�E�n���h��(���ʎq)
	MSG msg;	//���b�Z�[�W���i�[����ϐ�
	RECT rect = { 0,0,CProcess::SCREEN_WIDTH,CProcess::SCREEN_HEIGHT };//��ʃT�C�Y�̍\����
	DWORD dwCurrentTime;	//���ݎ���
	DWORD dwExecLastTime;	//�Ō�ɏ�����������
	DWORD dwFrameCount;		//�t���[���J�E���g
	DWORD dwFPSLastTime;	//�Ō��FPS���v����������

	//�E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�B���h�E�𐶐�
	hWnd = CreateWindowEx(0,	//�g���E�B���h�E�X�^�C��
		CLASS_NAME,
		WINDOW_NAME,			//�E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,	//�E�B���h�E�X�^�C��
		CW_USEDEFAULT,			//�E�B���h�E�̍���X���W
		CW_USEDEFAULT,			//�E�B���h�E�̍���Y���W
		(rect.right - rect.left),			//�E�B���h�E�̕�
		(rect.bottom - rect.top),			//�E�B���h�E�̍���
		NULL,					//�e�E�B���h�E�̃n���h��
		NULL,					//���j���[�n���h��or�q�E�B���h�EID
		hInstance,				//�C���X�^���X�n���h��
		NULL);					//�E�B���h�E�쐬�f�[�^


	g_pManager = new CManager;	//�}�l�[�W���[����

	//����������
	g_pManager->Init(hInstance, hWnd, TRUE);

	//����\��ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0;	//������
	dwExecLastTime = timeGetTime();	//���ݎ������擾�A�ۑ�
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//�E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);	//�E�B���h�E�\����Ԃ�ݒ�
	UpdateWindow(hWnd);			//�N���C�A���g�̈���X�V

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)//0��Ԃ�
		{//windows�̏���
			if (msg.message == WM_QUIT)
			{//WM_QUIT���b�Z���󂯎�����烁�b�Z���[�v�𔲂���
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);	//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�ɕϊ�
				DispatchMessage(&msg);	//�E�B���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{//DirectX�̏���
			if ((dwCurrentTime - dwFPSLastTime) >= CProcess::OBSTIME)
			{//0.5�b�v��
				//FPS���v��		0.5�b���ƂɌv������FPS��\������
				g_pManager->GetProcess()->m_nCntFPS = (dwFrameCount * CProcess::MEASURE_MS) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;	//FPS��ۑ�����������ۑ�
				dwFrameCount = 0;	//�t���[���J�E���g���N���A
			}

			dwCurrentTime = timeGetTime();	//���ݎ������擾

			//60F��16.666�c�~���b�Ȃ̂�
			if ((dwCurrentTime - dwExecLastTime) >= (CProcess::MEASURE_MS / CProcess::FPS_BASE))
			{
				dwExecLastTime = dwCurrentTime;	//�����J�n�̎���(���ݎ���)��ۑ�

				//�X�V����
				g_pManager->Update();

				//�`�揈��
				g_pManager->Draw();
				dwFrameCount++;	//�t���[���J�E���g�����Z
			}
		}
	}

	//�I������
	g_pManager->Uninit();
	delete g_pManager;
	g_pManager = nullptr;

	// ���������[�N���o
	_CrtDumpMemoryLeaks();

	//�E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//====================================================
//�E�B���h�E�v���V�[�W��
//====================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:
		//WM_QUIT���b�Z
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(NULL, "���܂����H", "�I��", MB_YESNO);
			if (nID == IDYES)
			{
				//�E�B���h�E��j��
				DestroyWindow(hWnd);
				break;
			}
			else if (nID == IDNO)
			{
				MessageBox(hWnd, "�I�����L�����Z�����܂���", "�L�����Z��", MB_OK);
			}
		}
		break;
	case WM_CLOSE:
		nID = MessageBox(NULL, "���܂����H", "�I��", MB_YESNO);
		if (nID == IDYES)
		{
			//�E�B���h�E��j��
			DestroyWindow(hWnd);
			break;
		}
		else if (nID == IDNO)
		{
			MessageBox(hWnd, "�I�����L�����Z�����܂���", "�L�����Z��", MB_OK);
			return 0;
		}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
