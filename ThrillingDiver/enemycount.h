//
//	敵数表示用
//
//
//
#pragma once

//
//	インクルード
//
#include "object2D.h"
#include <string>
#include <vector>

//	数値表示ポリゴンクラス
class CNumber :public CObject2D
{
public:
	CNumber();
	~CNumber()override;

	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CNumber* Create(int value, D3DXVECTOR3 pos);
	void SetValue(int value);
private:
	int m_digit;	//桁数
	int m_value;	//数値
	bool m_isVisible;	//表示非表示
	static const std::string FILE_NUMBER;
};

//	敵数表示用背景クラス
class CEnemyCountBG :public CObject2D
{
public:
	CEnemyCountBG();
	~CEnemyCountBG()override;

	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CEnemyCountBG* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
private:
	static const std::string FILE_ENMCNTBG;
};

//	敵数表示マネージャークラス
class CENMCNTDisplayManager
{
public:
	CENMCNTDisplayManager();
	~CENMCNTDisplayManager();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetValue(int value);	//表示する値を更新
	static CENMCNTDisplayManager* Create(D3DXVECTOR3 pos, int value);
private:
	int m_nCurrentValue;				//現在の表示値
	std::vector<CNumber*> m_numbers;	//桁ごとの数字ポリゴンインスタンス管理
	D3DXVECTOR3 m_basePos;				//表示の基準位置
	float m_digitSpacing;				//桁数の間隔
	void UpdateNumbers();				//数値の変更に応じてインスタンスを更新
	CEnemyCountBG* m_pENMCNTBG;			//背景ポリゴンポインタ
};
