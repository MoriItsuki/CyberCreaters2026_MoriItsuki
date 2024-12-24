//
//	“G”ro’Ç‰Á{İƒwƒbƒ_
//
//
//
#pragma once

//
//	ƒCƒ“ƒNƒ‹[ƒh
//
#include "objectX.h"
#include "manager.h"
#include <iostream>
#include <random>

//	“G”ro¶¬{İ”ŠÇ—ƒNƒ‰ƒX
class CEnmCreManager
{
private:
	static int m_EnmCreCnt;	//“G{İ‘”
	static int m_EnmCnt;	//“G‘”
	static LPD3DXFONT m_pFont;		//ƒtƒHƒ“ƒgİ’è
public:
	CEnmCreManager();
	~CEnmCreManager();
	static void IncEnmCnt() { ++m_EnmCreCnt; }	//“G{İ”‰ÁZ
	static void DecEnmCnt() { --m_EnmCreCnt; }	//“G{İ”Œ¸Z
	static int GetEnmCreCnt() { return m_EnmCreCnt; }	//“G{İ”æ“¾
	static int GetEnmCnt() { return m_EnmCnt; }			//“G”æ“¾
	static HRESULT Init();
	static void Uninit();
	static void Update();
	static void Draw();
	static void DrawEnmCreInfo();
};

//	“G”ro¶¬{İƒNƒ‰ƒX
class CEnmCreate :public CObjectX
{
public:
	CEnmCreate();
	~CEnmCreate()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CEnmCreate* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int remCre);
private:
	int m_nRemainCreCnt;	//c‚è‰½‘Ì“G‚ğ¶¬o—ˆ‚é‚©
	float m_fCreCoolTime;	//¶¬‘Ò‚¿ŠÔ
};