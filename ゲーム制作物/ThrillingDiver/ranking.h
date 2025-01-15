//
//	���U���g�����L���O
//
//
//
#pragma once

//
//	�C���N���[�h
//
#include "object2D.h"
#include <string>
#include <vector>

//	���l�\���|���S���N���X
class CNumber;

struct RankingEntry
{
	float time;	//�^�C��
};

//�����L���O�}�l�[�W���[
class CRankingManager
{
public:
	CRankingManager();
	~CRankingManager();

	void Init(const std::string& filename);
	void AddTime(float time);	//�^�C���������L���O�ǉ�
	void SortRanking();			//�����L���O����ׂ���
	void SaveRanking();			//�����L���O��ۑ�
	void DisplayRanking();		//�����L���O��\��
private:
	std::vector<RankingEntry> m_ranking;	//�����L���O�f�[�^
	std::string m_fileName;					//�O���t�@�C����
	static const int MAX_ENTRIES = 10;		//�����L���O�̍ő吔
};