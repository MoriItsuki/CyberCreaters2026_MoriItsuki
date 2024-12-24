//
//	�G�r�o�ǉ��{�݃w�b�_
//
//
//
#pragma once

//
//	�C���N���[�h
//
#include "objectX.h"
#include "manager.h"
#include <iostream>
#include <random>

//	�G�r�o�����{�ݐ��Ǘ��N���X
class CEnmCreManager
{
private:
	static int m_EnmCreCnt;	//�G�{�ݑ���
	static int m_EnmCnt;	//�G����
	static LPD3DXFONT m_pFont;		//�t�H���g�ݒ�
public:
	CEnmCreManager();
	~CEnmCreManager();
	static void IncEnmCnt() { ++m_EnmCreCnt; }	//�G�{�ݐ����Z
	static void DecEnmCnt() { --m_EnmCreCnt; }	//�G�{�ݐ����Z
	static int GetEnmCreCnt() { return m_EnmCreCnt; }	//�G�{�ݐ��擾
	static int GetEnmCnt() { return m_EnmCnt; }			//�G���擾
	static HRESULT Init();
	static void Uninit();
	static void Update();
	static void Draw();
	static void DrawEnmCreInfo();
};

//	�G�r�o�����{�݃N���X
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
	int m_nRemainCreCnt;	//�c�艽�̓G�𐶐��o���邩
	float m_fCreCoolTime;	//�����҂�����
};