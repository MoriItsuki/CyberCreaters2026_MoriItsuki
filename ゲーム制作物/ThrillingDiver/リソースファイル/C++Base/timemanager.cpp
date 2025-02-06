//==========================================================================================
//	時間管理CPP
//
//
//==========================================================================================

//====================================
//	インクルード
//====================================
#include "timemanager.h"
#include "time.h"
#include "manager.h"
#include <stdio.h>

//====================================
//	定数
//====================================
namespace
{
	constexpr float TIME_X = 60.0f;				//時間文字の横サイズ
	constexpr float TIME_Y = 40.0f;				//時間文字の縦サイズ
	constexpr int	MAX_DIGITS = 6;				//時間表示の最大桁数
	constexpr float TIME_SPACE_FACT = 1.3f;		//時間文字の間隔倍率
}

//====================================
//	コンストラクタ
//====================================
CTimeManager::CTimeManager() :
	m_TimeRemaining(0.0f),
	m_timeUp(false),
	m_pFont(nullptr),
	m_digitTimers(),
	m_RefPos(0.0f, 0.0f, 0.0f),
	m_bStop(true)
{
	//デフォルトでは時間制限なしで初期化
}

//====================================
//	時間制限付きコンストラクタ
//====================================
CTimeManager::CTimeManager(float timeLimit) :
	m_TimeRemaining(timeLimit), 
	m_timeUp(false),
	m_pFont(nullptr),
	m_digitTimers()
{
	//指定された値で時間初期化
}

//====================================
//	デストラクタ
//====================================
CTimeManager::~CTimeManager()
{

}

//======================================================================================================
//	再初期化用メソッド
//======================================================================================================
HRESULT CTimeManager::Init(float timeLimit)
{
	// フォント設定の構造体を初期化
	D3DXFONT_DESC fontDesc = {};
	fontDesc.Height = 24;                           //	フォントサイズ
	fontDesc.Width = 0;								//	フォントの幅(0->自動調整)
	fontDesc.Weight = FW_BOLD;						//	フォントの太さ
	fontDesc.MipLevels = 1;							//	ミップマップレベル
	fontDesc.Italic = false;						//	イタリック体
	fontDesc.CharSet = DEFAULT_CHARSET;				//	文字セット
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;	//	出力精度
	fontDesc.Quality = DEFAULT_QUALITY;				//	描画品質
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;	//ピッチとファミリ
	strcpy_s(fontDesc.FaceName, "Arial");           // フォントの種類

	// フォントの作成
	if (FAILED(D3DXCreateFontIndirect(CManager::GetRenderer()->GetDevice(), &fontDesc, &m_pFont)))
	{
		return E_FAIL;
	}

	if (timeLimit > 0.0f)
	{
		//	引数が正数→その数値で初期化
		m_TimeRemaining = timeLimit;
	}
	else
	{
		//	引数が無or負数→0.0f初期化
		m_TimeRemaining = 0.0f;
	}
	m_timeUp = false;

	//タイマー全体の幅を計算
	float totalWidth = MAX_DIGITS * TIME_X * TIME_SPACE_FACT;

	//基準点の設定
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
//	終了処理
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

	//タイマーリスト全要素を削除
	m_digitTimers.clear();
}

//======================================================================================================
//	更新
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

	//残り時間を桁ごとに分解
	auto digits = GetDigits();
	size_t digitsCount = digits.size();

	//タイマー全体の幅を計算
	float totalWidth = digitsCount * TIME_X * TIME_SPACE_FACT;	//現在の桁数に基づく幅

	size_t digitTimerSize = m_digitTimers.size();

	for (size_t i = 0; i < digitsCount; ++i)
	{
		float offsetX = -((totalWidth - TIME_X) * 0.5f) + (i * TIME_X * TIME_SPACE_FACT);

		if (digits[i] == 11)
		{
			//小数点の処理
			m_digitTimers[i]->SetDigit(11);
		}
		else
		{
			//通常の数字
			m_digitTimers[i]->SetDigit(digits[i]);
		}
		m_digitTimers[i]->SetPos(D3DXVECTOR3(m_RefPos.x + offsetX, m_RefPos.y, 0.0f));
	}

	//	桁ごとの数値設定と位置調整
	for (auto it = m_digitTimers.begin() + digitsCount; it != m_digitTimers.end();)
	{
		//オブジェクトの削除要請
		//==========================
		//	deleteは絶対にしない!!!
		//==========================
		(*it)->Uninit();	
		it = m_digitTimers.erase(it);
	}
}

//======================================================================================================
//	描画
//======================================================================================================
void CTimeManager::Draw()
{

}

//======================================================================================================
//	情報表示
//======================================================================================================
void CTimeManager::DrawInfo()
{
	RECT rect = { 0,0,CProcess::SCREEN_WIDTH,CProcess::SCREEN_HEIGHT };
	char aStr[256];

	snprintf(&aStr[0],
		256,
		"残り時間:%.2f\n終わったかどうか:%s\n",
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
//	時間加算
//======================================================================================================
void CTimeManager::AddTime(float addtime)
{
	//	負数は加算できないようにする
	if (addtime > 0.0f)
	{
		m_TimeRemaining += addtime;
		m_timeUp = false;	//時間追加でフラグ解除
	}
}

//======================================================================================================
//	時間減算
//======================================================================================================
void CTimeManager::SubTime(float subtime)
{
	//	負数は減算できないようにする
	if (subtime > 0.0f)
	{
		m_TimeRemaining -= subtime;
		if (m_TimeRemaining <= 0.0f)
		{//	減算した結果終わってたら
			//	時間切れフラグ
			m_TimeRemaining = 0.0f;
			m_timeUp = true;
		}
	}
}

//======================================================================================================
//	残り時間取得
//======================================================================================================
float CTimeManager::GetTimeRemaining()const
{
	return m_TimeRemaining;
}

//======================================================================================================
//	時間切れフラグ取得
//======================================================================================================
bool CTimeManager::IsTimeUp()const
{
	return m_timeUp;
}

//======================================================================================================
//	時間停止フラグ取得
//======================================================================================================
bool CTimeManager::IsTimeStop() const
{
	return m_bStop;
}

//======================================================================================================
//	ストップするかどうか
//======================================================================================================
void CTimeManager::SetStop(bool bstop)
{
	m_bStop = bstop;
}

//======================================================================================================
//	表示設定
//======================================================================================================
void CTimeManager::SetDisp(bool bdisp)
{
	for (CTimer* pTimer : m_digitTimers)
	{
		pTimer->SetUse(bdisp);
	}
}

//======================================================================================================
//	時間分割保持
//======================================================================================================
std::vector<int> CTimeManager::GetDigits()const
{
	std::vector<int> digits;
	int timeInSecond = static_cast<int>(m_TimeRemaining);	//秒単位に変換
	float decimalPart = m_TimeRemaining - timeInSecond;		//小数点以下に分裂
	do
	{
		digits.push_back(timeInSecond % 10);	//下位桁を取得
		timeInSecond /= 10;						//桁を１つずらす
	} while (timeInSecond > 0);

	//	桁を正しい順に並び替え
	std::reverse(digits.begin(), digits.end());

	//小数点の位置を挿入
	digits.push_back(11);	//「11」を小数点として扱う

	//小数点を分割(小数点以下2ケタ)
	constexpr int decimalDigits = 2;	//表示する小数点以下の桁数
	for (int i = 0; i < decimalDigits; ++i)
	{
		decimalPart *= 10;
		int digit = static_cast<int>(decimalPart) % 10;
		digits.push_back(digit);
	}

	return digits;
}