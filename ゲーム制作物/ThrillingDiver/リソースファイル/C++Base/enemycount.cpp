//==============================================================================================================================
//	�G�̐��Ǘ�	(enemycount.cpp)
//
//
//==============================================================================================================================

//==================================================
//	�C���N���[�h
//==================================================
#include "enemycount.h"

#include "texture.h"

//==================================================
//	�萔
//==================================================
namespace
{
	using vec3 = D3DXVECTOR3;
	using vec2 = D3DXVECTOR2;

	//CNumber
	constexpr float NUMBER_X = 50.0f;
	constexpr float NUMBER_Y = 40.0f;
	constexpr int DIVISION = 10;	//�e�N�X�`���̕�����

	//CEnemyCountBG
	constexpr float BG_X = 200.0f;
	constexpr float BG_Y = 100.0f;

	const vec3 BGCorrectPos = { 30.0f, 5.0f, 0.0f };
}

//==================================================
//	�ÓI�����o�ϐ�������
//==================================================
const std::string CNumber::FILE_NUMBER = "data\\texture\\numbers.png";
const std::string CEnemyCountBG::FILE_ENMCNTBG = "data\\texture\\enemycount.png";

//==============================================================================================================================
//	�����|���S���P��
//==============================================================================================================================

//==================================================
//	�R���X�g���N�^
//==================================================
CNumber::CNumber() :
	m_digit(-1),
	m_value(0),
	m_isVisible(true)
{

}

//==================================================
//	�f�X�g���N�^
//==================================================
CNumber::~CNumber()
{

}

//==================================================
//	������
//==================================================
HRESULT CNumber::Init()
{
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	m_pos = vec3(CProcess::SCREEN_WIDTH * 0.5f, CProcess::SCREEN_HEIGHT * 0.5f, 0.0f);
	m_size = vec3(NUMBER_X, NUMBER_Y, 0.0f);
	m_color = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`���쐬
	CTexture* pTex = CManager::GetTexture();	//�e�N�X�`�����擾
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_NUMBER)));	//�ݒ�

	return S_OK;
}

//==================================================
//	�I��
//==================================================
void CNumber::Uninit()
{
	CObject2D::Uninit();
}

//==================================================
//	�X�V����
//==================================================
void CNumber::Update()
{
	VERTEX_2D* pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = vec3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[1].pos = vec3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = vec3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = vec3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	//	�����ɉ����������T�C�Y
	float divX = 1.0f / DIVISION;

	//���l���Ƃ̃e�N�X�`�����W�̐ݒ�
	float divXIn = m_value * divX;
	float divXOut = (m_value + 1) * divX;

	pVtx[0].tex = vec2(divXIn, 0.0f);
	pVtx[1].tex = vec2(divXOut, 0.0f);
	pVtx[2].tex = vec2(divXIn, 0.5f);
	pVtx[3].tex = vec2(divXOut, 0.5f);

	//rhw�̐ݒ�(�Œ�)	���_�J���[�̐ݒ�
	for (int nCnt = 0; nCnt < CProcess::MAX_VERTEX; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
		pVtx[nCnt].col = m_color;
	}

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//==================================================
//	�`��
//==================================================
void CNumber::Draw()
{
	if (m_isVisible)
	{
		CObject2D::Draw();
	}
}

//==================================================
//	����
//==================================================
CNumber* CNumber::Create(int value, D3DXVECTOR3 pos)
{
	CNumber* pNum = new CNumber();
	pNum->Init();
	pNum->SetPos(pos);
	pNum->SetValue(value);

	return pNum;
}

//==================================================
//	���l�ݒ�
//==================================================
void CNumber::SetValue(int value)
{
	if (value < -1 || value > 9)
	{
		//�f�o�b�O�p
		OutputDebugStringA("CNumber::SetValue: value is out of range [-1, 9]\n");
		return;
	}

	if (value == -1)
	{
		//����l -1: ��\���ɂ���
		m_isVisible = false;
	}
	else
	{
		//�ʏ퐔�l�͈�
		m_isVisible = true;
		m_value = value;
	}

	//Debug Output 
	char debugMsg[128];
	sprintf_s(debugMsg, "CNumber::SetValue: value = %d, visible = %s\n", value, m_isVisible ? "true" : "false");
	OutputDebugStringA(debugMsg);
}

//==============================================================================================================================
//	�w�i
//==============================================================================================================================

//==================================================
//	�R���X�g���N�^
//==================================================
CEnemyCountBG::CEnemyCountBG()
{

}

//==================================================
//	�f�X�g���N�^
//==================================================
CEnemyCountBG::~CEnemyCountBG()
{

}

//==================================================
//	������
//==================================================
HRESULT CEnemyCountBG::Init()
{
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	m_pos = vec3(CProcess::SCREEN_WIDTH * 0.5f, CProcess::SCREEN_HEIGHT * 0.5f, 0.0f);
	m_size = vec3(BG_X, BG_Y, 0.0f);

	//�e�N�X�`���쐬
	CTexture* pTex = CManager::GetTexture();	//�e�N�X�`�����擾
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_ENMCNTBG)));	//�ݒ�

	return S_OK;
}

//==================================================
//	�I��
//==================================================
void CEnemyCountBG::Uninit()
{
	CObject2D::Uninit();
}

//==================================================
//	�X�V����
//==================================================
void CEnemyCountBG::Update()
{
	CObject2D::Update();
}

//==================================================
//	�`��
//==================================================
void CEnemyCountBG::Draw()
{
	CObject2D::Draw();
}

//==================================================
//	��������
//==================================================
CEnemyCountBG* CEnemyCountBG::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CEnemyCountBG* pCntBG = new CEnemyCountBG();
	pCntBG->Init();
	pCntBG->SetPos(pos);
	pCntBG->SetSize(size);

	return pCntBG;
}

//==============================================================================================================================
//	���l�Ǘ��}�l�[�W���[
//==============================================================================================================================

//==================================================
//	�R���X�g���N�^
//==================================================
CENMCNTDisplayManager::CENMCNTDisplayManager() :
	m_nCurrentValue(0),
	m_basePos(0.0f, 0.0f, 0.0f),
	m_digitSpacing(60.0f),
	m_numbers(),
	m_pENMCNTBG(nullptr)
{

}

//==================================================
//	�f�X�g���N�^
//==================================================
CENMCNTDisplayManager::~CENMCNTDisplayManager()
{
	Uninit();
}

//==================================================
//	������
//==================================================
HRESULT CENMCNTDisplayManager::Init()
{
	//�w�i����ԉ��ɂȂ�悤�ɍŏ��ɐ���
	m_pENMCNTBG = CEnemyCountBG::Create(m_basePos, vec3(200.0f, 80.0f, 0.0f));

	//���l��0������
	m_nCurrentValue = 0;

	//�C���X�^���X�X�V
	UpdateNumbers();

	return S_OK;
}

//==================================================
//	�I������
//==================================================
void CENMCNTDisplayManager::Uninit()
{
	for (CNumber* number : m_numbers)
	{
		number->Uninit();
	}
	m_numbers.clear();
}

//==================================================
//	�X�V����
//==================================================
void CENMCNTDisplayManager::Update()
{
	for (CNumber* number : m_numbers)
	{
		number->Update();
	}
}

//==================================================
//	�`�揈��
//==================================================
void CENMCNTDisplayManager::Draw()
{
	for (CNumber* number : m_numbers)
	{
		number->Draw();
	}
}

//==================================================
//	�\�����l�ݒ�
//==================================================
void CENMCNTDisplayManager::SetValue(int value)
{
	if (m_nCurrentValue != value)
	{
		m_nCurrentValue = value;
		UpdateNumbers();	//�����̕\�����X�V
	}
}

//==================================================
//	�\�����l�}�l�[�W���[
//==================================================
CENMCNTDisplayManager* CENMCNTDisplayManager::Create(D3DXVECTOR3 pos, int value)
{
	CENMCNTDisplayManager* pENMCNTManager = new CENMCNTDisplayManager();
	pENMCNTManager->m_basePos = pos;
	pENMCNTManager->Init();
	pENMCNTManager->SetValue(value);

	return pENMCNTManager;
}

//==================================================
//	���l�ύX�ɉ����ăC���X�^���X�X�V
//==================================================
void CENMCNTDisplayManager::UpdateNumbers()
{
	//���݂̐��l�𕶎��񉻂��Č������擾
	std::string valueStr = std::to_string(m_nCurrentValue);
	size_t digitCount = valueStr.size();
	size_t numbersSize = m_numbers.size();

	//�����ɍ��킹��CNumber�C���X�^���X�𒲐�
	if (numbersSize < digitCount)
	{
		//�������������ꍇ�ɐV����CNumber�C���X�^���X�ǉ�
		for (size_t i = numbersSize; i < digitCount; ++i)
		{
			D3DXVECTOR3 pos = m_basePos + BGCorrectPos;
			pos.x += i * m_digitSpacing;	//�����̊Ԋu���l�������ʒu

			CNumber* number = CNumber::Create(0, pos);
			m_numbers.push_back(number);
		}
	}
	else if (numbersSize > digitCount)
	{
		size_t numbersSize = m_numbers.size();

		//�������������ꍇ�A�s�v�ȃC���X�^���X���\���ɂ���
		for (size_t i = digitCount; i < numbersSize; ++i)
		{
			m_numbers[i]->SetValue(-1);	//��\���ɂ���
		}
	}

	//�e���̒l���X�V
	for (size_t i = 0; i < numbersSize; ++i)
	{
		int digitValue = valueStr[i] - '0';	//�����𐔒l�ɕϊ�
		m_numbers[i]->SetValue(digitValue);

		//���̑��ύX�_
		if (m_nCurrentValue != 0)
		{
			if (m_nCurrentValue <= 5)
			{
				m_numbers[i]->SetColor(D3DCOLOR_XRGB(255, 0, 100));
			}
		}
	}
}
