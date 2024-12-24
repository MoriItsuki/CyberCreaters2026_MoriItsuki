//============================================================================
//	プレイヤーCPP(player.cpp)
//
//
//============================================================================
//==========================================================
//インクルード
//==========================================================
#include "main.h"
#include "player.h"
#include "manager.h"
#include "block.h"
#include "obstacle.h"
#include "texture.h"
#include "dirmarker.h"
#include "enemy.h"
#include "dashgauge.h"
#include "item.h"

#include <windows.h>

//==========================================================
//	定数
//==========================================================
namespace
{
	using vec3 = D3DXVECTOR3;

	constexpr float pool = 5.0f;
	constexpr float PLSIZE = 30.0f;
	constexpr float BLOCKSIZE = 10.0f;
	constexpr float BOXSIZE = 30.0f;
	constexpr float ITEMSIZE = 50.0f;
	constexpr float PL_FRICION = 0.005f;

	//移動値
	constexpr float NORMAL = 0.2f;		//通常走行
	constexpr float HIGHSPEED = 30.0f;	//ダッシュ

	constexpr int INITIALDASHCNT = 3;	//ダッシュできる初期回数
	constexpr int ITEMTODASH = 5;		//アイテムを何個ゲットしたらダッシュ回数に変換されるか

	constexpr float FPRot = 0.02f;		//プレイヤーが曲がったときに1フレームに回る角度
	constexpr float LIM_PL_ROT_Z = D3DX_PI * 0.15f;	//プレイヤーが曲がったときの傾きの限界+-

	constexpr int DASH_MAX = 10;		//貯められるダッシュの上限
}

//==========================================================
//	静的メンバ関数初期化
//==========================================================
const std::string CPlayer::FILE_PLAYER = "data\\texture\\machine.png";

//==========================================================
//	コンストラクタ
//==========================================================
CPlayer::CPlayer() :
	m_nDashCnt(0),
	m_nItemCnt(0),
	m_moveValue(0.0f),
	m_Oldpos(0.0f, 0.0f, 0.0f),
	m_vecFront(0.0f, 0.0f, 0.0f),
	m_nState(STATE::NONE),
	m_MoveState(MOVESTATE::STOP),
	m_fFriction(PL_FRICION),
	m_bStop(false),
	m_bDash(false),
	m_pNearestEnemy(nullptr),
	m_NearestEnmPos(0.0f, 0.0f, 0.0f),
	m_pDashGauge(nullptr)
{
	m_pFont = nullptr;
	Init();
}

//==========================================================
//	デストラクタ
//==========================================================
CPlayer::~CPlayer()
{
	
}

//==========================================================
//	初期設定(頂点バッファ生成)
//==========================================================
HRESULT CPlayer::Init()
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

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_moveValue = 0.5f;

	m_ModelFile = "data\\model\\player.x";

	//ダッシュ可能回数をリセット
	m_nDashCnt = INITIALDASHCNT;

	//テクスチャ作成
	CTexture* pTex = CManager::GetTexture();	//テクスチャ情報取得
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_PLAYER)));	//設定

	CObject::SetType(PLAYER);	//オブジェクトにプレイヤータイプを付与

	CDIRMARKER::Create(m_pos);

	m_pDashGauge = CDashgauge::Create();

	return S_OK;
}

//==========================================================
//	終了処理(頂点バッファの破棄)
//==========================================================
void CPlayer::Uninit()
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
void CPlayer::Update()
{
	CTimeManager* pTimeManager = CManager::GetTimeManager();
	bool bStop = pTimeManager->IsTimeStop();
	if (bStop)
	{
		return;
	}

	CObjectX::Update();

	//位置を過去に
	m_Oldpos = m_pos;

	//移動値を通常に
	m_moveValue = 0.0f;

	//	プレイヤーの入力処理
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();
	CInputGamePad* pButton = CManager::GetGamepad();
	CCamera* pCamera = CManager::GetCamera();
	
	//	トリガーの値を取得
	float NorTriggerValue = pButton->GetNorTriggerValue();	//適正化0.0～2.0

	//===============================
	//			    X
	//		 "ダッシュ切替"
	//===============================
	m_bDash = false;
	if (pButton->GetTrigger(2) == true &&
		m_nDashCnt > 0 && 
		m_MoveState != MOVESTATE::STOP)
	{
		m_bDash = true;
		--m_nDashCnt;	//ダッシュできる回数を減らす
	}

	//===============================
	//			右トリガー
	//			"前進"
	//===============================
	if (NorTriggerValue < 1.0)
	{
		if (!m_bDash)
		{
			//前進状態にする
			SetState(MOVESTATE::FORWARD);

			m_moveValue = NORMAL;
		}
		else
		{
			//ダッシュ状態にする
			SetState(MOVESTATE::DASH);

			m_moveValue = HIGHSPEED;
		}
	}
	else
	{
		SetState(MOVESTATE::STOP);
	}
	//===============================
	//			左トリガー
	//		"ブレーキ・後進"
	//===============================
	if (pButton->GetPress(5) == true)
	{//	右ショルダー押した時
		//後進状態にする
		SetState(MOVESTATE::BACK);

		m_fFriction += 0.0045f;

		m_bStop = true;
	}
	if (pButton->GetRelease(5) == true)
	{//	右ショルダー離した時
		m_fFriction = PL_FRICION;

		m_bStop = false;
	}

	//回転
	if (pButton->GetNorStick_lx() > 1.8)
	{
		//一定だけ曲がる
		m_rotgo.y += FPRot;
		//ブレーキが掛かっていたら
		if (m_bStop)	m_rotgo.y += FPRot;
		if (m_rot.z < LIM_PL_ROT_Z)	m_rot.z += FPRot;
	}
	if (pButton->GetNorStick_lx() < 0.2)
	{
		//一定だけ曲がる
		m_rotgo.y -= 0.02f;
		//ブレーキが掛かっていたら
		if (m_bStop)	m_rotgo.y -= FPRot;
		if (m_rot.z > -LIM_PL_ROT_Z)	m_rot.z -= FPRot;
	}
	if (pButton->GetNorStick_lx() == 1.0)
	{
		if (m_rot.z > 0.0)
		{
			m_rot.z -= FPRot;
		}
		if (m_rot.z < 0.0)
		{
			m_rot.z += FPRot;
		}
	}

	//=============================================================
	//	ダッシュゲージの更新
	//=============================================================
	float ratioDashgauge = static_cast<float>(m_nDashCnt) / static_cast<float>(DASH_MAX);
	m_pDashGauge->Update(ratioDashgauge);

#ifdef _DEBUG
		if (pKeyboard->GetPress(DIK_TAB) == true)
		{
			m_pos.y += 0.5f;
		}
		if (pKeyboard->GetPress(DIK_LSHIFT) == true)
		{
			m_pos.y -= 0.5f;
		}

		//デバッグ用ダッシュ回数追加
		if (pButton->GetTrigger(4) == true)
		{
			++m_nDashCnt;
		}
#endif // _DEBUG

	//移動値から軸ごとのムーブ値を算出
	m_move.x -= sinf(m_rot.y) * m_moveValue;
	m_move.z -= cosf(m_rot.y) * m_moveValue;

	//ベクトルを算出
	float fLength = -sqrtf(m_move.x * m_move.x + m_move.z * m_move.z);
	
	//プレイヤーの向きに合わせて移動量を補正
	m_move.x = sinf(m_rot.y) * fLength;
	m_move.z = cosf(m_rot.y) * fLength;

	//	Y値以外の移動値に減算処理
	m_move.x += (0.0f - m_move.x) * m_fFriction;
	m_move.z += (0.0f - m_move.z) * m_fFriction;

	//	ポジションにムーブ値を加算
	m_pos += m_move;

	//減算した結果動いていなければ
	if (m_move.x == 0.0f && m_move.z == 0.0f)
	{//	移動状態を"停止"にする
		SetState(MOVESTATE::STOP);
	}

	//	敵との長さ測定用
	float closestEnmDist = FLT_MAX;

	//オブジェクトとの当たり判定
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		//オブジェクト取得
		CObject* pObj = CObject::GetObj(3, nCntObj);

		if (pObj != nullptr)
		{//	使われていたら
			//	オブジェクトのタイプを取得
			CObject::TYPE type = pObj->GetType();

			//保存用変数
			D3DXVECTOR3 pos;	//止まる判定用変数
			D3DXVECTOR3 move(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 size;
			D3DXVECTOR3 rot;

			if (type == BLOCK)
			{//オブジェクトのタイプがブロックなら

				//オブジェクトのタイプをブロック型にキャスト
				CBlock* pBlock = dynamic_cast<CBlock*>(pObj);
				//	ブロックが定義されていなかったら飛ばす
				if (!pBlock) return;

				D3DXVECTOR3 scale = pBlock->GetScale();

				//ブロックの位置を取得
				pos = pBlock->GetPos();

				//プレイヤーとブロックの当たり判定のサイズ決定
				size = D3DXVECTOR3(PLSIZE + BLOCKSIZE * scale.x, PLSIZE + BLOCKSIZE * scale.y, PLSIZE + BLOCKSIZE * scale.z);

				rot = pBlock->GetRot();
			}
			else if (type == OBSTACLE)
			{//オブジェクトのタイプが障害物なら
				//オブジェクトのタイプを障害物型にキャスト
				CObstacle* pObs = dynamic_cast<CObstacle*>(pObj);
				//	障害物が定義されていなかったら飛ばす
				if (!pObs) return;

				D3DXVECTOR3 scale = pObs->GetScale();

				pos = pObs->GetPos();

				//プレイヤーと障害物の当たり判定のサイズ決定
				size = D3DXVECTOR3(PLSIZE + BLOCKSIZE * scale.x, PLSIZE + BLOCKSIZE * scale.y, PLSIZE + BLOCKSIZE * scale.z);
			}
			else if (type == ENEMY)
			{//オブジェクトのタイプが敵なら
				//オブジェクトのタイプをエネミー型にキャスト
				CEnemy* pEnemy = dynamic_cast<CEnemy*>(pObj);
				//	敵が定義されていなかったら飛ばす
				if (!pEnemy) return;
				//	位置取得
				vec3 enmPos = pEnemy->GetPos();
				//	方向ベクトルを求める
				vec3 direction = enmPos - m_pos;
				//	長さを算出
				float distSq = D3DXVec3LengthSq(&direction);

				//最寄りの敵を更新
				if (distSq < closestEnmDist)
				{//	さらに近い敵が見つかった場合
					closestEnmDist = distSq;	//敵との距離を更新
					m_pNearestEnemy = pEnemy;	//最寄りの敵を保持
					m_NearestEnmPos = enmPos;	//敵の位置を保持
				}
			}
			else if (type == ITEM)
			{//タイプがアイテム
				//アイテム型にキャスト
				CItem* pItem = dynamic_cast<CItem*>(pObj);
				if (!pItem) return;
				//	位置取得
				vec3 itemPos = pItem->GetPos();

				vec3 vecDir = m_pos - itemPos;

				float diffItem = D3DXVec3LengthSq(&vecDir);

				if (diffItem < ITEMSIZE * ITEMSIZE)
				{//	範囲内に入ったら
					if (m_nDashCnt >= DASH_MAX)
					{//	ダッシュ可能数がMAXだったら

					}
					else
					{//	それ以内だったら
						m_nItemCnt++;	//所持アイテム数を加算
						pItem->OutOn();	//消すフラグを立てる
					}
				}
			}

			//	アイテムが一定数に到達したら
			if (m_nItemCnt >= ITEMTODASH)
			{
				//	アイテム数をリセット
				m_nItemCnt = 0;
				//	ダッシュ回数を加算
				m_nDashCnt++;
			}

			//移動量を確認
			float absMoveY = std::abs(m_move.y);
			float absMoveX = std::abs(m_move.x);
			float absMoveZ = std::abs(m_move.z);

			//一番大きい移動量を持つ軸がどれかを確認
			if (absMoveY >= absMoveX && absMoveY >= absMoveZ)
			{//Y軸が一番大きければ
				//X軸の判定
				if (m_pos.z < pos.z + size.z && m_pos.z > pos.z - size.z &&
					m_pos.y < pos.y + size.y && m_pos.y > pos.y - size.y)
				{
					//	左から
					if (m_Oldpos.x <= pos.x - size.x && m_pos.x > pos.x - size.x)
					{
						{
							m_pos.x = pos.x - size.x;
							m_move.x = 0.0f;
							//m_moveValue = -pool;
						}
					}
					//	右から
					else if (m_Oldpos.x >= pos.x + size.x && m_pos.x < pos.x + size.x)
					{
						{
							m_pos.x = pos.x + size.x;
							m_move.x = 0.0f;
							//m_moveValue = -pool;
						}
					}
				}
				//Z軸の判定
				if (m_pos.x < pos.x + size.x && m_pos.x > pos.x - size.x &&
					m_pos.y < pos.y + size.y && m_pos.y > pos.y - size.y)
				{
					if (m_Oldpos.z <= pos.z - size.z && m_pos.z > pos.z - size.z)
					{
						{
							m_pos.z = pos.z - size.z;
							m_move.z = 0.0f;
							//m_moveValue = -pool;

						}
					}
					else if (m_Oldpos.z >= pos.z + size.z && m_pos.z < pos.z + size.z)
					{
						{
							m_pos.z = pos.z + size.z;
							m_move.z = 0.0f;
							//m_moveValue = -pool;

						}
					}
				}
				//Y軸の判定
				if (m_pos.x < pos.x + size.x && m_pos.x > pos.x - size.x &&
					m_pos.z < pos.z + size.z && m_pos.z > pos.z - size.z)
				{
					//上から落下
					if (m_Oldpos.y >= pos.y + size.y && m_pos.y < pos.y + size.y)
					{
						m_pos.y = pos.y + size.y;	//プレイヤーをブロックの上にセット
						m_move.y = 0.0f;
						//m_moveValue = -pool;
					}
					//下から上昇
					else if (m_Oldpos.y <= pos.y - size.y && m_pos.y > pos.y - size.y)
					{
						m_pos.y = pos.y - size.y;
						m_move.y = 0.0f;
						//m_moveValue = -pool;

					}
				}
			}
			else if (absMoveX >= absMoveY && absMoveX >= absMoveZ)
			{//X軸が一番大きければ
				//Y軸の判定
				if (m_pos.x < pos.x + size.x && m_pos.x > pos.x - size.x &&
					m_pos.z < pos.z + size.z && m_pos.z > pos.z - size.z)
				{
					//上から落下
					if (m_Oldpos.y >= pos.y + size.y && m_pos.y < pos.y + size.y)
					{
						m_pos.y = pos.y + size.y;	//プレイヤーをブロックの上にセット
						m_move.y = 0.0f;
						//m_moveValue = -pool;
					}
					//下から上昇
					else if (m_Oldpos.y <= pos.y - size.y && m_pos.y > pos.y - size.y)
					{
						m_pos.y = pos.y - size.y;
						m_move.y = 0.0f;
						//m_moveValue = -pool;

					}
				}
				//Z軸の判定
				if (m_pos.x < pos.x + size.x && m_pos.x > pos.x - size.x &&
					m_pos.y < pos.y + size.y && m_pos.y > pos.y - size.y)
				{
					if (m_Oldpos.z <= pos.z - size.z && m_pos.z > pos.z - size.z)
					{
						
						{
							m_pos.z = pos.z - size.z;
							m_move.z = 0.0f;
							//m_moveValue = -pool;

						}
					}
					else if (m_Oldpos.z >= pos.z + size.z && m_pos.z < pos.z + size.z)
					{
						
						{
							m_pos.z = pos.z + size.z;
							m_move.z = 0.0f;
							//m_moveValue = -pool;

						}

					}
				}
				//X軸の判定
				if (m_pos.z < pos.z + size.z && m_pos.z > pos.z - size.z &&
					m_pos.y < pos.y + size.y && m_pos.y > pos.y - size.y)
				{
					//	左から
					if (m_Oldpos.x <= pos.x - size.x && m_pos.x > pos.x - size.x)
					{
						
						{
							m_pos.x = pos.x - size.x;
							m_move.x = 0.0f;
							//m_moveValue = -pool;

						}
					}
					//	右から
					else if (m_Oldpos.x >= pos.x + size.x && m_pos.x < pos.x + size.x)
					{
						
						{
							m_pos.x = pos.x + size.x;
							m_move.x = 0.0f;
							//m_moveValue = -pool;

						}
					}
				}
			}
			else if (absMoveZ >= absMoveY && absMoveZ >= absMoveX)
			{//Z軸が一番大きければ
				//Y軸の判定
				if (m_pos.x < pos.x + size.x && m_pos.x > pos.x - size.x &&
					m_pos.z < pos.z + size.z && m_pos.z > pos.z - size.z)
				{
					//上から落下
					if (m_Oldpos.y >= pos.y + size.y && m_pos.y < pos.y + size.y)
					{
						m_pos.y = pos.y + size.y;	//プレイヤーをブロックの上にセット
						m_move.y = 0.0f;
						//m_moveValue = -pool;
					}
					//下から上昇
					else if (m_Oldpos.y <= pos.y - size.y && m_pos.y > pos.y - size.y)
					{
						m_pos.y = pos.y - size.y;
						m_move.y = 0.0f;
						//m_moveValue = -pool;

					}
				}
				//X軸の判定
				if (m_pos.z < pos.z + size.z && m_pos.z > pos.z - size.z &&
					m_pos.y < pos.y + size.y && m_pos.y > pos.y - size.y)
				{
					//	左から
					if (m_Oldpos.x <= pos.x - size.x && m_pos.x > pos.x - size.x)
					{
						
						{
							m_pos.x = pos.x - size.x;
							m_move.x = 0.0f;
							//m_moveValue = -pool;

						}
					}
					//	右から
					else if (m_Oldpos.x >= pos.x + size.x && m_pos.x < pos.x + size.x)
					{
						
						{
							m_pos.x = pos.x + size.x;
							m_move.x = 0.0f;
							//m_moveValue = -pool;

						}
					}
				}
				//Z軸の判定
				if (m_pos.x < pos.x + size.x && m_pos.x > pos.x - size.x &&
					m_pos.y < pos.y + size.y && m_pos.y > pos.y - size.y)
				{
					if (m_Oldpos.z <= pos.z - size.z && m_pos.z > pos.z - size.z)
					{
						
						{
							m_pos.z = pos.z - size.z;
							m_move.z = 0.0f;
							//m_moveValue = -pool;

						}
					}
					else if (m_Oldpos.z >= pos.z + size.z && m_pos.z < pos.z + size.z)
					{
						
						{
							m_pos.z = pos.z + size.z;
							m_move.z = 0.0f;
							//m_moveValue = -pool;

						}
					}
				}
			}
		}
	}

	if ((m_rotgo.y) > D3DX_PI)
	{
		float fOverRot = fabsf(m_rotgo.y) - D3DX_PI;
		m_rotgo.y = -(D3DX_PI - fOverRot);
	}
	else if ((m_rotgo.y) < -D3DX_PI)
	{
		float fOverRot = fabsf(m_rotgo.y) - D3DX_PI;
		m_rotgo.y = (D3DX_PI - fOverRot);
	}

	//目的の角度と現在の角度の差を求める
	float rotDiff = m_rotgo.y - m_rot.y;

	if (rotDiff > D3DX_PI)
	{//目的と現在の角度の差が3.14…以上なら
		m_rotgo.y -= (D3DX_PI * 2.0f);
		rotDiff = m_rotgo.y - m_rot.y;
	}
	else if (rotDiff < -D3DX_PI)
	{//-3.14…以下なら
		m_rotgo.y += (D3DX_PI * 2.0f);
		rotDiff = m_rotgo.y - m_rot.y;
	}

	m_rot.y += (rotDiff * 0.100f);	//だんだんと目的の角度に補正していく

	if ((m_rot.y) > D3DX_PI)
	{
		float overRot = fabsf(m_rot.y) - D3DX_PI;
		m_rot.y = -(D3DX_PI - overRot);
	}
	else if ((m_rot.y) < -D3DX_PI)
	{
		float overRot = fabsf(m_rot.y) - D3DX_PI;
		m_rot.y = (D3DX_PI - overRot);
	}
}

//==========================================================
//	描画処理(ポリゴン描画)
//==========================================================
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイスのポインタ

	CObjectX::Draw();
	m_pDashGauge->Draw(pDevice);

#ifdef _DEBUG
	DrawPLInfo();
#endif // _DEBUG
}

//==========================================================
//	CPlayer生成
//==========================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer* pPlayer = new CPlayer;
	pPlayer->Init();
	pPlayer->m_pos = pos;
	pPlayer->m_rot = rot;

	return pPlayer;
}

//==========================================================
//	プレイヤーのポジションセット
//==========================================================
void CPlayer::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==========================================================
//	プレイヤーの移動値セット
//==========================================================
void CPlayer::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//==========================================================
//	反射
//==========================================================
void CPlayer::SetReflec()
{

}

//==========================================================
//	プレイヤー移動状態設定
//==========================================================
void CPlayer::SetState(MOVESTATE state)
{
	m_MoveState = state;
}

//==========================================================
//	プレイヤーのポジションゲット
//==========================================================
D3DXVECTOR3 CPlayer::GetPos()
{
	return m_pos;
}

//==========================================================
//	プレイヤーのポジションゲット
//==========================================================
D3DXVECTOR3 CPlayer::GetVecFront()
{
	return m_vecFront;
}

//==========================================================
//	プレイヤーの状態取得
//==========================================================
int CPlayer::GetState()
{
	return m_nState;
}

//==========================================================
//	最寄りの敵取得
//==========================================================
CEnemy* CPlayer::GetNearestEnm()const
{
	return m_pNearestEnemy;
}

//==========================================================
//	最寄りの敵位置取得
//==========================================================
D3DXVECTOR3 CPlayer::GetNearestEnmPos() const
{
	return m_NearestEnmPos;
}

//==========================================================
//	残りダッシュ回数取得
//==========================================================
int CPlayer::GetDashCnt() const
{
	return m_nDashCnt;
}

//==========================================================
//	情報表記
//==========================================================
void CPlayer::DrawPLInfo()
{
	RECT rect = { 0,0,CProcess::SCREEN_WIDTH,CProcess::SCREEN_HEIGHT };
	char aStr[256];

	snprintf(&aStr[0],
		256,
		"<POS>\nX:%f\nY:%f\nZ:%f\n<ROT>\nX:%f\nY:%f\nZ:%f\n移動値:%f\n最寄り敵位置\nX:%f\nY:%f\nZ:%f\nダッシュ可能回数:%d\nアイテム:%d",
		m_pos.x, m_pos.y, m_pos.z,
		m_rot.x, m_pos.y, m_rot.z,
		m_moveValue,
		m_NearestEnmPos.x,
		m_NearestEnmPos.y,
		m_NearestEnmPos.z,
		m_nDashCnt,
		m_nItemCnt
	);

	m_pFont->DrawText(nullptr,
		&aStr[0],
		-1,
		&rect,
		DT_RIGHT,
		D3DCOLOR_RGBA(0, 0, 0, 255));
}

//==========================================================
//	当たり判定
//==========================================================
bool CPlayer::IsColliding(const vec3& pos1, const vec3& size1, const vec3& pos2, const vec3& size2)
{
	return
		(
			pos1.x - size1.x <= pos2.x + size2.x && pos1.x + size1.x >= pos2.x - size2.x &&
			pos1.y - size1.y <= pos2.y + size2.y && pos1.y + size1.y >= pos2.y - size2.y &&
			pos1.z - size1.z <= pos2.z + size2.z && pos1.z + size1.z >= pos2.z - size2.z
		);
}

