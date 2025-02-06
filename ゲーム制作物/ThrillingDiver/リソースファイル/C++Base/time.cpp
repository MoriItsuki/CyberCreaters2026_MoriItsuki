//
//	時間表示CPP
//
//
//

//
//	インクルード
//
#include "time.h"
#include "process.h"

using namespace std;

//
//	定数
//
namespace
{
	using vec3 = D3DXVECTOR3;
	using vec2 = D3DXVECTOR2;
	constexpr int DIVISION = 10;	//数字テクスチャの分割数
	constexpr float TIME_X = 60.0f;
	constexpr float TIME_Y = 40.0f;
	constexpr float TIME_XL = 120.0f;
	constexpr float TIME_YL = 080.0f;
}

//
//	静的メンバ変数初期化
//
const string CTimer::FILE_TIME = "data\\texture\\numbers.png";

//
//	コンストラクタ
//
CTimer::CTimer() :
	m_digit(-1),
	m_bUse(true)
{

}

//
//	デストラクタ
//
CTimer::~CTimer()
{

}

//
//	初期化
//
HRESULT CTimer::Init()
{
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	m_pos = vec3(CProcess::SCREEN_WIDTH * 0.5, CProcess::SCREEN_HEIGHT * 0.5, 0.0f);
	m_size = vec3(TIME_X, TIME_Y, 0.0f);

	//テクスチャ作成
	CTexture* pTex = CManager::GetTexture();	//テクスチャ情報取得
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_TIME)));	//設定

	return S_OK;
}

//
//	終了処理
//
void CTimer::Uninit()
{
	CObject2D::Uninit();
}

//
//	更新処理
//
void CTimer::Update()
{
	if (m_digit == -1)
	{
		m_bUse = false;
	}

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

	if (m_digit != 11)
	{
		//数値ごとのテクスチャ座標の設定
		float divXIn = m_digit * divX;
		float divXOut = (m_digit + 1) * divX;

		pVtx[0].tex = vec2(divXIn, 0.0f);
		pVtx[1].tex = vec2(divXOut, 0.0f);
		pVtx[2].tex = vec2(divXIn, 0.5f);
		pVtx[3].tex = vec2(divXOut, 0.5f);
	}
	else
	{//
		pVtx[0].tex = vec2(0.0f, 0.0f);
		pVtx[1].tex = vec2(0.0f, 0.0f);
		pVtx[2].tex = vec2(0.0f, 0.5f);
		pVtx[3].tex = vec2(0.0f, 0.5f);
	}

	//rhwの設定(固定)	頂点カラーの設定
	for (int nCnt = 0; nCnt < CProcess::MAX_VERTEX; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
		pVtx[nCnt].col = D3DCOLOR_RGBA(0, 0, 0, 255);
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//
//	描画処理
//
void CTimer::Draw()
{
	CRenderer* pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pRendererDevice = pRenderer->GetDevice();	//デバイスのポインタ

	//頂点バッファをデータストリームに設定
	pRendererDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pRendererDevice->SetFVF(CProcess::FVF_VERTEX_2D);

	if (m_bUse)
	{
		//テクスチャの設定
		pRendererDevice->SetTexture(0, m_pTexture);

		pRendererDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);
	}
}

//
//	表示数値設定
//
void CTimer::SetDigit(int digit)
{
	m_digit = digit;

	if (digit != 11)
	{
		if (digit < 0 || digit >= DIVISION)
		{
			m_digit = -1;
		}
	}
}

//
//	基準位置設定
//
void CTimer::SetPos(const D3DXVECTOR3& pos)
{
	m_pos = pos;
}

//
//	生成処理
//
CTimer* CTimer::Create(D3DXVECTOR3 pos)
{
	CTimer* pTimer = new CTimer;
	pTimer->Init();
	pTimer->m_pos = pos;

	return pTimer;
}


//
//	使用状況設定
//
void CTimer::SetUse(bool buse)
{
	m_bUse = buse;
}