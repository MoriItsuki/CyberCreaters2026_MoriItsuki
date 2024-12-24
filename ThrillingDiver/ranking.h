//
//	リザルトランキング
//
//
//
#pragma once

//
//	インクルード
//
#include "object2D.h"
#include <string>
#include <vector>

//	数値表示ポリゴンクラス
class CNumber;

struct RankingEntry
{
	float time;	//タイム
};

//ランキングマネージャー
class CRankingManager
{
public:
	CRankingManager();
	~CRankingManager();

	void Init(const std::string& filename);
	void AddTime(float time);	//タイムをランキング追加
	void SortRanking();			//ランキングを並べかえ
	void SaveRanking();			//ランキングを保存
	void DisplayRanking();		//ランキングを表示
private:
	std::vector<RankingEntry> m_ranking;	//ランキングデータ
	std::string m_fileName;					//外部ファイル名
	static const int MAX_ENTRIES = 10;		//ランキングの最大数
};