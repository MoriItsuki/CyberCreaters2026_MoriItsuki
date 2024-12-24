//
//	ライトヘッダ(light.h)
//
//
//
#ifndef _LIGHT_H_
#define _LIGHT_H_

//インクルード
#include "main.h"

//マクロ定義
#define MAX_LIGHT	(4)	//ライトの最大数

//ライト管理クラス
class CLight
{
public:
	CLight();
	~CLight();
	HRESULT Init();
	void Uninit();
	void Update();
private:
	D3DLIGHT9 m_aLight[MAX_LIGHT];	//ライト構造体
};

#endif // !_LIGHT_H_
