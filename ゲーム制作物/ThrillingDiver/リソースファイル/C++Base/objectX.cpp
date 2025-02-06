//
//	オブジェクトXCPP
//
//
//

//
//	インクルード
//
#include "main.h"
#include "objectX.h"
#include "manager.h"
//
//	静的メンバ関数初期
//

//
//	コンストラクタ
//
CObjectX::CObjectX(int nPriority) :CObject(nPriority)
{
	m_pVtxBuff = nullptr;
	m_pTexture = nullptr;
	Init();
}

//
//	デストラクタ
//
CObjectX::~CObjectX()
{

}

//
//	初期設定
//
HRESULT CObjectX::Init()
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	//Xファイルの読み込み
	D3DXLoadMeshFromX(m_ModelFile,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);

	return S_OK;
}

//
//	終了処理
//
void CObjectX::Uninit()
{
	//メッシュの破棄
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}
	//マテリアルの破棄
	if (m_pBuffMat != nullptr)
	{
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}
}

//
//	更新処理
//
void CObjectX::Update()
{

}

//
//	描画処理
//
void CObjectX::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
 	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	//計算用マトリックス
	D3DMATERIAL9 matDef;	//現在のマテリアル保存用
	D3DXMATERIAL* pMat;		//マテリアルデータのポインタ
	D3DXMATRIX mtxParent;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//大きさを反映
	D3DXMatrixScaling(&mtxScale,
		m_scale.x,
		m_scale.y,
		m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y,
		m_rot.x,
		m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x,
		m_pos.y,
		m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックス
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_pTexture);

		//プレイヤー(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//==========================================================
//	ObjectX生成
//==========================================================
CObjectX* CObjectX::Create()
{
	CObjectX* pCObjectX = new CObjectX;

	pCObjectX->Init();

	return pCObjectX;
}

//==========================================================
//	テクスチャ設定
//==========================================================
void CObjectX::BindTexture(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTexture = pTex;
}
