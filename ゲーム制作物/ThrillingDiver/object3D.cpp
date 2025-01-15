//
//	オブジェクト３D
//
//
//

//
//	インクルード
//
#include "main.h"
#include "object3D.h"
#include "manager.h"

//
//	コンストラクタ
//
CObject3D::CObject3D()
{
	m_pVtxBuff = nullptr;
	m_pTexture = nullptr;
	Init();

}

//
//	デストラクタ
//
CObject3D::~CObject3D()
{

}

//
//	初期設定
//
HRESULT CObject3D::Init()
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * CProcess::MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		CProcess::FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D* pVtx;	//頂点情報へのポインタ

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, 0.0f, +m_size.z);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, 0.0f, +m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, 0.0f, -m_size.z);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//
//	終了処理
//
void CObject3D::Uninit()
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

}

//==========================================================
//	更新処理(頂点情報の更新)
//==========================================================
void CObject3D::Update()
{
	VERTEX_3D* pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, 0.0f, +m_size.z);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, 0.0f, +m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, 0.0f, -m_size.z);

	////頂点座標の設定
	//pVtx[0].pos.x = m_pos.x + sinf(m_rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
	//pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
	//pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	//pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	//pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * m_fLength;
	//pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * m_fLength;
	//pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * m_fLength;
	//pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * m_fLength;

	////テクスチャ座標の設定
	//pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//テクスチャ座標の設定
	//pVtx[0].tex = D3DXVECTOR2((nCntAnim - 1) * KOMAWIDTH, 0.0f);
	//pVtx[1].tex = D3DXVECTOR2(nCntAnim * KOMAWIDTH, 0.0f);
	//pVtx[2].tex = D3DXVECTOR2((nCntAnim - 1) * KOMAWIDTH, 1.0f);
	//pVtx[3].tex = D3DXVECTOR2(nCntAnim * KOMAWIDTH, 1.0f);

	//頂点カラーの設定
	/*for (int nCnt = 0; nCnt < CProcess::MAX_VERTEX; nCnt++)
	{
		pVtx[nCnt].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}*/

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//==========================================================
//	描画処理(ポリゴン描画)
//==========================================================
void CObject3D::Draw()
{
	CRenderer* pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pRendererDevice = pRenderer->GetDevice();	//デバイスのポインタ

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y,
		m_rot.x,
		m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックス
	pRendererDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pRendererDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pRendererDevice->SetFVF(CProcess::FVF_VERTEX_3D);

	//テクスチャの設定
	pRendererDevice->SetTexture(0, m_pTexture);

	pRendererDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//==========================================================
//	Object3D生成
//==========================================================
CObject3D* CObject3D::Create()
{
	CObject3D* pCObject2D = new CObject3D;

	pCObject2D->Init();

	return pCObject2D;
}

//==========================================================
//	テクスチャ設定
//==========================================================
void CObject3D::BindTexture(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTexture = pTex;
}

//==========================================================
//	サイズ変更
//==========================================================
void CObject3D::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}
