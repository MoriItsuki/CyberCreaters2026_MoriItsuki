//
//	��Q���Ǘ��w�b�_
//
//
//
#pragma once

//
//	�C���N���[�h
//
#include "objectX.h"
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <map>

using namespace std;

//
//	�萔
//
namespace
{
	using vec3 = D3DXVECTOR3;
}


//��Q���Ǘ��N���X
class CObsManager
{
private:
	static int m_ObsCnt;	//��Q������
public:
	static void IncCnt() { ++m_ObsCnt; };	//�G�����Z
	static void DecCnt() { --m_ObsCnt; };	//�G�����Z
	static int GetCnt() { return m_ObsCnt; };	//�G���擾
	static 	map<string, vector<vec3>> m_targetLists;	//�^�[�Q�b�g�n�_���X�g
	static vector<string> m_targetListnameLists;		//�^�[�Q�b�g�n�_���X�g�̖��O���X�g
	static void SetTargetLists(const map<string, vector<vec3>>& newtargetLists);
	static const vector<vec3>& GetTargetList(const string& listname);
};

//
//	�O���錾
//
class CObstacle;

//��Q���̎�ނ̃X�[�p�[
class ObsType
{
public:
	virtual ~ObsType() = default;
	virtual void AddUpdate(CObstacle* pObs) = 0;	//��ނ��Ƃ̍X�V
};

//	�m�[�}���N���X
class NomalType :public ObsType
{
public:
	~NomalType()override {};
	void AddUpdate(CObstacle* pObs)override;
};

//	�����̂���N���X
class MoveingType :public ObsType
{
public:
	~MoveingType()override {};
	void AddUpdate(CObstacle* pObs)override;
};

//��Q���N���X
class CObstacle :public CObjectX
{
public:
	CObstacle();
	~CObstacle()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void Moveing();			//�����ݒ�
	static CObstacle* Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale);	//�m�[�}���^�C�v�Œ�
	static CObstacle* Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, const std::vector<vec3>& targetList, float speed, int moveIdx = 0);	//����ȊO�̃^�C�v�̎��̓^�C�v�Ɠ����E�����ݒ�
	void SetState(ObsType* obstype);	//�G�̏�Ԑݒ�
	bool IsColliding(const vec3& pos1, const vec3& size1, const vec3& pos2, const vec3& size2);
private:
	ObsType* m_Type;				//��Q���̎��
	D3DXVECTOR3 m_BlowDir;			//���˕���
	D3DXVECTOR3 m_Oldpos;			//�ߋ��ʒu
	float m_diffPL;					//�v���C���[�Ƃ̋���
	D3DXVECTOR3 m_RefPos;			//���Ԃœ������߂̊�_
	std::vector<vec3> m_movePnt;	//�ړ���̈ʒu���X�g	
	int m_movePntIdx;				//�ړ���̃C���f�b�N�X
	float m_speed;					//�ړ��X�s�[�h
	static const std::string FILE_OBSTACLE;
};