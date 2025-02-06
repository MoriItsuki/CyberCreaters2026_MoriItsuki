//======================================================================================
//	背景CPP(BG.cpp)
//
//
//======================================================================================

//==========================================================
//	インクルード
//==========================================================
#include "BG.h"
#include "manager.h"

//==========================================================
//	静的メンバ関数初期化
//==========================================================
const std::string CBG::FILE_BG[MAX] = { "data\\texture\\title1.png","data\\texture\\BG.png" ,"data\\texture\\result.png" };

//==========================================================
//	コンストラクタ
//==========================================================
CBG::CBG(int nPriority) :
	m_textureType(TYPE::TITLE), 
	CObject2D(nPriority)
{
	m_PosTexBGV = 0.0f;
	m_PosTexBGU = 0.0f;
	//Init();
}

//==========================================================
//	デストラクタ
//==========================================================
CBG::~CBG()
{

}

//==========================================================
//	初期設定(頂点バッファ生成)
//==========================================================
HRESULT CBG::Init()
{
	if (FAILED(CObject2D::Init()))	//頂点バッファ作成
	{
		return E_FAIL;
	}

	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	SetSize(D3DXVECTOR3(CProcess::SCREEN_WIDTH * 0.5f, CProcess::SCREEN_HEIGHT * 0.5f, 0.0f));

	return S_OK;
}

//==========================================================
//	終了処理(頂点バッファの破棄)
//==========================================================
void CBG::Uninit()
{
	CObject2D::Uninit();
}

//==========================================================
//	更新処理(頂点情報の更新)
//==========================================================
void CBG::Update()
{
	CObject2D::Update();

	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	//m_PosTexBGV += 0.001f;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(m_PosTexBGV, m_PosTexBGU);
	pVtx[1].tex = D3DXVECTOR2(m_PosTexBGV + 1.0f, m_PosTexBGU);
	pVtx[2].tex = D3DXVECTOR2(m_PosTexBGV, m_PosTexBGU + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_PosTexBGV + 1.0f, m_PosTexBGU + 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//==========================================================
//	描画処理(ポリゴン描画)
//==========================================================
void CBG::Draw()
{
	CObject2D::Draw();
}

//==========================================================
//	BG生成
//==========================================================
CBG* CBG::Create(TYPE type)
{
	CBG* pCBG = new CBG;

	pCBG->Init();

	CTexture* pTex = CManager::GetTexture();	//テクスチャ情報取得

	pCBG->BindTexture(pTex->GetAddress(pTex->Regist(FILE_BG[type])));	//設定

	return pCBG;
}