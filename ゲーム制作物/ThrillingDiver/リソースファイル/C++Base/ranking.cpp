//=========================================================================================
//	�����L���O
//
//
//=========================================================================================

//===========================================
//	�C���N���[�h
//===========================================
#include "ranking.h"
#include "process.h"
#include "texture.h"

#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <iomanip>

//===========================================
//	�萔
//===========================================
namespace
{
	using vec2 = D3DXVECTOR2;
	using vec3 = D3DXVECTOR3;

	constexpr float NUMBER_X = 50.0f;	//�����|���S���̉��T�C�Y
	constexpr float NUMBER_Y = 40.0f;	//�����|���S���̏c�T�C�Y
	constexpr int DIVISION = 10;	//�e�N�X�`���̕�����

	constexpr int DECIMALPOINT = 2;	//�^�C���̏������݂������_�ȉ��ǂꂾ���ɂ��邩

	constexpr float DIGIT_SPACING = 70.0f;	//���Ԃ̋�

	constexpr int VERTICAL_RANKING = 5;	//	�����L���O�̍s��(�Q��ɂ���Ƃ�)

	constexpr float CURRENTTIMESIZE_MAG = 1.2f;	//�����L���O�\�����̍���̃^�C���\���̃T�C�Y�{��

	const D3DXVECTOR3 RANKINGBASEPOSITION = { CProcess::SCREEN_WIDTH * 0.5, CProcess::SCREEN_HEIGHT - 50.0f, 0.0f };
}

//==================================================
//	�ÓI�����o�ϐ�������
//==================================================
const std::string CNumDisp::FILE_NUMBER = "data\\texture\\numbers.png";

//==============================================================================================================================
//	�����|���S���P��
//==============================================================================================================================

//==================================================
//	�R���X�g���N�^
//==================================================
CNumDisp::CNumDisp() :
	m_digit(-1),
	m_value(0),
	m_isVisible(true)
{

}

//==================================================
//	�f�X�g���N�^
//==================================================
CNumDisp::~CNumDisp()
{

}

//==================================================
//	������
//==================================================
HRESULT CNumDisp::Init()
{
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	m_pos = vec3(CProcess::SCREEN_WIDTH * 0.5f, CProcess::SCREEN_HEIGHT * 0.5f, 0.0f);
	m_size = vec3(NUMBER_X, NUMBER_Y, 0.0f);
	m_color = D3DCOLOR_XRGB(0, 0, 0);

	//�e�N�X�`���쐬
	CTexture* pTex = CManager::GetTexture();	//�e�N�X�`�����擾
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_NUMBER)));	//�ݒ�

	return S_OK;
}

//==================================================
//	�I��
//==================================================
void CNumDisp::Uninit()
{
	CObject2D::Uninit();
}

//==================================================
//	�X�V����
//==================================================
void CNumDisp::Update()
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
	float divYIn = 0.0f;
	float divYOut = 0.5f;

	//�����_�㏑���ݒ�
	if (m_value == -1)
	{
		divXIn = 0.0f;
		divXOut = 0.1f;
		divYIn = 0.5f;
		divYOut = 1.0f;
	}

	pVtx[0].tex = vec2(divXIn, divYIn);
	pVtx[1].tex = vec2(divXOut, divYIn);
	pVtx[2].tex = vec2(divXIn, divYOut);
	pVtx[3].tex = vec2(divXOut, divYOut);

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
void CNumDisp::Draw()
{
	if (m_isVisible)
	{
		CObject2D::Draw();
	}
}

//==================================================
//	����
//==================================================
CNumDisp* CNumDisp::Create(int value, D3DXVECTOR3 pos)
{
	CNumDisp* pNum = new CNumDisp();
	if (SUCCEEDED(pNum->Init()))
	{
		pNum->SetPos(pos);
		pNum->SetValue(value);
		return pNum;
	}

	delete pNum;
	return nullptr;
}

//==================================================
//	���l�ݒ�
//==================================================
void CNumDisp::SetValue(int value)
{
	if (value < -1 || value > 9)
	{
		//�f�o�b�O�p
		OutputDebugStringA("CNumDisp::SetValue: value is out of range [-1, 9]\n");
		return;
	}

	m_value = value;
}

//===========================================
//	�\���ݒ�
//===========================================
void CNumDisp::SetVisibility(bool isvisible)
{
	m_isVisible = isvisible;
}

//==============================================================================================================================
//	���l�Ǘ�(���������|���S��)
//==============================================================================================================================

//===========================================
//	�R���X�g���N�^
//===========================================
CNumberGroup::CNumberGroup() :
	m_numbers(),
	m_pos(0.0f, 0.0f, 0.0f),
	m_isVisible(true),
	m_currentvalue(0),
	m_color(D3DCOLOR_XRGB(0, 0, 0))	//�f�t�H���g������
{

}

//===========================================
//	�f�X�g���N�^
//===========================================
CNumberGroup::~CNumberGroup()
{
	Uninit();
}

//===========================================
//	������
//===========================================
HRESULT CNumberGroup::Init()
{
	return S_OK;
}

//===========================================
//	�I��
//===========================================
void CNumberGroup::Uninit()
{
	for (CNumDisp* number : m_numbers)
	{
		if (number)
		{
			//number->Uninit();	//TODO
			//delete number;
		}
	}
	m_numbers.clear();
}

//===========================================
//	�X�V����
//===========================================
void CNumberGroup::Update()
{
	if (!m_isVisible) return;

	for (CNumDisp* number : m_numbers)
	{
		if (number)
		{
			number->Update();
		}
	}
}

//===========================================
//	�`�揈��
//===========================================
void CNumberGroup::Draw()
{
	if (!m_isVisible) return;

	for (CNumDisp* number : m_numbers)
	{
		if (number)
		{
			number->Draw();
		}
	}
}

//===========================================
//	���l�ݒ�
//===========================================
void CNumberGroup::SetNumber(float number)
{
	m_currentvalue = number;
	std::stringstream ss;
	ss << std::fixed << std::setprecision(DECIMALPOINT) << number;
	std::string numStr = ss.str();
	
	auto numStrSize = numStr.size();

	//	�K�v�Ȍ������v�Z���Am_numbers���g��
	while (m_numbers.size() < numStrSize)
	{
		CNumDisp* newNum = new CNumDisp();
		if (SUCCEEDED(newNum->Init()))
		{
			m_numbers.push_back(newNum);
		}
		else
		{
			delete newNum;	//	�������Ɏ��s������폜
		}
	}

	//
	//	��̏����ł͉�邽�тɕς�邩�玖�O�u�������͂��Ȃ�
	//	�������ł͉񐔂����߂邾��������u���������Ă�����
	auto numberssize = m_numbers.size();

	for (size_t i = 0; i < numberssize; ++i)
	{
		if (i < numStrSize)
		{
			if (numStr[i] == '.')
			{//	'.'�̏ꍇ��-1��n��
				m_numbers[i]->SetValue(-1);
			}
			else
			{//	�����̏ꍇ�͂��̂܂�
				m_numbers[i]->SetValue(numStr[i] - '0');
			}
			m_numbers[i]->SetVisibility(true);
		}
		else
		{
			m_numbers[i]->SetVisibility(false);
		}
	}
}

//===========================================
//	�ꏊ�ݒ�
//===========================================
void CNumberGroup::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;

	//	���Ԋu�v�Z
	float digitSpace = DIGIT_SPACING * m_fSizeMag;

	//	�����̕����v�Z(���� - 1) * ���ԃX�y�[�X
	auto numbersSize = m_numbers.size();
	float totalWidth = static_cast<float>(numbersSize - 1) * digitSpace;

	//������offsetX���E�����ɂȂ�悤�ɒ���
	float offsetX = -totalWidth;

	// �����̈ʒu���X�V
	for (CNumDisp* num : m_numbers)
	{
		if (num)
		{
			num->SetPos(m_pos + D3DXVECTOR3(offsetX, 0.0f, 0.0f));
			offsetX += digitSpace;
		}
	}
}

//===========================================
//	�\���ݒ�
//===========================================
void CNumberGroup::SetVisibility(bool isVisible)
{
	m_isVisible = isVisible;

	// �eCNumDisp�̉���Ԃ�ݒ�
	for (CNumDisp* num : m_numbers)
	{
		if (num)
		{
			num->SetVisibility(isVisible);
		}
	}
}

//===========================================
//	�F�ύX
//===========================================
void CNumberGroup::SetColor(D3DCOLOR color)
{
	m_color = color;

	//	�eCNumDisp�̐F��ݒ�
	for (CNumDisp* num : m_numbers)
	{
		if (num)
		{
			num->SetColor(color);
		}
	}
}

//===========================================
//	�T�C�Y�{���ύX
//===========================================
void CNumberGroup::SetSizeMag(float mag)
{
	m_fSizeMag = mag;

	//	�eCNumDisp�̃T�C�Y�{����ݒ�
	for (CNumDisp* num : m_numbers)
	{
		if (num)
		{
			num->SetSizeMag(mag);
		}
	}
}

//==============================================================================================================================
//	�����L���O����
//==============================================================================================================================

//===========================================
//	�R���X�g���N�^
//===========================================
CRankingManager::CRankingManager() :
	m_fileName(""),
	m_basePos(RANKINGBASEPOSITION),
	m_entryOffset(0.0f, -90.0f, 0),
	m_currentPlayTime(0.0f),
	isPlayTimeInitialized(false)
{

}

//===========================================
//	�f�X�g���N�^
//===========================================
CRankingManager::~CRankingManager()
{
	Uninit();
}

//===========================================
//	������
//===========================================
HRESULT CRankingManager::Init(const std::string& filename)
{
	m_fileName = filename;

	//	���݂̃v���C�^�C���\���p�O���[�v��������
	m_currentPlaytimeGroup = std::make_unique<CNumberGroup>();
	if (FAILED(m_currentPlaytimeGroup->Init()))
	{
		return E_FAIL;
	}

	LoadRanking();

	return S_OK;
}

//===========================================
//	�I��
//===========================================
void CRankingManager::Uninit()
{
	m_numberGroup.clear();
}

//===========================================
//	�X�V
//===========================================
void CRankingManager::Update()
{
	for (auto& group : m_numberGroup)
	{
		if (group)
		{
			group->Update();
		}
	}

	//	���݂̃^�C����ݒ�
	if (m_currentPlaytimeGroup)
	{
		m_currentPlaytimeGroup->SetNumber(m_currentPlayTime);

		//	�ʒu�ݒ�
		vec3 topPosition = { CProcess::SCREEN_WIDTH * 0.5 + 250.0f, 180.0f,0.0f };
		m_currentPlaytimeGroup->SetPosition(topPosition);

		//	�F�ύX
		m_currentPlaytimeGroup->SetColor(D3DCOLOR_XRGB(0, 255, 0));

		if (!isPlayTimeInitialized)
		{//	�T�C�Y�ύX�t���O
			//	�T�C�Y�ύX
			m_currentPlaytimeGroup->SetSizeMag(CURRENTTIMESIZE_MAG);

			isPlayTimeInitialized = true;
		}
	}
}

//===========================================
//	�`��
//===========================================
void CRankingManager::Draw()
{
	for (auto& group : m_numberGroup)
	{
		if (group)
		{
			group->Draw();
		}
	}

	//	���݂̃^�C��
	if (m_currentPlaytimeGroup)
	{
		m_currentPlaytimeGroup->Draw();
	}
}

//===========================================
//	�����L���O�ǉ�
//===========================================
void CRankingManager::AddTime(float time)
{
	m_ranking.push_back({ time });

	//�\�[�g��ɍő�G���g���[���𒴂�����폜����
	SortRanking();
	if (m_ranking.size() > MAX_ENTRIES)
	{
		m_ranking.pop_back();
	}
}

//===========================================
//	�����L���O���\�[�g
//===========================================
void CRankingManager::SortRanking()
{
	std::sort(m_ranking.begin(), m_ranking.end(), [](const RankingEntry& a, const RankingEntry& b)
		{
			return a.time < b.time;	//	����
		});
}

//===========================================
//	�����L���O��ۑ�
//===========================================
void CRankingManager::SaveRanking()
{
	std::ofstream ofs(m_fileName);
	if (!ofs.is_open())
	{
		throw std::runtime_error("Failed to open file for saving ranking.");
	}

	for (const RankingEntry& entry : m_ranking)
	{
		ofs << entry.time << std::endl;
	}
}

//===========================================
//	�����L���O���擾
//===========================================
void CRankingManager::LoadRanking()
{
	std::ifstream ifs(m_fileName);
	if (!ifs.is_open())
	{
		return;
	}

	m_ranking.clear();
	RankingEntry entry;

	while (ifs >> entry.time)
	{
		m_ranking.push_back(entry);
	}

	SortRanking();
}

//===========================================
//	���l�\���̍X�V
//===========================================
void CRankingManager::UpdateNumberGroup()
{
	auto rankingsize = m_ranking.size();
	auto numGroupSize = m_numberGroup.size();

	//	���ʂ��Ƃ̐F���X�g��p��
	std::vector<D3DCOLOR> rankColor =
	{
		D3DCOLOR_XRGB(255, 215, 0),		//���F
		D3DCOLOR_XRGB(150, 150, 150),	//��F
		D3DCOLOR_XRGB(205, 127, 50)		//���F
	};

	for (size_t i = 0; i < rankingsize; ++i)
	{
		if (i >= numGroupSize)
		{
			auto newGroup = std::make_unique<CNumberGroup>();
			if (SUCCEEDED(newGroup->Init()))
			{
				m_numberGroup.emplace_back(std::move(newGroup));
			}
		}

		auto& group = m_numberGroup[i];
		group->SetNumber(m_ranking[i].time);
		group->SetVisibility(true);
		group->SetSizeMag(0.8f);

		//�����N���Ƃ̐F�t���p�T�C�Y
		auto rankcolorsize = rankColor.size();

		if (m_ranking[i].time == m_currentPlayTime)
		{//	����̃v���C�^�C���Ȃ�F��ύX
			group->SetColor(D3DCOLOR_XRGB(255, 30, 30));	//�ԐF(����̃v���C�^�C��)
			group->SetSizeMag(1.1f);
			m_basePos += {50.0f, 0.0f, 0.0f};
		}
		else if (i < rankcolorsize)
		{//	���ʂ��Ƃ̐F��ݒ�
			group->SetColor(rankColor[i]);	//rankColor���̐F
			m_basePos = RANKINGBASEPOSITION;
		}
		else
		{//	����ȊO
			group->SetColor(D3DCOLOR_XRGB(0, 0, 0));	//���F(�f�t�H���g)
			m_basePos = RANKINGBASEPOSITION;
		}

		//�ʒu�ݒ�
		//	��
		float xOffset = (i < VERTICAL_RANKING) ? m_entryOffset.x + 80.0f: m_entryOffset.x + 520.0f;	//������0�A�E��͉E�ɂ��炷
		//	�s
		float yOffset = m_entryOffset.y * static_cast<float>(4 - (i % 5));	//	0�`4�͈̔͂ŏc�ɕ���

		//�V�����ʒu��ݒ�
		vec3 newPos = m_basePos + vec3(xOffset, yOffset, 0.0f);
		group->SetPosition(newPos);
	}

	for (size_t i = rankingsize; i < numGroupSize; ++i)
	{
		m_numberGroup[i]->SetVisibility(false);
	}
}

//===========================================
//	���݂̃v���C�^�C����ݒ�
//===========================================
void CRankingManager::SetCurrentPlayTime(float time)
{
	m_currentPlayTime = time;
}

//===========================================
//	�C���X�^���X����
//===========================================
CRankingManager* CRankingManager::Create(const std::string& filename)
{
	CRankingManager* pRankingManager = new CRankingManager();
	if (pRankingManager)
	{
		if (FAILED(pRankingManager->Init(filename)))
		{
			delete pRankingManager;	//	���������s��
			return nullptr;
		}
	}

	return pRankingManager;
}
