//
//	�G�w�b�_(enemy.h)
//
//
//
#ifndef _ENEMY_H_
#define _ENEMY_H_

//
//	�C���N���[�h
//
#include "objectX.h"
#include <vector>
#include <iostream>
#include <memory>
#include <map>

using namespace std;

namespace
{
	using vec3 = D3DXVECTOR3;
}

//�G�l�~�[���Ǘ��N���X
class CEnmManager
{
private:
	static int m_EnmCnt;	//�G����
public:
	static void IncEnmCnt() { ++m_EnmCnt; };	//�G�����Z
	static void DecEnmCnt() { --m_EnmCnt; };	//�G�����Z
	static int GetEnmCnt() { return m_EnmCnt; };	//�G���擾
	static 	map<string, vector<vec3>> m_targetLists;	//�^�[�Q�b�g�n�_���X�g
	static vector<string> m_targetListnameLists;		//�^�[�Q�b�g�n�_���X�g�̖��O���X�g
	static void SetTargetLists(const map<string, vector<vec3>>& newtargetLists);
	static const vector<vec3>& GetTargetList(const string& listname);
};

//�O���錾
class CEnemy;

//�G�l�~�[��Ԃ̃X�[�p�[�N���X
class EnemyState
{
public:
	virtual ~EnemyState() = default;
	virtual void Addupdate(CEnemy* pEnemy) = 0;	//��Ԃ��Ƃ̒ǉ��������s���֐�
};

//	��ړ����
class NormalState :public EnemyState
{
public:
	~NormalState()override {};
	void Addupdate(CEnemy* enemy)override;
};

//	��΂�����
class KnockbackState :public EnemyState
{
public:
	~KnockbackState()override {};
	void Addupdate(CEnemy* enemy)override;
};

//	�G�l�~�[�Ǘ��N���X
class CEnemy :public CObjectX
{
public:
	typedef enum
	{
		NONE = 0,
		MAX
	}ENMTYPE;
	
	CEnemy(std::vector<D3DXVECTOR3> value);
	~CEnemy()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CEnemy* Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, std::vector<D3DXVECTOR3> movePos);
	D3DXVECTOR3& GetMove() { return m_move; }	//�ړ��l�擾
	void DrawEnmInfo();
	void SetState(EnemyState* pNewstate);	//�G�̏�Ԑݒ�
	void BlowOff();	//������ԓ���

	static LPDIRECT3DTEXTURE9 GetTextures(const TYPE& inType);
private:
	D3DXVECTOR3 m_Oldpos;	//�L�^�p�ʒu
	D3DXVECTOR3 m_move;		//�ړ���
	bool m_bLand;			//���n�t���O
	float m_moveValue;		//�ړ��l
	ENMTYPE m_EnmType;		//�G��
	float m_diffPL;			//�v���C���[�Ƃ̋���
	D3DXVECTOR3 m_directionPL;	//�v���C���[�Ƃ̃x�N�g��
	bool m_InitPos;			//�X�V�ɓ���Ƃ��ɂ��̈ʒu����1�ԋ߂��Ƃ�����X�^�[�g�ɂ������ǂ���
	float m_blowTime;		//���ł��������܂ł̎���
	D3DXVECTOR3 m_BlowDir;	//���ł�������
	std::vector<D3DXVECTOR3> m_movePnt;	//�ړ���̈ʒu	���I�z��(�x�N�^�[)��ێ�
	int m_movePntIdx = 0;	//�ړ���̃C���f�b�N�X	'0' ������
	static const std::string FILE_ENEMY;
	static LPDIRECT3DTEXTURE9 m_textures[ENMTYPE::MAX];	//�^�C�v�̕������e�N�X�`�������
	EnemyState* m_currentState;	//�G�̃G�l�~�[�̏��

	LPD3DXFONT m_pFont;
};

#endif // !_ENEMY_H_
