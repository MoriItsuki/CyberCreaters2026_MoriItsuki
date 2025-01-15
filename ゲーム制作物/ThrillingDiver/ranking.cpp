//
//	�����L���O
//
//
//

//
//	�C���N���[�h
//
#include "ranking.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <iomanip>

//
//	�R���X�g���N�^
//
CRankingManager::CRankingManager():
	m_fileName("")
{

}

//
//	�f�X�g���N�^
//
CRankingManager::~CRankingManager()
{

}

//
//	������
//
void CRankingManager::Init(const std::string& filename)
{
	m_fileName = filename;

	//�O���t�@�C�����J���ă����L���O�Ǎ�
	std::ifstream file(filename);
	if (file.is_open())
	{
		m_ranking.clear();
		float time;
		while (file >> time)
		{
			m_ranking.push_back({ time });
		}
		file.close();
	}

	//�\�[�g���Ă���
	SortRanking();
}

//
//	�����L���O�ǉ�
//
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

//
//	�����L���O���\�[�g
//
void CRankingManager::SortRanking()
{
	std::sort
	(
		m_ranking.begin(), m_ranking.end(), [](const RankingEntry& a, const RankingEntry& b)
		{
			return a.time < b.time;	//�����Ƀ\�[�g(�Z�����Ԃ����)
		}
	);
}

//
//	�����L���O��ۑ�
//
void CRankingManager::SaveRanking()
{

}

//
//	�����L���O��\��
//
void CRankingManager::DisplayRanking()
{

}
