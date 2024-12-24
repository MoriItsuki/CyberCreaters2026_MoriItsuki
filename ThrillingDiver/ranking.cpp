//
//	ランキング
//
//
//

//
//	インクルード
//
#include "ranking.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <iomanip>

//
//	コンストラクタ
//
CRankingManager::CRankingManager():
	m_fileName("")
{

}

//
//	デストラクタ
//
CRankingManager::~CRankingManager()
{

}

//
//	初期化
//
void CRankingManager::Init(const std::string& filename)
{
	m_fileName = filename;

	//外部ファイルを開いてランキング読込
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

	//ソートしておく
	SortRanking();
}

//
//	ランキング追加
//
void CRankingManager::AddTime(float time)
{
	m_ranking.push_back({ time });

	//ソート後に最大エントリー数を超えたら削除する
	SortRanking();
	if (m_ranking.size() > MAX_ENTRIES)
	{
		m_ranking.pop_back();
	}
}

//
//	ランキングをソート
//
void CRankingManager::SortRanking()
{
	std::sort
	(
		m_ranking.begin(), m_ranking.end(), [](const RankingEntry& a, const RankingEntry& b)
		{
			return a.time < b.time;	//昇順にソート(短い時間が上位)
		}
	);
}

//
//	ランキングを保存
//
void CRankingManager::SaveRanking()
{

}

//
//	ランキングを表示
//
void CRankingManager::DisplayRanking()
{

}
