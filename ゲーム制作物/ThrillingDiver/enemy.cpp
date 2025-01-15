//======================================================================================
//	敵CPP(enemy.cpp)
//
//
//======================================================================================

//==========================================================
//	インクルード
//==========================================================
#include "enemy.h"
#include "manager.h"
#include "texture.h"
#include "player.h"
#include "dirmarker.h"

//==========================================================
//	定数
//==========================================================
namespace
{
	using vec3 = D3DXVECTOR3;
	constexpr float ENMSIZE = 100.0f;	//敵の大きさ
	constexpr float BLOWTIMER = 180.0f;	//PL接触から消えるまでの時間
	constexpr float BLOWSCALE = 50.0f;	//PL接触時の飛んでいくのベクトルへの倍率
	const vec3 BlowVecNor = { 0.0f,5.0f,0.0f };
}

//==========================================================
//	静的メンバ関数初期化
//==========================================================
const std::string CEnemy::FILE_ENEMY = "data\\texture\\BG.png";
LPDIRECT3DTEXTURE9 CEnemy::m_textures[ENMTYPE::MAX] = {};
int CEnmManager::m_EnmCnt = 0;	//0初期化
//ターゲット地点リスト
map<string, vector<vec3>> CEnmManager::m_targetLists =	
{
	{//	時計周りの巡回
		"時計回り",
		{
			vec3(+4750.0f, 0.0f, -4750.0f),
			vec3(-4750.0f, 0.0f, -4750.0f),
			vec3(-4750.0f, 0.0f, +4750.0f),
			vec3(+4750.0f, 0.0f, +4750.0f)
		}
	},
	{//	反時計回りの巡回
		"反時計回り",
		{
			vec3(-4250.0f, 0.0f, +4250.0f),
			vec3(-4250.0f, 0.0f, -4250.0f),
			vec3(+4250.0f, 0.0f, -4250.0f),
			vec3(+4250.0f, 0.0f, +4250.0f)
		}
	},
	{//	巡回しない(止まる)
		"動きなし",
		{
			
		}
	}
};
//ターゲット地点リストの名前リスト
vector<string> CEnmManager::m_targetListnameLists = 	
{
	"時計回り",
	"反時計回り",
	"動きなし"
};

//==========================================================
//	コンストラクタ
//==========================================================
CEnemy::CEnemy(std::vector<vec3> value) :
	CObjectX(),
	m_EnmType(ENMTYPE::NONE),
	m_moveValue(0.0f),
	m_Oldpos(0.0f, 0.0f, 0.0f),
	m_bLand(false),
	m_move{ 0.0f, 0.0f, 0.0f },
	m_movePnt(value),
	m_movePntIdx(0),
	m_InitPos(false),
	m_currentState(nullptr),
	m_blowTime(BLOWTIMER),
	m_pFont(nullptr),
	m_diffPL(0.0f),
	m_directionPL(0.0f, 0.0f, 0.0f),
	m_BlowDir(0.0f, 0.0f, 0.0f)
{
	Init();

	//	カウントアップ
	CEnmManager::IncEnmCnt();
}

//==========================================================
//	デストラクタ
//==========================================================
CEnemy::~CEnemy()
{
	//	カウントダウン
	//CEnmManager::DecEnmCnt();
}

//==========================================================
//	初期設定(頂点バッファ生成)
//==========================================================
HRESULT CEnemy::Init()
{
	// フォント設定の構造体を初期化
	D3DXFONT_DESC fontDesc = {};
	fontDesc.Height = 24;                           //	フォントサイズ
	fontDesc.Width = 0;								//	フォントの幅(0->自動調整)
	fontDesc.Weight = FW_BOLD;						//	フォントの太さ
	fontDesc.MipLevels = 1;							//	ミップマップレベル
	fontDesc.Italic = false;						//	イタリック体
	fontDesc.CharSet = DEFAULT_CHARSET;				//	文字セット
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;	//	出力精度
	fontDesc.Quality = DEFAULT_QUALITY;				//	描画品質
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;	//ピッチとファミリ
	strcpy_s(fontDesc.FaceName, "Arial");           // フォントの種類

	// フォントの作成
	if (FAILED(D3DXCreateFontIndirect(CManager::GetRenderer()->GetDevice(), &fontDesc, &m_pFont)))
	{
		return E_FAIL;
	}

	if (FAILED(CObjectX::Init()))	//頂点バッファ作成
	{
		return E_FAIL;
	}

	m_pos = vec3(0.0f, 0.0f, 0.0f);

	CObject::SetType(ENEMY);

	m_moveValue = 0.5f;

	m_ModelFile = "data\\model\\enemy.x";

	m_scale = { 1.0f, 1.0f, 1.0f };

	//テクスチャ作成
	CTexture* pTex = CManager::GetTexture();	//テクスチャ情報取得
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_ENEMY)));	//設定

	SetState(new NormalState());

	return S_OK;
}

//==========================================================
//	終了処理(頂点バッファの破棄)
//==========================================================
void CEnemy::Uninit()
{
	CObjectX::Uninit();

	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
}

//==========================================================
//	更新処理(頂点情報の更新)
//==========================================================
void CEnemy::Update()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	CObjectX::Update();

	if (m_currentState)
	{//	状態ごとの更新情報をここで入れる
		m_currentState->Addupdate(this);
		
		if (dynamic_cast<KnockbackState*>(m_currentState) != nullptr)
		{//	状態
			if (m_blowTime > 0.0f)
			{
				m_blowTime -= 1.0f;

				if (m_blowTime <= 0.0f)
				{
					this->Release();
				}
			}

			return;
		}
	}

	CInputKeyboard* pKeyboard = CManager::GetKeyboard();
	
	if (!m_movePnt.empty())
	{//	ターゲット地点リストが空でなければ
		if (!m_InitPos)
		{
			//最近ターゲット位置を検索し、IDセット
			float minDistance = FLT_MAX;

			//ターゲット地点の最大取得																		
			size_t size = m_movePnt.size();
			for (size_t i = 0; i < size; i++)
			{
				vec3 diff = m_movePnt[i] - m_pos;
				float distance = D3DXVec3Length(&diff);
				if (distance < minDistance)
				{
					minDistance = distance;
					m_movePntIdx = i;
				}
			}
			m_InitPos = true;	//1回しかやらない
		}

		//	現在のターゲット地点を取得
		vec3 tgtPos = m_movePnt[m_movePntIdx];

		//	現在の位置とターゲット位置の方向ベクトルを算出
		vec3 direction = tgtPos - m_pos;

		//移動量を指定
		float speed = 20.0f;

		//単位ベクトルに正規化
		D3DXVec3Normalize(&direction, &direction);

		//向きを移動方向に合わせる
		if (D3DXVec3Length(&direction) > 0.0f)
		{
			//z軸を前方向とした場合、移動方向ベクトルから回転を計算
			float rotationY = atan2f(-direction.x, -direction.z);	//y軸周りの回転角を計算
			m_rot.y = rotationY;	//y軸回転に反映
		}

		//	ベクトルに速さをかけて移動値を計算
		m_move = direction * speed;

		//	移動値加算
		m_pos += m_move;

		//	ターゲット地点に近い場合に次のターゲットを指定する
		vec3 targetDiff = tgtPos - m_pos;
		float distanceToTarget = D3DXVec3Length(&targetDiff);

		//	現在地からターゲット地点までの距離が速さ未満なら
		size_t size = m_movePnt.size();
		if (distanceToTarget < speed)
		{//	次のターゲット地点を指定する
			m_movePntIdx = (m_movePntIdx + 1) % size;
		}
	}

	//最近敵との距離
	float closestDistance = FLT_MAX;	//初期値設定
	CEnemy* pNearestEnemy = nullptr;	//最近敵用変数

	//オブジェクトとの当たり判定
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		//オブジェクト取得
		CObject* pObj = CObject::GetObj(3, nCntObj);

		if (pObj != nullptr)
		{//	使われていたら

			CObject::TYPE type = pObj->GetType();

			if (type == PLAYER)
			{//オブジェクトのタイプがプレイヤーなら
				//オブジェクトのタイプをプレイヤー型にダウンキャスト
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);

				//プレイヤーの位置を取得
				vec3 PLPos = pPlayer->GetPos();

				m_directionPL = m_pos - PLPos;
				//	ベクトル用
				vec3 collisionDirection = m_directionPL;

				D3DXVec3Normalize(&m_BlowDir, &collisionDirection);

				//	距離の累乗を計算
				m_diffPL = D3DXVec3LengthSq(&m_directionPL);

				//	プレイヤーがこの敵の当たり判定内かどうかを判定
				if (m_diffPL <= ENMSIZE * ENMSIZE)
				{
					//	飛んでいく状態にする
					SetState(new KnockbackState);

					//	当たった瞬間に数を減らす
					CEnmManager::DecEnmCnt();
				}
			}
		}
	}
}

//==========================================================
//	描画処理(ポリゴン描画)
//==========================================================
void CEnemy::Draw()
{
	CObjectX::Draw();

#ifdef _DEBUG
	DrawEnmInfo();
#endif // _DEBUG
}

//==========================================================
//	敵生成
//==========================================================
CEnemy* CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, std::vector<vec3> movePos)
{
	CEnemy* pEnemy = new CEnemy(movePos);

	pEnemy->Init();
	pEnemy->SetPos(pos);
	pEnemy->m_scale = scale;
	pEnemy->m_movePnt = movePos;
	
	return pEnemy;
}

//================================================
//	テクスチャ取得
//================================================
LPDIRECT3DTEXTURE9 CEnemy::GetTextures(const TYPE& inType)
{
	return m_textures[inType];
}

//================================================
//	情報表記
//================================================
void CEnemy::DrawEnmInfo()
{
	RECT rect = { 0,0,CProcess::SCREEN_WIDTH,CProcess::SCREEN_HEIGHT };
	char aStr[256];
	float judgedis = ENMSIZE * ENMSIZE;

	snprintf(&aStr[0],
		256,
		"敵総数:%d\ntimer:%f\n差差差差:%f\n判定距離%f",
		CEnmManager::GetEnmCnt(),
		m_blowTime,
		m_diffPL,
		judgedis
	);

	m_pFont->DrawText(nullptr,
		&aStr[0],
		-1,
		&rect,
		DT_CENTER,
		D3DCOLOR_RGBA(255, 0, 0, 255));
}


//================================================
//	エネミーの状態設定
//================================================
void CEnemy::SetState(EnemyState* pNewstate)
{
	if (m_currentState)
	{
		//古い状態の削除
		delete m_currentState;
		m_currentState = nullptr;
	}

	//新しい状態の設定
	m_currentState = pNewstate;
}

//================================================
//	吹っ飛ぶ動き
//================================================
void CEnemy::BlowOff()
{		
	vec3 BlowAmount = m_BlowDir * BLOWSCALE + BlowVecNor;
	SetPos(m_pos + BlowAmount);
	m_rot.y += (D3DX_PI * 0.05f);
	m_rot.x += (D3DX_PI * 0.05f);
}

//==============================================
//	状態ごとの追加更新処理
//==============================================

//================================================
//	通常移動状態の追加更新処理
//================================================
void NormalState::Addupdate(CEnemy* enemy)
{
	//現状通常状態での主な追加更新はナシ
}

//================================================
//	飛ばされたときの追加更新処理
//================================================
void KnockbackState::Addupdate(CEnemy* enemy)
{
	//	飛ばされる処理
	enemy->BlowOff();
}

//================================================
//	ターゲットリストの設定
//================================================
void CEnmManager::SetTargetLists(const map<string, vector<vec3>>& newtargetLists)
{
	m_targetLists = newtargetLists;
}

//================================================
//	ターゲットリスト取得
//================================================
const vector<vec3>& CEnmManager::GetTargetList(const string& listname)
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
