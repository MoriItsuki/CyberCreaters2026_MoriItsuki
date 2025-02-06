//
//	スタート準備用ヘッダ
//
//
//
#pragma once

//
//	インクルード
//
#include "object2D.h"

#include <string>

//	長押し時間値1桁表示用ポリゴンクラス
class CPressTime :public CObject2D
{
public:
	CPressTime();
	~CPressTime()override;

	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CPressTime* Create(int value, D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void SetValue(int value);
private:
	int m_value;	//数値
	bool m_isVisible;	//表示非表示
	static const std::string FILE_NUMBER;
};

//	プレイヤー確認用アイコンポリゴンクラス
class CStartConfirm :public CObject2D
{
public:
	CStartConfirm();
	~CStartConfirm();

	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CStartConfirm* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	bool IsLBPressed()const;
	bool IsRBPressed()const;
	bool IsStartCan()const;	//スタート出来るか取得
private:
	static const std::string FILE_STARTCONFIRM;
	bool m_isLBPressed;	//LBが押されているか
	bool m_isRBPressed;	//RBが押されているか
	D3DXCOLOR m_currentColor;	//色
	float m_fLongPressTime;	//長押し経過時間
	bool m_bstart;			//スタート出来るか
	CPressTime* m_pPressTime;	//長押し時間
};

//	確認画面文字表示用クラス
class CStartSpell :public CObject2D
{
public:
	CStartSpell();
	~CStartSpell();

	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CStartSpell* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void SetNum(int num);	//数値設定
private:
	static const std::string FILE_STARTSPELL;
	int m_nNum;	//分割数の何番目か
};

//	スタート準備用画面全体ポリゴンクラス
class CStartReserve :public CObject2D
{
public:
	CStartReserve();
	~CStartReserve();

	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CStartReserve* Create();
	bool IsStart()const;	//スタートできるできるか取得
private:
	static const std::string FILE_STARTRESERVE;
	CStartSpell* m_pSpell;		//文字表示用ポリゴン
	CStartConfirm* m_pConfirm;	//アイコンポリゴンポインタ
	bool m_bstart;	//スタートできるか
};

