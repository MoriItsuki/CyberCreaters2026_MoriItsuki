//
//	���ԕ\���w�b�_
//
//
//
#pragma once

using namespace std;

//
//	�C���N���[�h
//
#include "main.h"
#include "texture.h"
//#include "timemanager.h"
#include "object2D.h"

#include <iostream>

//�^�C���\���N���X
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
	void SetDigit(int digit);	//�\�����l�ݒ�
	void SetPos(const D3DXVECTOR3& pos);	//��ʒu�ݒ�
	void SetUse(bool buse);
	static CTimer* Create(D3DXVECTOR3 pos);	//����
private:
	int m_digit;			//���݂̕\������(-1:��\��)
	bool m_bUse;			//�\��
	static const std::string FILE_TIME;
};