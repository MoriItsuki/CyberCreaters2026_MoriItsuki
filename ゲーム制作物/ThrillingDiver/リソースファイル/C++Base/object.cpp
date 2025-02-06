//=======================================================================
//	�I�u�W�F�N�gCPP(object.cpp)
//
//
//=======================================================================

//=====================================
//�C���N���[�h
//=====================================
#include "object.h"
#include "renderer.h"

//=====================================
//�ÓI�����o�ϐ�������
//=====================================
int CObject::m_nNumAll = 0;
CObject* CObject::m_apObject[NUM_PRIORITY][MAX_OBJECT] = {};

//=====================================
//	�R���X�g���N�^(�������g�̓o�^)
//=====================================
CObject::CObject(int nPriority) :m_nID(0), m_type(NONE), m_nPriority(3)
{
	m_nPriority = nPriority;

	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		if (m_apObject[m_nPriority][nCnt] == nullptr)
		{
			m_apObject[m_nPriority][nCnt] = this;	//�������g����
			m_nID = nCnt;				//�������g��ID����
			m_nNumAll++;				//�������J�E���g�A�b�v
			break;						//���[�v�𔲂���
		}
	}
}

//=====================================
//	�f�X�g���N�^
//=====================================
CObject::~CObject()
{

}

//=====================================
//	�������g�̊J��
//=====================================
void CObject::Release()
{
	int nID = m_nID;
	int nPriority = m_nPriority;
	if (m_apObject[nPriority][nID] != nullptr)
	{
		delete m_apObject[nPriority][nID];
		m_apObject[nPriority][nID] = nullptr;
		m_nNumAll--;				//�������J�E���g�_�E��
	}
}

//=====================================
//	�S�I�u�W�F�N�g�̊J��
//=====================================
void CObject::ReleaseAll()
{
	int nID;
	int nPriority;
	for (nPriority = 0; nPriority < NUM_PRIORITY; nPriority++)
	{
		for (nID = 0; nID < MAX_OBJECT; nID++)
		{
			if (m_apObject[nPriority][nID] != nullptr)
			{
				m_apObject[nPriority][nID]->Release();
			}
		}
	}
}

//=====================================
//	�S�I�u�W�F�N�g�̍X�V
//=====================================
void CObject::UpdateAll()
{
	int nID;
	int nPriority;
	for (nPriority = 0; nPriority < NUM_PRIORITY; nPriority++)
	{
		for (nID = 0; nID < MAX_OBJECT; nID++)
		{
			if (m_apObject[nPriority][nID] != nullptr)
			{
				m_apObject[nPriority][nID]->Update();
			}
		}
	}
}

//=====================================
//	�S�I�u�W�F�N�g�̕`��
//=====================================
void CObject::DrawAll()
{
	int nID;
	int nPriority;
	for (nPriority = 0; nPriority < NUM_PRIORITY; nPriority++)
	{
		for (nID = 0; nID < MAX_OBJECT; nID++)
		{
			if (m_apObject[nPriority][nID] != nullptr)
			{
				m_apObject[nPriority][nID]->Draw();
			}
		}
	}
}

//=====================================
//	�I�u�W�F�N�g�擾
//=====================================
CObject* CObject::GetObj(int nPri, int nIdx)
{
	return m_apObject[nPri][nIdx];
}

//=====================================
//	�^�C�v�擾
//=====================================
CObject::TYPE CObject::GetType()
{
	return m_type;
}

//=====================================
//	�^�C�v�ݒ�
//=====================================
void CObject::SetType(TYPE type)
{
	m_type = type;
}