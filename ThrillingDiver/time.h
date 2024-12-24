//
//	時間表示ヘッダ
//
//
//
#pragma once

using namespace std;

//
//	インクルード
//
#include "main.h"
#include "texture.h"
//#include "timemanager.h"
#include "object2D.h"

#include <iostream>

//タイム表示クラス
class CTimer :public CObject2D
{
public:
	enum class TYPE
	{
		NORMAL,
		MAX
	};
	CTimer();
	~CTimer()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void SetDigit(int digit);	//表示数値設定
	void SetPos(const D3DXVECTOR3& pos);	//基準位置設定
	void SetUse(bool buse);
	static CTimer* Create(D3DXVECTOR3 pos);	//生成
private:
	int m_digit;			//現在の表示数字(-1:非表示)
	bool m_bUse;			//表示
	static const std::string FILE_TIME;
};