//
//	�G���\���p
//
//
//
#pragma once

//
//	�C���N���[�h
//
#include "object2D.h"
#include <string>
#include <vector>

//	���l�\���|���S���N���X
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
	int m_digit;	//����
	int m_value;	//���l
	bool m_isVisible;	//�\����\��
	static const std::string FILE_NUMBER;
};

//	�G���\���p�w�i�N���X
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

//	�G���\���}�l�[�W���[�N���X
class CENMCNTDisplayManager
{
public:
	CENMCNTDisplayManager();
	~CENMCNTDisplayManager();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetValue(int value);	//�\������l���X�V
	static CENMCNTDisplayManager* Create(D3DXVECTOR3 pos, int value);
private:
	int m_nCurrentValue;				//���݂̕\���l
	std::vector<CNumber*> m_numbers;	//�����Ƃ̐����|���S���C���X�^���X�Ǘ�
	D3DXVECTOR3 m_basePos;				//�\���̊�ʒu
	float m_digitSpacing;				//�����̊Ԋu
	void UpdateNumbers();				//���l�̕ύX�ɉ����ăC���X�^���X���X�V
	CEnemyCountBG* m_pENMCNTBG;			//�w�i�|���S���|�C���^
};
