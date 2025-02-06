//==========================================================================================
//	���ԊǗ�CPP
//
//
//==========================================================================================

//====================================
//	�C���N���[�h
//====================================
#include "timemanager.h"
#include "time.h"
#include "manager.h"
#include <stdio.h>

//====================================
//	�萔
//====================================
namespace
{
	constexpr float TIME_X = 60.0f;				//���ԕ����̉��T�C�Y
	constexpr float TIME_Y = 40.0f;				//���ԕ����̏c�T�C�Y
	constexpr int	MAX_DIGITS = 6;				//���ԕ\���̍ő包��
	constexpr float TIME_SPACE_FACT = 1.3f;		//���ԕ����̊Ԋu�{��
}

//====================================
//	�R���X�g���N�^
//====================================
CTimeManager::CTimeManager() :
	m_TimeRemaining(0.0f),
	m_timeUp(false),
	m_pFont(nullptr),
	m_digitTimers(),
	m_RefPos(0.0f, 0.0f, 0.0f),
	m_bStop(true)
{
	//�f�t�H���g�ł͎��Ԑ����Ȃ��ŏ�����
}

//====================================
//	���Ԑ����t���R���X�g���N�^
//====================================
CTimeManager::CTimeManager(float timeLimit) :
	m_TimeRemaining(timeLimit), 
	m_timeUp(false),
	m_pFont(nullptr),
	m_digitTimers()
{
	//�w�肳�ꂽ�l�Ŏ��ԏ�����
}

//====================================
//	�f�X�g���N�^
//====================================
CTimeManager::~CTimeManager()
{

}

//======================================================================================================
//	�ď������p���\�b�h
//======================================================================================================
HRESULT CTimeManager::Init(float timeLimit)
{
	// �t�H���g�ݒ�̍\���̂�������
	D3DXFONT_DESC fontDesc = {};
	fontDesc.Height = 24;                           //	�t�H���g�T�C�Y
	fontDesc.Width = 0;								//	�t�H���g�̕�(0->��������)
	fontDesc.Weight = FW_BOLD;						//	�t�H���g�̑���
	fontDesc.MipLevels = 1;							//	�~�b�v�}�b�v���x��
	fontDesc.Italic = false;						//	�C�^���b�N��
	fontDesc.CharSet = DEFAULT_CHARSET;				//	�����Z�b�g
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;	//	�o�͐��x
	fontDesc.Quality = DEFAULT_QUALITY;				//	�`��i��
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;	//�s�b�`�ƃt�@�~��
	strcpy_s(fontDesc.FaceName, "Arial");           // �t�H���g�̎��

	// �t�H���g�̍쐬
	if (FAILED(D3DXCreateFontIndirect(CManager::GetRenderer()->GetDevice(), &fontDesc, &m_pFont)))
	{
		return E_FAIL;
	}

	if (timeLimit > 0.0f)
	{
		//	���������������̐��l�ŏ�����
		m_TimeRemaining = timeLimit;
	}
	else
	{
		//	��������or������0.0f������
		m_TimeRemaining = 0.0f;
	}
	m_timeUp = false;

	//�^�C�}�[�S�̂̕����v�Z
	float totalWidth = MAX_DIGITS * TIME_X * TIME_SPACE_FACT;

	//��_�̐ݒ�
	m_RefPos = { CProcess::SCREEN_WIDTH * 0.5f, CProcess::SCREEN_HEIGHT * 0.1f, 0.0f };

	for (int i = 0; i < MAX_DIGITS; ++i)
	{
		float offsetX = -((totalWidth - TIME_X) * 0.5f) + (i * TIME_X * TIME_SPACE_FACT);
		auto timer = CTimer::Create(D3DXVECTOR3(m_RefPos.x + offsetX, m_RefPos.y, 0.0f));
		m_digitTimers.push_back(timer);
	}

	return S_OK;
}

//======================================================================================================
//	�I������
//======================================================================================================
void CTimeManager::Uninit()
{
	m_TimeRemaining = 0.0f;
	m_timeUp = false;

	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}

	//�^�C�}�[���X�g�S�v�f���폜
	m_digitTimers.clear();
}

//======================================================================================================
//	�X�V
//======================================================================================================
void CTimeManager::Update(float deltatime)
{
	if (m_TimeRemaining > 0.0f && m_bStop == false)
	{
		m_TimeRemaining -= deltatime;
		if (m_TimeRemaining <= 0.0f)
		{
			m_TimeRemaining = 0.0f;
			m_timeUp = true;
		}
	}

	//�c�莞�Ԃ������Ƃɕ���
	auto digits = GetDigits();
	size_t digitsCount = digits.size();

	//�^�C�}�[�S�̂̕����v�Z
	float totalWidth = digitsCount * TIME_X * TIME_SPACE_FACT;	//���݂̌����Ɋ�Â���

	size_t digitTimerSize = m_digitTimers.size();

	for (size_t i = 0; i < digitsCount; ++i)
	{
		float offsetX = -((totalWidth - TIME_X) * 0.5f) + (i * TIME_X * TIME_SPACE_FACT);

		if (digits[i] == 11)
		{
			//�����_�̏���
			m_digitTimers[i]->SetDigit(11);
		}
		else
		{
			//�ʏ�̐���
			m_digitTimers[i]->SetDigit(digits[i]);
		}
		m_digitTimers[i]->SetPos(D3DXVECTOR3(m_RefPos.x + offsetX, m_RefPos.y, 0.0f));
	}

	//	�����Ƃ̐��l�ݒ�ƈʒu����
	for (auto it = m_digitTimers.begin() + digitsCount; it != m_digitTimers.end();)
	{
		//�I�u�W�F�N�g�̍폜�v��
		//==========================
		//	delete�͐�΂ɂ��Ȃ�!!!
		//==========================
		(*it)->Uninit();	
		it = m_digitTimers.erase(it);
	}
}

//======================================================================================================
//	�`��
//======================================================================================================
void CTimeManager::Draw()
{

}

//======================================================================================================
//	���\��
//======================================================================================================
void CTimeManager::DrawInfo()
{
	RECT rect = { 0,0,CProcess::SCREEN_WIDTH,CProcess::SCREEN_HEIGHT };
	char aStr[256];

	snprintf(&aStr[0],
		256,
		"�c�莞��:%.2f\n�I��������ǂ���:%s\n",
		m_TimeRemaining,
		m_timeUp ? "true" : "false"
	);

	m_pFont->DrawText(nullptr,
		&aStr[0],
		-1,
		&rect,
		DT_BOTTOM | DT_RIGHT,
		D3DCOLOR_RGBA(0, 0, 0, 255));
}

//======================================================================================================
//	���ԉ��Z
//======================================================================================================
void CTimeManager::AddTime(float addtime)
{
	//	�����͉��Z�ł��Ȃ��悤�ɂ���
	if (addtime > 0.0f)
	{
		m_TimeRemaining += addtime;
		m_timeUp = false;	//���Ԓǉ��Ńt���O����
	}
}

//======================================================================================================
//	���Ԍ��Z
//======================================================================================================
void CTimeManager::SubTime(float subtime)
{
	//	�����͌��Z�ł��Ȃ��悤�ɂ���
	if (subtime > 0.0f)
	{
		m_TimeRemaining -= subtime;
		if (m_TimeRemaining <= 0.0f)
		{//	���Z�������ʏI����Ă���
			//	���Ԑ؂�t���O
			m_TimeRemaining = 0.0f;
			m_timeUp = true;
		}
	}
}

//======================================================================================================
//	�c�莞�Ԏ擾
//======================================================================================================
float CTimeManager::GetTimeRemaining()const
{
	return m_TimeRemaining;
}

//======================================================================================================
//	���Ԑ؂�t���O�擾
//======================================================================================================
bool CTimeManager::IsTimeUp()const
{
	return m_timeUp;
}

//======================================================================================================
//	���Ԓ�~�t���O�擾
//======================================================================================================
bool CTimeManager::IsTimeStop() const
{
	return m_bStop;
}

//======================================================================================================
//	�X�g�b�v���邩�ǂ���
//======================================================================================================
void CTimeManager::SetStop(bool bstop)
{
	m_bStop = bstop;
}

//======================================================================================================
//	�\���ݒ�
//======================================================================================================
void CTimeManager::SetDisp(bool bdisp)
{
	for (CTimer* pTimer : m_digitTimers)
	{
		pTimer->SetUse(bdisp);
	}
}

//======================================================================================================
//	���ԕ����ێ�
//======================================================================================================
std::vector<int> CTimeManager::GetDigits()const
{
	std::vector<int> digits;
	int timeInSecond = static_cast<int>(m_TimeRemaining);	//�b�P�ʂɕϊ�
	float decimalPart = m_TimeRemaining - timeInSecond;		//�����_�ȉ��ɕ���
	do
	{
		digits.push_back(timeInSecond % 10);	//���ʌ����擾
		timeInSecond /= 10;						//�����P���炷
	} while (timeInSecond > 0);

	//	���𐳂������ɕ��ёւ�
	std::reverse(digits.begin(), digits.end());

	//�����_�̈ʒu��}��
	digits.push_back(11);	//�u11�v�������_�Ƃ��Ĉ���

	//�����_�𕪊�(�����_�ȉ�2�P�^)
	constexpr int decimalDigits = 2;	//�\�����鏬���_�ȉ��̌���
	for (int i = 0; i < decimalDigits; ++i)
	{
		decimalPart *= 10;
		int digit = static_cast<int>(decimalPart) % 10;
		digits.push_back(digit);
	}

	return digits;
}