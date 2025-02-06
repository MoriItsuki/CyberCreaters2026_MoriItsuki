//===============================================================
//	シーン管理CPP
//
//
//===============================================================

//
//	インクルード
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
//	静的メンバ変数初期化
//
const std::string CAnnounceEvent::FILE_ANNOUNCE = "data\\texture\\announce.png";

//
//	コンストラクタ
//
CScene::CScene() :
	m_mode(MODE::MODE_TITLE),
	m_pTimeManager(nullptr)
{

}

//
//	デストラクタ
//
CScene::~CScene()
{

}

//
//	初期設定
//
HRESULT CScene::Init()
{
	return S_OK;
}

//
//	終了処理
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
//	更新処理
//
void CScene::Update()
{
	
}

//
//	描画処理
//
void CScene::Draw()
{

}

//
//	生成処理
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
//	時間管理設定
//
void CScene::SetTimeManager(CTimeManager* timemanager)
{
	m_pTimeManager = timemanager;
}

//
//	共有データの設定
//
void CScene::SetShareData(std::shared_ptr<ShereData> data)
{
	m_ShareData = data;
}

//
//	シーン間で共有するデータの取得
//
std::weak_ptr<ShereData> CScene::GetShareData() const
{
	return m_ShareData;
}

//===============================================================
//	タイトル
//
//
//===============================================================

//
//	コンストラクタ
//
CTitle::CTitle()
{
	CBG::Create(CBG::TYPE::TITLE);
}

//
//	デストラクタ
//
CTitle::~CTitle()
{

}

//
//	初期設定
//
HRESULT CTitle::Init()
{
	return S_OK;
}

//
//	終了処理
//
void CTitle::Uninit()
{
	CScene::Uninit();
}

//
//	更新処理
//
void CTitle::Update()
{
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();
	CInputGamePad* pGamepad = CManager::GetGamepad();
	CTimeManager* pTimeManager = CManager::GetTimeManager();
	
	//タイム非表示
	pTimeManager->SetDisp(false);

	if (pKeyboard->GetTrigger(DIK_RETURN) == true || pGamepad->GetTrigger(0) == true)
	{
		Uninit();
		CManager::SetMode(MODE_GAME);
	}
}

//
//	描画処理
//
void CTitle::Draw()
{

}

//===============================================================
//	ゲーム
//
//
//===============================================================

//
//	コンストラクタ
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
//	デストラクタ
//
CGame::~CGame()
{

}

//
//	初期設定
//
HRESULT CGame::Init()
{
	using vec3 = D3DXVECTOR3;

	vec3
		n = { 1.0f,1.0f,1.0f },
		//========================================================
			//	壁によるスケール
		W_SideOut = { 001.0f,30.0f,500.0f },	//左右壁
		W_CurveOut = { 500.0f,30.0f,001.0f },	//前後壁
		W_In = { 400.0f,30.0f,400.0f },			//中央壁
		W_ = { 1.0f,1.0f,1.0f },					//仮設置
	//========================================================
		//	敵サイズののスケール
		E_ = { 5.0f,5.0f,5.0f },					//仮設置
	//========================================================
		//	敵施設スケール
		EC_ = { 5.0f,5.0f,5.0f },					//仮設置
	//========================================================
		//	障害物スケール
		O_ = { 10.0f,20.0f,10.0f },					//仮配置
		O_CUBE = { 15.0f,15.0f,15.0f },			//正方形
		O_RECTside = { 100.0f,10.0f,10.0f },	//横長方形
		O_RECTver = { 5.0f,25.0f,5.0f },		//縦長方形
		O_cube = { 3.0f,3.0f,3.0f },			//縦長方形
		O_BOARD = { 1.0f,10.0f,10.0f }			//板
	//========================================================
		
	;
	
	//	巡回ルートの種類ごとのリストを取得
	auto
		//=========================================================================================
			//	敵巡回リスト
		E_CW = CEnmManager::m_targetLists.find("時計回り"),			//	"時計回り"	で検索
		E_unCW = CEnmManager::m_targetLists.find("反時計回り"),		//	"反時計回り"で検索
		E_Nomove = CEnmManager::m_targetLists.find("動きなし"),		//	"動きなし"	で検索
	//=========================================================================================
		//	障害物巡回リスト
		O_LRx = CObsManager::m_targetLists.find("左右x"),			//	"左右x"で検索
		O_LRz = CObsManager::m_targetLists.find("左右z"),			//	"左右z"で検索
		O_RLx = CObsManager::m_targetLists.find("右左x"),			//	"右左x"で検索
		O_RLz = CObsManager::m_targetLists.find("右左z"),			//	"右左z"で検索
		O_UpDown = CObsManager::m_targetLists.find("上下"),			//	"上下"で検索
		O_DownUp = CObsManager::m_targetLists.find("下上"),			//	"下上"で検索
		O_lLR = CObsManager::m_targetLists.find("長左右"),			//	"長左右"で検索
		O_CW_xy = CObsManager::m_targetLists.find("右回転"),		//	"右回転"で検索
		O_unCW_xy = CObsManager::m_targetLists.find("左回転")		//	"左回転"で検索
	//=========================================================================================

		;

	if (E_CW == CEnmManager::m_targetLists.end())
	{ // 存在しない
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
	//	アイテム
	LoadInfo("data\\Load\\item.txt", m_ItemList);

	//===============================================================================================================
	//	プレイヤー
	m_pPlayer = CPlayer::Create(vec3(4500.0f, 0.0f, 4000.0f), vec3(0.0f, 0.0f, 0.0f));

	//===============================================================================================================
	//	障害物
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
	//	敵
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
	//	敵施設（実験）
	CEnmCreate::Create(vec3(+4800.0f, -250.0f, -4800.0f), vec3(0.0f, D3DX_PI * +0.75f, 0.0f), EC_, 3);
	CEnmCreate::Create(vec3(+4800.0f, -250.0f, +4800.0f), vec3(0.0f, D3DX_PI * +0.25f, 0.0f), EC_, 3);
	CEnmCreate::Create(vec3(-4800.0f, -250.0f, +4800.0f), vec3(0.0f, D3DX_PI * -0.25f, 0.0f), EC_, 3);
	CEnmCreate::Create(vec3(-4800.0f, -250.0f, -4800.0f), vec3(0.0f, D3DX_PI * -0.75f, 0.0f), EC_, 3);
	//===============================================================================================================
	//	外壁
	CBlock::Create(vec3(-5000.0f, 0.0f, +0000.0f), vec3(0.0f, 0.0f, 0.0f), CBlock::BLTYPE::GROUND, W_SideOut);
	CBlock::Create(vec3(+5000.0f, 0.0f, +0000.0f), vec3(0.0f, 0.0f, 0.0f), CBlock::BLTYPE::GROUND, W_SideOut);
	CBlock::Create(vec3(+0000.0f, 0.0f, +5000.0f), vec3(0.0f, 0.0f, 0.0f), CBlock::BLTYPE::GROUND, W_CurveOut);
	CBlock::Create(vec3(+0000.0f, 0.0f, -5000.0f), vec3(0.0f, 0.0f, 0.0f), CBlock::BLTYPE::GROUND, W_CurveOut);
	//===============================================================================================================
	//	内壁
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
//	終了処理
//
void CGame::Uninit()
{
	//内容保存
#ifdef _DEBUG
	//アイテム
	//SaveInfo("data\\Load\\item.txt", m_ItemList);
#endif // _DEBUG

	//メンバポインタ削除
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
//	更新処理
//
void CGame::Update()
{
	CInputGamePad* pGamepad = CManager::GetGamepad();
	CTimeManager* pTimeManager = CManager::GetTimeManager();
	CProcess* pProcess = CManager::GetProcess();

	if (pTimeManager->IsTimeStop() == true)
	{//	ストップ状態なら更新せずに準備確認をする
		if (m_pStartRes != nullptr)
		{//	スタート準備用の画面が定義済みなら
			if (m_pStartRes->IsStart())
			{//ゲーム開始したら
				m_pStartRes->Uninit();	//削除
				m_pStartRes = nullptr;	//nullptr

				//時間停止を解除
				pTimeManager->SetStop(false);	
			}
			else
			{//ゲーム開始まではこれしかできないようにする
				return;
			}
		}
	}

	static float transitionTime = 3.0f;
	bool bend = false;	//ゲーム終了フラグを下ろして初期化

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
	{//	ゲーム中の敵総数が0以下だったら
		//ゲーム終了フラグを立てる
		bend = true;
		//時間を停止
		pTimeManager->SetStop(true);
		//終了合図
		m_pAnnoEve->SetNum(1);
	}

	if (pTimeManager)
	{//	TimeManagerが定義されていたら
		//	敵総数によるフラグが立っていなければ
		if (bend != true)
		{//	タイムアップしているか取得
			bend = pTimeManager->IsTimeUp();
		}
	}

	if (bend == true)
	{//	終了フラグが立っていれば遷移待機時間をスタート
		transitionTime -= pProcess->GetDeltaTime();
	}
	if (transitionTime <= 0.0f)
	{//	遷移待機時間が終わったら
		//遷移待機時間をリセット
		transitionTime = 3.0f;
		auto pShare = GetShareData();
		if (pShare.lock())
		{
			if (pTimeManager)
			{
				pShare.lock()->Resukttime = pTimeManager->GetTimeRemaining();
			}
		}

		//	リザルト画面へ
		CManager::SetMode(MODE_RESULT);
	}
#ifdef _DEBUG
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();

	if (pKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		CManager::SetMode(MODE_RESULT);
	}

	//テスト/画面揺らし
	CCamera* pCamera = CManager::GetCamera();
	if (pKeyboard->GetTrigger(DIK_Y) == true)
	{
		pCamera->SetShake(180, 20);
	}
#endif // _DEBUG
}

//
//	描画処理
//
void CGame::Draw()
{
	
}

//================================================
//	場所データをロード＆生成
//================================================
void CGame::LoadInfo(const string& filename, vector<CItem*>& items)
{
	ifstream file(filename);

	if (!file.is_open())
	{
		//	ファイルが開けない時
		MessageBox(0, "Failed to open item file!", "Error", MB_OK);
		return;
	}

	string line;

	while (getline(file, line))
	{
		//	行をトリムして空白やタブを除去
		line = Trim(line);

		//"POS"を含む行だけ処理する
		if (line.find("POS") == 0)
		{
			//"POS = "を削除して座標部分を抽出
			string data = line.substr(6);	//"POS = "は6文字なので
			istringstream iss(data);
			float x, y, z;

			//座標を読み取る
			if (iss >> x >> y >> z)
			{
				vec3 pos = { x, y, z };

				//アイテム生成
				CItem* newItem = CItem::Create(pos);
				items.push_back(newItem);
			}
			else
			{
				//正しくない座標データが入ったとき
				MessageBox(0, "Invalid pos data format!", "Error", MB_OK);
			}
		}
	}

	file.close();
}

//================================================
//	場所データを記録
//================================================
void CGame::SaveInfo(const std::string& filename, std::vector<CItem*>& items)
{
	ofstream file(filename);
	if (!file.is_open())
	{
		MessageBox(0, "Failed to open file saving items!", "Error", MB_OK);
		return;
	}

	//小数点以下2桁で固定小数点形式を使用
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
//	文字列のトリム処理(文字列の最初、末尾から空白を消す)
//===================================================================
std::string CGame::Trim(const std::string& str)
{
	//	先頭の空白、タブを削除
	size_t start = str.find_first_not_of(" \t");
	if (start == string::npos) return "";	//文字がすべて空白だった場合

	//	末尾の空白やタブを削除
	size_t end = str.find_last_not_of(" \t");
	return str.substr(start, end - start + 1);
}

//===============================================================
//	リザルト
//
//
//===============================================================

//
//	コンストラクタ
//
CResult::CResult() :
	m_pRanking(nullptr)
{
	CBG::Create(CBG::TYPE::RESULT);
}

//
//	デストラクタ
//
CResult::~CResult()
{

}

//
//	初期設定
//
HRESULT CResult::Init()
{
	m_pRanking = std::make_unique<CRankingManager>();
	m_pRanking->Init("data\\Load\\ranking.txt");

	return S_OK;
}

//
//	終了処理
//
void CResult::Uninit()
{
	CScene::Uninit();
}

//
//	更新処理
//
void CResult::Update()
{
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();
	CInputGamePad* pGamepad = CManager::GetGamepad();
	CTimeManager* pTimeManager = CManager::GetTimeManager();
	static bool isTimeAdded = false;	//タイム追加フラグ
	auto pShare = GetShareData().lock();

	//そのままだと時間表示が出たままなので消す
	pTimeManager->SetDisp(false);

	if (pShare && !isTimeAdded)
	{//	共有データが定義されていて、データを保存していなかったら
		float time = 500.0f - pShare->Resukttime;
		m_pRanking->SetCurrentPlayTime(time);	//	今回のプレイのタイム記録
		m_pRanking->AddTime(time);				//	タイムの追加
		m_pRanking->SaveRanking();				//	タイムの保存
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
//	描画処理
//
void CResult::Draw()
{
	if (m_pRanking)
	{
		m_pRanking->Draw();
	}
}

//===============================================================
//	イベントアナウンス用ポリゴン表示
//===============================================================

//
//	コンストラクタ
//
CAnnounceEvent::CAnnounceEvent() :
	m_nNum(-1),
	m_isVisible(false)
{

}

//
//	デストラクタ
//
CAnnounceEvent::~CAnnounceEvent()
{

}

//
//	初期化
//
HRESULT CAnnounceEvent::Init()
{
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	//テクスチャ作成
	CTexture* pTex = CManager::GetTexture();	//テクスチャ情報取得
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_ANNOUNCE)));	//設定

	return S_OK;
}

//
//	終了
//
void CAnnounceEvent::Uninit()
{
	CObject2D::Uninit();
}

//
//	更新
//
void CAnnounceEvent::Update()
{
	CObject2D::Update();

	VERTEX_2D* pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = vec3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[1].pos = vec3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = vec3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = vec3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	//	桁数に応じた分割サイズ
	float divY = 1.0f / DIVISION_ANNOUNCE;

	pVtx[0].tex = vec2(0.0f, divY * m_nNum);
	pVtx[1].tex = vec2(1.0f, divY * m_nNum);
	pVtx[2].tex = vec2(0.0f, divY * (m_nNum + 1));
	pVtx[3].tex = vec2(1.0f, divY * (m_nNum + 1));

	//rhwの設定(固定)	頂点カラーの設定
	for (int nCnt = 0; nCnt < CProcess::MAX_VERTEX; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
		pVtx[nCnt].col = m_color;
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//
//	描画
//
void CAnnounceEvent::Draw()
{
	if (m_isVisible)
	{
		CObject2D::Draw();
	}
}

//
//	生成処理
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
//	番号設定
//
void CAnnounceEvent::SetNum(int num)
{
	if (num < -1 || num > DIVISION_ANNOUNCE - 1)
	{
		return;
	}

	if (num == -1)
	{
		//特殊値 -1: 非表示にする
		m_isVisible = false;
	}
	else
	{
		//通常範囲
		m_isVisible = true;
		m_nNum = num;
	}
}
