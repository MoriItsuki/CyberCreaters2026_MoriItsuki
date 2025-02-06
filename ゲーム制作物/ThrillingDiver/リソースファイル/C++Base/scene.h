//
//	�V�[���Ǘ��w�b�_
//
//
//
#ifndef _SCENE_H_
#define _SCENE_H_

//
//	�C���N���[�h
//
#include "main.h"
#include "timemanager.h"
#include "manager.h"

#include "dashgauge.h"
#include "item.h"
#include "enemycount.h"
#include "startreserve.h"
#include "object2D.h"

#include "ranking.h"

#include <cassert>  // assert���g�p���邽�߂ɕK�v
#include <string>
#include <vector>

//�I�n�̍��}�Ȃǂ�\������|���S���N���X
class CAnnounceEvent :public CObject2D
{
public:
	CAnnounceEvent();
	~CAnnounceEvent();

	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CAnnounceEvent* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void SetNum(int num);
private:
	int m_nNum;	//�ԍ�
	bool m_isVisible;
	static const std::string FILE_ANNOUNCE;
};

//�V�[�����m�ŋ��L����f�[�^
struct ShereData
{
	float Resukttime;	//�Q�[�����ŋL�^���ꂽ����
};

//�V�[���Ǘ��N���X
class CScene
{
public:
	//��ʂ̎��
	typedef enum
	{
		MODE_TITLE = 0,
		MODE_GAME,
		MODE_RESULT,
		MODE_MAX
	}MODE;
	CScene();	//�R���X�g���N�^
	virtual ~CScene();	//�f�X�g���N�^
	virtual HRESULT Init();	//�����ݒ�
	virtual void Uninit();	//�I��
	virtual void Update();	//�X�V
	virtual void Draw();	//�`��
	static CScene* Create(MODE mode);	//����
	void SetTimeManager(CTimeManager* timeManager);
	void SetShareData(std::shared_ptr<ShereData> data);
	std::weak_ptr<ShereData> GetShareData()const;
protected:
	CTimeManager* m_pTimeManager; //���ԊǗ��|�C���^
private:
	MODE m_mode;	//���݂̃��[�h
	std::weak_ptr<ShereData> m_ShareData;
};

//�^�C�g���V�[���Ǘ��N���X
class CTitle :public CScene
{
public:
	CTitle();
	~CTitle();
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
private:

};

class CPlayer;

//�Q�[���V�[���Ǘ��N���X
class CGame :public CScene
{
public:
	CGame();
	~CGame();

	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static void LoadInfo(const std::string& filename, std::vector<CItem*>& items);	//�Q�[���J�n���ɒ��̏���ǂݍ���ŃA�C�e���𐶐�
	static void SaveInfo(const std::string& filename, std::vector<CItem*>& items);	//�Q�[���I�����ɒ��̏��������o���ăA�C�e�������L�^

	//	�w���p�[�֐�
	static std::string Trim(const std::string& str);	//������̃g��������
private:
	std::vector<CItem*> m_ItemList;
	CPlayer* m_pPlayer;							//�A�C�e�������p�v���C���[
	CENMCNTDisplayManager* m_pENMCNTManager;	//�G���\���}�l�[�W���[
	CStartReserve* m_pStartRes;					//�Q�[���J�n�O�����ҋ@�p�|���S��
	CAnnounceEvent* m_pAnnoEve;					//�Q�[���I�n�̃A�i�E���X�\���p�|���S��
};

//���U���g�V�[���Ǘ��N���X
class CResult :public CScene
{
public:
	CResult();
	~CResult();
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
private:
	std::unique_ptr<CRankingManager> m_pRanking;	//���U���g�\���p�����L���O
};

#endif // !_SCENE_H_
