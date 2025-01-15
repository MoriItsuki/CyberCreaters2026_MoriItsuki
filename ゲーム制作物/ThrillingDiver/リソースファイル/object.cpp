//=======================================================================
//	オブジェクトCPP(object.cpp)
//
//
//=======================================================================

//=====================================
//インクルード
//=====================================
#include "object.h"
#include "renderer.h"

//=====================================
//静的メンバ変数初期化
//=====================================
int CObject::m_nNumAll = 0;
CObject* CObject::m_apObject[NUM_PRIORITY][MAX_OBJECT] = {};

//=====================================
//	コンストラクタ(自分自身の登録)
//=====================================
CObject::CObject(int nPriority) :m_nID(0), m_type(NONE), m_nPriority(3)
{
	m_nPriority = nPriority;

	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		if (m_apObject[m_nPriority][nCnt] == nullptr)
		{
			m_apObject[m_nPriority][nCnt] = this;	//自分自身を代入
			m_nID = nCnt;				//自分自身のIDを代入
			m_nNumAll++;				//総数をカウントアップ
			break;						//ループを抜ける
		}
	}
}

//=====================================
//	デストラクタ
//=====================================
CObject::~CObject()
{

}

//=====================================
//	自分自身の開放
//=====================================
void CObject::Release()
{
	int nID = m_nID;
	int nPriority = m_nPriority;
	if (m_apObject[nPriority][nID] != nullptr)
	{
		delete m_apObject[nPriority][nID];
		m_apObject[nPriority][nID] = nullptr;
		m_nNumAll--;				//総数をカウントダウン
	}
}

//=====================================
//	全オブジェクトの開放
//=====================================
void CObject::ReleaseAll()
{
	int nID;
	int nPriority;
	for (nPriority = 0; nPriority < NUM_PRIORITY; nPriority++)
	{
		for (nID = 0; nID < MAX_OBJECT; nID++)
		{
			if (m_apObject[nPriority][nID] != nullptr)
			{
				m_apObject[nPriority][nID]->Release();
			}
		}
	}
}

//=====================================
//	全オブジェクトの更新
//=====================================
void CObject::UpdateAll()
{
	int nID;
	int nPriority;
	for (nPriority = 0; nPriority < NUM_PRIORITY; nPriority++)
	{
		for (nID = 0; nID < MAX_OBJECT; nID++)
		{
			if (m_apObject[nPriority][nID] != nullptr)
			{
				m_apObject[nPriority][nID]->Update();
			}
		}
	}
}

//=====================================
//	全オブジェクトの描画
//=====================================
void CObject::DrawAll()
{
	int nID;
	int nPriority;
	for (nPriority = 0; nPriority < NUM_PRIORITY; nPriority++)
	{
		for (nID = 0; nID < MAX_OBJECT; nID++)
		{
			if (m_apObject[nPriority][nID] != nullptr)
			{
				m_apObject[nPriority][nID]->Draw();
			}
		}
	}
}

//=====================================
//	オブジェクト取得
//=====================================
CObject* CObject::GetObj(int nPri, int nIdx)
{
	return m_apObject[nPri][nIdx];
}

//=====================================
//	タイプ取得
//=====================================
CObject::TYPE CObject::GetType()
{
	return m_type;
}

//=====================================
//	タイプ設定
//=====================================
void CObject::SetType(TYPE type)
{
	m_type = type;
}