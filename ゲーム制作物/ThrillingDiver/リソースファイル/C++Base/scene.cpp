//===============================================================
//	�V�[���Ǘ�CPP
//
//
//===============================================================

//
//	�C���N���[�h
//
#include "input.h"
#include "manager.h"
#include "camera.h"
#include "texture.h"

#include "object.h"
#include "scene.h"
#include "BG.h"
#include "enemy.h"
#include "object3D.h"
#include "object2D.h"
#include "player.h"
#include "block.h"
#include "obstacle.h"
#include "enmcreate.h"
#include "dirmarker.h"
#include "item.h"

#include "time.h"
#include "dashgauge.h"
#include "enemycount.h"

#include <iostream>
#include <map>
#include <list>
#include <array>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>


namespace
{
	constexpr int DIVISION_ANNOUNCE = 2;
}

//
//	�ÓI�����o�ϐ�������
//
const std::string CAnnounceEvent::FILE_ANNOUNCE = "data\\texture\\announce.png";

//
//	�R���X�g���N�^
//
CScene::CScene() :
	m_mode(MODE::MODE_TITLE),
	m_pTimeManager(nullptr)
{

}

//
//	�f�X�g���N�^
//
CScene::~CScene()
{

}

//
//	�����ݒ�
//
HRESULT CScene::Init()
{
	return S_OK;
}

//
//	�I������
//
void CScene::Uninit()
{
	if (m_pTimeManager)
	{
		m_pTimeManager->Uninit();
		m_pTimeManager = nullptr;
	}

	CObject::ReleaseAll();
}

//
//	�X�V����
//
void CScene::Update()
{
	
}

//
//	�`�揈��
//
void CScene::Draw()
{

}

//
//	��������
//
CScene* CScene::Create(MODE mode)
{
	CScene* Scene = nullptr;

	switch (mode)
	{
	case MODE_TITLE:
		Scene = new CTitle;
		break;
	case MODE_GAME:
		Scene = new CGame;
		break;
	case MODE_RESULT:
		Scene = new CResult;
		break;
	default:
		break;
	}

	if (Scene != nullptr)
	{
		Scene->Init();
		return Scene;
	}

	return nullptr;
}

//
//	���ԊǗ��ݒ�
//
void CScene::SetTimeManager(CTimeManager* timemanager)
{
	m_pTimeManager = timemanager;
}

//
//	���L�f�[�^�̐ݒ�
//
void CScene::SetShareData(std::shared_ptr<ShereData> data)
{
	m_ShareData = data;
}

//
//	�V�[���Ԃŋ��L����f�[�^�̎擾
//
std::weak_ptr<ShereData> CScene::GetShareData() const
{
	return m_ShareData;
}

//===============================================================
//	�^�C�g��
//
//
//===============================================================

//
//	�R���X�g���N�^
//
CTitle::CTitle()
{
	CBG::Create(CBG::TYPE::TITLE);
}

//
//	�f�X�g���N�^
//
CTitle::~CTitle()
{

}

//
//	�����ݒ�
//
HRESULT CTitle::Init()
{
	return S_OK;
}

//
//	�I������
//
void CTitle::Uninit()
{
	CScene::Uninit();
}

//
//	�X�V����
//
void CTitle::Update()
{
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();
	CInputGamePad* pGamepad = CManager::GetGamepad();
	CTimeManager* pTimeManager = CManager::GetTimeManager();
	
	//�^�C����\��
	pTimeManager->SetDisp(false);

	if (pKeyboard->GetTrigger(DIK_RETURN) == true || pGamepad->GetTrigger(0) == true)
	{
		Uninit();
		CManager::SetMode(MODE_GAME);
	}
}

//
//	�`�揈��
//
void CTitle::Draw()
{

}

//===============================================================
//	�Q�[��
//
//
//===============================================================

//
//	�R���X�g���N�^
//
CGame::CGame() :
	m_ItemList(),
	m_pPlayer(nullptr),
	m_pENMCNTManager(nullptr),
	m_pStartRes(nullptr),
	m_pAnnoEve(nullptr)
{

}

//
//	�f�X�g���N�^
//
CGame::~CGame()
{

}

//
//	�����ݒ�
//
HRESULT CGame::Init()
{
	using vec3 = D3DXVECTOR3;

	vec3
		n = { 1.0f,1.0f,1.0f },
		//========================================================
			//	�ǂɂ��X�P�[��
		W_SideOut = { 001.0f,30.0f,500.0f },	//���E��
		W_CurveOut = { 500.0f,30.0f,001.0f },	//�O���
		W_In = { 400.0f,30.0f,400.0f },			//������
		W_ = { 1.0f,1.0f,1.0f },					//���ݒu
	//========================================================
		//	�G�T�C�Y�̂̃X�P�[��
		E_ = { 5.0f,5.0f,5.0f },					//���ݒu
	//========================================================
		//	�G�{�݃X�P�[��
		EC_ = { 5.0f,5.0f,5.0f },					//���ݒu
	//========================================================
		//	��Q���X�P�[��
		O_ = { 10.0f,20.0f,10.0f },					//���z�u
		O_CUBE = { 15.0f,15.0f,15.0f },			//�����`
		O_RECTside = { 100.0f,10.0f,10.0f },	//�������`
		O_RECTver = { 5.0f,25.0f,5.0f },		//�c�����`
		O_cube = { 3.0f,3.0f,3.0f },			//�c�����`
		O_BOARD = { 1.0f,10.0f,10.0f }			//��
	//========================================================
		
	;
	
	//	���񃋁[�g�̎�ނ��Ƃ̃��X�g���擾
	auto
		//=========================================================================================
			//	�G���񃊃X�g
		E_CW = CEnmManager::m_targetLists.find("���v���"),			//	"���v���"	�Ō���
		E_unCW = CEnmManager::m_targetLists.find("�����v���"),		//	"�����v���"�Ō���
		E_Nomove = CEnmManager::m_targetLists.find("�����Ȃ�"),		//	"�����Ȃ�"	�Ō���
	//=========================================================================================
		//	��Q�����񃊃X�g
		O_LRx = CObsManager::m_targetLists.find("���Ex"),			//	"���Ex"�Ō���
		O_LRz = CObsManager::m_targetLists.find("���Ez"),			//	"���Ez"�Ō���
		O_RLx = CObsManager::m_targetLists.find("�E��x"),			//	"�E��x"�Ō���
		O_RLz = CObsManager::m_targetLists.find("�E��z"),			//	"�E��z"�Ō���
		O_UpDown = CObsManager::m_targetLists.find("�㉺"),			//	"�㉺"�Ō���
		O_DownUp = CObsManager::m_targetLists.find("����"),			//	"����"�Ō���
		O_lLR = CObsManager::m_targetLists.find("�����E"),			//	"�����E"�Ō���
		O_CW_xy = CObsManager::m_targetLists.find("�E��]"),		//	"�E��]"�Ō���
		O_unCW_xy = CObsManager::m_targetLists.find("����]")		//	"����]"�Ō���
	//=========================================================================================

		;

	if (E_CW == CEnmManager::m_targetLists.end())
	{ // ���݂��Ȃ�
		assert(false);
	}
	else if (E_unCW == CEnmManager::m_targetLists.end())
	{
		assert(false);
	}
	else if (E_Nomove == CEnmManager::m_targetLists.end())
	{
		assert(false);
	}

	if (O_LRx == CObsManager::m_targetLists.end())
	{
		assert(false);
	}
	else if (O_LRz == CObsManager::m_targetLists.end())
	{
		assert(false);
	}
	else if (O_RLx == CObsManager::m_targetLists.end())
	{
		assert(false);
	}
	else if (O_RLz == CObsManager::m_targetLists.end())
	{
		assert(false);
	}
	else if (O_UpDown == CObsManager::m_targetLists.end())
	{
		assert(false);
	}
	else if (O_DownUp == CObsManager::m_targetLists.end())
	{
		assert(false);
	}
	else if (O_lLR == CObsManager::m_targetLists.end())
	{
		assert(false);
	}
	else if (O_CW_xy == CObsManager::m_targetLists.end())
	{
		assert(false);
	}
	else if (O_unCW_xy == CObsManager::m_targetLists.end())
	{
		assert(false);
	}
	
	//===============================================================================================================
	//	�A�C�e��
	LoadInfo("data\\Load\\item.txt", m_ItemList);

	//===============================================================================================================
	//	�v���C���[
	m_pPlayer = CPlayer::Create(vec3(4500.0f, 0.0f, 4000.0f), vec3(0.0f, 0.0f, 0.0f));

	//===============================================================================================================
	//	��Q��
	CObstacle::Create(vec3(+4100.0f, 0.0f, +1500.0f), O_CUBE, O_DownUp->second, 05.0f);
	CObstacle::Create(vec3(+4900.0f, 0.0f, +1500.0f), O_CUBE, O_UpDown->second, 05.0f);
	CObstacle::Create(vec3(+4500.0f, 0.0f, +1500.0f), O_, O_LRx->second, 05.0f);

	CObstacle::Create(vec3(-1000.0f, 0.0f, -4500.0f), O_CUBE, O_UpDown->second, 05.0f);
	CObstacle::Create(vec3(-1000.0f, 0.0f, -4900.0f), O_CUBE, O_DownUp->second, 05.0f);
	CObstacle::Create(vec3(-1000.0f, 0.0f, -4100.0f), O_CUBE, O_DownUp->second, 05.0f);

	CObstacle::Create(vec3(-4500.0f, +200.0f, +1000.0f), O_RECTside, O_lLR->second, 05.0f);
	CObstacle::Create(vec3(-4500.0f, -200.0f, +1000.0f), O_RECTside, O_lLR->second, 05.0f);
	CObstacle::Create(vec3(-5000.0f, +000.0f, +1000.0f), O_RECTver , O_lLR->second, 05.0f);
	CObstacle::Create(vec3(-4750.0f, +000.0f, +1000.0f), O_RECTver , O_lLR->second, 05.0f);
	CObstacle::Create(vec3(-4500.0f, +000.0f, +1000.0f), O_RECTver , O_lLR->second, 05.0f);
	CObstacle::Create(vec3(-4250.0f, +000.0f, +1000.0f), O_RECTver , O_lLR->second, 05.0f);
	CObstacle::Create(vec3(-4000.0f, +000.0f, +1000.0f), O_RECTver, O_lLR->second, 05.0f);

	auto direction = 
		array<pair<decltype(O_CW_xy), decltype(O_unCW_xy)>, 2>
	{
		make_pair(O_CW_xy, O_unCW_xy),
		make_pair(O_unCW_xy, O_CW_xy),
	};

	for (int c : {0, 1, 2})
	{
		for (int a : {0, 1, 2, 3})
		{
			const auto& dir = direction[a % 2];

			for (int b = 0; b < 8; ++b)
			{
				CObstacle::Create(vec3(+1000.0f - (1700.0f * c) + (200.0f * a), +000.0f, +4300.0f), O_cube, dir.first->second, 10.0f, b);
				CObstacle::Create(vec3(+1000.0f - (1700.0f * c) + (200.0f * a), +000.0f, +4700.0f), O_cube, dir.second->second, 10.0f, b);
			}
		}

		auto it = O_LRz;
		if (c % 2 == 0)
		{
			it = O_RLz;
		}
		CObstacle::Create(vec3(+1700.0f - (1700.0f * c), +000.0f, +4500.0f), O_BOARD, it->second, 02.0f);
	}
	//===============================================================================================================
	//	�G
	CEnemy::Create(vec3(+4750.0f, 0.0f, +4750.0f), E_, E_CW->second);
	CEnemy::Create(vec3(+4000.0f, 0.0f, +4750.0f), E_, E_CW->second);
	CEnemy::Create(vec3(-4000.0f, 0.0f, +4000.0f), E_, E_CW->second);
	CEnemy::Create(vec3(-3800.0f, 0.0f, +4000.0f), E_, E_CW->second);
	CEnemy::Create(vec3(-3600.0f, 0.0f, +4000.0f), E_, E_CW->second);
	CEnemy::Create(vec3(-3400.0f, 0.0f, +4000.0f), E_, E_CW->second);
	CEnemy::Create(vec3(-3200.0f, 0.0f, +4000.0f), E_, E_CW->second);
	CEnemy::Create(vec3(-3000.0f, 0.0f, +4000.0f), E_, E_CW->second);
	CEnemy::Create(vec3(+4500.0f, 0.0f, +4500.0f), E_, E_unCW->second);
	CEnemy::Create(vec3(+3500.0f, 0.0f, +4200.0f), E_, E_unCW->second);
	CEnemy::Create(vec3(+3500.0f, 0.0f, -4200.0f), E_, E_unCW->second);
	CEnemy::Create(vec3(+3300.0f, 0.0f, -4200.0f), E_, E_unCW->second);
	CEnemy::Create(vec3(+3100.0f, 0.0f, -4200.0f), E_, E_unCW->second);
	CEnemy::Create(vec3(+3500.0f, 0.0f, -4400.0f), E_, E_unCW->second);
	CEnemy::Create(vec3(+3500.0f, 0.0f, -4600.0f), E_, E_unCW->second);
	CEnemy::Create(vec3(+3500.0f, 0.0f, -4800.0f), E_, E_unCW->second);
	CEnemy::Create(vec3(+3200.0f, 0.0f, -4200.0f), E_, E_unCW->second);
	CEnemy::Create(vec3(+4500.0f, 0.0f, +4500.0f), E_, E_Nomove->second);
	CEnemy::Create(vec3(+4100.0f, 0.0f, +3300.0f), E_, E_Nomove->second);
	CEnemy::Create(vec3(+4100.0f, 0.0f, +3100.0f), E_, E_Nomove->second);
	CEnemy::Create(vec3(+4100.0f, 0.0f, +2800.0f), E_, E_Nomove->second);
	//===============================================================================================================
	//	�G�{�݁i�����j
	CEnmCreate::Create(vec3(+4800.0f, -250.0f, -4800.0f), vec3(0.0f, D3DX_PI * +0.75f, 0.0f), EC_, 3);
	CEnmCreate::Create(vec3(+4800.0f, -250.0f, +4800.0f), vec3(0.0f, D3DX_PI * +0.25f, 0.0f), EC_, 3);
	CEnmCreate::Create(vec3(-4800.0f, -250.0f, +4800.0f), vec3(0.0f, D3DX_PI * -0.25f, 0.0f), EC_, 3);
	CEnmCreate::Create(vec3(-4800.0f, -250.0f, -4800.0f), vec3(0.0f, D3DX_PI * -0.75f, 0.0f), EC_, 3);
	//===============================================================================================================
	//	�O��
	CBlock::Create(vec3(-5000.0f, 0.0f, +0000.0f), vec3(0.0f, 0.0f, 0.0f), CBlock::BLTYPE::GROUND, W_SideOut);
	CBlock::Create(vec3(+5000.0f, 0.0f, +0000.0f), vec3(0.0f, 0.0f, 0.0f), CBlock::BLTYPE::GROUND, W_SideOut);
	CBlock::Create(vec3(+0000.0f, 0.0f, +5000.0f), vec3(0.0f, 0.0f, 0.0f), CBlock::BLTYPE::GROUND, W_CurveOut);
	CBlock::Create(vec3(+0000.0f, 0.0f, -5000.0f), vec3(0.0f, 0.0f, 0.0f), CBlock::BLTYPE::GROUND, W_CurveOut);
	//===============================================================================================================
	//	����
	CBlock::Create(vec3(+0000.0f, 0.0f, +0000.0f), vec3(0.0f, 0.0f, 0.0f), CBlock::BLTYPE::GROUND, W_In);
	//===============================================================================================================
	//	UI
	m_pENMCNTManager = CENMCNTDisplayManager::Create(vec3(CProcess::SCREEN_WIDTH - 200.0f, CProcess::SCREEN_HEIGHT - 80.0f, 0.0f), CEnmManager::GetEnmCnt());
	m_pStartRes = CStartReserve::Create();
	m_pAnnoEve = CAnnounceEvent::Create(vec3(CProcess::SCREEN_WIDTH * 0.5f, 300.0f, 0.0f), vec3(300.0f, 200.0f, 0.0f));
	//===============================================================================================================

	return S_OK;
}

//
//	�I������
//
void CGame::Uninit()
{
	//���e�ۑ�
#ifdef _DEBUG
	//�A�C�e��
	//SaveInfo("data\\Load\\item.txt", m_ItemList);
#endif // _DEBUG

	//�����o�|�C���^�폜
	if (m_pENMCNTManager)
	{
		m_pENMCNTManager->Uninit();
		m_pENMCNTManager = nullptr;
	}
	if (m_pPlayer)
	{
		m_pPlayer->Uninit();
		m_pPlayer = nullptr;
	}
	if (m_pStartRes)
	{
		m_pStartRes->Uninit();
		m_pStartRes = nullptr;
	}
	if (m_pTimeManager)
	{
		m_pTimeManager->Uninit();
		m_pTimeManager = nullptr;
	}
	if (m_pAnnoEve)
	{
		m_pAnnoEve->Uninit();
		m_pAnnoEve = nullptr;
	}

	CScene::Uninit();
}

//
//	�X�V����
//
void CGame::Update()
{
	CInputGamePad* pGamepad = CManager::GetGamepad();
	CTimeManager* pTimeManager = CManager::GetTimeManager();
	CProcess* pProcess = CManager::GetProcess();

	if (pTimeManager->IsTimeStop() == true)
	{//	�X�g�b�v��ԂȂ�X�V�����ɏ����m�F������
		if (m_pStartRes != nullptr)
		{//	�X�^�[�g�����p�̉�ʂ���`�ς݂Ȃ�
			if (m_pStartRes->IsStart())
			{//�Q�[���J�n������
				m_pStartRes->Uninit();	//�폜
				m_pStartRes = nullptr;	//nullptr

				//���Ԓ�~������
				pTimeManager->SetStop(false);	
			}
			else
			{//�Q�[���J�n�܂ł͂��ꂵ���ł��Ȃ��悤�ɂ���
				return;
			}
		}
	}

	static float transitionTime = 3.0f;
	bool bend = false;	//�Q�[���I���t���O�����낵�ď�����

#ifdef _DEBUG
	/*if (pGamepad->GetTrigger(3) == true)
	{
		CItem* pnewItem = CItem::Create(m_pPlayer->GetPos());
		m_ItemList.push_back(pnewItem);
	}*/
#endif // _DEBUG

	m_pENMCNTManager->SetValue(CEnmManager::GetEnmCnt());
	m_pENMCNTManager->Update();

	if (CEnmManager::GetEnmCnt() <= 0)
	{//	�Q�[�����̓G������0�ȉ���������
		//�Q�[���I���t���O�𗧂Ă�
		bend = true;
		//���Ԃ��~
		pTimeManager->SetStop(true);
		//�I�����}
		m_pAnnoEve->SetNum(1);
	}

	if (pTimeManager)
	{//	TimeManager����`����Ă�����
		//	�G�����ɂ��t���O�������Ă��Ȃ����
		if (bend != true)
		{//	�^�C���A�b�v���Ă��邩�擾
			bend = pTimeManager->IsTimeUp();
		}
	}

	if (bend == true)
	{//	�I���t���O�������Ă���ΑJ�ڑҋ@���Ԃ��X�^�[�g
		transitionTime -= pProcess->GetDeltaTime();
	}
	if (transitionTime <= 0.0f)
	{//	�J�ڑҋ@���Ԃ��I�������
		//�J�ڑҋ@���Ԃ����Z�b�g
		transitionTime = 3.0f;
		auto pShare = GetShareData();
		if (pShare.lock())
		{
			if (pTimeManager)
			{
				pShare.lock()->Resukttime = pTimeManager->GetTimeRemaining();
			}
		}

		//	���U���g��ʂ�
		CManager::SetMode(MODE_RESULT);
	}
#ifdef _DEBUG
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();

	if (pKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		CManager::SetMode(MODE_RESULT);
	}

	//�e�X�g/��ʗh�炵
	CCamera* pCamera = CManager::GetCamera();
	if (pKeyboard->GetTrigger(DIK_Y) == true)
	{
		pCamera->SetShake(180, 20);
	}
#endif // _DEBUG
}

//
//	�`�揈��
//
void CGame::Draw()
{
	
}

//================================================
//	�ꏊ�f�[�^�����[�h������
//================================================
void CGame::LoadInfo(const string& filename, vector<CItem*>& items)
{
	ifstream file(filename);

	if (!file.is_open())
	{
		//	�t�@�C�����J���Ȃ���
		MessageBox(0, "Failed to open item file!", "Error", MB_OK);
		return;
	}

	string line;

	while (getline(file, line))
	{
		//	�s���g�������ċ󔒂�^�u������
		line = Trim(line);

		//"POS"���܂ލs������������
		if (line.find("POS") == 0)
		{
			//"POS = "���폜���č��W�����𒊏o
			string data = line.substr(6);	//"POS = "��6�����Ȃ̂�
			istringstream iss(data);
			float x, y, z;

			//���W��ǂݎ��
			if (iss >> x >> y >> z)
			{
				vec3 pos = { x, y, z };

				//�A�C�e������
				CItem* newItem = CItem::Create(pos);
				items.push_back(newItem);
			}
			else
			{
				//�������Ȃ����W�f�[�^���������Ƃ�
				MessageBox(0, "Invalid pos data format!", "Error", MB_OK);
			}
		}
	}

	file.close();
}

//================================================
//	�ꏊ�f�[�^���L�^
//================================================
void CGame::SaveInfo(const std::string& filename, std::vector<CItem*>& items)
{
	ofstream file(filename);
	if (!file.is_open())
	{
		MessageBox(0, "Failed to open file saving items!", "Error", MB_OK);
		return;
	}

	//�����_�ȉ�2���ŌŒ菬���_�`�����g�p
	file << fixed << setprecision(2);

	for (const auto& item : items)
	{
		D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
		pos = item->GetPos();
		file << "	POS = " << pos.x << " " << pos.y << " " << pos.z << endl;
	}

	file.close();
}

//===================================================================
//	������̃g��������(������̍ŏ��A��������󔒂�����)
//===================================================================
std::string CGame::Trim(const std::string& str)
{
	//	�擪�̋󔒁A�^�u���폜
	size_t start = str.find_first_not_of(" \t");
	if (start == string::npos) return "";	//���������ׂċ󔒂������ꍇ

	//	�����̋󔒂�^�u���폜
	size_t end = str.find_last_not_of(" \t");
	return str.substr(start, end - start + 1);
}

//===============================================================
//	���U���g
//
//
//===============================================================

//
//	�R���X�g���N�^
//
CResult::CResult() :
	m_pRanking(nullptr)
{
	CBG::Create(CBG::TYPE::RESULT);
}

//
//	�f�X�g���N�^
//
CResult::~CResult()
{

}

//
//	�����ݒ�
//
HRESULT CResult::Init()
{
	m_pRanking = std::make_unique<CRankingManager>();
	m_pRanking->Init("data\\Load\\ranking.txt");

	return S_OK;
}

//
//	�I������
//
void CResult::Uninit()
{
	CScene::Uninit();
}

//
//	�X�V����
//
void CResult::Update()
{
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();
	CInputGamePad* pGamepad = CManager::GetGamepad();
	CTimeManager* pTimeManager = CManager::GetTimeManager();
	static bool isTimeAdded = false;	//�^�C���ǉ��t���O
	auto pShare = GetShareData().lock();

	//���̂܂܂��Ǝ��ԕ\�����o���܂܂Ȃ̂ŏ���
	pTimeManager->SetDisp(false);

	if (pShare && !isTimeAdded)
	{//	���L�f�[�^����`����Ă��āA�f�[�^��ۑ����Ă��Ȃ�������
		float time = 500.0f - pShare->Resukttime;
		m_pRanking->SetCurrentPlayTime(time);	//	����̃v���C�̃^�C���L�^
		m_pRanking->AddTime(time);				//	�^�C���̒ǉ�
		m_pRanking->SaveRanking();				//	�^�C���̕ۑ�
		isTimeAdded = true;
		m_pRanking->UpdateNumberGroup();
	}

	m_pRanking->Update();

	if (pKeyboard->GetTrigger(DIK_RETURN) == true || pGamepad->GetTrigger(0) == true)
	{
		Uninit();
		CManager::SetMode(MODE_TITLE);
		isTimeAdded = false;
	}
}

//
//	�`�揈��
//
void CResult::Draw()
{
	if (m_pRanking)
	{
		m_pRanking->Draw();
	}
}

//===============================================================
//	�C�x���g�A�i�E���X�p�|���S���\��
//===============================================================

//
//	�R���X�g���N�^
//
CAnnounceEvent::CAnnounceEvent() :
	m_nNum(-1),
	m_isVisible(false)
{

}

//
//	�f�X�g���N�^
//
CAnnounceEvent::~CAnnounceEvent()
{

}

//
//	������
//
HRESULT CAnnounceEvent::Init()
{
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	//�e�N�X�`���쐬
	CTexture* pTex = CManager::GetTexture();	//�e�N�X�`�����擾
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_ANNOUNCE)));	//�ݒ�

	return S_OK;
}

//
//	�I��
//
void CAnnounceEvent::Uninit()
{
	CObject2D::Uninit();
}

//
//	�X�V
//
void CAnnounceEvent::Update()
{
	CObject2D::Update();

	VERTEX_2D* pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = vec3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[1].pos = vec3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = vec3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = vec3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	//	�����ɉ����������T�C�Y
	float divY = 1.0f / DIVISION_ANNOUNCE;

	pVtx[0].tex = vec2(0.0f, divY * m_nNum);
	pVtx[1].tex = vec2(1.0f, divY * m_nNum);
	pVtx[2].tex = vec2(0.0f, divY * (m_nNum + 1));
	pVtx[3].tex = vec2(1.0f, divY * (m_nNum + 1));

	//rhw�̐ݒ�(�Œ�)	���_�J���[�̐ݒ�
	for (int nCnt = 0; nCnt < CProcess::MAX_VERTEX; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
		pVtx[nCnt].col = m_color;
	}

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//
//	�`��
//
void CAnnounceEvent::Draw()
{
	if (m_isVisible)
	{
		CObject2D::Draw();
	}
}

//
//	��������
//
CAnnounceEvent* CAnnounceEvent::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CAnnounceEvent* pAnnoEve = new CAnnounceEvent();
	pAnnoEve->Init();
	pAnnoEve->SetPos(pos);
	pAnnoEve->SetSize(size);

	return pAnnoEve;
}

//
//	�ԍ��ݒ�
//
void CAnnounceEvent::SetNum(int num)
{
	if (num < -1 || num > DIVISION_ANNOUNCE - 1)
	{
		return;
	}

	if (num == -1)
	{
		//����l -1: ��\���ɂ���
		m_isVisible = false;
	}
	else
	{
		//�ʏ�͈�
		m_isVisible = true;
		m_nNum = num;
	}
}
