//
//	�v���C���[�w�b�_(player.h)
//
//
//
#ifndef _PLAYER_H_
#define _PLAYER_H_

//==================================
//	�C���N���[�h
//==================================
#include "objectX.h"
#include "camera.h"
#include "enemy.h"
#include "dashgauge.h"

#include <string>

//
//	�萔
//
namespace
{
	using vec3 = D3DXVECTOR3;
	using vec2 = D3DXVECTOR2;
}

//�v���C���[�Ǘ��N���X
class CPlayer :public CObjectX
{
public:
	//�����̏��
	typedef enum
	{
		NONE = 0,
		DEATH,
		MAX
	}STATE;
	//�ړ��̏��
	typedef enum
	{
		STOP = 0,	//��~
		FORWARD,	//�O�i
		BACK,		//��i
		DASH,		//�_�b�V��
	}MOVESTATE;	
	CPlayer();
	~CPlayer()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	//player�̐���
	void SetPos(D3DXVECTOR3 pos);
	void SetMove(D3DXVECTOR3 move);
	void SetReflec();		//��Q������
	void SetState(MOVESTATE state);		//�v���C���[��Ԑݒ�
	D3DXVECTOR3 GetPos();
	D3DXVECTOR3 GetVecFront();
	int GetState();
	CEnemy* GetNearestEnm()const;	//�Ŋ��̓G�擾
	D3DXVECTOR3 GetNearestEnmPos()const;	//�Ŋ��̓G�ʒu�擾
	int GetDashCnt()const;	//�c��_�b�V���񐔎擾
	void DrawPLInfo();
	bool IsColliding(const vec3& pos1, const vec3& size1, const vec3& pos2, const vec3& size2);
private:
	int m_nDashCnt;				//�_�b�V���ł����
	int m_nItemCnt;				//�����Ă���A�C�e���̐�
	D3DXVECTOR3 m_rotgo;		//�ړI�̊p�x
	D3DXVECTOR3 m_Oldpos;		//�L�^�p�ʒu
	float m_fFriction;			//���C�W��
	float m_moveValue;			//�ړ��l
	int m_nState;				//�v���C���[�̏��
	MOVESTATE m_MoveState;		//�ړ��̏��
	D3DXVECTOR3 m_vecFront;		//�v���C���[�̑O�����̃x�N�g��
	bool m_bStop;				//�~�܂��Ă��邩
	bool m_bDash;				//�_�b�V�����Ă��邩
	CEnemy* m_pNearestEnemy = nullptr;	//�ł��߂��G�|�C���^
	D3DXVECTOR3 m_NearestEnmPos;	//�Ŋ��̓G�ʒu
	CDashgauge* m_pDashGauge;		//�_�b�V����񎋊o���Q�[�W

	static const std::string FILE_PLAYER;
	LPD3DXFONT m_pFont;
};

#endif // !_PLAYER_H_

