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
#include <memory>
#include <stdexcept>

//	数値表示ポリゴンクラス
class CNumDisp :public CObject2D
{
public:
	CNumDisp();
	~CNumDisp()override;

	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CNumDisp* Create(int value, D3DXVECTOR3 pos);
	void SetValue(int value);
	void SetVisibility(bool isvisible);
private:
	int m_digit;	//桁数
	int m_value;	//数値
	bool m_isVisible;	//表示非表示
	static const std::string FILE_NUMBER;
};

//	複数桁数値の数字ポリゴンの管理クラス
class CNumberGroup
{
public:
	CNumberGroup();
	~CNumberGroup();

	HRESULT Init();							//	初期化
	void Uninit();							//	解放
	void Update();							//	更新処理
	void Draw();							//	描画処理

	void SetNumber(float number);			//	数値を設定(小数点対応)
	void SetPosition(D3DXVECTOR3 pos);		//	グループの表示位置
	void SetVisibility(bool isVisible);		//	表示/非表示を設定
	void SetColor(D3DCOLOR color);			//	色変更
	void SetSizeMag(float mag);				//	サイズ倍率
	D3DCOLOR GetColor()const { return m_color; }	//色取得
private:
	std::vector<CNumDisp*> m_numbers;		//	数字ポリゴンのリスト
	D3DXVECTOR3 m_pos;						//	グループの基準位置
	bool m_isVisible;						//	グループの表示状態
	float m_currentvalue;					//	現在の数値
	float m_fSizeMag = 1;					//	サイズの倍率
	D3DCOLOR m_color;						//	色を保持する変数
};

//ランキング要素
struct RankingEntry
{
	float time;	//	タイム

	bool operator<(const RankingEntry& other) const
	{
		return time < other.time;
	}
};

//ランキングマネージャー
class CRankingManager
{
public:
	CRankingManager();
	~CRankingManager();

	HRESULT Init(const std::string& filename);
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画
	void AddTime(float time);	//タイムをランキング追加
	void SortRanking();			//ランキングを並べかえ
	void SaveRanking();			//ランキングを保存
	void LoadRanking();			//ランキングを取得
	void UpdateNumberGroup();	//表示用データ更新
	void SetCurrentPlayTime(float time);	//現在のプレイタイムを設定
	static CRankingManager* Create(const std::string& filename);
private:
	std::vector<RankingEntry> m_ranking;		//	ランキングデータ
	std::vector<std::unique_ptr<CNumberGroup>> m_numberGroup;	//	各順位のタイム表示用
	std::unique_ptr<CNumberGroup> m_currentPlaytimeGroup;		//	今回のプレイタイム表示用
	std::string m_fileName;						//	外部ファイル名
	static const int MAX_ENTRIES = 10;			//	ランキングの最大数
	D3DXVECTOR3 m_basePos;								//	表示基準位置
	D3DXVECTOR3 m_entryOffset;							//	各エントリの表示間隔
	float m_currentPlayTime;	//	今回のプレイタイム
	bool isPlayTimeInitialized = false;
};