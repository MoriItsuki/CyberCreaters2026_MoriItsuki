//
//	�ŋߓG�����\���pUI(dirmarker.h)
//
//
//
#pragma once

//
//	�C���N���[�h
//
#include "objectX.h"

//���N���X
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
	bool m_bDisp;	//�\�����邩�ǂ���
};