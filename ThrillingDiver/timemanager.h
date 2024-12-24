//
//	時間管理ヘッダ
//
//
//
#pragma once

//
//	インクルード
//
#include "main.h"
#include <iostream>
#include <memory>
#include <vector>

//
//	前方宣言
//
class CTimer;

//	時間管理クラス
class CTimeManager
{
public:
	CTimeManager();				//デフォルトコンストラクタ
	explicit CTimeManager(float timeLimit);	//時間制限付きコンストラクタ
	~CTimeManager();							//デストラクタ
	HRESULT Init(float timeLimit = 0.0f);				//再初期化用メソッド
	void Uninit();
	void Update(float deltatime);			//更新
	void Draw();							//描画
	void DrawInfo();						//情報表示
	void AddTime(float additionalTime);		//時間加算
	void SubTime(float penaltyTime);	//時間減算
	float GetTimeRemaining()const;			//残り時間取得
	bool IsTimeUp()const;					//時間切れフラグ取得
	bool IsTimeStop()const;					//時間停止フラグ取得
	void SetStop(bool bstop);				//ストップ設定
	std::vector<int> GetDigits()const;
private:
	float m_TimeRemaining = 0.0f;	//残り時間
	bool m_timeUp = false;			//時間切れフラグ
	std::vector<CTimer*> m_digitTimers;	//桁ごとの数字表示用
	bool m_bStop = true;				//時間を停止するかどうか
	D3DXVECTOR3 m_RefPos;				//タイマーの基準点
	LPD3DXFONT m_pFont;
};