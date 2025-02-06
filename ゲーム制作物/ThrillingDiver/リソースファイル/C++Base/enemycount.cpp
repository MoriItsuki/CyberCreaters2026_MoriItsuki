//==============================================================================================================================
//	敵の数管理	(enemycount.cpp)
//
//
//==============================================================================================================================

//==================================================
//	インクルード
//==================================================
#include "enemycount.h"

#include "texture.h"

//==================================================
//	定数
//==================================================
namespace
{
	using vec3 = D3DXVECTOR3;
	using vec2 = D3DXVECTOR2;

	//CNumber
	constexpr float NUMBER_X = 50.0f;
	constexpr float NUMBER_Y = 40.0f;
	constexpr int DIVISION = 10;	//テクスチャの分割数

	//CEnemyCountBG
	constexpr float BG_X = 200.0f;
	constexpr float BG_Y = 100.0f;

	const vec3 BGCorrectPos = { 30.0f, 5.0f, 0.0f };
}

//==================================================
//	静的メンバ変数初期化
//==================================================
const std::string CNumber::FILE_NUMBER = "data\\texture\\numbers.png";
const std::string CEnemyCountBG::FILE_ENMCNTBG = "data\\texture\\enemycount.png";

//==============================================================================================================================
//	数字ポリゴン単体
//==============================================================================================================================

//==================================================
//	コンストラクタ
//==================================================
CNumber::CNumber() :
	m_digit(-1),
	m_value(0),
	m_isVisible(true)
{

}

//==================================================
//	デストラクタ
//==================================================
CNumber::~CNumber()
{

}

//==================================================
//	初期化
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

	//テクスチャ作成
	CTexture* pTex = CManager::GetTexture();	//テクスチャ情報取得
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_NUMBER)));	//設定

	return S_OK;
}

//==================================================
//	終了
//==================================================
void CNumber::Uninit()
{
	CObject2D::Uninit();
}

//==================================================
//	更新処理
//==================================================
void CNumber::Update()
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

	pVtx[0].tex = vec2(divXIn, 0.0f);
	pVtx[1].tex = vec2(divXOut, 0.0f);
	pVtx[2].tex = vec2(divXIn, 0.5f);
	pVtx[3].tex = vec2(divXOut, 0.5f);

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
void CNumber::Draw()
{
	if (m_isVisible)
	{
		CObject2D::Draw();
	}
}

//==================================================
//	生成
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
//	数値設定
//==================================================
void CNumber::SetValue(int value)
{
	if (value < -1 || value > 9)
	{
		//デバッグ用
		OutputDebugStringA("CNumber::SetValue: value is out of range [-1, 9]\n");
		return;
	}

	if (value == -1)
	{
		//特殊値 -1: 非表示にする
		m_isVisible = false;
	}
	else
	{
		//通常数値範囲
		m_isVisible = true;
		m_value = value;
	}

	//Debug Output 
	char debugMsg[128];
	sprintf_s(debugMsg, "CNumber::SetValue: value = %d, visible = %s\n", value, m_isVisible ? "true" : "false");
	OutputDebugStringA(debugMsg);
}

//==============================================================================================================================
//	背景
//==============================================================================================================================

//==================================================
//	コンストラクタ
//==================================================
CEnemyCountBG::CEnemyCountBG()
{

}

//==================================================
//	デストラクタ
//==================================================
CEnemyCountBG::~CEnemyCountBG()
{

}

//==================================================
//	初期化
//==================================================
HRESULT CEnemyCountBG::Init()
{
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	m_pos = vec3(CProcess::SCREEN_WIDTH * 0.5f, CProcess::SCREEN_HEIGHT * 0.5f, 0.0f);
	m_size = vec3(BG_X, BG_Y, 0.0f);

	//テクスチャ作成
	CTexture* pTex = CManager::GetTexture();	//テクスチャ情報取得
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_ENMCNTBG)));	//設定

	return S_OK;
}

//==================================================
//	終了
//==================================================
void CEnemyCountBG::Uninit()
{
	CObject2D::Uninit();
}

//==================================================
//	更新処理
//==================================================
void CEnemyCountBG::Update()
{
	CObject2D::Update();
}

//==================================================
//	描画
//==================================================
void CEnemyCountBG::Draw()
{
	CObject2D::Draw();
}

//==================================================
//	生成処理
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
//	数値管理マネージャー
//==============================================================================================================================

//==================================================
//	コンストラクタ
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
//	デストラクタ
//==================================================
CENMCNTDisplayManager::~CENMCNTDisplayManager()
{
	Uninit();
}

//==================================================
//	初期化
//==================================================
HRESULT CENMCNTDisplayManager::Init()
{
	//背景が一番下になるように最初に生成
	m_pENMCNTBG = CEnemyCountBG::Create(m_basePos, vec3(200.0f, 80.0f, 0.0f));

	//数値は0初期化
	m_nCurrentValue = 0;

	//インスタンス更新
	UpdateNumbers();

	return S_OK;
}

//==================================================
//	終了処理
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
//	更新処理
//==================================================
void CENMCNTDisplayManager::Update()
{
	for (CNumber* number : m_numbers)
	{
		number->Update();
	}
}

//==================================================
//	描画処理
//==================================================
void CENMCNTDisplayManager::Draw()
{
	for (CNumber* number : m_numbers)
	{
		number->Draw();
	}
}

//==================================================
//	表示数値設定
//==================================================
void CENMCNTDisplayManager::SetValue(int value)
{
	if (m_nCurrentValue != value)
	{
		m_nCurrentValue = value;
		UpdateNumbers();	//数字の表示を更新
	}
}

//==================================================
//	表示数値マネージャー
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
//	数値変更に応じてインスタンス更新
//==================================================
void CENMCNTDisplayManager::UpdateNumbers()
{
	//現在の数値を文字列化して桁数を取得
	std::string valueStr = std::to_string(m_nCurrentValue);
	size_t digitCount = valueStr.size();
	size_t numbersSize = m_numbers.size();

	//桁数に合わせてCNumberインスタンスを調整
	if (numbersSize < digitCount)
	{
		//桁数が増えた場合に新たにCNumberインスタンス追加
		for (size_t i = numbersSize; i < digitCount; ++i)
		{
			D3DXVECTOR3 pos = m_basePos + BGCorrectPos;
			pos.x += i * m_digitSpacing;	//桁数の間隔を考慮した位置

			CNumber* number = CNumber::Create(0, pos);
			m_numbers.push_back(number);
		}
	}
	else if (numbersSize > digitCount)
	{
		size_t numbersSize = m_numbers.size();

		//桁数が減った場合、不要なインスタンスを非表示にする
		for (size_t i = digitCount; i < numbersSize; ++i)
		{
			m_numbers[i]->SetValue(-1);	//非表示にする
		}
	}

	//各桁の値を更新
	for (size_t i = 0; i < numbersSize; ++i)
	{
		int digitValue = valueStr[i] - '0';	//文字を数値に変換
		m_numbers[i]->SetValue(digitValue);

		//その他変更点
		if (m_nCurrentValue != 0)
		{
			if (m_nCurrentValue <= 5)
			{
				m_numbers[i]->SetColor(D3DCOLOR_XRGB(255, 0, 100));
			}
		}
	}
}
