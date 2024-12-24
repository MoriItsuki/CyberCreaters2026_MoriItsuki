//
//	�v���Z�X
//
//
//
#include "process.h"
#include "manager.h"
#include "camera.h"
#include "stdio.h"

#include "player.h"

//�O���[�o���ϐ�
int CProcess::m_nCntFPS = 0;

//
//	�R���X�g���N�^
//
CProcess::CProcess() :
	m_DeltaTime(0.0f),
	m_LastTime({ 0 })
{

}

//
//	�f�X�g���N�^
//
CProcess::~CProcess()
{

}

//
//	�����ݒ�
//
HRESULT CProcess::Init()
{
	//�p�t�H�[�}���X�J�E���^�̏�����
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&m_LastTime);	//�ŏ��̃^�C���X�^���v�擾

	return S_OK;
}

//
//	�I���ݒ�
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
//	�X�V����
//
void CProcess::Update()
{
	//���݂̃^�C���X�^���v���擾
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	//�O��ƌ��݂̃t���[�����Ԃ̍����v�Z
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	m_DeltaTime = static_cast<float>(currentTime.QuadPart - m_LastTime.QuadPart) / frequency.QuadPart;

	//����t���[���p�̌��ݎ��Ԃ�ۑ�
	m_LastTime = currentTime;
}

//
//	�f���^�^�C�����擾
//
float CProcess::GetDeltaTime()const
{
	return m_DeltaTime;
}

//
//	FPS�\��
//
void CProcess::DrawFPS()
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];
	CInputGamePad* pGamepad = CManager::GetGamepad();

	snprintf(&aStr[0],
		256,
		"FPS:%d\n�f���^�^�C��:%f\n�g���K�[�l:%f\n�J�����̊p�x:%f\n<�X�e�B�b�N�̒l>\n	LX:%f\n	LY:%f\n	RX:%f\n	RY:%f",
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