//=========================================================================================
//	障害物CPP
//
//
//=========================================================================================

//======================================
//	インクルード
//======================================
#include "obstacle.h"
#include "texture.h"
#include "player.h"

//======================================
//	静的メンバ変数初期化
//======================================
int CObsManager::m_ObsCnt = 0;
const std::string CObstacle::FILE_OBSTACLE = "data\\texture\\machine.png";

//ターゲット地点リスト
map<string, vector<vec3>> CObsManager::m_targetLists =
{
	{
		"左右x",
		{
			vec3(+200.0f,0.0f,0.0f),
			vec3(-200.0f,0.0f,0.0f)
		}
	},
	{
		"左右z",
		{
			vec3(0.0f,0.0f,+200.0f),
			vec3(0.0f,0.0f,-200.0f)
		}
	},
	{
		"右左x",
		{
			vec3(-200.0f,0.0f,0.0f),
			vec3(+200.0f,0.0f,0.0f)
		}
	},
{
		"右左z",
		{
			vec3(0.0f,0.0f,-200.0f),
			vec3(0.0f,0.0f,+200.0f)
		}
	},
	{
		"上下",
		{
			vec3(0.0f,+250.0f,0.0f),
			vec3(0.0f,-250.0f,0.0f)
		}
	},
	{
		"下上",
		{
			vec3(0.0f,-250.0f,0.0f),
			vec3(0.0f,+250.0f,0.0f)
		}
	},
	{
		"長左右",
		{
			vec3(+200.0f,0.0f,0.0f),
			vec3(-200.0f,0.0f,0.0f)
		}
	},
	{
		"右回転",
		{
			vec3(0.0f,+200.0f,0.0f),
			vec3(0.0f,+150.0f,-150.0f),
			vec3(0.0f,0.0f,-200.0f),
			vec3(0.0f,-150.0f,-150.0f),
			vec3(0.0f,-200.0f,0.0f),
			vec3(0.0f,-150.0f,+150.0f),
			vec3(0.0f,0.0f,+200.0f),
			vec3(0.0f,+150.0f,+150.0f),
		}
	},
	{
		"左回転",
		{
			vec3(0.0f,-200.0f,0.0f),
			vec3(0.0f,-150.0f,-150.0f),
			vec3(0.0f,0.0f,-200.0f),
			vec3(0.0f,+150.0f,-150.0f),
			vec3(0.0f,+200.0f,0.0f),
			vec3(0.0f,+150.0f,+150.0f),
			vec3(0.0f,0.0f,+200.0f),
			vec3(0.0f,-150.0f,+150.0f),
		}
	},
};

//ターゲット地点リストの名前リスト
vector<string> CObsManager::m_targetListnameLists =
{
	"左右x",
	"左右z",
	"右左x",
	"右左z",
	"上下",
	"下上",
	"長左右",
	"右回転",
	"左回転",
};

//
//	定数
//
namespace
{
	using vec3 = D3DXVECTOR3;
	constexpr float PLSIZE = 30.0f;		//プレイヤーのサイズ
	constexpr float REFSIZE = 10.0f;	//障害物基準のサイズ
	constexpr float SPEED = 10.0f;	//障害物動きスピード
}

//======================================
//	コンストラクタ
//======================================
CObstacle::CObstacle() :
	m_Type(nullptr),
	m_diffPL(0.0f),
	m_BlowDir(0.0f, 0.0f, 0.0f),
	m_Oldpos(0.0f, 0.0f, 0.0f),
	m_RefPos(0.0f, 0.0f, 0.0f),
	m_movePnt(),
	m_movePntIdx(0),
	m_speed(0.0f)
{
	Init();
	CObsManager::IncCnt();
}

//======================================
//	デストラクタ
//======================================
CObstacle::~CObstacle()
{
	CObsManager::DecCnt();
}

//======================================
//	初期化
//======================================
HRESULT CObstacle::Init()
{
	if (FAILED(CObjectX::Init()))	//頂点バッファ作成
	{
		return E_FAIL;
	}

	m_ModelFile = "data\\model\\cube.x";

	m_scale = { 1.0f, 1.0f, 1.0f };

	m_speed = 0.0f;

	CObject::SetType(OBSTACLE);

	//テクスチャ作成
	CTexture* pTex = CManager::GetTexture();	//テクスチャ情報取得
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_OBSTACLE)));	//設定

	return S_OK;
}

//======================================
//	終了処理
//======================================
void CObstacle::Uninit()
{
	CObjectX::Uninit();
}

//======================================
//	更新処理
//======================================
void CObstacle::Update()
{
	CCamera* pCamera = CManager::GetCamera();

	CObjectX::Update();

	//位置を過去に
	m_Oldpos = m_pos;

	if (m_Type)
	{//	タイプがあればそれごとの追加更新
		m_Type->AddUpdate(this);
	}

	//オブジェクトとの当たり判定
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		//オブジェクト取得
		CObject* pObj = CObject::GetObj(3, nCntObj);

		if (pObj != nullptr)
		{//	使われていたら
			if (pObj->GetType() == CObject::TYPE::PLAYER)
			{//オブジェクトのタイプがプレイヤーなら
				//オブジェクトのタイプをプレイヤー型にダウンキャスト
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);

				//プレイヤーの位置を取得
				vec3 targetpos = pPlayer->GetPos();
				vec3 targetSize = { PLSIZE,PLSIZE,PLSIZE };
				vec3 MySize = m_scale * REFSIZE;

				if (IsColliding(m_pos, MySize, targetpos, targetSize))
				{
					//pCamera->SetShake(60, 20);
				}
			}
		}
	}
}

//======================================
//	描画処理
//======================================
void CObstacle::Draw()
{
	CObjectX::Draw();
}

//======================================
//	動きの追加
//======================================
void CObstacle::Moveing()
{
	if (!m_movePnt.empty())
	{//	ターゲット地点リストが空でなければ
		//	現在のターゲット地点を取得
		const vec3& tgtPos = m_RefPos + m_movePnt[m_movePntIdx];
		
		//	現在の位置とターゲット位置の方向ベクトルを算出
		vec3 direction = tgtPos - m_pos;

		//移動量を指定
		float Speed = m_speed;

		//単位ベクトルに正規化
		D3DXVec3Normalize(&direction, &direction);

		//	ベクトルに速さをかけて移動値を計算
		m_move = direction * Speed;

		//	移動値加算
		m_pos += m_move;

		//	ターゲット地点に近い場合に次のターゲットを指定する
		vec3 targetDiff = tgtPos - m_pos;
		float distanceToTarget = D3DXVec3Length(&targetDiff);

		//	現在地からターゲット地点までの距離が速さ未満なら
		size_t size = m_movePnt.size();
		if (distanceToTarget < Speed)
		{//	次のターゲット地点を指定する
			m_movePntIdx = (m_movePntIdx + 1) % size;
		}
	}
}

//======================================
//	生成処理(ノーマルタイプ固定)
//======================================
CObstacle* CObstacle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	CObstacle* pObs = new CObstacle;

	pObs->Init();
	pObs->m_RefPos = pos;
	pObs->m_pos = pos;
	pObs->m_scale = scale;
	pObs->SetState(new NomalType());

	return pObs;
}

//======================================
//	生成処理
//======================================
CObstacle* CObstacle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, const std::vector<vec3>& targetList, float speed, int moveIdx)
{
	CObstacle* pObs = new CObstacle;

	pObs->Init();
	pObs->m_RefPos = pos;
	pObs->m_pos = pos;
	pObs->m_scale = scale;
	pObs->SetState(new MoveingType);
	pObs->m_movePnt = targetList;
	pObs->m_speed = speed;
	pObs->m_movePntIdx = moveIdx;

	return pObs;
}

//======================================
//	状態設定
//======================================
void CObstacle::SetState(ObsType* obstype)
{
	if (m_Type)
	{
		//古い状態の削除
		delete m_Type;
		m_Type = nullptr;
	}

	//	新しい状態載設定
	m_Type = obstype;
}

//======================================
//	当たり判定
//======================================
bool CObstacle::IsColliding(const vec3& pos1, const vec3& size1, const vec3& pos2, const vec3& size2)
{
	return 
		(pos1.x - size1.x <= pos2.x + size2.x && pos1.x + size1.x >= pos2.x - size2.x &&
		pos1.y - size1.y <= pos2.y + size2.y && pos1.y + size1.y >= pos2.y - size2.y &&
		pos1.z - size1.z <= pos2.z + size2.z && pos1.z + size1.z >= pos2.z - size2.z)
		;
}

//============================================================================
//	状態ごとの追加更新処理
//============================================================================

//======================================
//	ノーマルタイプ
//======================================
void NomalType::AddUpdate(CObstacle* pObs)
{

}

//======================================
//	動くタイプ
//======================================
void MoveingType::AddUpdate(CObstacle* pObs)
{
	//動き関数
	pObs->Moveing();
}

//======================================
//	ターゲットリスト設定
//======================================
void CObsManager::SetTargetLists(const map<string, vector<vec3>>& newtargetLists)
{
	m_targetLists = newtargetLists;
}

//======================================
//	ターゲットリスト取得
//======================================
const vector<vec3>& CObsManager::GetTargetList(const string& listname)
{
	static const vector<vec3> emptylist;	//静的空リスト
	auto it = m_targetLists.find(listname);
	if (it != m_targetLists.end())
	{
		return it->second;
	}
	else
	{//	見つからなければ空リスト
		return emptylist;
	}
}
