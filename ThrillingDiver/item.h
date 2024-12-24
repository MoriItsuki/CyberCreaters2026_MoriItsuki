//
//	アイテムヘッダ
//
//
//
#pragma once

//
//	インクルード
//
#include "objectX.h"

//アイテムクラス
class CItem :public CObjectX
{
public:
	CItem();
	~CItem();
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CItem* Create(D3DXVECTOR3 pos);
	void OutOn();	//消える予定にする
private:
	bool m_bOut;	//消えるかどうか
	static const std::string FILE_ITEM;
};