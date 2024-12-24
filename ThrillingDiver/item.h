//
//	�A�C�e���w�b�_
//
//
//
#pragma once

//
//	�C���N���[�h
//
#include "objectX.h"

//�A�C�e���N���X
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
	void OutOn();	//������\��ɂ���
private:
	bool m_bOut;	//�����邩�ǂ���
	static const std::string FILE_ITEM;
};