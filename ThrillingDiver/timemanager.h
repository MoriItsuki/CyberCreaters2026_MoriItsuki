//
//	���ԊǗ��w�b�_
//
//
//
#pragma once

//
//	�C���N���[�h
//
#include "main.h"
#include <iostream>
#include <memory>
#include <vector>

//
//	�O���錾
//
class CTimer;

//	���ԊǗ��N���X
class CTimeManager
{
public:
	CTimeManager();				//�f�t�H���g�R���X�g���N�^
	explicit CTimeManager(float timeLimit);	//���Ԑ����t���R���X�g���N�^
	~CTimeManager();							//�f�X�g���N�^
	HRESULT Init(float timeLimit = 0.0f);				//�ď������p���\�b�h
	void Uninit();
	void Update(float deltatime);			//�X�V
	void Draw();							//�`��
	void DrawInfo();						//���\��
	void AddTime(float additionalTime);		//���ԉ��Z
	void SubTime(float penaltyTime);	//���Ԍ��Z
	float GetTimeRemaining()const;			//�c�莞�Ԏ擾
	bool IsTimeUp()const;					//���Ԑ؂�t���O�擾
	bool IsTimeStop()const;					//���Ԓ�~�t���O�擾
	void SetStop(bool bstop);				//�X�g�b�v�ݒ�
	std::vector<int> GetDigits()const;
private:
	float m_TimeRemaining = 0.0f;	//�c�莞��
	bool m_timeUp = false;			//���Ԑ؂�t���O
	std::vector<CTimer*> m_digitTimers;	//�����Ƃ̐����\���p
	bool m_bStop = true;				//���Ԃ��~���邩�ǂ���
	D3DXVECTOR3 m_RefPos;				//�^�C�}�[�̊�_
	LPD3DXFONT m_pFont;
};