//========================================================================================================================
//	スタート準備用CPP(startreserve.cpp)
//
//
//========================================================================================================================

//
//	インクルード
//
#include "startreserve.h"
#include "process.h"
#include "texture.h"

//
//	定数
//
namespace
{
	using vec2 = D3DXVECTOR2;
	using vec3 = D3DXVECTOR3;
	
	constexpr int DIVISION_SPELL = 2;	//文字の分割数
	constexpr int DIVISION_NUMBER = 10;	//数字の分割数
	constexpr float PRESSSTART = 4.0f;
}

//
//	静的メンバ変数初期化
//
const std::string CStartReserve::FILE_STARTRESERVE = "data\\texture\\.png";
const std::string CStartConfirm::FILE_STARTCONFIRM = "data\\texture\\confirm.png";
const std::string CStartSpell::FILE_STARTSPELL = "data\\texture\\startspell.png";
const std::string CPressTime::FILE_NUMBER = "data\\texture\\numbers.png";

//========================================================================================================================
//	背景透過ポリゴン
//========================================================================================================================

//
//	コンストラクタ
//
CStartReserve::CStartReserve() :
	m_pConfirm(nullptr),
	m_pSpell(nullptr),
	m_bstart(false)
{

}

//
//	デストラクタ
//
CStartReserve::~CStartReserve()
{

}

//
//	初期化
//
HRESULT CStartReserve::Init()
{
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	//色＆α値設定
	SetColor(D3DCOLOR_RGBA(0, 0, 0, 150));

	//	確認アイコン初期化
	m_pConfirm = CStartConfirm::Create(m_pos + vec3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(200.0f, 200.0f, 0.0f));
	m_pSpell = CStartSpell::Create(m_pos + vec3(0.0f, 300.0f, 0.0f), D3DXVECTOR3(300.0f, 40.0f, 0.0f));

	//テクスチャ作成
	CTexture* pTex = CManager::GetTexture();	//テクスチャ情報取得
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_STARTRESERVE)));	//設定

	return S_OK;
}

//
//	終了処理
//
void CStartReserve::Uninit()
{
	m_pConfirm->Uninit();
	m_pSpell->Uninit();

	CObject2D::Uninit();
}

//
//	更新処理
//
void CStartReserve::Update()
{
	CObject2D::Update();

	if (m_pConfirm->IsLBPressed() && 
		m_pConfirm->IsRBPressed())
	{
		m_pSpell->SetNum(1);
	}
	else
	{
		m_pSpell->SetNum(0);
	}

	if (m_pConfirm->IsStartCan())
	{
		m_bstart = true;
	}
}

//
//	描画
//
void CStartReserve::Draw()
{
	CObject2D::Draw();
}

//
//	生成
//
CStartReserve* CStartReserve::Create()
{
	CStartReserve* pstartReserve = new CStartReserve();
	pstartReserve->Init();
	auto
		pos = D3DXVECTOR3(CProcess::SCREEN_WIDTH * 0.5f, CProcess::SCREEN_HEIGHT * 0.5, 0.0f),
		size = D3DXVECTOR3(CProcess::SCREEN_WIDTH * 0.5f, CProcess::SCREEN_HEIGHT * 0.5, 0.0f)
		;

	pstartReserve->SetPos(pos);
	pstartReserve->SetSize(size);

	return pstartReserve;
}

//
//	スタートできるか取得
//
bool CStartReserve::IsStart() const
{
	return m_bstart;
}

//========================================================================================================================
//	確認ポリゴン
//========================================================================================================================

//
//	コンストラクタ
//
CStartConfirm::CStartConfirm() :
	m_isLBPressed(false),
	m_isRBPressed(false),
	m_currentColor(D3DCOLOR_XRGB(255, 255, 255)),	//初期化は白
	m_fLongPressTime(PRESSSTART),
	m_bstart(false),
	m_pPressTime(nullptr)
{

}

//
//	デストラクタ
//
CStartConfirm::~CStartConfirm()
{

}

//
//	初期化
//
HRESULT CStartConfirm::Init()
{
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	m_pPressTime = CPressTime::Create(-1, m_pos + vec3(-140.0f, 290.0f, 0.0f), vec3(30.0f, 50.0f, 0.0f));

	//テクスチャ作成
	CTexture* pTex = CManager::GetTexture();	//テクスチャ情報取得
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_STARTCONFIRM)));	//設定

	return S_OK;
}

//
//	終了処理
//
void CStartConfirm::Uninit()
{
	m_pPressTime->Uninit();

	CObject2D::Uninit();
}

//
//	更新処理
//
void CStartConfirm::Update()
{
	CProcess* pProcess = CManager::GetProcess();

	//入力状態を取得
	CInputGamePad* pPad = CManager::GetGamepad();
	m_isLBPressed = pPad->GetPress(4);
	m_isRBPressed = pPad->GetPress(5);

	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	CObject2D::Update();

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//rhwの設定(固定)
	//頂点カラーの設定
	for (int nCnt = 0; nCnt < CProcess::MAX_VERTEX; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
	}

	auto white = D3DCOLOR_XRGB(255, 255, 255);

	//色の状態を更新
	if (m_isLBPressed && m_isRBPressed)
	{
		m_currentColor = D3DCOLOR_XRGB(255, 255, 0);	//黄色
		pVtx[0].col = m_currentColor;
		pVtx[1].col = m_currentColor;
		pVtx[2].col = m_currentColor;
		pVtx[3].col = m_currentColor;

		//	黄色状態の時は
		m_fLongPressTime -= pProcess->GetDeltaTime();
		m_pPressTime->SetValue((int)m_fLongPressTime);
	}
	else if (m_isLBPressed)
	{
		m_currentColor = D3DCOLOR_XRGB(255, 100, 0);	//桃色
		pVtx[0].col = m_currentColor;
		pVtx[1].col = m_currentColor;
		pVtx[2].col = white;
		pVtx[3].col = white;

		m_fLongPressTime = PRESSSTART;
		m_pPressTime->SetValue(-1);
	}
	else if (m_isRBPressed)
	{
		m_currentColor = D3DCOLOR_XRGB(100, 0, 255);	//紫色
		pVtx[0].col = white;
		pVtx[1].col = white;
		pVtx[2].col = m_currentColor;
		pVtx[3].col = m_currentColor;

		m_fLongPressTime = PRESSSTART;
		m_pPressTime->SetValue(-1);
	}
	else
	{
		m_currentColor = D3DCOLOR_XRGB(255, 255, 255);	//白色
		pVtx[0].col = white;
		pVtx[1].col = white;
		pVtx[2].col = white;
		pVtx[3].col = white;

		m_fLongPressTime = PRESSSTART;
		m_pPressTime->SetValue(-1);
	}

	if (m_fLongPressTime <= 0.0f)
	{
		m_bstart = true;
		m_fLongPressTime = PRESSSTART;
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//
//	描画処理
//
void CStartConfirm::Draw()
{
	CObject2D::Draw();
}

//
//	生成処理
//
CStartConfirm* CStartConfirm::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CStartConfirm* pStartCon = new CStartConfirm();
	pStartCon->Init();
	pStartCon->SetPos(pos);
	pStartCon->SetSize(size);

	return pStartCon;
}

//
//	LBフラグ
//
bool CStartConfirm::IsLBPressed() const
{
	return m_isLBPressed;
}

//
//	RBフラグ
//
bool CStartConfirm::IsRBPressed() const
{
	return m_isRBPressed;
}

//
//	スタートできるか取得
//
bool CStartConfirm::IsStartCan() const
{
	return m_bstart;
}

//========================================================================================================================
//	確認ポリゴン
//========================================================================================================================

//
//	コンストラクタ
//
CStartSpell::CStartSpell() :
	m_nNum(0)
{

}

//
//	デストラクタ
//
CStartSpell::~CStartSpell()
{

}

//
//	初期化
//
HRESULT CStartSpell::Init()
{
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}
	
	m_color = D3DCOLOR_XRGB(255, 0, 0);

	m_nNum = 0;

	//テクスチャ作成
	CTexture* pTex = CManager::GetTexture();	//テクスチャ情報取得
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_STARTSPELL)));	//設定

	return S_OK;
}

//
//	終了処理
//
void CStartSpell::Uninit()
{
	CObject2D::Uninit();
}

//
//	更新
//
void CStartSpell::Update()
{
	CObject2D::Update();

	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = vec3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[1].pos = vec3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = vec3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = vec3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	//	桁数に応じた分割サイズ
	float divY = 1.0f / DIVISION_SPELL;

	pVtx[0].tex = vec2(0.0f, divY * m_nNum);
	pVtx[1].tex = vec2(1.0f, divY * m_nNum);
	pVtx[2].tex = vec2(0.0f, divY * (m_nNum + 1));
	pVtx[3].tex = vec2(1.0f, divY * (m_nNum + 1));

	//rhwの設定(固定)	頂点カラーの設定
	for (int nCnt = 0; nCnt < CProcess::MAX_VERTEX; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
		pVtx[nCnt].col = m_color;
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//
//	描画
//
void CStartSpell::Draw()
{
	CObject2D::Draw();
}

//
//	生成
//
CStartSpell* CStartSpell::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CStartSpell* pSpell = new CStartSpell();
	pSpell->Init();
	pSpell->SetPos(pos);
	pSpell->SetSize(size);

	return pSpell;
}

//
//	分割数値設定
//
void CStartSpell::SetNum(int num)
{
	m_nNum = num;
}

//========================================================================================================================
//	長押し時間値1桁表示用ポリゴン
//========================================================================================================================

//
//	コンストラクタ
//
CPressTime::CPressTime() :
	m_value(-1),
	m_isVisible(false)
{

}

//
//	デストラクタ
//
CPressTime::~CPressTime()
{

}

//
//	初期化
//
HRESULT CPressTime::Init()
{
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	m_isVisible = true;

	//テクスチャ作成
	CTexture* pTex = CManager::GetTexture();	//テクスチャ情報取得
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_NUMBER)));	//設定

	return S_OK;
}

//
//	終了
//
void CPressTime::Uninit()
{
	CObject2D::Uninit();
}

//
//	更新
//
void CPressTime::Update()
{
	CObject2D::Update();

	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = vec3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[1].pos = vec3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = vec3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = vec3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	//	桁数に応じた分割サイズ
	float divX = 1.0f / DIVISION_NUMBER;
	
	pVtx[0].tex = vec2(divX * m_value, 0.0f);
	pVtx[1].tex = vec2(divX * (m_value + 1), 0.0f);
	pVtx[2].tex = vec2(divX * m_value, 0.5f);
	pVtx[3].tex = vec2(divX * (m_value + 1), 0.5f);

	//rhwの設定(固定)	頂点カラーの設定
	for (int nCnt = 0; nCnt < CProcess::MAX_VERTEX; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
		pVtx[nCnt].col = m_color;
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//
//	描画
//
void CPressTime::Draw()
{
	if (m_isVisible)
	{
		CObject2D::Draw();
	}
}

//
//	生成
//
CPressTime* CPressTime::Create(int value, D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CPressTime* pPressTime = new CPressTime();
	pPressTime->Init();
	pPressTime->SetValue(value);
	pPressTime->SetPos(pos);
	pPressTime->SetSize(size);

	return pPressTime;
}

//
//	数値設定
//
void CPressTime::SetValue(int value)
{
	if (value < -1 || value > 9)
	{
		return;
	}

	if (value == -1)
	{
		//特殊値 -1: 非表示にする
		m_isVisible = false;
	}
	else
	{
		//通常範囲
		m_isVisible = true;
		m_value = value;
	}
}
