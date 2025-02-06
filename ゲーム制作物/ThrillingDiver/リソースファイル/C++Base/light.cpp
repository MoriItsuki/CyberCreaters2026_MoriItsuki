//==========================================================================
//	ライトCPP(light.cpp)
//
//
//==========================================================================

//===========================================
//	インクルード
//===========================================
#include "light.h"
#include "manager.h"

//===========================================
//	コンストラクタ
//===========================================
CLight::CLight()
{
	for (int nCntlight = 0; nCntlight < MAX_LIGHT; nCntlight++)
	{
		//ライトの情報をクリアする
		ZeroMemory(&m_aLight[nCntlight], sizeof(D3DLIGHT9));
	}
}

//===========================================
//	デストラクタ
//===========================================
CLight::~CLight()
{

}

//===========================================
//	初期設定
//===========================================
HRESULT CLight::Init()
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();
	D3DXVECTOR3 vecDir[MAX_LIGHT];		//設定用方向ベクトル

	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		//ライトの種類の設定
		m_aLight[nCnt].Type = D3DLIGHT_DIRECTIONAL;

		//ライトの拡散光
		m_aLight[nCnt].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		switch (nCnt)
		{
		case 0:
			vecDir[nCnt] = D3DXVECTOR3(0.4f, -0.8f, 0.2f);
			break;
		case 1:
			vecDir[nCnt] = D3DXVECTOR3(-0.4f, -0.8f, -0.2f);
			break;
		case 2:
			vecDir[nCnt] = D3DXVECTOR3(0.2f, -0.8f, 0.2f);
			break;
		case 3:
			vecDir[nCnt] = D3DXVECTOR3(-0.2f, -0.8f, -0.2f);
			break;
		default:
			//vecDir[nCnt] = D3DXVECTOR3(0.2f, 0.3f, 0.4f);
			break;
		}
		//ライトの方向を指定

		//ベクトルを正規化
		D3DXVec3Normalize(&vecDir[nCnt], &vecDir[nCnt]);
		m_aLight[nCnt].Direction = vecDir[nCnt];

		//ライトを設定
		pDevice->SetLight(nCnt, &m_aLight[nCnt]);

		//ライトの有効化
		pDevice->LightEnable(nCnt, TRUE);
	}
	return S_OK;
}

//===========================================
//	終了処理
//===========================================
void CLight::Uninit()
{

}

//===========================================
//	更新処理
//===========================================
void CLight::Update()
{

}