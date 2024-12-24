//
//	最近敵方向表示用矢印
//
//
//

//
//	インクルード
//
#include "dirmarker.h"
#include "player.h"
#include "enemy.h"

namespace
{
	const int DIRDISPCNT = 5;	//敵総数が残りこれだけになったら最寄り敵方向を表示
	constexpr float DIR_ROT_X = D3DX_PI * 0.05f;	//マーカーを見やすくするための傾き加減(x軸)
}

//
//	コンストラクタ
//
CDIRMARKER::CDIRMARKER() :
	m_bDisp(false)
{
	Init();
}

//
//	デストラクタ
//
CDIRMARKER::~CDIRMARKER()
{

}

//
//	初期化
//
HRESULT CDIRMARKER::Init()
{
	if (FAILED(CObjectX::Init()))	//頂点バッファ作成
	{
		return E_FAIL;
	}

	m_ModelFile = "data\\model\\dirmarker.x";
	m_scale = { 1.0f,1.0f,1.0f };

	//矢印のタイプをセット
	SetType(DIRMARKER);

	return S_OK;
}

//
//	終了処理
//
void CDIRMARKER::Uninit()
{
	CObjectX::Uninit();
}

//
//	更新処理
//
void CDIRMARKER::Update()
{
	//ゲーム中の敵総数を取得
	int enmCnt = CEnmManager::GetEnmCnt();
	if (enmCnt <= DIRDISPCNT)
	{//敵総数が一定以下になったら
		//	方向表示をONにする
		m_bDisp = true;
	}
	if (enmCnt <= 0)
	{//	全部飛ばしたら消す
		m_bDisp = false;
	}

	CObjectX::Update();

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
				vec3 targetpos = pPlayer->GetPos();
				vec3 targetrot = pPlayer->GetRot();
				//取得してきた情報でマトリックスを作成
				D3DXMATRIX rotmtx;
				D3DXMatrixRotationYawPitchRoll(&rotmtx, targetrot.y, targetrot.x, targetrot.z);

				//任意のオフセット
				vec3 offset = { 0.0f,20.0f,0.0f };

				//オフセットをワールド座標に変換
				vec3 offsetInWorld;
				offsetInWorld.x = offset.x * rotmtx._11 + offset.y * rotmtx._21 + offset.z * rotmtx._31;
				offsetInWorld.y = offset.x * rotmtx._12 + offset.y * rotmtx._22 + offset.z * rotmtx._32;
				offsetInWorld.z = offset.x * rotmtx._13 + offset.y * rotmtx._23 + offset.z * rotmtx._33;

				//位置計算
				vec3 newPos =
				{
					targetpos.x + offsetInWorld.x,
					targetpos.y + offsetInWorld.y,
					targetpos.z + offsetInWorld.z
				};

				//	位置を設定
				SetPos(newPos);

				//	最寄りの敵取得
				CEnemy* pNearestEnemy = pPlayer->GetNearestEnm();

				if (pNearestEnemy)
				{//最寄りの敵情報が保存されていれば
					//敵位置取得
					vec3 enmPos = pNearestEnemy->GetPos();

					//プレイヤーの位置から最寄りの敵の方向を計算
					vec3 direction = targetpos - enmPos;
					D3DXVec3Normalize(&direction, &direction);	//正規化

					//マーカーの回転を計算
					float yaw = atan2f(direction.x, direction.z);	//y軸回転

					//	マーカーの回転を設定
					SetRot({ DIR_ROT_X, yaw, 0.0f });
				}
			}
		}
	}
}

//
//	描画処理
//
void CDIRMARKER::Draw()
{
	if (m_bDisp)
	{
		CObjectX::Draw();
	}
}

//
//	生成処理
//
CDIRMARKER* CDIRMARKER::Create(D3DXVECTOR3 pos)
{
	CDIRMARKER* pDirmarker = new CDIRMARKER;
	pDirmarker->Init();
	pDirmarker->m_pos = pos;

	return pDirmarker;
}
