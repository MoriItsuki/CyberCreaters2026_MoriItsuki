//
//	���U���g�����L���O
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
#include <memory>
#include <stdexcept>

//	���l�\���|���S���N���X
class CNumDisp :public CObject2D
{
public:
	CNumDisp();
	~CNumDisp()override;

	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CNumDisp* Create(int value, D3DXVECTOR3 pos);
	void SetValue(int value);
	void SetVisibility(bool isvisible);
private:
	int m_digit;	//����
	int m_value;	//���l
	bool m_isVisible;	//�\����\��
	static const std::string FILE_NUMBER;
};

//	���������l�̐����|���S���̊Ǘ��N���X
class CNumberGroup
{
public:
	CNumberGroup();
	~CNumberGroup();

	HRESULT Init();							//	������
	void Uninit();							//	���
	void Update();							//	�X�V����
	void Draw();							//	�`�揈��

	void SetNumber(float number);			//	���l��ݒ�(�����_�Ή�)
	void SetPosition(D3DXVECTOR3 pos);		//	�O���[�v�̕\���ʒu
	void SetVisibility(bool isVisible);		//	�\��/��\����ݒ�
	void SetColor(D3DCOLOR color);			//	�F�ύX
	void SetSizeMag(float mag);				//	�T�C�Y�{��
	D3DCOLOR GetColor()const { return m_color; }	//�F�擾
private:
	std::vector<CNumDisp*> m_numbers;		//	�����|���S���̃��X�g
	D3DXVECTOR3 m_pos;						//	�O���[�v�̊�ʒu
	bool m_isVisible;						//	�O���[�v�̕\�����
	float m_currentvalue;					//	���݂̐��l
	float m_fSizeMag = 1;					//	�T�C�Y�̔{��
	D3DCOLOR m_color;						//	�F��ێ�����ϐ�
};

//�����L���O�v�f
struct RankingEntry
{
	float time;	//	�^�C��

	bool operator<(const RankingEntry& other) const
	{
		return time < other.time;
	}
};

//�����L���O�}�l�[�W���[
class CRankingManager
{
public:
	CRankingManager();
	~CRankingManager();

	HRESULT Init(const std::string& filename);
	void Uninit();	//�I��
	void Update();	//�X�V
	void Draw();	//�`��
	void AddTime(float time);	//�^�C���������L���O�ǉ�
	void SortRanking();			//�����L���O����ׂ���
	void SaveRanking();			//�����L���O��ۑ�
	void LoadRanking();			//�����L���O���擾
	void UpdateNumberGroup();	//�\���p�f�[�^�X�V
	void SetCurrentPlayTime(float time);	//���݂̃v���C�^�C����ݒ�
	static CRankingManager* Create(const std::string& filename);
private:
	std::vector<RankingEntry> m_ranking;		//	�����L���O�f�[�^
	std::vector<std::unique_ptr<CNumberGroup>> m_numberGroup;	//	�e���ʂ̃^�C���\���p
	std::unique_ptr<CNumberGroup> m_currentPlaytimeGroup;		//	����̃v���C�^�C���\���p
	std::string m_fileName;						//	�O���t�@�C����
	static const int MAX_ENTRIES = 10;			//	�����L���O�̍ő吔
	D3DXVECTOR3 m_basePos;								//	�\����ʒu
	D3DXVECTOR3 m_entryOffset;							//	�e�G���g���̕\���Ԋu
	float m_currentPlayTime;	//	����̃v���C�^�C��
	bool isPlayTimeInitialized = false;
};