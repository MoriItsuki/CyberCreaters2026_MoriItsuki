//============================================================================
//	オブジェクト2DCPP(object2D.cpp)
//
//
//============================================================================
//==========================================================
//インクルード
//==========================================================
#include "main.h"
#include "object2D.h"
#include "manager.h"

//==========================================================
//	マクロ定義
//==========================================================

//==========================================================
//	グローバル変数宣言
//==========================================================

//==========================================================
//	コンストラクタ
//==========================================================
CObject2D::CObject2D(int nPriority) :
	m_fAngle(0.0f),
	m_fLength(0.0f),
	CObject(nPriority),
	m_pVtxBuff(nullptr),
	m_pTexture(nullptr),
	m_color(D3DCOLOR_RGBA(255, 255, 255, 255))
{
	//Init();
}

//==========================================================
//	デストラクタ
//==========================================================
CObject2D::~CObject2D()
{

}

//==========================================================
//	初期設定(頂点バッファ生成)
//==========================================================
HRESULT CObject2D::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイスのポインタ

	//頂点バッファの生成
	HRESULT hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * CManager::GetProcess()->MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		CManager::GetProcess()->FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,	
		NULL);

	m_pos = D3DXVECTOR3(CProcess::SCREEN_WIDTH * 0.5, CProcess::SCREEN_HEIGHT * 0.5, 0.0f);

	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

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
		pVtx[nCnt].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==========================================================
//	終了処理(頂点バッファの破棄)
//==========================================================
void CObject2D::Uninit()
{
	CObject::Release();
}

//==========================================================
//	更新処理(頂点情報の更新)
//==========================================================
void CObject2D::Update()
{
	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//対角線の長さを算出
	//m_fLength = sqrtf(MACHINESIZE * MACHINESIZE + MACHINESIZE * MACHINESIZE) * 0.5f;
	//対角線の角度を算出
	//m_fAngle = atan2f(MACHINESIZE, MACHINESIZE);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	////頂点座標の設定
	//pVtx[0].pos.x = m_pos.x + sinf(m_rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
	//pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
	//pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	//pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	//pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * m_fLength;
	//pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * m_fLength;
	//pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * m_fLength;
	//pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * m_fLength;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//テクスチャ座標の設定
	//pVtx[0].tex = D3DXVECTOR2((nCntAnim - 1) * KOMAWIDTH, 0.0f);
	//pVtx[1].tex = D3DXVECTOR2(nCntAnim * KOMAWIDTH, 0.0f);
	//pVtx[2].tex = D3DXVECTOR2((nCntAnim - 1) * KOMAWIDTH, 1.0f);
	//pVtx[3].tex = D3DXVECTOR2(nCntAnim * KOMAWIDTH, 1.0f);

	//rhwの設定(固定)
	//頂点カラーの設定
	for (int nCnt = 0; nCnt < CProcess::MAX_VERTEX; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
		pVtx[nCnt].col = m_color;
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//==========================================================
//	描画処理(ポリゴン描画)
//==========================================================
void CObject2D::Draw()
{
	CRenderer* pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pRendererDevice = pRenderer->GetDevice();	//デバイスのポインタ

	//頂点バッファをデータストリームに設定
	pRendererDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pRendererDevice->SetFVF(CProcess::FVF_VERTEX_2D);

	//テクスチャの設定(nullptrなら無効化)
	pRendererDevice->SetTexture(0, m_pTexture);

	pRendererDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		2);
}

//==========================================================
//	Object2D生成
//==========================================================
CObject2D* CObject2D::Create()
{
	CObject2D* pCobject2D = new CObject2D;

	pCobject2D->Init();

	return pCobject2D;
}

//==========================================================
//	テクスチャ設定
//==========================================================
void CObject2D::BindTexture(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTexture = pTex;
}

//==========================================================
//	サイズセット
//==========================================================
void CObject2D::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

//==========================================================
//	サイズセット
//==========================================================
void CObject2D::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==========================================================
//	サイズセット
//==========================================================
void CObject2D::SetColor(const D3DCOLOR color)
{
	m_color = color;
}
