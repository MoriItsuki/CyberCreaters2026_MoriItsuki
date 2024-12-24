//
//	�X�^�[�g�����p�w�b�_
//
//
//
#pragma once

//
//	�C���N���[�h
//
#include "object2D.h"

#include <string>

//	���������Ԓl1���\���p�|���S���N���X
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
	int m_value;	//���l
	bool m_isVisible;	//�\����\��
	static const std::string FILE_NUMBER;
};

//	�v���C���[�m�F�p�A�C�R���|���S���N���X
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
	bool IsStartCan()const;	//�X�^�[�g�o���邩�擾
private:
	static const std::string FILE_STARTCONFIRM;
	bool m_isLBPressed;	//LB��������Ă��邩
	bool m_isRBPressed;	//RB��������Ă��邩
	D3DXCOLOR m_currentColor;	//�F
	float m_fLongPressTime;	//�������o�ߎ���
	bool m_bstart;			//�X�^�[�g�o���邩
	CPressTime* m_pPressTime;	//����������
};

//	�m�F��ʕ����\���p�N���X
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
	void SetNum(int num);	//���l�ݒ�
private:
	static const std::string FILE_STARTSPELL;
	int m_nNum;	//�������̉��Ԗڂ�
};

//	�X�^�[�g�����p��ʑS�̃|���S���N���X
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
	bool IsStart()const;	//�X�^�[�g�ł���ł��邩�擾
private:
	static const std::string FILE_STARTRESERVE;
	CStartSpell* m_pSpell;		//�����\���p�|���S��
	CStartConfirm* m_pConfirm;	//�A�C�R���|���S���|�C���^
	bool m_bstart;	//�X�^�[�g�ł��邩
};

