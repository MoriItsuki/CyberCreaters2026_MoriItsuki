//=========================================================================================
//	ランキング
//
//
//=========================================================================================

//===========================================
//	インクルード
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
//	定数
//===========================================
namespace
{
	using vec2 = D3DXVECTOR2;
	using vec3 = D3DXVECTOR3;

	constexpr float NUMBER_X = 50.0f;	//数字ポリゴンの横サイズ
	constexpr float NUMBER_Y = 40.0f;	//数字ポリゴンの縦サイズ
	constexpr int DIVISION = 10;	//テクスチャの分割数

	constexpr int DECIMALPOINT = 2;	//タイムの書き込みを小数点以下どれだけにするか

	constexpr float DIGIT_SPACING = 70.0f;	//桁間の空白

	constexpr int VERTICAL_RANKING = 5;	//	ランキングの行数(２列にするとき)

	constexpr float CURRENTTIMESIZE_MAG = 1.2f;	//ランキング表示時の今回のタイム表示のサイズ倍率

	const D3DXVECTOR3 RANKINGBASEPOSITION = { CProcess::SCREEN_WIDTH * 0.5, CProcess::SCREEN_HEIGHT - 50.0f, 0.0f };
}

//==================================================
//	静的メンバ変数初期化
//==================================================
const std::string CNumDisp::FILE_NUMBER = "data\\texture\\numbers.png";

//==============================================================================================================================
//	数字ポリゴン単体
//==============================================================================================================================

//==================================================
//	コンストラクタ
//==================================================
CNumDisp::CNumDisp() :
	m_digit(-1),
	m_value(0),
	m_isVisible(true)
{

}

//==================================================
//	デストラクタ
//==================================================
CNumDisp::~CNumDisp()
{

}

//==================================================
//	初期化
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

	//テクスチャ作成
	CTexture* pTex = CManager::GetTexture();	//テクスチャ情報取得
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_NUMBER)));	//設定

	return S_OK;
}

//==================================================
//	終了
//==================================================
void CNumDisp::Uninit()
{
	CObject2D::Uninit();
}

//==================================================
//	更新処理
//==================================================
void CNumDisp::Update()
{
	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = vec3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[1].pos = vec3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = vec3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = vec3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	//	桁数に応じた分割サイズ
	float divX = 1.0f / DIVISION;

	//数値ごとのテクスチャ座標の設定
	float divXIn = m_value * divX;
	float divXOut = (m_value + 1) * divX;
	float divYIn = 0.0f;
	float divYOut = 0.5f;

	//小数点上書き設定
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

	//rhwの設定(固定)	頂点カラーの設定
	for (int nCnt = 0; nCnt < CProcess::MAX_VERTEX; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
		pVtx[nCnt].col = m_color;
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//==================================================
//	描画
//==================================================
void CNumDisp::Draw()
{
	if (m_isVisible)
	{
		CObject2D::Draw();
	}
}

//==================================================
//	生成
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
//	数値設定
//==================================================
void CNumDisp::SetValue(int value)
{
	if (value < -1 || value > 9)
	{
		//デバッグ用
		OutputDebugStringA("CNumDisp::SetValue: value is out of range [-1, 9]\n");
		return;
	}

	m_value = value;
}

//===========================================
//	表示設定
//===========================================
void CNumDisp::SetVisibility(bool isvisible)
{
	m_isVisible = isvisible;
}

//==============================================================================================================================
//	数値管理(複数数字ポリゴン)
//==============================================================================================================================

//===========================================
//	コンストラクタ
//===========================================
CNumberGroup::CNumberGroup() :
	m_numbers(),
	m_pos(0.0f, 0.0f, 0.0f),
	m_isVisible(true),
	m_currentvalue(0),
	m_color(D3DCOLOR_XRGB(0, 0, 0))	//デフォルトを黒に
{

}

//===========================================
//	デストラクタ
//===========================================
CNumberGroup::~CNumberGroup()
{
	Uninit();
}

//===========================================
//	初期化
//===========================================
HRESULT CNumberGroup::Init()
{
	return S_OK;
}

//===========================================
//	終了
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
//	更新処理
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
//	描画処理
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
//	数値設定
//===========================================
void CNumberGroup::SetNumber(float number)
{
	m_currentvalue = number;
	std::stringstream ss;
	ss << std::fixed << std::setprecision(DECIMALPOINT) << number;
	std::string numStr = ss.str();
	
	auto numStrSize = numStr.size();

	//	必要な桁数を計算し、m_numbersを拡張
	while (m_numbers.size() < numStrSize)
	{
		CNumDisp* newNum = new CNumDisp();
		if (SUCCEEDED(newNum->Init()))
		{
			m_numbers.push_back(newNum);
		}
		else
		{
			delete newNum;	//	初期化に失敗したら削除
		}
	}

	//
	//	上の処理では回るたびに変わるから事前置き換えはしない
	//	↓ここでは回数を決めるだけだから置き換えしてもいい
	auto numberssize = m_numbers.size();

	for (size_t i = 0; i < numberssize; ++i)
	{
		if (i < numStrSize)
		{
			if (numStr[i] == '.')
			{//	'.'の場合は-1を渡す
				m_numbers[i]->SetValue(-1);
			}
			else
			{//	数字の場合はそのまま
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
//	場所設定
//===========================================
void CNumberGroup::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;

	//	桁間隔計算
	float digitSpace = DIGIT_SPACING * m_fSizeMag;

	//	数字の幅を計算(桁数 - 1) * 桁間スペース
	auto numbersSize = m_numbers.size();
	float totalWidth = static_cast<float>(numbersSize - 1) * digitSpace;

	//初期のoffsetXを右揃えになるように調整
	float offsetX = -totalWidth;

	// 数字の位置を更新
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
//	表示設定
//===========================================
void CNumberGroup::SetVisibility(bool isVisible)
{
	m_isVisible = isVisible;

	// 各CNumDispの可視状態を設定
	for (CNumDisp* num : m_numbers)
	{
		if (num)
		{
			num->SetVisibility(isVisible);
		}
	}
}

//===========================================
//	色変更
//===========================================
void CNumberGroup::SetColor(D3DCOLOR color)
{
	m_color = color;

	//	各CNumDispの色を設定
	for (CNumDisp* num : m_numbers)
	{
		if (num)
		{
			num->SetColor(color);
		}
	}
}

//===========================================
//	サイズ倍率変更
//===========================================
void CNumberGroup::SetSizeMag(float mag)
{
	m_fSizeMag = mag;

	//	各CNumDispのサイズ倍率を設定
	for (CNumDisp* num : m_numbers)
	{
		if (num)
		{
			num->SetSizeMag(mag);
		}
	}
}

//==============================================================================================================================
//	ランキング処理
//==============================================================================================================================

//===========================================
//	コンストラクタ
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
//	デストラクタ
//===========================================
CRankingManager::~CRankingManager()
{
	Uninit();
}

//===========================================
//	初期化
//===========================================
HRESULT CRankingManager::Init(const std::string& filename)
{
	m_fileName = filename;

	//	現在のプレイタイム表示用グループを初期化
	m_currentPlaytimeGroup = std::make_unique<CNumberGroup>();
	if (FAILED(m_currentPlaytimeGroup->Init()))
	{
		return E_FAIL;
	}

	LoadRanking();

	return S_OK;
}

//===========================================
//	終了
//===========================================
void CRankingManager::Uninit()
{
	m_numberGroup.clear();
}

//===========================================
//	更新
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

	//	現在のタイムを設定
	if (m_currentPlaytimeGroup)
	{
		m_currentPlaytimeGroup->SetNumber(m_currentPlayTime);

		//	位置設定
		vec3 topPosition = { CProcess::SCREEN_WIDTH * 0.5 + 250.0f, 180.0f,0.0f };
		m_currentPlaytimeGroup->SetPosition(topPosition);

		//	色変更
		m_currentPlaytimeGroup->SetColor(D3DCOLOR_XRGB(0, 255, 0));

		if (!isPlayTimeInitialized)
		{//	サイズ変更フラグ
			//	サイズ変更
			m_currentPlaytimeGroup->SetSizeMag(CURRENTTIMESIZE_MAG);

			isPlayTimeInitialized = true;
		}
	}
}

//===========================================
//	描画
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

	//	現在のタイム
	if (m_currentPlaytimeGroup)
	{
		m_currentPlaytimeGroup->Draw();
	}
}

//===========================================
//	ランキング追加
//===========================================
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

//===========================================
//	ランキングをソート
//===========================================
void CRankingManager::SortRanking()
{
	std::sort(m_ranking.begin(), m_ranking.end(), [](const RankingEntry& a, const RankingEntry& b)
		{
			return a.time < b.time;	//	昇順
		});
}

//===========================================
//	ランキングを保存
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
//	ランキングを取得
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
//	数値表示の更新
//===========================================
void CRankingManager::UpdateNumberGroup()
{
	auto rankingsize = m_ranking.size();
	auto numGroupSize = m_numberGroup.size();

	//	順位ごとの色リストを用意
	std::vector<D3DCOLOR> rankColor =
	{
		D3DCOLOR_XRGB(255, 215, 0),		//金色
		D3DCOLOR_XRGB(150, 150, 150),	//銀色
		D3DCOLOR_XRGB(205, 127, 50)		//銅色
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

		//ランクごとの色付け用サイズ
		auto rankcolorsize = rankColor.size();

		if (m_ranking[i].time == m_currentPlayTime)
		{//	今回のプレイタイムなら色を変更
			group->SetColor(D3DCOLOR_XRGB(255, 30, 30));	//赤色(今回のプレイタイム)
			group->SetSizeMag(1.1f);
			m_basePos += {50.0f, 0.0f, 0.0f};
		}
		else if (i < rankcolorsize)
		{//	順位ごとの色を設定
			group->SetColor(rankColor[i]);	//rankColor内の色
			m_basePos = RANKINGBASEPOSITION;
		}
		else
		{//	それ以外
			group->SetColor(D3DCOLOR_XRGB(0, 0, 0));	//黒色(デフォルト)
			m_basePos = RANKINGBASEPOSITION;
		}

		//位置設定
		//	列
		float xOffset = (i < VERTICAL_RANKING) ? m_entryOffset.x + 80.0f: m_entryOffset.x + 520.0f;	//左側は0、右列は右にずらす
		//	行
		float yOffset = m_entryOffset.y * static_cast<float>(4 - (i % 5));	//	0～4の範囲で縦に並ぶ

		//新しい位置を設定
		vec3 newPos = m_basePos + vec3(xOffset, yOffset, 0.0f);
		group->SetPosition(newPos);
	}

	for (size_t i = rankingsize; i < numGroupSize; ++i)
	{
		m_numberGroup[i]->SetVisibility(false);
	}
}

//===========================================
//	現在のプレイタイムを設定
//===========================================
void CRankingManager::SetCurrentPlayTime(float time)
{
	m_currentPlayTime = time;
}

//===========================================
//	インスタンス生成
//===========================================
CRankingManager* CRankingManager::Create(const std::string& filename)
{
	CRankingManager* pRankingManager = new CRankingManager();
	if (pRankingManager)
	{
		if (FAILED(pRankingManager->Init(filename)))
		{
			delete pRankingManager;	//	初期化失敗時
			return nullptr;
		}
	}

	return pRankingManager;
}
