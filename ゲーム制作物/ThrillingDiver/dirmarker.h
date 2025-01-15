//
//	最近敵方向表示用UI(dirmarker.h)
//
//
//
#pragma once

//
//	インクルード
//
#include "objectX.h"

//矢印クラス
class CDIRMARKER :public CObjectX
{
public:
	CDIRMARKER();
	~CDIRMARKER()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CDIRMARKER* Create(D3DXVECTOR3 pos);
private:
	bool m_bDisp;	//表示するかどうか
};