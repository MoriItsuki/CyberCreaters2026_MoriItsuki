//
//	テクスチャ一括管理cpp
//
//
//

//
//	インクルード
//
#include "texture.h"

//
//	静的メンバ関数初期化
//
int CTexture::m_nNumTexAll = 0;		//テクスチャ数初期化

//
//	コンストラクタ
//
CTexture::CTexture():
	m_TexName()
{
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		m_aTexture[nCnt] = nullptr;
		m_nNumTexAll = 0;
	}
}

//
//	デストラクタ
//
CTexture::~CTexture()
{

}

//
//	テクスチャ破棄
//
void CTexture::Unload()
{
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		if (m_aTexture[nCnt] != nullptr)
		{
			m_aTexture[nCnt]->Release();
			m_aTexture[nCnt] = nullptr;
		}
	}
	m_nNumTexAll = 0;
}

//
//	テクスチャ読込
//
void CTexture::Init()
{

}

//
//	テクスチャ登録
//
int CTexture::Regist(std::string pTexName)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	int nIdx = 0;
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		if (m_aTexture[nCnt] == nullptr)
		{//	登録されていない
			D3DXCreateTextureFromFile(pDevice,
				pTexName.c_str(),
				&m_aTexture[nCnt]);

			//新規テクスチャの作成
			m_TexName[nCnt] = pTexName;
			nIdx = nCnt;	//ID設定
			m_nNumTexAll++;	//テクスチャ数加算
			break;
		}
		else if (m_TexName[nCnt] == pTexName)
		{//	生成済みのテクスチャと一致
			nIdx = nCnt;
			break;
		}
	}
	return nIdx;	//IDを返す
}

//
//	テクスチャ取得
//
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	return m_aTexture[nIdx];
}
